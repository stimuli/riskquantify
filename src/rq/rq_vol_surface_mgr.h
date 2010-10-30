/**
 * \file rq_vol_surface_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_vol_surface_mgr object manages a set of volatility
 * surfaces.
 */
/*
** rq_vol_surface_mgr.h
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
#ifndef rq_vol_surface_mgr_h
#define rq_vol_surface_mgr_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_vol_surface.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */

typedef struct rq_vol_surface_mgr {
    rq_tree_rb_t vol_surfaces;
} * rq_vol_surface_mgr_t;

typedef struct rq_vol_surface_mgr_iterator {
    rq_tree_rb_iterator_t vol_surface_it;
} * rq_vol_surface_mgr_iterator_t;


/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_vol_surface_mgr is NULL */
RQ_EXPORT int rq_vol_surface_mgr_is_null(rq_vol_surface_mgr_t obj);

/**
 * Allocate a new vol surface manager.
 */
RQ_EXPORT rq_vol_surface_mgr_t rq_vol_surface_mgr_alloc();

/**
 * Make a deep copy of the vol surface manager
 */
RQ_EXPORT rq_vol_surface_mgr_t rq_vol_surface_mgr_clone(rq_vol_surface_mgr_t vsmgr);

/**
 * Free a vol surface manager (and all the surfaces it manages).
 */
RQ_EXPORT void rq_vol_surface_mgr_free(rq_vol_surface_mgr_t vol_surface_mgr);

/**
 * Free all the volatility surfaces managed by the vol surface manager
 */
RQ_EXPORT void rq_vol_surface_mgr_clear(rq_vol_surface_mgr_t vol_surface_mgr);

/**
 * Add a new vol surface to the manager.
 */
RQ_EXPORT void 
rq_vol_surface_mgr_add(
    rq_vol_surface_mgr_t vol_surface_mgr,
    rq_vol_surface_t vol_surface
    );

/**
 * Get a vol surface from the manager by the asset id.
 */
RQ_EXPORT rq_vol_surface_t
rq_vol_surface_mgr_get(
    const rq_vol_surface_mgr_t vol_surface_mgr,
    const char *termstruct_id
    );


/**
 * Allocate a new iterater of the vol surface manager
 */
RQ_EXPORT rq_vol_surface_mgr_iterator_t rq_vol_surface_mgr_iterator_alloc();

/**
 * Start an iteration of the vol surface manager
 */
RQ_EXPORT void rq_vol_surface_mgr_begin(rq_vol_surface_mgr_t vol_surface_mgr, rq_vol_surface_mgr_iterator_t it);

/**
 * test to see if we are at the end of the iteration
 */
RQ_EXPORT int rq_vol_surface_mgr_at_end(rq_vol_surface_mgr_iterator_t it);

/**
 * Move to the next vol surface in the iteration
 */
RQ_EXPORT void rq_vol_surface_mgr_next(rq_vol_surface_mgr_iterator_t it);

/**
 * Free the vol surface iterator
 */
RQ_EXPORT void rq_vol_surface_mgr_iterator_free(rq_vol_surface_mgr_iterator_t it);

/**
 * Dereference the vol surface manager iterator
 */
RQ_EXPORT rq_vol_surface_t rq_vol_surface_mgr_iterator_deref(rq_vol_surface_mgr_iterator_t it);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
