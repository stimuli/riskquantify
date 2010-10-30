/**
 * \file rq_array.h
 * \author Brett Hutley
 *
 * \brief rq_array is a container class that implements an array type.
 */
/*
** rq_array.h
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
#ifndef rq_array_h
#define rq_array_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_iterator.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif


typedef struct rq_array {
    void **array; /**< The array itself */
    unsigned max; /**< The maximum number of pointers */
    unsigned size; /**< The # pts in the array */
    unsigned grow_size; /**< The amount to grow the array by */

    void (*free_func)(void *);
} *rq_array_t;

/** Determine if an rq_array is NULL - if the array hasn't been
 * allocated.
 */
RQ_EXPORT int rq_array_is_null(rq_array_t obj);

/** Allocate an object which initially has a NIL type.
 */
RQ_EXPORT rq_array_t rq_array_alloc(void (*free_func)(void *));

/** Free the allocated array.
 */
RQ_EXPORT void rq_array_free(rq_array_t array);

/** Clear a array, freeing the contained objects.
 */
RQ_EXPORT void rq_array_clear(rq_array_t array);

/** Get the number of objects managed by the array.
 */
RQ_EXPORT unsigned int  rq_array_size(rq_array_t array);

/** Get the object at a particular offset.
 */
RQ_EXPORT void *rq_array_get_at(rq_array_t array, unsigned int offset);

/** Add an object to the end of the array.
 */
RQ_EXPORT void rq_array_push_back(rq_array_t array, void *obj);

/** Insert an object at the specified offset in the array.
 */
RQ_EXPORT void rq_array_insert_at(rq_array_t array, unsigned long offset, void *obj);

/** Remove an object at the specified offset in the array.
 *
 * @return the pointer to the object.
 */
RQ_EXPORT void *rq_array_remove_at(rq_array_t array, unsigned long offset);

/** Get a generic iterator for this array.
 */
RQ_EXPORT rq_iterator_t rq_array_get_iterator(rq_array_t array);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
