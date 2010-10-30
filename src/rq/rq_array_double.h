/**
 * \file rq_array_double.h
 * \author Brett Hutley
 *
 * \brief rq_array_double is a container class that implements an
 * array of doubles.
 */
/*
** rq_array_double.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#ifndef rq_array_double_h
#define rq_array_double_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif


typedef struct rq_array_double {
    double *array; /**< The array itself */
    unsigned max; /**< The maximum number of doubles */
    unsigned size; /**< The current # doubles in the array */
    unsigned grow_size; /**< The amount to grow the array by */
} *rq_array_double_t;

/** Determine if an rq_array_double is NULL - if the array hasn't been
 * allocated.
 */
RQ_EXPORT int rq_array_double_is_null(rq_array_double_t obj);

/** Allocate an rq_array_double object.
 */
RQ_EXPORT rq_array_double_t rq_array_double_alloc();

/** Allocate an rq_array_double object and specify the initial size.
 */
RQ_EXPORT rq_array_double_t rq_array_double_alloc_with_size(unsigned max_size);

/** Free the allocated array.
 */
RQ_EXPORT void rq_array_double_free(rq_array_double_t array);

/** Clear a array, freeing the contained objects.
 */
RQ_EXPORT void rq_array_double_clear(rq_array_double_t array);

/** Get the number of objects managed by the array.
 */
RQ_EXPORT unsigned int  rq_array_double_size(rq_array_double_t array);

/** Get the object at a particular offset.
 */
RQ_EXPORT double rq_array_double_get_at(rq_array_double_t array, unsigned int offset);

/** Add a number to the end of the array.
 */
RQ_EXPORT void rq_array_double_push_back(rq_array_double_t array, double num);

/** Insert a number at the specified offset in the array.
 */
RQ_EXPORT void rq_array_double_insert_at(rq_array_double_t array, unsigned long offset, double num);

/** Set a number at the specified offset in the array.
 *
 * This function will overwrite any existing number at this position.
 */
RQ_EXPORT void rq_array_double_set_at(rq_array_double_t array, unsigned long offset, double num);

/** Remove an object at the specified offset in the array.
 *
 * @return the pointer to the object.
 */
RQ_EXPORT double rq_array_double_remove_at(rq_array_double_t array, unsigned long offset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
