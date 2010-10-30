/**
 * \file rq_future_curve_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_future_curve_mgr implements a manager class for
 * future curves.
 */
/*
** rq_future_curve_mgr.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#ifndef rq_future_curve_mgr_h
#define rq_future_curve_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_future_curve.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_future_curve_mgr {
    rq_tree_rb_t tree;
} *rq_future_curve_mgr_t;

typedef struct rq_future_curve_mgr_iterator {
    rq_tree_rb_iterator_t future_curve_it;
} * rq_future_curve_mgr_iterator_t;


/** Test whether the rq_future_curve_mgr is NULL */
RQ_EXPORT int rq_future_curve_mgr_is_null(rq_future_curve_mgr_t obj);

/**
 * Allocate a new future curve manager
 */
RQ_EXPORT rq_future_curve_mgr_t rq_future_curve_mgr_alloc();

/**
 * Make a deep copy of the future curve manager
 */
RQ_EXPORT rq_future_curve_mgr_t rq_future_curve_mgr_clone(rq_future_curve_mgr_t fcmgr);

/**
 * Free the future curves managed by the future curve manager
 */
RQ_EXPORT void rq_future_curve_mgr_clear(rq_future_curve_mgr_t mgr);

/**
 * Free the future curve manager itself
 */
RQ_EXPORT void rq_future_curve_mgr_free(rq_future_curve_mgr_t m);

/**
 * Add a future curve to the future curve manager
 */
RQ_EXPORT void rq_future_curve_mgr_add(rq_future_curve_mgr_t m, rq_future_curve_t c);

/**
 * Get a future curve by the asset ID
 */
RQ_EXPORT rq_future_curve_t rq_future_curve_mgr_get(const rq_future_curve_mgr_t m, const char *ccypair_asset_id);

/**
 * Allocate a future curve iterator
 */
RQ_EXPORT rq_future_curve_mgr_iterator_t rq_future_curve_mgr_iterator_alloc();

/**
 * Free the future curve iterator
 */
RQ_EXPORT void rq_future_curve_mgr_iterator_free(rq_future_curve_mgr_iterator_t it);

/**
 * Initialize the future curve iterator to the start of the list of
 * future curves.
 */
RQ_EXPORT void rq_future_curve_mgr_begin(rq_future_curve_mgr_t tree, rq_future_curve_mgr_iterator_t it);

/**
 * Test the iterator to see if we have moved passed the end of the
 * the list of future curves.
 */
RQ_EXPORT int rq_future_curve_mgr_at_end(rq_future_curve_mgr_iterator_t it);

/**
 * Move to the next future curve in the iteration.
 */
RQ_EXPORT void rq_future_curve_mgr_next(rq_future_curve_mgr_iterator_t i);

/**
 * Get the future curve pointed to by the iterator
 */
RQ_EXPORT rq_future_curve_t rq_future_curve_mgr_iterator_deref(rq_future_curve_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
