/**
 * \file rq_object_schema_node.h
 * \author Brett Hutley
 *
 * \brief A object schema node stores the representation of a member
 * or property of a Risk Quantify type.
 */
/*
** rq_object_schema_node.h
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
#ifndef rq_object_schema_node_h
#define rq_object_schema_node_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the object schema node */
typedef struct rq_object_schema_node {
    const char *property_name;
    const char *type_name;
} *rq_object_schema_node_t;

/** Test whether the rq_object_schema_node is NULL */
RQ_EXPORT int rq_object_schema_node_is_null(rq_object_schema_node_t obj);

/**
 * Allocate a schema node
 *
 * @param property_name the name of the element or attribute
 * @param type_name the name of the type
 */
RQ_EXPORT rq_object_schema_node_t rq_object_schema_node_alloc(
    const char *property_name, 
    const char *type_name
    );

/**
 * Free the allocated schema node
 */
RQ_EXPORT void rq_object_schema_node_free(rq_object_schema_node_t node);

/**
 * Get the type name for this node
 */
RQ_EXPORT const char *rq_object_schema_node_get_type_name(rq_object_schema_node_t schema_node);

/**
 * Get the property name for this node
 */
RQ_EXPORT const char *rq_object_schema_node_get_property_name(rq_object_schema_node_t schema_node);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
