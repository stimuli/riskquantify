/**
 * \file rq_future_curve.h
 * \author Brett Hutley
 *
 * \brief The rq_future_curve object implements a future curve.
 */
/*
** rq_future_curve.h
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
#ifndef rq_future_curve_h
#define rq_future_curve_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_termstruct.h"
#include "rq_future_price.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/**
 * \file
 * This class represents a series of future exchange prices.
 */

typedef struct rq_future_curve {
    struct rq_termstruct termstruct;
    /* const char *ccypair_asset_id; */ /**< The asset id of the currency pair */
    unsigned int num_future_prices; /**< The number of future prices in array */
    unsigned int max_future_prices; /**< The maximum number of future prices that can be held by the array */
    struct rq_future_price *future_prices; /**< The array of future prices */
} *rq_future_curve_t;


/** Test whether the rq_future_curve is NULL. */
RQ_EXPORT int rq_future_curve_is_null(rq_future_curve_t obj);

/** Construct a new future curve object given the underlying asset id.
 */
RQ_EXPORT rq_future_curve_t rq_future_curve_build(const char *curve_id, const char *ccypair_asset_id);

/** Free a constructed future curve.
 */
RQ_EXPORT void rq_future_curve_free(rq_future_curve_t fc);

/** Get the curve id for the future curve.
 */
RQ_EXPORT const char *rq_future_curve_get_curve_id(const rq_future_curve_t fc);

/** Get the asset id from the future curve
 */
RQ_EXPORT const char *rq_future_curve_get_asset_id(const rq_future_curve_t future_curve);

/** Get the underlying asset ID associated with this future curve.
 */
RQ_EXPORT const char *
rq_future_curve_get_underlying_asset_id(const rq_future_curve_t yc);

/** Set the underlying asset ID associated with this future curve.
 */
RQ_EXPORT void
rq_future_curve_set_underlying_asset_id(rq_future_curve_t yc, const char *asset_id);

/** Set the price for a particular date in the future curve
 */
RQ_EXPORT void rq_future_curve_set_price(rq_future_curve_t future_curve, const char* asset_id, double price, int quote);

/** Get a price from the future curve
 *
 * @return RQ_OK if successful.
 */
RQ_EXPORT int 
rq_future_curve_get_price(
    const rq_future_curve_t future_curve,
    const char *asset_id,
    double *price  /**< The output parameter, the returned future price */
    );

/** Get the implied cost of carry from the future curve
 *
 * @return RQ_OK if successful.
 */
RQ_EXPORT int 
rq_future_curve_get_cost_of_carry(
    const rq_future_curve_t future_curve,
    const char *asset_id1,
    const char *asset_id2,
    double *price  /**< The output parameter, the returned future price */
    );

/** Clone a future curve.
 */
RQ_EXPORT rq_future_curve_t rq_future_curve_clone(const rq_future_curve_t fc);

/** Get the number of points on the future curve.
 */
RQ_EXPORT unsigned int rq_future_curve_size(const rq_future_curve_t fc);

/** Get the future price structure at a specific offset on the future
 * curve.  
 */
RQ_EXPORT const struct rq_future_price *
rq_future_curve_element_at(
    const rq_future_curve_t fc,
    unsigned int offset
    );

/** Get the address of the future price at a specific offset.
 */
RQ_EXPORT double *
rq_future_curve_price_at(
    rq_future_curve_t fc,
    unsigned int offset
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
