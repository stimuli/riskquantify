/*
** rq_stream.c
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
#include "rq_stream.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>


RQ_EXPORT rq_stream_t 
_rq_stream_alloc(const char *stream_type, void *data, struct rq_stream_callbacks *callbacks)
{
    struct rq_stream *s = (struct rq_stream *)RQ_CALLOC(1, sizeof(struct rq_stream));
    s->stream_type = RQ_STRDUP(stream_type);
    s->data = data;
    s->open_func = callbacks->open_func;
    s->is_open_func = callbacks->is_open_func;
    s->at_end_func = callbacks->at_end_func;
    s->close_func = callbacks->close_func;
    s->read_func = callbacks->read_func;
    s->write_func = callbacks->write_func;
    s->tell_func = callbacks->tell_func;
    s->seek_func = callbacks->seek_func;
    s->rewind_func = callbacks->rewind_func;
    s->free_func = callbacks->free_func;

    return s;
}

RQ_EXPORT void 
rq_stream_free(rq_stream_t s)
{
    if (s->data && s->free_func)
        (*s->free_func)(s->data);
   RQ_FREE((char *)s->stream_type);
   RQ_FREE(s);
}

RQ_EXPORT int 
rq_stream_open(rq_stream_t s)
{
    return (*s->open_func)(s->data);
}

RQ_EXPORT short 
rq_stream_is_open(rq_stream_t s)
{
    return (*s->is_open_func)(s->data);
}

RQ_EXPORT short 
rq_stream_at_end(rq_stream_t s)
{
    return (*s->at_end_func)(s->data);
}

RQ_EXPORT void 
rq_stream_close(rq_stream_t s)
{
    (*s->close_func)(s->data);
}

RQ_EXPORT int 
rq_stream_read(rq_stream_t s, char *buf, int num_bytes)
{
    return (*s->read_func)(s->data, buf, num_bytes);
}

RQ_EXPORT int 
rq_stream_read_line(rq_stream_t s, char *buf, unsigned maxbuflen)
{
    unsigned int i;
    int ret = -1;
    char *p = buf;

    for (i = 0; i < maxbuflen - 1; )
    {
        ret = (*s->read_func)(s->data, p, 1);
        if (ret == 1)
        {
            i++;
            if (*p == '\n')
                break;
            p++;
        }
        else if (ret < 0)
            return ret;
        else if (ret == 0)
            break;
    }

    buf[i] = '\0';

    return i;
}

RQ_EXPORT int 
rq_stream_write(rq_stream_t s, const char *buf, int num_bytes)
{
    return (*s->write_func)(s->data, buf, num_bytes);
}

RQ_EXPORT int 
rq_stream_write_string(rq_stream_t stream, const char *buf)
{
	if (buf)
		return rq_stream_write(stream, buf, strlen(buf));
	else
		return rq_stream_write(stream, "(null)", 6);
}

RQ_EXPORT int 
rq_stream_write_integer_as_string(rq_stream_t stream, int i)
{
    char buf[50];
    sprintf(buf, "%d", i);
    return rq_stream_write_string(stream, buf);
}

RQ_EXPORT int 
rq_stream_write_double_as_string(rq_stream_t stream, double d, int dp)
{
    char buf[150];
    sprintf(buf, "%.*f", dp, d);
    return rq_stream_write_string(stream, buf);
}

RQ_EXPORT int 
rq_stream_printf(rq_stream_t stream, const char *format, ...)
{
    va_list ap;
    int ret;
    char *buf;

    va_start(ap, format);

    ret = vasprintf(&buf, format, ap);
    rq_stream_write_string(stream, buf);
    free(buf);

    va_end(ap);

    return ret;
}

RQ_EXPORT long 
rq_stream_tell(rq_stream_t stream)
{
    return (*stream->tell_func)(stream->data);
}

/** Get the current position of the stream. */
RQ_EXPORT rq_error_code 
rq_stream_seek(rq_stream_t stream, long offset)
{
    return (*stream->seek_func)(stream->data, offset);
}

RQ_EXPORT void 
rq_stream_rewind(rq_stream_t s)
{
    if (s->rewind_func)
        (*s->rewind_func)(s->data);
}

RQ_EXPORT void *
_rq_stream_get_data(rq_stream_t s)
{
    return s->data;
}

RQ_EXPORT int
rq_stream_is_null(rq_stream_t obj)
{
    return (obj == NULL);
}

