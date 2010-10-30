/**
 * \file rq_linked_list.h
 * \author Brett Hutley
 *
 * \brief The rq_linked_list files provide linked list management
 * routines.
 */
/*
** rq_linked_list.h
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
#ifndef rq_linked_list_h
#define rq_linked_list_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_iterator.h"

/* -- typedefs ---------------------------------------------------- */
struct rq_linked_list_node {
    struct rq_linked_list_node *next;
    void *data;
};

typedef struct rq_linked_list {
    struct rq_linked_list_node *head;
    struct rq_linked_list_node *tail;
    unsigned long count;
    void (*free_func)(void *);
} *rq_linked_list_t;

typedef struct rq_linked_list_node *rq_linked_list_iterator_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_linked_list is NULL */
RQ_EXPORT int rq_linked_list_is_null(rq_linked_list_t obj);

/** Allocate a new linked list.
 */
RQ_EXPORT rq_linked_list_t rq_linked_list_alloc(void (*free_func)(void *));

/** Free an allocated linked list.
 */
RQ_EXPORT void rq_linked_list_free(rq_linked_list_t linked_list);

/** Get a generic iterator for this linked list.
 */
RQ_EXPORT rq_iterator_t rq_linked_list_get_iterator(rq_linked_list_t linked_list);

/** Return an iterator initialized to the start of the list.
 */
RQ_EXPORT rq_linked_list_iterator_t rq_linked_list_begin(rq_linked_list_t linked_list);

/** Test whether the iterator has moved past the end of the list.
 */
RQ_EXPORT short rq_linked_list_iterator_at_end(rq_linked_list_iterator_t iter);

/** Increment the iterator.
 */
RQ_EXPORT rq_linked_list_iterator_t rq_linked_list_iterator_incr(rq_linked_list_iterator_t iter);

/** Return the data pointed to by the iterator.
 */
RQ_EXPORT void *rq_linked_list_iterator_get_data(rq_linked_list_iterator_t iter);

/** Append an item to the list.
 */
RQ_EXPORT void rq_linked_list_append(rq_linked_list_t linked_list, void *data);

/** Return the number of items in the list.
 */
RQ_EXPORT unsigned long rq_linked_list_size(rq_linked_list_t linked_list);

#endif
