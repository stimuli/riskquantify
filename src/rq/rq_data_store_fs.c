/*
** rq_data_store_fs.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2009 Brett Hutley
**
** This file is part of the Risk Quantify Library
**
** Risk Quantify is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public
** License as published by the Free Software Foundation; either
** version 2 of the License, or (at your option) any later version.
**
** Risk Quantify is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
**
** You should have received a copy of the GNU Library General Public
** License along with Risk Quantify; if not, write to the Free
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include "rq_data_store_fs.h"
#include "rq_error.h"
#include "rq_stream_file.h"
#include "rq_calendar_mgr.h"
#include "rq_iterator.h"

/*
** dir/rq.xml
** dir/calendars
** dir/assets
** dir/bootstrapconfigs
*/
/* -- structs ----------------------------------------------------- */
struct rq_data_store_fs {
    char *base_dir;
};

/* -- statics ----------------------------------------------------- */
static const char *index_file_name = "rq.xml";

static const char *system_subdirs[] = {
    "calendars",
    "assets",
    "bootstrapconfigs",
    NULL
};
    
/* -- code -------------------------------------------------------- */
static char *
rq_data_store_fs_get_full_path(const char *dir, const char *file)
{
    char *fullpath = (char *)RQ_MALLOC(strlen(dir) + 2 + strlen(file));
    strcpy(fullpath, dir);
    strcat(fullpath, "/");
    strcat(fullpath, file);

    return fullpath;
}

static void 
rq_data_store_fs_free(void *store_data)
{
    struct rq_data_store_fs *data = (struct rq_data_store_fs *)store_data;

    if (data->base_dir)
        RQ_FREE(data->base_dir);

    RQ_FREE(data);
}

static FILE *
rq_data_store_open_file(const char *dirname, const char *filename)
{
    FILE *fh = 0;

    char *filepath = rq_data_store_fs_get_full_path(dirname, filename);

    /* we always open the file and create it. */
    fh = fopen(filepath, "w+");

    RQ_FREE(filepath);

    return fh;
}

static rq_error_code
rq_data_store_create_directory(const char *dirname)
{
    int err = mkdir(dirname, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
    if (err != 0 && errno != EEXIST)
        return (rq_error_code)errno;
    return RQ_OK;
}

static rq_error_code
rq_data_store_create_subdirectory(const char *dirname, const char *subdir)
{
    rq_error_code errcode;

    char *dirpath = rq_data_store_fs_get_full_path(dirname, subdir);

    errcode = rq_data_store_create_directory(dirpath);

    RQ_FREE(dirpath);

    return errcode;
}

static rq_error_code
rq_data_store_fs_unlink_file(const char *dirname, const char *filename)
{
    rq_error_code errcode;

    char *filepath = rq_data_store_fs_get_full_path(dirname, filename);

    errcode = unlink(filepath);

    RQ_FREE(filepath);

    return errcode;
}

static rq_error_code
rq_data_store_subdirectory_unlink_files(const char *dirname, const char *subdir)
{
    rq_error_code errcode;
    DIR *dir;

    char *dirpath = rq_data_store_fs_get_full_path(dirname, subdir);

    dir = opendir(dirpath);
    if (!dir)
        errcode = RQ_ENOENT;
    else
    {
        struct dirent *de;

        while ((de = readdir(dir)) != NULL)
        {
            if (de->d_type == DT_REG || de->d_type == DT_LNK)
            {
                rq_data_store_fs_unlink_file(dirpath, de->d_name);
            }
        }

        closedir(dir);
    }

    RQ_FREE(dirpath);

    return errcode;
}

static rq_error_code 
rq_data_store_fs_create(void *store_data, const char *dirname)
{
    struct rq_data_store_fs *d = (struct rq_data_store_fs *)store_data;
    FILE *fh;
    rq_error_code err;
    int i;

    if (d->base_dir)
    {
        RQ_FREE(d->base_dir);
        d->base_dir = NULL;
    }

    err = rq_data_store_create_directory(dirname);
    if (err != RQ_OK)
        return err;

    for (i = 0; system_subdirs[i]; i++)
        if ((err = rq_data_store_create_subdirectory(dirname, system_subdirs[i])) != RQ_OK)
            return err;

    if ((fh = rq_data_store_open_file(dirname, index_file_name)) == NULL)
        return (rq_error_code)errno;

    fclose(fh);

    d->base_dir = strdup(dirname);

    return RQ_OK;
}

static rq_error_code 
rq_data_store_fs_open(void *store_data, const char *dirname)
{
    struct rq_data_store_fs *d = (struct rq_data_store_fs *)store_data;
    FILE *fh;

    if (d->base_dir)
    {
        RQ_FREE(d->base_dir);
        d->base_dir = NULL;
    }

    if ((fh = rq_data_store_open_file(dirname, index_file_name)) == NULL)
        return (rq_error_code)errno;

    fclose(fh);

    d->base_dir = strdup(dirname);

    return RQ_OK;
}

rq_array_t 
rq_data_store_fs_list_files_in_subdir(const char *base_dir, const char *subdir)
{
    rq_array_t ar = rq_array_alloc(RQ_FREE);
    char *dirpath = rq_data_store_fs_get_full_path(base_dir, subdir);

    DIR *dir = opendir(dirpath);
    if (dir)
    {
        struct dirent *de;

        while ((de = readdir(dir)) != NULL)
        {
            if (de->d_type == DT_REG || de->d_type == DT_LNK)
            {
                char *filepath = rq_data_store_fs_get_full_path(dirpath, de->d_name);
                rq_array_push_back(ar, filepath);
            }
        }

        closedir(dir);
    }

    RQ_FREE(dirpath);

    return ar;
}

static rq_error_code
rq_data_store_fs_close(void *store_data)
{
    struct rq_data_store_fs *d = (struct rq_data_store_fs *)store_data;

    if (d->base_dir)
    {
        RQ_FREE(d->base_dir);
        d->base_dir = 0;
    }

    return RQ_OK;
}

static rq_error_code
rq_data_store_fs_system_load(void *store_data, rq_system_t system)
{
    struct rq_data_store_fs *d = (struct rq_data_store_fs *)store_data;

    if (d->base_dir)
    {
        rq_array_t ar = rq_data_store_fs_list_files_in_subdir(d->base_dir, "calendars");
        unsigned int i;

        rq_calendar_mgr_t calmgr = rq_system_get_calendar_mgr(system);
        rq_calendar_mgr_clear(calmgr);

        for (i = 0; i < rq_array_size(ar); i++)
        {
            const char *cal_file = (const char *)rq_array_get_at(ar, i);
            rq_stream_t stream = rq_stream_file_open(cal_file, "r");

            if (stream)
            {
                rq_calendar_t cal = rq_calendar_alloc(NULL);
                
                if (rq_calendar_read_from_stream(cal, stream) == RQ_OK)
                    rq_calendar_mgr_add(calmgr, cal);
                else
                    rq_calendar_free(cal);

                rq_stream_close(stream);
            }
        }

        rq_array_free(ar);

        return RQ_OK;
    }

    return RQ_FAILED;
}

static rq_error_code 
rq_data_store_fs_system_save(void *store_data, rq_system_t system)
{
    struct rq_data_store_fs *d = (struct rq_data_store_fs *)store_data;

    if (d->base_dir)
    {
        int i;
        rq_calendar_mgr_t calmgr;
        rq_iterator_t iter;

        char *calpath = rq_data_store_fs_get_full_path(d->base_dir, "calendars");

        /* remove all files from directory. */

        for (i = 0; system_subdirs[i]; i++)
            rq_data_store_subdirectory_unlink_files(d->base_dir, system_subdirs[i]);

        calmgr = rq_system_get_calendar_mgr(system);

        for (iter = rq_calendar_mgr_get_iterator(calmgr); !rq_iterator_at_end(iter); rq_iterator_incr(iter))
        {
            struct rq_variant var = rq_iterator_get_value(iter);
            rq_calendar_t cal = (rq_calendar_t)rq_variant_get_object_ptr(&var);

            char *calfile;
            char calfilename[512];

            snprintf(calfilename, 512, "%s.xml", rq_calendar_get_id(cal));

            calfile = rq_data_store_fs_get_full_path(calpath, calfilename);

            rq_stream_t calstream = rq_stream_file_open(calfile, "w+");

            rq_calendar_write_to_stream(cal, calstream);

            rq_stream_close(calstream);

            RQ_FREE(calfile);
            
        }
        
        RQ_FREE(calpath);

        return RQ_OK;
    }

    return RQ_FAILED;
}

static rq_error_code
rq_data_store_fs_market_load(void *store_data, rq_market_t market)
{
    struct rq_data_store_fs *d = (struct rq_data_store_fs *)store_data;

    return RQ_OK;
}

static rq_error_code 
rq_data_store_fs_market_save(void *store_data, rq_market_t market)
{
    struct rq_data_store_fs *d = (struct rq_data_store_fs *)store_data;

    return RQ_OK;
}


RQ_EXPORT rq_data_store_t 
rq_data_store_fs_alloc()
{
    struct rq_data_store_fs *store_data = 
        (struct rq_data_store_fs *)RQ_CALLOC(
            1, 
            sizeof(struct rq_data_store_fs)
            );
    rq_data_store_t store = _rq_data_store_alloc(
        rq_data_store_fs_free,
        rq_data_store_fs_create,
        rq_data_store_fs_open,
        rq_data_store_fs_close,
        rq_data_store_fs_system_load,
        rq_data_store_fs_system_save,
        rq_data_store_fs_market_load,
        rq_data_store_fs_market_save,
        store_data
    );


    return store;
}
