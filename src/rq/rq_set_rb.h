/**
 * @file
 *
 * This file implements a set using a red-black tree.
 */
/*
** rq_set_rb.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2004-2008 Brett Hutley
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
#ifndef rq_set_rb_h
#define rq_set_rb_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_set_rb_node {
    unsigned short is_red;
    struct rq_set_rb_node *left;
    struct rq_set_rb_node *right;
    const void *key;
};

/** The set_rb structure.
 */
typedef struct rq_set_rb {
    struct rq_set_rb_node *head;
    void (*free_func)(void *);
    int (*cmp_func)(const void *, const void *);
    unsigned depth;
} *rq_set_rb_t;


/**
 * Allocate a new red-black set. Pass in the free callback function
 * and the key comparison callback function as parameters.
 */
RQ_EXPORT rq_set_rb_t rq_set_rb_alloc(void (*free_func)(void *), int (*cmp_func)(const void *, const void *));

/**
 * Clone the red-black set. The data is copied using the copy function
 * callback parameter. The key function parameter is used to get the
 * key out of the cloned data.
 */
RQ_EXPORT rq_set_rb_t rq_set_rb_clone(rq_set_rb_t set, void *(*cpy_func)(const void *));

/**
 * Copy the red-black set into another set. The data is copied using
 * the copy function callback parameter. The key function parameter is
 * used to get the key out of the cloned data.  
*/
RQ_EXPORT void rq_set_rb_copy(rq_set_rb_t set_dst, rq_set_rb_t set_src, void *(*cpy_func)(const void *));

/**
 * Free the red-black set
 */
RQ_EXPORT void rq_set_rb_free(rq_set_rb_t set);

/**
 * Clear the set, freeing all the nodes.
 */
RQ_EXPORT void rq_set_rb_clear(rq_set_rb_t set);

/** Test whether the rq_set_rb is NULL */
RQ_EXPORT int rq_set_rb_is_null(rq_set_rb_t obj);

/**
 * Find the data in the set associated with a certain key.
 */
RQ_EXPORT const void *rq_set_rb_find(rq_set_rb_t set, const void *key);


/**
 * Add the data to the set, using the key parameter as the key.
 */
RQ_EXPORT void rq_set_rb_add(rq_set_rb_t set, const void *key);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
