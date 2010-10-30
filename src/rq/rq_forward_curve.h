/**
 * \file rq_forward_curve.h
 * \author Brett Hutley
 *
 * \brief The rq_forward_curve object implements a forward curve.
 */
/*
** rq_forward_curve.h
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
#ifndef rq_forward_curve_h
#define rq_forward_curve_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_termstruct.h"
#include "rq_forward_rate.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/**
 * \file
 * This class represents a series of forward exchange rates.
 */

typedef struct rq_forward_curve {
    struct rq_termstruct termstruct;
    /* const char *ccypair_asset_id; */ /**< The asset id of the currency pair */
    unsigned int num_forward_rates; /**< The number of forward rates in array */
    unsigned int max_forward_rates; /**< The maximum number of forward rates that can be held by the array */
    struct rq_forward_rate *forward_rates; /**< The array of forward rates */
} *rq_forward_curve_t;


/** Test whether the rq_forward_curve is NULL. */
RQ_EXPORT int rq_forward_curve_is_null(rq_forward_curve_t obj);

/** Construct a new forward curve object given the underlying asset id.
 */
RQ_EXPORT rq_forward_curve_t rq_forward_curve_build(const char *curve_id, const char *ccypair_asset_id);

/** Free a constructed forward curve.
 */
RQ_EXPORT void rq_forward_curve_free(rq_forward_curve_t fc);

/** Get the curve id for the forward curve.
 */
RQ_EXPORT const char *rq_forward_curve_get_curve_id(const rq_forward_curve_t fc);

/** Get the asset id from the forward curve
 */
RQ_EXPORT const char *rq_forward_curve_get_asset_id(const rq_forward_curve_t forward_curve);

/** Get the underlying asset ID associated with this forward curve.
 */
RQ_EXPORT const char *
rq_forward_curve_get_underlying_asset_id(const rq_forward_curve_t yc);

/** Set the underlying asset ID associated with this forward curve.
 */
RQ_EXPORT void
rq_forward_curve_set_underlying_asset_id(rq_forward_curve_t yc, const char *asset_id);

/** Set the rate for a particular date in the forward curve
 */
RQ_EXPORT void rq_forward_curve_set_rate(rq_forward_curve_t forward_curve, rq_date date, double rate, int quote);

/** Get a rate from the forward curve
 *
 * @return RQ_OK if successful.
 */
RQ_EXPORT int 
rq_forward_curve_get_rate(
    const rq_forward_curve_t forward_curve,
    rq_date date,
    double *rate  /**< The output parameter, the returned forward rate */
    );

RQ_EXPORT int 
rq_forward_curve_get_forward_rate(
    const rq_forward_curve_t forward_curve,
    rq_date date,
    struct rq_forward_rate *rate  /**< The output parameter, the returned forward rate */
   );

/** Get the implied cost of carry from the forward curve
 *
 * @return RQ_OK if successful.
 */
RQ_EXPORT int 
rq_forward_curve_get_cost_of_carry(
    const rq_forward_curve_t forward_curve,
    rq_date from_date,
    rq_date to_date,
    double *rate  /**< The output parameter, the returned forward rate */
    );

/** Clone a forward curve.
 */
RQ_EXPORT rq_forward_curve_t rq_forward_curve_clone(const rq_forward_curve_t fc);

/** Get the number of points on the forward curve.
 */
RQ_EXPORT unsigned int rq_forward_curve_size(const rq_forward_curve_t fc);

/** Get the forward rate structure at a specific offset on the forward
 * curve.  
 */
RQ_EXPORT const struct rq_forward_rate *
rq_forward_curve_element_at(
    const rq_forward_curve_t fc,
    unsigned int offset
    );

/** Get the address of the forward rate at a specific offset.
 */
RQ_EXPORT double *
rq_forward_curve_rate_at(
    rq_forward_curve_t fc,
    unsigned int offset
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
