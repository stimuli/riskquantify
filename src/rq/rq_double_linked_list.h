/**
 * \file rq_double_linked_list.h
 * \author Brett Hutley
 *
 * \brief The rq_double_linked_list files provide double linked list
 * management routines.
 */
/*
** rq_double_linked_list.h
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
#ifndef rq_double_linked_list_h
#define rq_double_linked_list_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_iterator.h"

/* -- typedefs ---------------------------------------------------- */
struct rq_double_linked_list_node {
    struct rq_double_linked_list_node *next;
    struct rq_double_linked_list_node *prev;
    void *data;
};

typedef struct rq_double_linked_list {
    struct rq_double_linked_list_node *head;
    struct rq_double_linked_list_node *tail;
    unsigned long count;
    void (*free_func)(void *);
} *rq_double_linked_list_t;

typedef struct rq_double_linked_list_node *rq_double_linked_list_iterator_t;


/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_double_linked_list is NULL */
RQ_EXPORT int rq_double_linked_list_is_null(rq_double_linked_list_t obj);

/** Allocate a new double linked list.
 */
RQ_EXPORT rq_double_linked_list_t rq_double_linked_list_alloc(void (*free_func)(void *));

/** Free the allocated double linked list.
 */
RQ_EXPORT void rq_double_linked_list_free(rq_double_linked_list_t linked_list);

/** Return the number of elements managed by the double linked list.
 */
RQ_EXPORT unsigned rq_double_linked_list_size(rq_double_linked_list_t linked_list);

/** Get a generic iterator for the double linked list.
 */
RQ_EXPORT rq_iterator_t rq_double_linked_list_get_iterator(rq_double_linked_list_t linked_list);

/** Return an iterator from the start of the list.
 */
RQ_EXPORT rq_double_linked_list_iterator_t rq_double_linked_list_begin(rq_double_linked_list_t linked_list);

/** Test the iterator to see if we have moved past the end of the list.
 */
RQ_EXPORT short rq_double_linked_list_iterator_at_end(rq_double_linked_list_iterator_t iter);

/** Return an iterator from the end of the list.
 */
RQ_EXPORT rq_double_linked_list_iterator_t rq_double_linked_list_rbegin(rq_double_linked_list_t linked_list);

/** Test to see if we have moved past the start of the list.
 */
RQ_EXPORT short rq_double_linked_list_iterator_at_rend(rq_double_linked_list_iterator_t iter);

/** Increment the iterator.
 */
RQ_EXPORT rq_double_linked_list_iterator_t rq_double_linked_list_iterator_incr(rq_double_linked_list_iterator_t iter);

/** Decrement the iterator.
 */
RQ_EXPORT rq_double_linked_list_iterator_t rq_double_linked_list_iterator_decr(rq_double_linked_list_iterator_t iter);

/** Get the data pointed to by the iterator.
 */
RQ_EXPORT void *rq_double_linked_list_iterator_get_data(rq_double_linked_list_iterator_t iter);

/** Append an item to the end of the list.
 */
RQ_EXPORT void rq_double_linked_list_append(rq_double_linked_list_t linked_list, void *data);

/** Prepend an item to the beginning of the list.
 */
RQ_EXPORT void rq_double_linked_list_prepend(rq_double_linked_list_t linked_list, void *data);

/** Remove an item from the list. Return an iterator positioned at the
 * next item in the list. 
 */
RQ_EXPORT rq_double_linked_list_iterator_t rq_double_linked_list_erase(rq_double_linked_list_t linked_list, rq_double_linked_list_iterator_t iter);

#endif
