/**
 * \file rq_stream_string.h
 * \author Brett Hutley
 *
 * \brief The rq_stream_string object specializes the rq_stream object
 * to work with strings.
 */
/*
** rq_stream_string.h
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
#ifndef rq_stream_string_h
#define rq_stream_string_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_stream.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- structs ----------------------------------------------------- */
struct rq_stream_string {
    char *buffer;
    unsigned int max_buffer_size;
    unsigned int buffer_grow_size;
    unsigned int buffer_len;
    char *ptr;
    short is_open;
};

/* -- globals ------------------------------------------------------ */
extern const char *rq_stream_string_stream_type;

/* -- prototypes --------------------------------------------------- */
RQ_EXPORT rq_stream_t rq_stream_string_alloc();

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
