/**
 * \file rq_market.h
 * \author Brett Hutley
 *
 * \brief The rq_market object contains all the objects that define a
 * market at a point in time.
 */
/*
** rq_market.h
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
#ifndef rq_market_h
#define rq_market_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_rate_mgr.h"
#include "rq_yield_curve_mgr.h"
#include "rq_forward_curve_mgr.h"
#include "rq_future_curve_mgr.h"
#include "rq_vol_surface_mgr.h"
#include "rq_ir_vol_surface_mgr.h"
#include "rq_exchange_rate_mgr.h"
#include "rq_asset_correlation_mgr.h"
#include "rq_spot_price_mgr.h"
#include "rq_equity_curve_mgr.h"
#include "rq_spread_curve_mgr.h"
#include "rq_cds_curve_mgr.h"
#include "rq_external_termstruct_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_market {
    rq_date market_date; /**< The date of all the curves etc are based on */
    rq_rate_mgr_t rate_mgr; /**< This holds the base rates that the term structs are based on */
    rq_yield_curve_mgr_t yield_curve_mgr;
    rq_forward_curve_mgr_t forward_curve_mgr;
    rq_vol_surface_mgr_t vol_surface_mgr;
    rq_ir_vol_surface_mgr_t ir_vol_surface_mgr;
    rq_exchange_rate_mgr_t exchange_rate_mgr;
    rq_asset_correlation_mgr_t asset_correlation_mgr;
    rq_spot_price_mgr_t spot_price_mgr;
    rq_equity_curve_mgr_t equity_curve_mgr;
    rq_spread_curve_mgr_t spread_curve_mgr;
	rq_cds_curve_mgr_t cds_curve_mgr;
    rq_external_termstruct_mgr_t external_termstruct_mgr;
    rq_future_curve_mgr_t future_curve_mgr;
} *rq_market_t;


/** Test whether the rq_market is NULL. */
RQ_EXPORT int rq_market_is_null(rq_market_t obj);

/** Allocate a new market object based on the market date parameter.
 */
RQ_EXPORT rq_market_t rq_market_alloc(rq_date market_date);

/** Free the allocated market.
 */
RQ_EXPORT void rq_market_free(rq_market_t market);

/** Get the market date from the market object.
 */
RQ_EXPORT rq_date rq_market_get_market_date(const rq_market_t market);

/** Get the rate manager from the market.
 */
RQ_EXPORT rq_rate_mgr_t rq_market_get_rate_mgr(const rq_market_t m);

/** Get the yield curve manager from the market.
 */
RQ_EXPORT rq_yield_curve_mgr_t rq_market_get_yield_curve_mgr(const rq_market_t m);

/** Get the forward curve manager from the market.
 */
RQ_EXPORT rq_forward_curve_mgr_t rq_market_get_forward_curve_mgr(const rq_market_t m);

/** Get the forward curve manager from the market.
 */
RQ_EXPORT rq_future_curve_mgr_t rq_market_get_future_curve_mgr(const rq_market_t m);

/** Get the volatility surface manager from the market.
 */
RQ_EXPORT rq_vol_surface_mgr_t rq_market_get_vol_surface_mgr(const rq_market_t m);

/** Get the ir volatility surface manager from the market.
 */
RQ_EXPORT rq_ir_vol_surface_mgr_t rq_market_get_ir_vol_surface_mgr(const rq_market_t m);

/** Get the exchange rate manager from the market.
 */
RQ_EXPORT rq_exchange_rate_mgr_t rq_market_get_exchange_rate_mgr(const rq_market_t m);

/** Get the asset correlation manager from the market.
 */
RQ_EXPORT rq_asset_correlation_mgr_t rq_market_get_asset_correlation_mgr(const rq_market_t m);

/** Get the spot price manager from the market.
 */
RQ_EXPORT rq_spot_price_mgr_t rq_market_get_spot_price_mgr(const rq_market_t m);

/** Get the equity curve manager.
 */
RQ_EXPORT rq_equity_curve_mgr_t rq_market_get_equity_curve_mgr(const rq_market_t m);

/** Get the spread curve manager.
 */
RQ_EXPORT rq_spread_curve_mgr_t rq_market_get_spread_curve_mgr(const rq_market_t m);

/** Get the CDS curve manager.
 */
RQ_EXPORT rq_cds_curve_mgr_t rq_market_get_cds_curve_mgr(const rq_market_t m);

/** Get the external termstruct manager.
 */
RQ_EXPORT rq_external_termstruct_mgr_t rq_market_get_external_termstruct_mgr(const rq_market_t m);

/** Get a general term structure from the market.
 */
RQ_EXPORT void *rq_market_get_termstruct(const rq_market_t m, enum rq_termstruct_type termstruct_type, const char *termstruct_id);

/** Perform a deep copy of the market.
 */
RQ_EXPORT rq_market_t rq_market_clone(rq_market_t market);

/** Clear the market.
 */
RQ_EXPORT void rq_market_clear(rq_market_t market);

/** Set the market date in the market object.
 */
RQ_EXPORT void rq_market_set_market_date(rq_market_t market, rq_date market_date);

/** Get a discount factor from the appropriate term structure.
 *
 * @return 0 if successful otherwise a non-zero value.
 */
RQ_EXPORT int 
rq_market_get_discount_factor(
    rq_market_t market, 
    const char *asset_id, 
    rq_date to_date,
    double *df  /**< The returned discount factor */
    );

/** Get a forward rate from the appropriate forward curve.
 *
 * @return 0 if successful, otherwise non-zero.
 */
RQ_EXPORT int
rq_market_get_forward_rate(
    rq_market_t market, 
    const char *asset_ccypair_id, 
    rq_date to_date,
    double *forward_rate
    );

/** Get a volatility from the appropriate volatility surface.
 *
 * @return 0 if successful, otherwise non-zero.
 */
RQ_EXPORT int 
rq_market_get_volatility(
    rq_market_t market, 
    const char *asset_ccypair_id, 
    double delta, 
    rq_date to_date,
    double *volatility
    );

/** Get an ir volatility from the appropriate volatility surface.
 *
 * @return 0 if successful, otherwise non-zero.
 */
RQ_EXPORT int 
rq_market_get_ir_volatility(
    rq_market_t market, 
    const char *asset_underlying_id, 
    double underlyingTenor, 
    rq_date to_date,
    double *volatility
    );

/** Transition a market through time.
 *
 * Create a new market, being the base market transitioned through time,
 * with all the dates on the rates changed accordingly.
 */
RQ_EXPORT rq_market_t rq_market_transition_through_time(rq_market_t base_market, rq_date to_date);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
