/**
 * \file rq_termstruct_cache.h
 * \author Brett Hutley
 *
 * \brief A cache for various kinds of term structures
 */
/*
** rq_termstruct_cache.h
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
#ifndef rq_termstruct_cache_h
#define rq_termstruct_cache_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_enum.h"
#include "rq_date.h"
#include "rq_tree_rb.h"
#include "rq_market_requirements.h"
#include "rq_yield_curve.h"
#include "rq_forward_curve.h"
#include "rq_future_curve.h"
#include "rq_vol_surface.h"
#include "rq_ir_vol_surface.h"
#include "rq_spread_curve.h"
#include "rq_cds_curve.h"
#include "rq_equity_curve.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif


/* -- defines ----------------------------------------------------- */

/** The termstruct_cache structure.
 */
typedef struct rq_termstruct_cache {
    rq_tree_rb_t termstruct_cache;
} *rq_termstruct_cache_t;

/* -- structures -------------------------------------------------- */

/*
  The termstruct group ID specifies the PURPOSE of the valuation - for
  example, whether the termstructure is being use for valuation
  purposes and should include any risk premiums or is being used as a
  discount curve, and should be the riskless rate. This ID can also be
  used to do customised stress testing since you can direct different
  groups to different curves for the same asset.
*/
typedef struct rq_termstruct_specification_key {
	enum rq_termstruct_type termstruct_type;
	const char *termstruct_group_id;
    const char *asset_id;
} * rq_termstruct_specification_key_t;

typedef struct rq_termstruct_specification {
	rq_termstruct_specification_key_t key;
	enum rq_termstruct_type termstruct_type;
	const char *termstruct_group_id;
    const char *asset_id;
    const char *termstruct_id;
    rq_date maturity_date; /**< the term structure needs to be be built up to this point */
    void *termstruct;
} * rq_termstruct_specification_t;

typedef struct rq_termstruct_cache_iterator {
    rq_tree_rb_iterator_t tree_rb_iterator;
} * rq_termstruct_cache_iterator_t;


/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_termstruct_cache is NULL */
RQ_EXPORT int rq_termstruct_cache_is_null(rq_termstruct_cache_t obj);

/**
 * Allocate a new termstruct_cache
 */
RQ_EXPORT rq_termstruct_cache_t rq_termstruct_cache_alloc();

/**
 * Free an object of type rq_termstruct_cache
 */
RQ_EXPORT void rq_termstruct_cache_free(rq_termstruct_cache_t termstruct_cache);

/**
 * Clear the termstructure cache
 */
RQ_EXPORT void rq_termstruct_cache_clear(rq_termstruct_cache_t termstruct_cache);

/**
 * Initialize a termstructure cache from a market_requirements
 * specification.
 */
RQ_EXPORT void 
rq_termstruct_cache_init(
    rq_termstruct_cache_t termstruct_cache,
    rq_market_requirements_t market_requirements
    );

/**
 * Add a term structure to the list of requested term structures
 *
 * @return the offset of the term structure
 */
RQ_EXPORT struct rq_termstruct_specification *
rq_termstruct_cache_add_requested_term_structure(
    rq_termstruct_cache_t termstruct_cache,
    enum rq_termstruct_type termstruct_type,
    const char *termstruct_group_id,
    const char *asset_id,
    rq_date maturity_date
    );

/** Get a term structure from the cache by asset ID
 */
RQ_EXPORT void *
rq_termstruct_cache_find_term_structure(
    rq_termstruct_cache_t termstruct_cache,
    enum rq_termstruct_type termstruct_type,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get a yield curve from the cache by asset ID
 */
RQ_EXPORT rq_yield_curve_t
rq_termstruct_cache_find_yield_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get a forward curve from the cache by asset ID
 */
RQ_EXPORT rq_forward_curve_t
rq_termstruct_cache_find_forward_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

RQ_EXPORT rq_future_curve_t
rq_termstruct_cache_find_future_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get a vol surface from the cache by asset ID
 */
RQ_EXPORT rq_vol_surface_t
rq_termstruct_cache_find_vol_surface(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get an ir vol surface from the cache by asset ID
 */
RQ_EXPORT rq_ir_vol_surface_t
rq_termstruct_cache_find_ir_vol_surface(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get a spread curve from the cache by asset ID
 */
RQ_EXPORT rq_spread_curve_t
rq_termstruct_cache_find_spread_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get a CDS curve from the cache by asset ID
 */
RQ_EXPORT rq_spread_curve_t
rq_termstruct_cache_find_spread_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get a equity curve from the cache by asset ID
 */
RQ_EXPORT rq_equity_curve_t
rq_termstruct_cache_find_equity_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

/** Get a cds curve from the cache by asset ID
 */
RQ_EXPORT rq_cds_curve_t
rq_termstruct_cache_find_cds_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    );

RQ_EXPORT rq_termstruct_cache_iterator_t
rq_termstruct_cache_begin(rq_termstruct_cache_t tc);

RQ_EXPORT int 
rq_termstruct_cache_at_end(rq_termstruct_cache_iterator_t it);

RQ_EXPORT void
rq_termstruct_cache_next(rq_termstruct_cache_iterator_t it);

RQ_EXPORT rq_termstruct_specification_t
rq_termstruct_cache_iterator_deref(rq_termstruct_cache_iterator_t it);

RQ_EXPORT void
rq_termstruct_cache_iterator_free(rq_termstruct_cache_iterator_t it);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
