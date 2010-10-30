/**
 * \file rq_rate_class.h
 * \author Brett Hutley
 *
 * \brief This structure defines the characteristics of the rate
 */
/*
** rq_rate_class.h
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
#ifndef rq_rate_class_h
#define rq_rate_class_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private rate_class structure.
 */
typedef struct rq_rate_class {
    const char *id;
    const char *type_id;
    void *type_data;
    void (*free_func)(void *);
} *rq_rate_class_t;

/** Test whether the rq_rate_class is NULL */
RQ_EXPORT int rq_rate_class_is_null(rq_rate_class_t obj);

/**
 * Allocate a new rate_class. This should only be called by
 * a derived rate_class type.
 */
RQ_EXPORT rq_rate_class_t _rq_rate_class_alloc(const char *id, const char *type_id, void *type_data, void (*free_func)(void *));

/**
 * Free an object of type rq_rate_class
 */
RQ_EXPORT void rq_rate_class_free(rq_rate_class_t rate_class);

/**
 * Get the rate class ID
 */
RQ_EXPORT const char *rq_rate_class_get_id(rq_rate_class_t rate_class);

/**
 * Get the rate class type ID
 */
RQ_EXPORT const char *rq_rate_class_get_type_id(rq_rate_class_t rate_class);

/**
 * Get the rate class type data
 */
RQ_EXPORT void *rq_rate_class_get_type_data(rq_rate_class_t rate_class);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

