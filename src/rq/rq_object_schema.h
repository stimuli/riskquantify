/**
 * \file rq_object_schema.h
 * \author Brett Hutley
 *
 * \brief A object schema tree stores the representation of Risk
 * Quantify objects so that they can be reconstituted and validated.
 */
/*
** rq_object_schema.h
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
#ifndef rq_object_schema_h
#define rq_object_schema_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_linked_list.h"
#include "rq_object_schema_node.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the object schema */
typedef struct rq_object_schema {
    const char *type_name;

    rq_tree_rb_t properties;

    void *(*constructor)();
    void (*destructor)(void *);
    void *(*cloner)(void *);

    void (*set_value_string)(void *, const char *, const char *);
    void (*set_value_integer)(void *, const char *, long);
    void (*set_value_double)(void *, const char *, double);
    void (*set_value_date)(void *, const char *, rq_date);
    void (*set_value_object)(void *, const char *, void *);
} *rq_object_schema_t;

/** Test whether the rq_object_schema is NULL */
RQ_EXPORT int rq_object_schema_is_null(rq_object_schema_t obj);

/**
 * Allocate an schema
 *
 * @param type_name The type name of the schema
 */
RQ_EXPORT rq_object_schema_t rq_object_schema_alloc(
    const char *type_name,
    void *(*constructor)(),
    void (*destructor)(void *),
    void *(*cloner)(void *),
    void (*set_value_string)(void *, const char *, const char *),
    void (*set_value_integer)(void *, const char *, long),
    void (*set_value_double)(void *, const char *, double),
    void (*set_value_date)(void *, const char *, rq_date),
    void (*set_value_object)(void *, const char *, void *)
    );

/**
 * Free the allocated schema
 */
RQ_EXPORT void rq_object_schema_free(rq_object_schema_t schema);

/**
 * Get the type name of the schema
 */
RQ_EXPORT const char *rq_object_schema_get_type_name(rq_object_schema_t schema);

/**
 * Get the properties of this type
 */
RQ_EXPORT rq_linked_list_t rq_object_schema_get_properties(rq_object_schema_t schema);

/**
 * Add a child element to the list of properties for this schema
 */
RQ_EXPORT void rq_object_schema_add_property(rq_object_schema_t schema, rq_object_schema_node_t property);

/**
 * Find a property by name
 */
RQ_EXPORT rq_object_schema_node_t 
rq_object_schema_find_property(
    rq_object_schema_t schema,
    const char *property_name
    );

/**
 * Construct a new object of this type
 */
RQ_EXPORT void *rq_object_schema_construct_object(rq_object_schema_t schema);

/**
 * Destroy an object of this type
 */
RQ_EXPORT void rq_object_schema_destroy_object(rq_object_schema_t schema, void *object);

/**
 * Clone this object
 */
RQ_EXPORT void *rq_object_schema_clone_object(rq_object_schema_t schema, void *object);

/**
 * Set the string value of a property
 */
RQ_EXPORT void rq_object_schema_set_value_string(rq_object_schema_t schema, void *object, const char *property_name, const char *value);

/**
 * Set the integer value of a property
 */
RQ_EXPORT void rq_object_schema_set_value_integer(rq_object_schema_t schema, void *object, const char *property_name, long value);

/**
 * Set the double value of a property
 */
RQ_EXPORT void rq_object_schema_set_value_double(rq_object_schema_t schema, void *object, const char *property_name, double value);

/**
 * Set the date value of a property
 */
RQ_EXPORT void rq_object_schema_set_value_date(rq_object_schema_t schema, void *object, const char *property_name, rq_date value);

/**
 * Set the object value of a property
 */
RQ_EXPORT void rq_object_schema_set_value_object(rq_object_schema_t schema, void *object, const char *property_name, void *value);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
