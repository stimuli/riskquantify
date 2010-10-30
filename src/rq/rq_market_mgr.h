/**
 * \file rq_market_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_market_mgr object provides a manager class for
 * rq_market objects
 */
/*
** rq_market_mgr.h
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
#ifndef rq_market_mgr_h
#define rq_market_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_market.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_market_mgr {
    rq_tree_rb_t markets;
} *rq_market_mgr_t;

typedef struct rq_market_mgr_iterator {
    rq_tree_rb_iterator_t market_it;
} * rq_market_mgr_iterator_t;

/** Test whether the rq_market_mgr is NULL */
RQ_EXPORT int rq_market_mgr_is_null(rq_market_mgr_t obj);

/**
 * Allocate a new market manager
 */
RQ_EXPORT rq_market_mgr_t rq_market_mgr_alloc();

/**
 * Free the market manager
 */
RQ_EXPORT void rq_market_mgr_free(rq_market_mgr_t market_mgr);

/**
 * Add a new market to the manager
 */
RQ_EXPORT void rq_market_mgr_add(rq_market_mgr_t market_mgr, rq_market_t market);

/**
 * Get a particular market by date
 */
RQ_EXPORT rq_market_t rq_market_mgr_get(rq_market_mgr_t market_mgr, rq_date market_date);

/**
 * Allocate a market manager iterator
 */
RQ_EXPORT rq_market_mgr_iterator_t rq_market_mgr_iterator_alloc();

/**
 * Free an allocated market manager iterator.
 */
RQ_EXPORT void rq_market_mgr_iterator_free(rq_market_mgr_iterator_t it);

/**
 * Start an in-order walk of the markets.
 */
RQ_EXPORT void rq_market_mgr_begin(rq_market_mgr_t mgr, rq_market_mgr_iterator_t it);

/**
 * Test to see if we have passed the last market in the iteration.
 */
RQ_EXPORT int rq_market_mgr_at_end(rq_market_mgr_iterator_t it);

/**
 * Move to the next market in the manager.
 */
RQ_EXPORT void rq_market_mgr_next(rq_market_mgr_iterator_t i);

/**
 * Get the market associated with the current point in
 * the iteration
 */
RQ_EXPORT rq_market_t rq_market_mgr_iterator_deref(rq_market_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
