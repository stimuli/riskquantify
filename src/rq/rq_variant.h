/**
 * @file
 *
 * This file defines a variant type
 */
/*
** rq_variant.h
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
#ifndef rq_variant_h
#define rq_variant_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

enum rq_variant_type {
    RQ_VARIANT_TYPE_NIL,
    RQ_VARIANT_TYPE_STRING,
    RQ_VARIANT_TYPE_DOUBLE,
    RQ_VARIANT_TYPE_INTEGER,
    RQ_VARIANT_TYPE_DATE,
    RQ_VARIANT_TYPE_OBJECT
};

struct rq_variant_object {
    unsigned long obj_type;
    void *obj_ptr;
};

/** The variant structure.
 */
typedef struct rq_variant {
    enum rq_variant_type type;

    union {
        const char *str;
        double d;
        long i;
        rq_date date;

        struct rq_variant_object object;
    } value;
} *rq_variant_t;

/** Test whether the rq_variant is NULL */
RQ_EXPORT int rq_variant_is_null(rq_variant_t obj);

/** Allocate a new NIL variant.
 */
RQ_EXPORT rq_variant_t rq_variant_alloc();

/** Allocate a new string variant.
 */
RQ_EXPORT rq_variant_t rq_variant_alloc_string(const char *str);

/** Allocate a new double variant.
 */
RQ_EXPORT rq_variant_t rq_variant_alloc_double(double d);

/** Allocate a new integer variant.
 */
RQ_EXPORT rq_variant_t rq_variant_alloc_integer(long i);

/** Allocate a new date variant.
 */
RQ_EXPORT rq_variant_t rq_variant_alloc_date(rq_date dt);

/** Allocate a new object variant.
 */
RQ_EXPORT rq_variant_t rq_variant_alloc_object(unsigned long obj_type, void *obj_ptr);

/** Free an object of type rq_variant.
 */
RQ_EXPORT void rq_variant_free(rq_variant_t variant);

/** Initialize a NIL variant.
 */
RQ_EXPORT void rq_variant_init_nil(struct rq_variant *var);

/** Initialize a string variant.
 */
RQ_EXPORT void rq_variant_init_string(struct rq_variant *var, const char *str);

/** Initialize a double variant.
 */
RQ_EXPORT void rq_variant_init_double(struct rq_variant *var, double d);

/** Initialize an integer variant.
 */
RQ_EXPORT void rq_variant_init_integer(struct rq_variant *var, long i);

/** Initialize a date variant.
 */
RQ_EXPORT void rq_variant_init_date(struct rq_variant *var, rq_date dt);

/** Initialize a pointer variant.
 */
RQ_EXPORT void rq_variant_init_object(struct rq_variant *var, unsigned long obj_type, void *obj_ptr);

/** Get the type of a variant.
 */
RQ_EXPORT enum rq_variant_type rq_variant_get_type(rq_variant_t variant);

/** Get the string value from a variant.
 */
RQ_EXPORT const char *rq_variant_get_string(rq_variant_t variant);

/** Get the double value from a variant.
 */
RQ_EXPORT double rq_variant_get_double(rq_variant_t variant);

/** Get the integer value from a variant.
 */
RQ_EXPORT long rq_variant_get_integer(rq_variant_t variant);

/** Get the date value from a variant.
 */
RQ_EXPORT rq_date rq_variant_get_date(rq_variant_t variant);

/** Get the object's pointer value from a variant.
 */
RQ_EXPORT const void *rq_variant_get_object_ptr(rq_variant_t variant);

/** Get the object's type value from a variant.
 */
RQ_EXPORT unsigned long rq_variant_get_object_type(rq_variant_t variant);

/** Return a variant as a double.
 */
RQ_EXPORT double rq_variant_get_as_double(rq_variant_t variant);

/** Return a variant as an integer.
 */
RQ_EXPORT long rq_variant_get_as_integer(rq_variant_t variant);

/** Return a variant as a date.
 */
RQ_EXPORT rq_date rq_variant_get_as_date(rq_variant_t variant);

/** Coerce a variant to a string type.
 * This changes the type of the variant to a string, and then returns
 * the string.
 */
RQ_EXPORT const char *rq_variant_coerce_to_string(rq_variant_t variant);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
