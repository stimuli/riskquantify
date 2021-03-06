/**
 * \file rq_array_partitioned.h
 * \author Brett Hutley
 *
 * \brief rq_array_partitioned is a container class that implements an
 * array type. The array is partitioned so that the array can grow in
 * blocks without requiring a memory reallocation.
 */
/*
** rq_array_partitioned.h
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
#ifndef rq_array_partitioned_h
#define rq_array_partitioned_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- defines ----------------------------------------------------- */
#define RQ_DEFAULT_ARRAY_GROW_SIZE 20
#define RQ_DEFAULT_ARRAY_BLOCK_SIZE 10

/* -- structs ----------------------------------------------------- */
typedef struct rq_array_partitioned_node {
    /* unsigned size; */ /**< The current number of object in this array */
    void **array; /**< This node's array */
} * rq_array_partitioned_node_t;

typedef struct rq_array_partitioned_node_block {
    struct rq_array_partitioned_node_block *next;
    struct rq_array_partitioned_node **array_nodes;
} * rq_array_partitioned_node_block_t;

typedef struct rq_array_partitioned {
    struct rq_array_partitioned_node_block *head;
    struct rq_array_partitioned_node_block *tail;

    unsigned block_count; /**< The current number of blocks. */
    unsigned block_size; /**< The number of rq_array_partitioned_node pointers in each block. */
    unsigned objs_per_node; /**< The number of objects per node. */
    unsigned size; /**< The current number of objects in the array. */

    void (*free_func)(void *);
} *rq_array_partitioned_t;

/** Determine if an rq_array_partitioned is NULL - if the array hasn't been
 * allocated.
 */
RQ_EXPORT int rq_array_partitioned_is_null(rq_array_partitioned_t obj);

/** Allocate an object which initially has a NIL type.
 */
RQ_EXPORT rq_array_partitioned_t rq_array_partitioned_alloc(void (*free_func)(void *));

/** Free the allocated array.
 */
RQ_EXPORT void rq_array_partitioned_free(rq_array_partitioned_t array);

/** Clear a array, freeing the contained objects.
 */
RQ_EXPORT void rq_array_partitioned_clear(rq_array_partitioned_t array);

/** Get the number of objects managed by the array.
 */
RQ_EXPORT unsigned int  rq_array_partitioned_size(rq_array_partitioned_t array);

/** Get the object at a particular offset.
 */
RQ_EXPORT void *rq_array_partitioned_get_at(rq_array_partitioned_t array, unsigned int offset);

/** Add an object to the end of the array.
 */
RQ_EXPORT void rq_array_partitioned_push_back(rq_array_partitioned_t array, void *obj);

/** Insert an object at the specified offset in the array.
 */
RQ_EXPORT void rq_array_partitioned_insert_at(rq_array_partitioned_t array, unsigned long offset, void *obj);

/** Remove an object at the specified offset in the array.
 *
 * @return the pointer to the object.
 */
RQ_EXPORT void *rq_array_partitioned_remove_at(rq_array_partitioned_t array, unsigned long offset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
