/**
 * \file rq_forward_curve_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_forward_curve_mgr implements a manager class for
 * forward curves.
 */
/*
** rq_forward_curve_mgr.h
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
#ifndef rq_forward_curve_mgr_h
#define rq_forward_curve_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_forward_curve.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_forward_curve_mgr {
    rq_tree_rb_t tree;
} *rq_forward_curve_mgr_t;

typedef struct rq_forward_curve_mgr_iterator {
    rq_tree_rb_iterator_t forward_curve_it;
} * rq_forward_curve_mgr_iterator_t;


/** Test whether the rq_forward_curve_mgr is NULL */
RQ_EXPORT int rq_forward_curve_mgr_is_null(rq_forward_curve_mgr_t obj);

/**
 * Allocate a new forward curve manager
 */
RQ_EXPORT rq_forward_curve_mgr_t rq_forward_curve_mgr_alloc();

/**
 * Make a deep copy of the forward curve manager
 */
RQ_EXPORT rq_forward_curve_mgr_t rq_forward_curve_mgr_clone(rq_forward_curve_mgr_t fcmgr);

/**
 * Free the forward curves managed by the forward curve manager
 */
RQ_EXPORT void rq_forward_curve_mgr_clear(rq_forward_curve_mgr_t mgr);

/**
 * Free the forward curve manager itself
 */
RQ_EXPORT void rq_forward_curve_mgr_free(rq_forward_curve_mgr_t m);

/**
 * Add a forward curve to the forward curve manager
 */
RQ_EXPORT void rq_forward_curve_mgr_add(rq_forward_curve_mgr_t m, rq_forward_curve_t c);

/**
 * Get a forward curve by the asset ID
 */
RQ_EXPORT rq_forward_curve_t rq_forward_curve_mgr_get(const rq_forward_curve_mgr_t m, const char *ccypair_asset_id);

/**
 * Allocate a forward curve iterator
 */
RQ_EXPORT rq_forward_curve_mgr_iterator_t rq_forward_curve_mgr_iterator_alloc();

/**
 * Free the forward curve iterator
 */
RQ_EXPORT void rq_forward_curve_mgr_iterator_free(rq_forward_curve_mgr_iterator_t it);

/**
 * Initialize the forward curve iterator to the start of the list of
 * forward curves.
 */
RQ_EXPORT void rq_forward_curve_mgr_begin(rq_forward_curve_mgr_t tree, rq_forward_curve_mgr_iterator_t it);

/**
 * Test the iterator to see if we have moved passed the end of the
 * the list of forward curves.
 */
RQ_EXPORT int rq_forward_curve_mgr_at_end(rq_forward_curve_mgr_iterator_t it);

/**
 * Move to the next forward curve in the iteration.
 */
RQ_EXPORT void rq_forward_curve_mgr_next(rq_forward_curve_mgr_iterator_t i);

/**
 * Get the forward curve pointed to by the iterator
 */
RQ_EXPORT rq_forward_curve_t rq_forward_curve_mgr_iterator_deref(rq_forward_curve_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
