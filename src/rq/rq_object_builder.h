/**
 * \file rq_object_builder.h
 * \author Brett Hutley
 *
 * \brief A object builder acts as a kind of base class for building
 * objects
 */
/*
** rq_object_builder.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#ifndef rq_object_builder_h
#define rq_object_builder_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_stream.h"
#include "rq_object_schema_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the object builder */
typedef struct rq_object_builder {
    const char *type_name;

    void *derived_data;
    void (*free_func)(void *);
    void * (*build_func)(void *, rq_stream_t, rq_object_schema_mgr_t, const char *, const char *);
} *rq_object_builder_t;

/** Test whether the rq_object_builder is NULL */
RQ_EXPORT int rq_object_builder_is_null(rq_object_builder_t obj);

/**
 * Allocate an object builder. This should only be called
 * by a "derived" class.
 *
 */
RQ_EXPORT rq_object_builder_t _rq_object_builder_alloc(
    const char *type_name,
    void *derived_data,
    void (*free_func)(void *),
    void * (*build_func)(void *, rq_stream_t, rq_object_schema_mgr_t, const char *, const char *)
    );

/**
 * Free the allocated builder
 */
RQ_EXPORT void rq_object_builder_free(rq_object_builder_t builder);

/**
 * Get the builder type name
 */
RQ_EXPORT const char *rq_object_builder_get_type_name(rq_object_builder_t builder);

/**
 * Build an object from the stream
 */
RQ_EXPORT void *
rq_object_builder_build(
    rq_object_builder_t builder, 
    rq_stream_t stream, 
    rq_object_schema_mgr_t schema_mgr, 
    const char *path,
    const char *type_name
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
