/**
 * \file rq_equity_curve_mgr.h
 * \author Brett Hutley
 *
 * \brief A manager class for dividend yield curves
 */
/*
** rq_equity_curve_mgr.h
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
#ifndef rq_equity_curve_mgr_h
#define rq_equity_curve_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_equity_curve.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private equity_curve_mgr structure.
 */
typedef struct rq_equity_curve_mgr {
    rq_tree_rb_t curves;
} *rq_equity_curve_mgr_t;

typedef struct rq_equity_curve_mgr_iterator {
    rq_tree_rb_iterator_t curve_it;
} * rq_equity_curve_mgr_iterator_t;


/** Test whether the rq_equity_curve_mgr is NULL */
RQ_EXPORT int rq_equity_curve_mgr_is_null(rq_equity_curve_mgr_t obj);

/**
 * Allocate a new equity_curve_mgr
 */
RQ_EXPORT rq_equity_curve_mgr_t rq_equity_curve_mgr_alloc();

/**
 * Free an object of type rq_equity_curve_mgr
 */
RQ_EXPORT void rq_equity_curve_mgr_free(rq_equity_curve_mgr_t equity_curve_mgr);

/**
 * Clone a equity_curve_mgr
 */
RQ_EXPORT rq_equity_curve_mgr_t rq_equity_curve_mgr_clone(rq_equity_curve_mgr_t equity_curve_mgr);

/**
 * Clear the equity_curve_mgr
 */
RQ_EXPORT void rq_equity_curve_mgr_clear(rq_equity_curve_mgr_t equity_curve_mgr);

/**
 * Add a dividend yield curve to the list of dividend yield curves
 */
RQ_EXPORT void rq_equity_curve_mgr_add(rq_equity_curve_mgr_t equity_curve_mgr, rq_equity_curve_t equity_curve);

/**
 * Get a dividend yield curve from the dividend yield curve manager
 */
RQ_EXPORT rq_equity_curve_t rq_equity_curve_mgr_get(rq_equity_curve_mgr_t equity_curve_mgr, const char *curve_id);

/**
 * Allocate a dividend yield curve iterator
 */
RQ_EXPORT rq_equity_curve_mgr_iterator_t rq_equity_curve_mgr_iterator_alloc();

/**
 * Free the dividend yield curve iterator
 */
RQ_EXPORT void rq_equity_curve_mgr_iterator_free(rq_equity_curve_mgr_iterator_t it);

/**
 * Initialize the dividend yield curve iterator to the start of the list of
 * dividend yield curves.
 */
RQ_EXPORT void rq_equity_curve_mgr_begin(rq_equity_curve_mgr_t tree, rq_equity_curve_mgr_iterator_t it);

/**
 * Test the iterator to see if we have moved passed the end of the
 * the list of dividend yield curves.
 */
RQ_EXPORT int rq_equity_curve_mgr_at_end(rq_equity_curve_mgr_iterator_t it);

/**
 * Move to the next dividend yield curve in the iteration.
 */
RQ_EXPORT void rq_equity_curve_mgr_next(rq_equity_curve_mgr_iterator_t i);

/**
 * Get the dividend yield curve pointed to by the iterator
 */
RQ_EXPORT rq_equity_curve_t rq_equity_curve_mgr_iterator_deref(rq_equity_curve_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

