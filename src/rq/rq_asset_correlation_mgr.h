/**
 * \file rq_asset_correlation_mgr.h
 * \author Hendra
 *
 * \brief rq_asset_correlation_mgr provides a manager class for exchange
 * rate objects
 */
/*
** rq_asset_correlation_mgr.h
**
** Written by Hendra
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
#ifndef rq_asset_correlation_mgr_h
#define rq_asset_correlation_mgr_h

#include "rq_asset_correlation.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_asset_correlation_mgr {
    rq_tree_rb_t tree;
} * rq_asset_correlation_mgr_t;

typedef struct rq_asset_correlation_mgr_iterator {
    rq_tree_rb_iterator_t asset_correlation_it;
} * rq_asset_correlation_mgr_iterator_t;

/** Test whether the rq_asset_correlation_mgr is NULL */
RQ_EXPORT int rq_asset_correlation_mgr_is_null(rq_asset_correlation_mgr_t obj);

/**
 * Allocate a new asset correlation manager
 */
RQ_EXPORT rq_asset_correlation_mgr_t rq_asset_correlation_mgr_alloc();

/**
 * Make a deep copy of the asset correlation manager
 */
RQ_EXPORT rq_asset_correlation_mgr_t rq_asset_correlation_mgr_clone(rq_asset_correlation_mgr_t ermgr);

/**
 * Free the asset correlation manager
 */
RQ_EXPORT void rq_asset_correlation_mgr_free(rq_asset_correlation_mgr_t asset_correlation_mgr);

/**
 * Free the asset correlation managed by the asset correlation manager.
 */
RQ_EXPORT void rq_asset_correlation_mgr_clear(rq_asset_correlation_mgr_t asset_correlation_mgr);

/**
 * This function returns the asset correlation between asset1 and asset2.
 * 0: success, returns the correlation value
 * 1: fail, correlation set to 0.0
 */
RQ_EXPORT int 
rq_asset_correlation_mgr_get_correlation(
    rq_asset_correlation_mgr_t m, 
    const char* asset1,
    const char* asset2,
    double* correlation
    );

/**
 * Add an asset correlation to the manager
 */
RQ_EXPORT void
rq_asset_correlation_mgr_add(
    rq_asset_correlation_mgr_t m, 
    const char* asset1,
    const char* asset2,
    double correlation
    );

/**
 * Allocate an iterator that can iterator over the list of asset correlation
 */
RQ_EXPORT rq_asset_correlation_mgr_iterator_t rq_asset_correlation_mgr_iterator_alloc();

/**
 * Free an allocated asset correlation iterator
 */
RQ_EXPORT void rq_asset_correlation_mgr_iterator_free(rq_asset_correlation_mgr_iterator_t it);

/**
 * Initialize an iterator to the start of the list of asset correlations
 */
RQ_EXPORT void rq_asset_correlation_mgr_begin(rq_asset_correlation_mgr_t tree, rq_asset_correlation_mgr_iterator_t it);

/**
 * Test whether the iterator has passed the end of the list of asset correlations
 */
RQ_EXPORT int rq_asset_correlation_mgr_at_end(rq_asset_correlation_mgr_iterator_t it);

/**
 * Move to the next asset correlation in the list of asset correlations
 */
RQ_EXPORT void rq_asset_correlation_mgr_next(rq_asset_correlation_mgr_iterator_t i);

/**
 * Return the asset correlation pointed to by the iterator
 */
RQ_EXPORT rq_asset_correlation_t rq_asset_correlation_mgr_iterator_deref(rq_asset_correlation_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif
#endif
