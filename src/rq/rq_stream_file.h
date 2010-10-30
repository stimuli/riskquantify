/**
 * \file rq_stream_file.h
 * \author Brett Hutley
 *
 * \brief The rq_stream_file object specializes the rq_stream object
 * to work with files.
 */
/*
** rq_stream_file.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002-2008 Brett Hutley
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
#ifndef rq_stream_file_h
#define rq_stream_file_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_stream.h"
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#define RQ_STREAM_FILE_OPENMODE_READING 1
#define RQ_STREAM_FILE_OPENMODE_WRITING 2
#define RQ_STREAM_FILE_OPENMODE_READINGWRITINGCREATE 3

/* -- structs ----------------------------------------------------- */
struct rq_stream_file {
    const char *filename;
    FILE *fh;
	int open_mode;
};

/* -- globals ------------------------------------------------------ */
extern const char *rq_stream_file_stream_type;

/* -- prototypes --------------------------------------------------- */
/**
 * Allocate a new file stream
 */
RQ_EXPORT rq_stream_t rq_stream_file_alloc();

/**
 * Allocate and open a new file stream
 */
RQ_EXPORT rq_stream_t rq_stream_file_open(const char *filename, const char *openmode);

/** Get the filename associated with this stream
 */
RQ_EXPORT const char *rq_stream_file_get_filename(rq_stream_t stream);

/** Set the filename associated with this stream
 */
RQ_EXPORT void rq_stream_file_set_filename(rq_stream_t stream, const char *filename);

/** Get the open mode associated with this stream
 */
RQ_EXPORT int rq_stream_file_get_openmode(rq_stream_t stream);

/** Set the open mode associated with this stream
 */
RQ_EXPORT void rq_stream_file_set_openmode(rq_stream_t stream, int open_mode);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
