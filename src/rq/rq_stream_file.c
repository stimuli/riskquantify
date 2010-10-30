/*
** rq_stream_file.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
#include "rq_stream_file.h"
#include "rq_error.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

/* -- globals ------------------------------------------------------ */
const char *rq_stream_file_stream_type = "stream_file";

/* -- prototypes --------------------------------------------------- */
static void _rq_stream_file_close(void *d);

/* -- code --------------------------------------------------------- */
static void
_rq_stream_file_init(struct rq_stream_file *ss)
{
    ss->filename = NULL;
    ss->fh = NULL;
	ss->open_mode = RQ_STREAM_FILE_OPENMODE_READING;
}

static int 
_rq_stream_file_open(void *d)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (ss->fh)
    {
        fclose(ss->fh);
        ss->fh = NULL;
    }

    if (ss->filename)
    {
		/* TODO: Specify mode!!! */
		const char *open_mode = "r";
		if (ss->open_mode == RQ_STREAM_FILE_OPENMODE_WRITING)
			open_mode = "w";
		else if (ss->open_mode == RQ_STREAM_FILE_OPENMODE_READINGWRITINGCREATE)
			open_mode = "w+";

        ss->fh = fopen(ss->filename, open_mode);
        if (!ss->fh)
            return errno;
    }

    return 0;
}

static short
_rq_stream_file_is_open(void *d)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (!ss)
        return 0;
    return ss->fh != NULL;
}

static short
_rq_stream_file_at_end(void *d)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (!ss || !ss->fh)
        return 1;
    return feof(ss->fh);
}

static void 
_rq_stream_file_close(void *d)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (ss->fh)
    {
        fclose(ss->fh);
        ss->fh = NULL;
    }
}

static int 
_rq_stream_file_read(void *d, char *buffer, int num_bytes)
{
    int ret = -1;
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (ss->fh)
        ret = fread(buffer, 1, num_bytes, ss->fh);

    return ret;
}

static int 
_rq_stream_file_write(void *d, const char *buffer, int num_bytes)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    int ret = -1;

    if (ss->fh)
        ret = fwrite(buffer, num_bytes, 1, ss->fh);

    return ret;
}

static void 
_rq_stream_file_free(void *d)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (ss->fh)
        _rq_stream_file_close(d);
    if (ss->filename)
        RQ_FREE((char *)ss->filename);
    RQ_FREE(ss);
}

static long
_rq_stream_file_tell(void *d)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (!ss || !ss->fh)
        return -1;
    return ftell(ss->fh);
}

static rq_error_code
_rq_stream_file_seek(void *d, long offset)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    int ret;
    if (!ss || !ss->fh)
        return RQ_FAILED;
    return ((ret = fseek(ss->fh, offset, SEEK_SET)) == 0 ? RQ_OK : (rq_error_code)ret);
}

static void 
_rq_stream_file_rewind(void *d)
{
    struct rq_stream_file *ss = (struct rq_stream_file *)d;
    if (ss->fh)
        fseek(ss->fh, 0, SEEK_SET);
}

static struct rq_stream_callbacks stream_file_callbacks = {
    _rq_stream_file_open,
    _rq_stream_file_is_open,
    _rq_stream_file_at_end,
    _rq_stream_file_close,
    _rq_stream_file_read,
    _rq_stream_file_write,
    _rq_stream_file_tell,
    _rq_stream_file_seek,
    _rq_stream_file_rewind,
    _rq_stream_file_free
};

RQ_EXPORT rq_stream_t
rq_stream_file_alloc()
{
    struct rq_stream_file *ss = 
        (struct rq_stream_file *)RQ_CALLOC(1, sizeof(struct rq_stream_file));
    return _rq_stream_alloc(
        rq_stream_file_stream_type,
        ss,
        &stream_file_callbacks
        );
}

/**
 * Allocate and open a new file stream
 */
RQ_EXPORT rq_stream_t 
rq_stream_file_open(const char *filename, const char *openmode)
{
    rq_stream_t stream = rq_stream_file_alloc();
    int mode = RQ_STREAM_FILE_OPENMODE_READING;
    if (openmode)
    {
        if (*openmode == 'r')
            mode = RQ_STREAM_FILE_OPENMODE_READING;
        else if (*openmode == 'w')
        {
            if (openmode[1] == '+')
                mode = RQ_STREAM_FILE_OPENMODE_READINGWRITINGCREATE;
            else
                mode = RQ_STREAM_FILE_OPENMODE_WRITING;
        }
    }

    rq_stream_file_set_filename(stream, filename);
    rq_stream_file_set_openmode(stream, mode);

    if (rq_stream_open(stream) != 0)
    {
        rq_stream_free(stream);
        stream = NULL;
    }

    return stream;
}


RQ_EXPORT const char *
rq_stream_file_get_filename(rq_stream_t stream)
{
    struct rq_stream_file *ss = 
        (struct rq_stream_file *)_rq_stream_get_data(stream);
    return ss->filename;
}

RQ_EXPORT void 
rq_stream_file_set_filename(rq_stream_t stream, const char *filename)
{
    struct rq_stream_file *ss = 
        (struct rq_stream_file *)_rq_stream_get_data(stream);
    if (ss->filename)
    {
        if (ss->fh)
        {
            fclose(ss->fh);
            ss->fh = NULL;
        }
       RQ_FREE((char *)ss->filename);
    }

    ss->filename = RQ_STRDUP(filename);
}

RQ_EXPORT int
rq_stream_file_get_openmode(rq_stream_t stream)
{
    struct rq_stream_file *ss = 
        (struct rq_stream_file *)_rq_stream_get_data(stream);
	return ss->open_mode;
}

RQ_EXPORT void
rq_stream_file_set_openmode(rq_stream_t stream, int open_mode)
{
    struct rq_stream_file *ss = 
        (struct rq_stream_file *)_rq_stream_get_data(stream);
	ss->open_mode = open_mode;
}
