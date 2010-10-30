/**
 * \file rq_assetflow_list.h
 * \author Brett Hutley
 *
 * \brief The rq_assetflow_list class represents a list of
 * rq_assetflows, or a list of asset movements.
 */
/*
** rq_assetflow_list.h
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
#ifndef rq_assetflow_list_h
#define rq_assetflow_list_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_assetflow.h"
#include "rq_linked_list.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
typedef struct rq_assetflow_list {
    rq_linked_list_t list;
} *rq_assetflow_list_t;

typedef struct rq_assetflow_list_iterator {
    rq_linked_list_iterator_t list_it;
} * rq_assetflow_list_iterator_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_assetflow_list is NULL */
RQ_EXPORT int rq_assetflow_list_is_null(rq_assetflow_list_t obj);

/**
 * Allocate a new list of asset flows
 */
RQ_EXPORT rq_assetflow_list_t rq_assetflow_list_alloc();

/**
 * Free an allocated list of asset flows
 */
RQ_EXPORT void rq_assetflow_list_free(rq_assetflow_list_t aflow_list);

/**
 * Start iterating through a list of asset flows
 */
RQ_EXPORT rq_assetflow_list_iterator_t rq_assetflow_list_begin(rq_assetflow_list_t aflow_list);

/**
 * Test to see if we are at the end of a list of asset flows
 */
RQ_EXPORT short rq_assetflow_list_iterator_at_end(rq_assetflow_list_iterator_t iter);

/**
 * Increment to the next item in our iteration
 */
RQ_EXPORT rq_assetflow_list_iterator_t rq_assetflow_list_iterator_incr(rq_assetflow_list_iterator_t iter);

/**
 * Get the asset flow pointed to by the iterator
 */
RQ_EXPORT rq_assetflow_t rq_assetflow_list_iterator_get(rq_assetflow_list_iterator_t iter);

/**
 * Append a new asset flow to the list
 */
RQ_EXPORT void rq_assetflow_list_append(rq_assetflow_list_t aflow_list, rq_assetflow_t aflow);

/**
 * Size of the asset flow list
 */
RQ_EXPORT unsigned int rq_assetflow_list_size(rq_assetflow_list_t aflow_list);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
