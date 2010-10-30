/**
 * \file rq_stream.h
 * \author Brett Hutley
 *
 * \brief The rq_stream object provides an interface for working with
 * different kinds of streams (like file streams, or socket streams).
 */
/*
** rq_stream.h
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
#ifndef rq_stream_h
#define rq_stream_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- prototypes --------------------------------------------------- */
typedef struct rq_stream {
    const char *stream_type;
    void *data;
    int (*open_func)(void *);
    short (*is_open_func)(void *);
    short (*at_end_func)(void *);
    void (*close_func)(void *);
    int (*read_func)(void *, char *, int);
    int (*write_func)(void *, const char *, int);
    long (*tell_func)(void *);
    rq_error_code (*seek_func)(void *, long);
    void (*rewind_func)(void *);
    void (*free_func)(void *);
} *rq_stream_t;

struct rq_stream_callbacks {
    int (*open_func)(void *);
    short (*is_open_func)(void *);
    short (*at_end_func)(void *);
    void (*close_func)(void *);
    int (*read_func)(void *, char *, int);
    int (*write_func)(void *, const char *, int);
    long (*tell_func)(void *);
    rq_error_code (*seek_func)(void *, long);
    void (*rewind_func)(void *);
    void (*free_func)(void *);
};

/** Test whether the rq_stream is NULL */
RQ_EXPORT int rq_stream_is_null(rq_stream_t obj);

/**
 * This function should only be called by stream implementations
 */
RQ_EXPORT rq_stream_t _rq_stream_alloc(const char *stream_type, void *data, struct rq_stream_callbacks *callbacks);

RQ_EXPORT void rq_stream_free(rq_stream_t stream);


/** Open a stream.
 *
 * @param	stream	the stream to open
 * @return	RQ_OK if successful. Otherwise an error code
 */
RQ_EXPORT int rq_stream_open(rq_stream_t stream);

/**
 * Determine if a stream is open
 *
 * @param	stream	the stream to test
 * @return	non-zero if the stream is open.
 */
RQ_EXPORT short rq_stream_is_open(rq_stream_t stream);

/**
 * Determine if you are at the end of the stream
 *
 * @param	stream	the stream to test
 * @return	non-zero if the at end of stream
 */
RQ_EXPORT short rq_stream_at_end(rq_stream_t stream);

/**
 * Close a stream
 *
 * @param	stream	the stream to close
 * @return	RQ_OK if successful. Otherwise an error code
 */
RQ_EXPORT void rq_stream_close(rq_stream_t stream);

/**
 * Read bytes from a stream
 *
 * @param	stream	the stream to read from
 * @param	buf	the buffer to read the data into
 * @param	num_bytes	The number of bytes to read
 * @return	number of bytes actually read or negative status if failed
 */
RQ_EXPORT int rq_stream_read(rq_stream_t stream, char *buf, int num_bytes);

/**
 * Read a line of text from the stream.
 */
RQ_EXPORT int rq_stream_read_line(rq_stream_t stream, char *buf, unsigned maxbuflen);


/** Write bytes to a stream
 *
 * @param	stream	the stream to write to
 * @param	buf	pointer to the byte array to write to the stream
 * @param	num_bytes	the number of bytes to write to the stream
 * @return	number of bytes actually written or negative status on error
 */
RQ_EXPORT int rq_stream_write(rq_stream_t stream, const char *buf, int num_bytes);

RQ_EXPORT int rq_stream_write_string(rq_stream_t stream, const char *buf);

RQ_EXPORT int rq_stream_write_integer_as_string(rq_stream_t stream, int i);

RQ_EXPORT int rq_stream_write_double_as_string(rq_stream_t stream, double d, int dp);

RQ_EXPORT int rq_stream_printf(rq_stream_t stream, const char *format, ...);

/** Rewind the stream. */
RQ_EXPORT void rq_stream_rewind(rq_stream_t stream);

/** Get the current position of the stream. */
RQ_EXPORT long rq_stream_tell(rq_stream_t stream);

/** Get the current position of the stream. */
RQ_EXPORT rq_error_code rq_stream_seek(rq_stream_t stream, long offset);

/**
 * Get the data pointer from the stream. Should only be called by a stream 
 * implementation.
 *
 * @param	stream	the stream
 * @return	the implementation data pointer contained by the stream
 */
RQ_EXPORT void *_rq_stream_get_data(rq_stream_t stream);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
