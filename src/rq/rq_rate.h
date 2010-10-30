/**
 * \file rq_rate.h
 * \author Brett Hutley
 *
 * \brief The rq_rate object encapsulates a market-quoted rate or
 * price.
 */
/*
** rq_rate.h
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
#ifndef rq_rate_h
#define rq_rate_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_enum.h"
#include "rq_perturbation_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_rate {
    const char *rate_class_id;
    const char *asset_id;
    enum rq_rate_type rate_type;
    rq_date observation_date;
    rq_date value_date;
    double value;
	rq_perturbation_mgr_t perturbation_mgr;
	int perturbation_offset;
} *rq_rate_t;


/** Test whether the rq_rate is NULL */
RQ_EXPORT int rq_rate_is_null(rq_rate_t obj);

/** Allocate a new rate and fill it with the supplied parameters.
 */
RQ_EXPORT rq_rate_t 
rq_rate_build(
    const char *rate_class_id,
    const char *asset_id,
    enum rq_rate_type rate_type,
    rq_date observation_date,
    rq_date value_date,
    double value
    );

/** Free an allocated rate.
 */
RQ_EXPORT void rq_rate_free(rq_rate_t r);

/** Clone a rate.
 */
RQ_EXPORT rq_rate_t rq_rate_clone(const rq_rate_t rate);

/* -- getters ----------------------------------------------------- */
/** Get the asset ID from the rate.
 */
RQ_EXPORT const char *rq_rate_get_asset_id(const rq_rate_t rate);

/** Get the rate class ID from the rate.
 */
RQ_EXPORT const char *rq_rate_get_rate_class_id(const rq_rate_t rate);

/** Get the rate type from the rate.
 */
RQ_EXPORT enum rq_rate_type rq_rate_get_rate_type(const rq_rate_t rate);

/** Get the observation date from the rate.
 */
RQ_EXPORT rq_date rq_rate_get_observation_date(const rq_rate_t rate);

/** Get the value date from the rate.
 */
RQ_EXPORT rq_date rq_rate_get_value_date(const rq_rate_t rate);

/** Get the rate or price value.
 */
RQ_EXPORT double rq_rate_get_value(const rq_rate_t rate);

/** Get the rate or price value excluding any perturbations.
 */
RQ_EXPORT double rq_rate_get_unperturbed_value(const rq_rate_t r);

/* -- setters ----------------------------------------------------- */
/** Set the observation date in the rate.
 */
RQ_EXPORT void rq_rate_set_observation_date(rq_rate_t rate, rq_date observation_date);

/** Set the value date in the rate.
 */
RQ_EXPORT void rq_rate_set_value_date(rq_rate_t rate, rq_date value_date);

/** Set the value of the rate.
 */
RQ_EXPORT void rq_rate_set_value(rq_rate_t rate, double value);

/** Override the value of the rate, replacing any existing or future perturbations.
 */
RQ_EXPORT void rq_rate_override_value(rq_rate_t rate, double value);

/* -- helpers ----------------------------------------------------- */
/** Convert a simple rate to a discount factor.
 */
RQ_EXPORT double rq_simple_rate_to_discount_factor(double rate, rq_date from_date, rq_date to_date, double days_per_year);

/** Convert a discount factor to a annualized zero rate.
 */
RQ_EXPORT double rq_simple_rate_from_discount_factor(double df, rq_date from_date, rq_date to_date, enum rq_day_count_convention dateCountConv);

/** Calculate the last discount factor from a par rate and list of discount
 * factors.
 */
RQ_EXPORT double rq_discount_factor_from_par_rate(double par_rate, unsigned short num_discount_factors, double *discount_factors);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
