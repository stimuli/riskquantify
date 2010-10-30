/**
 * \file rq_object_schema_mgr.h
 * \author Brett Hutley
 *
 * \brief A object schema mgr stores the representation of Risk
 * Quantify objects so that they can be reconstituted and validated.
 */
/*
** rq_object_schema_mgr.h
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
#ifndef rq_object_schema_mgr_h
#define rq_object_schema_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_object_schema.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the object schema mgr */
typedef struct rq_object_schema_mgr {
    rq_tree_rb_t tree;
} *rq_object_schema_mgr_t;

/** Test whether the rq_object_schema_mgr is NULL */
RQ_EXPORT int rq_object_schema_mgr_is_null(rq_object_schema_mgr_t obj);

/**
 * Allocate an schema mgr
 */
RQ_EXPORT rq_object_schema_mgr_t rq_object_schema_mgr_alloc();

/**
 * Free the allocated schema mgr
 */
RQ_EXPORT void rq_object_schema_mgr_free(rq_object_schema_mgr_t schema_mgr);

/**
 * Add a schema to the schema manager
 */
RQ_EXPORT void rq_object_schema_mgr_add(rq_object_schema_mgr_t mgr, rq_object_schema_t os);

/**
 * Find a schema within the schema manager
 */
RQ_EXPORT rq_object_schema_t rq_object_schema_mgr_find(rq_object_schema_mgr_t schema_mgr, const char *object_type);

/**
 * Initialize all the object schemas in Risk Quantify
 */
RQ_EXPORT void rq_object_schema_mgr_init_object_schemas(rq_object_schema_mgr_t schema_mgr);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
