/*
** rq_stream_string.c
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
#include "rq_stream_string.h"
#include "rq_error.h"

#include <stdlib.h>
#include <string.h>

/* -- globals ------------------------------------------------------ */
const char *rq_stream_string_stream_type = "stream_string";


/* -- static vars -------------------------------------------------- */
static unsigned int s_initial_buffer_size = 1024;
static unsigned int s_initial_buffer_grow_size = 1024;

/* -- prototypes --------------------------------------------------- */
static void rq_stream_string_close(void *d);

/* -- code --------------------------------------------------------- */
static void
rq_stream_string_init(struct rq_stream_string *ss)
{
    ss->buffer = (char *)RQ_CALLOC(s_initial_buffer_size, sizeof(char));
    ss->max_buffer_size = s_initial_buffer_size;
    ss->buffer_grow_size = s_initial_buffer_grow_size;
    ss->buffer_len = 0;
    ss->ptr = ss->buffer;
    ss->is_open = 1;
}

static int 
rq_stream_string_open(void *d)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (ss->is_open)
        rq_stream_string_close(ss);

    rq_stream_string_init(ss);

    return 0;
}

static short 
rq_stream_string_is_open(void *d)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (!ss)
        return 0;
    return ss->is_open;
}

static short 
rq_stream_string_at_end(void *d)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (ss && ss->is_open)
    {
        int remaining = ss->buffer_len - (ss->ptr - ss->buffer);
        return remaining <= 0;
    }

    return 1;
}

static void 
rq_stream_string_close(void *d)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (ss->is_open)
    {
       RQ_FREE(ss->buffer);
        ss->buffer = NULL;
        ss->max_buffer_size = 0;
        ss->buffer_grow_size = 0;
        ss->buffer_len = 0;
        ss->ptr = NULL;
        ss->is_open = 0;
    }
}

static int 
rq_stream_string_read(void *d, char *buffer, int num_bytes)
{
    int ret = -1;
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (ss->is_open)
    {
        int num_to_read = num_bytes;
        int remaining = ss->buffer_len - (ss->ptr - ss->buffer);
        if (remaining < num_bytes)
            num_to_read = remaining;
        memcpy(buffer, ss->ptr, num_to_read);
        ss->ptr += num_to_read;
        ret = num_to_read;
    }

    return ret;
}

static int 
rq_stream_string_write(void *d, const char *buffer, int num_bytes)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    int ret = -1;

    if (ss->is_open)
    {
        unsigned int offset = ss->ptr - ss->buffer;
        int remaining = ss->buffer_len - offset;
        if (remaining <= num_bytes)
        {
            /* grow the buffer */
            int needed_to_grow = num_bytes - remaining;
            int i = 1;
            int amount_to_grow = ss->buffer_grow_size;
            while (amount_to_grow < needed_to_grow)
                amount_to_grow += ss->buffer_grow_size;

            ss->max_buffer_size += amount_to_grow;
            ss->buffer = realloc(ss->buffer, ss->max_buffer_size);
            ss->ptr = ss->buffer+offset; /* reset ptr to position in buffer */
        }

        memcpy(ss->ptr, buffer, num_bytes);
        ss->ptr += num_bytes;
        ss->buffer_len += num_bytes;
        ret = num_bytes;
    }

    return ret;
}

static void
rq_stream_string_free(void *d)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (ss->is_open)
        rq_stream_string_close(d);
   RQ_FREE(ss);
}

static long 
rq_stream_string_tell(void *d)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (!ss->is_open)
        return -1;
    return (long)(ss->ptr - ss->buffer);
}

static rq_error_code
rq_stream_string_seek(void *d, long offset)
{
    char *new_ptr;
    struct rq_stream_string *ss = (struct rq_stream_string *)d;
    if (!ss->is_open)
        return RQ_FAILED;

    new_ptr = ss->buffer + offset;

    if (new_ptr > ss->buffer + ss->buffer_len)
        return RQ_FAILED;

    ss->ptr = new_ptr;

    return RQ_OK;
}

static void
rq_stream_string_rewind(void *d)
{
    struct rq_stream_string *ss = (struct rq_stream_string *)d;

    if (ss->is_open)
        ss->ptr = ss->buffer;
}

static struct rq_stream_callbacks stream_string_callbacks = {
    rq_stream_string_open,
    rq_stream_string_is_open,
    rq_stream_string_at_end,
    rq_stream_string_close,
    rq_stream_string_read,
    rq_stream_string_write,
    rq_stream_string_tell,
    rq_stream_string_seek,
    rq_stream_string_rewind,
    rq_stream_string_free
};

RQ_EXPORT rq_stream_t
rq_stream_string_alloc()
{
    struct rq_stream_string *ss = 
        (struct rq_stream_string *)RQ_CALLOC(1, sizeof(struct rq_stream_string));
    return _rq_stream_alloc(
        rq_stream_string_stream_type,
        ss,
        &stream_string_callbacks
        );
}

