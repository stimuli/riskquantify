/**
 * \file rq_vector.h
 * \author Brett Hutley
 *
 * \brief rq_vector implements a mathematical vector of doubles.
 */
/*
** rq_vector.h
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
#ifndef rq_vector_h
#define rq_vector_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_iterator.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#define RQ_VECTOR_DATA_SIZE 100

#define RQ_VECTOR_BLOCK_SIZE 5

typedef struct rq_vector_data {
    double *vector; /**< The array of doubles */
} * rq_vector_data_t;

typedef struct rq_vector_block {
    struct rq_vector_block *next;
    struct rq_vector_data **vector_data;
} * rq_vector_block_t;

typedef struct rq_vector {
    struct rq_vector_block *head;
    struct rq_vector_block *tail;

    unsigned block_count; /**< The current number of blocks. */
    unsigned block_size; /**< The number of rq_vector_data pointers in each block. */
    unsigned vector_data_size; /**< The size of each vector data array. */
    unsigned size; /**< The current size of the vector. */
} *rq_vector_t;


/** Determine if an rq_vector is NULL - if the vector hasn't been
 * allocated.
 */
RQ_EXPORT int rq_vector_is_null(rq_vector_t obj);

/** Allocate an object which initially has a NIL type.
 */
RQ_EXPORT rq_vector_t rq_vector_alloc();

/** Free the allocated vector.
 */
RQ_EXPORT void rq_vector_free(rq_vector_t vector);

/** Get the size of the vector.
 */
RQ_EXPORT unsigned long rq_vector_size(rq_vector_t vector);

/** Get the value at a particular offset.
 */
RQ_EXPORT double rq_vector_get(rq_vector_t vector, unsigned long offset);

/** Set the value at a particular offset.
 */
RQ_EXPORT void rq_vector_set(rq_vector_t vector, unsigned long offset, double val);

/** Get a generic iterator for a vector.
 */
RQ_EXPORT rq_iterator_t rq_vector_get_iterator(rq_vector_t vector);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
