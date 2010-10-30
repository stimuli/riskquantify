/**
 * \file rq_spread_curve_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_spread_curve_mgr is a manager class for spread
 * curves.
 */
/*
** rq_spread_curve_mgr.h
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
#ifndef rq_spread_curve_mgr_h
#define rq_spread_curve_mgr_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_spread_curve.h"
#include "rq_enum.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
/** A handle to the spread curve manager object */
typedef struct rq_spread_curve_mgr {
    rq_tree_rb_t spread_curves;
} * rq_spread_curve_mgr_t;

typedef struct rq_spread_curve_mgr_iterator {
    rq_tree_rb_iterator_t spread_curve_it;
} * rq_spread_curve_mgr_iterator_t;


/** Test whether the rq_spread_curve_mgr is NULL */
RQ_EXPORT int rq_spread_curve_mgr_is_null(rq_spread_curve_mgr_t obj);

/* -- prototypes -------------------------------------------------- */
/**
 * allocate a new term structure manager.
 * 
 * @return the new term structure manager object.
 */
RQ_EXPORT rq_spread_curve_mgr_t rq_spread_curve_mgr_alloc();

/**
 * Make a deep copy of the spread curve manager
 */
RQ_EXPORT rq_spread_curve_mgr_t rq_spread_curve_mgr_clone(rq_spread_curve_mgr_t yc);

/**
 * Free all the spread curves managed by the spread curve manager
 */
RQ_EXPORT void rq_spread_curve_mgr_clear(rq_spread_curve_mgr_t ycm);

/**
 * Free a term structure manager.
 *
 * @param mgr the term structure manager to free.
 */
RQ_EXPORT void rq_spread_curve_mgr_free(rq_spread_curve_mgr_t mgr);

/**
 * add a term structure to the term structure manager. The manager
 * will own this term structure after the call, and will be
 * responsible for freeing it.
 *
 * @param m The manager
 * @param ts The term structure to add 
 * @return the id/offset of the new term structure.
 *
 */
RQ_EXPORT void rq_spread_curve_mgr_add(rq_spread_curve_mgr_t m, rq_spread_curve_t ts);

/**
 * get a spread curve from the spread curve manager by ID.
 *
 * @param m the spread curve manager
 * @param asset_id the asset ID of the spread curve to retrieve
 * @return the spread curve
 */
RQ_EXPORT rq_spread_curve_t 
rq_spread_curve_mgr_get(
    rq_spread_curve_mgr_t m, 
    const char *asset_id
    );

/**
 * Allocate a spread curve iterator
 */
RQ_EXPORT rq_spread_curve_mgr_iterator_t rq_spread_curve_mgr_iterator_alloc();

/**
 * Free the spread curve iterator
 */
RQ_EXPORT void rq_spread_curve_mgr_iterator_free(rq_spread_curve_mgr_iterator_t it);

/**
 * Initialize the spread curve iterator to the start of the list of
 * spread curves.
 */
RQ_EXPORT void rq_spread_curve_mgr_begin(rq_spread_curve_mgr_t tree, rq_spread_curve_mgr_iterator_t it);

/**
 * Test the iterator to see if we have moved passed the end of the
 * the list of spread curves.
 */
RQ_EXPORT int rq_spread_curve_mgr_at_end(rq_spread_curve_mgr_iterator_t it);

/**
 * Move to the next spread curve in the iteration.
 */
RQ_EXPORT void rq_spread_curve_mgr_next(rq_spread_curve_mgr_iterator_t i);

/**
 * Get the spread curve pointed to by the iterator
 */
RQ_EXPORT rq_spread_curve_t rq_spread_curve_mgr_iterator_deref(rq_spread_curve_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
