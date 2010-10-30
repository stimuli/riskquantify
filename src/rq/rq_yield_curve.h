/**
 * \file rq_yield_curve.h
 * \author Brett Hutley
 *
 * \brief The rq_yield_curve object implements a yield curve.
 */
/*
** rq_yield_curve.h
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
#ifndef rq_yield_curve_h
#define rq_yield_curve_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_rate.h"
#include "rq_enum.h"
#include "rq_termstruct.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- enums ------------------------------------------------------- */
enum rq_yield_curve_type {
    RQ_YIELD_CURVE_TYPE_DISCOUNTFACTOR, /**< Rates are stored as Discount Factors */
    RQ_YIELD_CURVE_TYPE_COMPOSITE /**< The yield curve acts as a facade for another yield curve and a spread curve. */
};

/* -- typedefs ---------------------------------------------------- */
/** This is the main controlling structure for a yield curve.
 */
typedef struct rq_yield_curve {
    struct rq_termstruct termstruct;
    rq_date from_date; /**< the date the curve is based from */
    unsigned int num_factors; /**< the number of discount factors managed by this structure */
    unsigned int max_factors; /**< max number of factors before this structure needs to grow */
    unsigned long factor_cache_size;
    double factor_cache[RQ_FACTOR_CACHE_SIZE]; /**< cache of discout factors for the first x days */
    struct rq_yield_curve_elem *discount_factors; /**< the actual discount factors */
    enum rq_interpolation_method interpolation_method; /**< the interpolation method used to get a rate between points */

    enum rq_yield_curve_type yield_curve_type; /**< This is how the rates are stored in the yield curve, or whether the curve acts as a facade on the base curve + spread. */
    struct rq_yield_curve *base_curve; /**< pointer to a base curve */
    struct rq_yield_curve *spread_curve; /**< pointer to a spread curve */
    double additive_factor; /**< A factor to add to the rate before returning */
    double multiplicative_factor; /**< A factor to multiply the rate by before returning. */
    enum rq_extrapolation_method curve_start_extrapolation_method; /**< The extrapolation method to use at the start of the curve */
    enum rq_extrapolation_method curve_end_extrapolation_method; /**< The extrapolation method to use at the end of the curve */
	enum rq_zero_method zero_method; /**< definition of zero rate */
	unsigned int zero_method_compound_frequency; /**< compound frequency (#times in a year) */
    enum rq_day_count_convention default_day_count_convention;
	const char *debug_filename; // If set then internal calculations will be logged here, set from Valuation Analysis.
} *rq_yield_curve_t;

/* -- structures -------------------------------------------------- */
/**
 * This structure defines an individual term structure element.
 */
struct rq_yield_curve_elem {
    rq_date date;
    double discount_factor;
};

/* -- prototypes -------------------------------------------------- */

/* Enable use of the cache. This should be called after bootstrapping. */
RQ_EXPORT void rq_yield_curve_cache_enable(rq_yield_curve_t);

/** Test whether the rq_yield_curve is NULL */
RQ_EXPORT int rq_yield_curve_is_null(rq_yield_curve_t obj);

/** Get the date out of the yield curve element.
 */
RQ_EXPORT rq_date rq_yield_curve_elem_get_date(const struct rq_yield_curve_elem *e);

/** Get the discount factor out of the yield curve element.
 */
RQ_EXPORT double rq_yield_curve_elem_get_discount_factor(const struct rq_yield_curve_elem *e);

/** Allocate a new yield curve.
 *
 * @param curve_id the Curve ID of the yield curve to build
 * @param from_date the date the yield curve is based from
 * @return the newly allocated yield curve. Caller is responsible for 
 * freeing.
 */
RQ_EXPORT rq_yield_curve_t 
rq_yield_curve_init(
    const char *curve_id,
    enum rq_interpolation_method interpolationMethod,
    enum rq_extrapolation_method extrapolationMethodStart,
    enum rq_extrapolation_method extrapolationMethodEnd,
	enum rq_zero_method zero_method,
	unsigned int zero_method_compound_frequency,
    enum rq_day_count_convention defaultDCC,
    rq_date from_date
    );

/** Deallocate the memory for a yield curve.
 */
RQ_EXPORT void rq_yield_curve_free(rq_yield_curve_t yc);

/** Get the curve ID associated with this yield curve.
 *
 */
RQ_EXPORT const char *rq_yield_curve_get_curve_id(const rq_yield_curve_t yc);

/** Get the underlying asset ID associated with this yield curve.
 */
RQ_EXPORT const char *
rq_yield_curve_get_underlying_asset_id(const rq_yield_curve_t yc);

RQ_EXPORT void
rq_yield_curve_set_debug_filename(rq_yield_curve_t yc, const char *filename);

/* NOTE: None of these _set_ functions can be called outside of bootstrapping because of the factor_cache. */

/** Set the underlying asset ID associated with this yield curve.
 */
RQ_EXPORT void
rq_yield_curve_set_underlying_asset_id(rq_yield_curve_t yc, const char *asset_id);

/** Get the date from which the yield curve is based.
 */
RQ_EXPORT rq_date rq_yield_curve_get_curve_date(const rq_yield_curve_t yc);

/** Get the first date on our term structure.
 */
RQ_EXPORT rq_date rq_yield_curve_get_first_date(const rq_yield_curve_t yc);

/** Get the last date on our term structure.
 */
RQ_EXPORT rq_date rq_yield_curve_get_last_date(const rq_yield_curve_t yc);

/** Get a discount factor from the yield curve.
 *
 * Get a discount factor from the yield curve. The discount factor 
 * discounts from the for_date specified to the from_date on the 
 * yield curve.
 */
RQ_EXPORT double rq_yield_curve_get_discount_factor(const rq_yield_curve_t ts, rq_date for_date);

/** Get a forward discount factor from the yield curve.
 *
 * Get a forward discount factor from the yield curve. The discount
 * factor returned discounts from the end_date to the start_date
 * specified.  
 */
RQ_EXPORT double rq_yield_curve_get_forward_discount_factor(const rq_yield_curve_t ts, rq_date start_date, rq_date end_date);

/** Set a discount factor in the yield curve.
 */
RQ_EXPORT void rq_yield_curve_set_discount_factor(rq_yield_curve_t ts, rq_date for_date, double discount_factor);

/**
 * Allocate a new yield curve object and fill it with the contents of
 * the yield curve passed as the argument.
 */
RQ_EXPORT rq_yield_curve_t rq_yield_curve_clone(const rq_yield_curve_t ts);

/** Get a simple (annualized) rate from the yield curve.
 */
RQ_EXPORT double 
rq_yield_curve_get_simple_rate(
    const rq_yield_curve_t ts,
    rq_date date,
    enum rq_day_count_convention day_count_convention
    );

/** Calculate a par or swap rate from the yield curve.
 *
 * Calculate a par or swap rate from the yield curve for the _last_
 * date in the array of dates handed to the function. The array of
 * dates is the date rolls based on the periods_per_year handed to
 * the function, or period length.
 */
RQ_EXPORT double 
rq_yield_curve_get_par_rate(
    const rq_yield_curve_t yield_curve,
    unsigned int num_dates,
    rq_date *dates,
    unsigned int periods_per_year
    );

RQ_EXPORT double 
rq_yield_curve_get_par_rate_period(
    const rq_yield_curve_t yield_curve,
    unsigned int num_dates,
    rq_date *dates,
    double period
    );

/** Calculate a par or swap rate from the yield curve.
 *
 * Calculate a par or swap rate from the yield curve for the _last_
 * date in the array of dates handed to the function. The array of
 * dates is the date rolls based on the day count convention handed to
 * the function.
 */
RQ_EXPORT double 
rq_yield_curve_get_par_rate_day_count(
    const rq_yield_curve_t yield_curve,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention *day_counts
    );

/** Calculate an accrual rate from the yield curve.
 *
 * Calculate a forward par or swap rate from the yield curve for the
 * _last_ date in the array of dates handed to the function. The array
 * of dates is the date rolls based on the periods_per_year handed to
 * the function.  
 */
RQ_EXPORT double 
rq_yield_curve_get_accrual(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    unsigned int periods_per_year
    );

/** Calculate an accrual rate from the yield curve.
 *
 * Calculate a forward par or swap rate from the yield curve for the
 * _last_ date in the array of dates handed to the function. The array
 * of dates is the date rolls based on the day_count convention handed to
 * the function.  
 */
RQ_EXPORT double 
rq_yield_curve_get_accrual_day_count(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    );

/** Calculate a forward par or forward swap rate from the yield curve.
 *
 * Calculate a forward par or swap rate from the yield curve for the
 * _last_ date in the array of dates handed to the function. The array
 * of dates is the date rolls based on the periods_per_year handed to
 * the function.  
 */
RQ_EXPORT double 
rq_yield_curve_get_forward_par_rate(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    unsigned int periods_per_year
    );
RQ_EXPORT double 
rq_yield_curve_get_forward_par_rate_period(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    double period
    );

/** Calculate a forward par or forward swap rate from the yield curve.
 *
 * Calculate a forward par or swap rate from the yield curve for the
 * _last_ date in the array of dates handed to the function. The array
 * of dates is the date rolls based on the periods_per_year handed to
 * the function.  
 */
RQ_EXPORT double 
rq_yield_curve_get_forward_par_rate_day_count(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    );

/* DS Added as this is the rate which should be used in Black Scholes Models */
/** Get a continuous compounded (annualized) forward rate from the yield curve.
 */
RQ_EXPORT double 
rq_yield_curve_get_continuous_rate(
    const rq_yield_curve_t ts,
    rq_date start_date,
    rq_date end_date,
    enum rq_day_count_convention day_count_convention
    );		
	
/** Get a simple (annualized) forward rate from the yield curve.
 */
RQ_EXPORT double 
rq_yield_curve_get_forward_simple_rate(
    const rq_yield_curve_t ts,
    rq_date start_date,
    rq_date end_date,
    enum rq_day_count_convention day_count_convention
    );

/** Get an effective forward rate for a period from the yield curve.
 *
 * 1 + the rate returned should equal the accrual factor for the forward
 * period passed as an argument.
 */
RQ_EXPORT double 
rq_yield_curve_get_forward_effective_rate(
    const rq_yield_curve_t yield_curve,
    rq_date start_date,
    rq_date end_date
    );

/** Return the number of points on the yield curve.
 */
RQ_EXPORT unsigned int rq_yield_curve_size(const rq_yield_curve_t ts);

/** Get a point on the yield curve by offset.
 *
 * The offset is zero based.
 */
RQ_EXPORT struct rq_yield_curve_elem *
rq_yield_curve_element_at(
    const rq_yield_curve_t ts,
    unsigned int offset
    );

/** Remove a point on the yield curve by offset.
 *
 * The offset is zero based.
 */
void
rq_yield_curve_remove_element_at(
    rq_yield_curve_t ts,
    unsigned int offset
    );

/** Remove a range of points on the yield curve by offset.
 *
 * The offset is zero based.
 */
void
rq_yield_curve_remove_element_range(
    rq_yield_curve_t ts,
    unsigned int from_offset,
    unsigned int to_offset
    );

/** Get the yield curve type.
 */
RQ_EXPORT enum rq_yield_curve_type
rq_yield_curve_get_yield_curve_type(const rq_yield_curve_t ts);

/** Set the type of this yield curve.
 */
RQ_EXPORT void
rq_yield_curve_set_yield_curve_type(
    const rq_yield_curve_t ts,
    enum rq_yield_curve_type yield_curve_type
    );

/** Get the base yield curve that this yield curve is acting as a facade for.
 */
RQ_EXPORT rq_yield_curve_t
rq_yield_curve_get_base_curve(rq_yield_curve_t ts);

/** Set the base yield curve that this yield curve is acting as a facade for.
 */
RQ_EXPORT void
rq_yield_curve_set_base_curve(
    rq_yield_curve_t ts,
    rq_yield_curve_t base_curve
    );

/** Set up a composite yield curve.
 * Set up a composite yield curve. This function will set the yield curve
 * type to composite, and set the base and spread curves.
 */
RQ_EXPORT void
rq_yield_curve_set_composite(
    rq_yield_curve_t yc,
    rq_yield_curve_t base_curve,
    rq_yield_curve_t spread_curve
    );

/** Get the spread yield curve that this yield curve is acting as a facade for.
 */
RQ_EXPORT rq_yield_curve_t
rq_yield_curve_get_spread_curve(rq_yield_curve_t ts);

/** Set the spread yield curve that this yield curve is acting as a facade for.
 */
RQ_EXPORT void
rq_yield_curve_set_spread_curve(
    rq_yield_curve_t ts,
    rq_yield_curve_t spread_curve
    );

/** Get the additive factor that will be applied for this yield curve.
 */
RQ_EXPORT double
rq_yield_curve_get_additive_factor(rq_yield_curve_t ts);

/** Set the additive factor that will be applied for this yield curve.
 */
RQ_EXPORT void
rq_yield_curve_set_additive_factor(
    rq_yield_curve_t ts,
    double additive_factor
    );

/** Get the multiplicative factor that will be applied for this yield curve.
 */
RQ_EXPORT double
rq_yield_curve_get_multiplicative_factor(
    rq_yield_curve_t ts
    );

/** Set the multiplicative factor that will be applied for this yield curve.
 */
RQ_EXPORT void
rq_yield_curve_set_multiplicative_factor(
    rq_yield_curve_t ts,
    double multiplicative_factor
    );


/** Set the default day count convention.
 */
RQ_EXPORT void rq_yield_curve_set_default_day_count_convention(
    rq_yield_curve_t yc,
    enum rq_day_count_convention day_count
    );

/** Get the default day count convention.
 */
RQ_EXPORT enum rq_day_count_convention 
rq_yield_curve_get_default_day_count_convention(rq_yield_curve_t yc);

double 
rq_curve_interpolation_get_discount_factor(
	enum rq_interpolation_method method,
	enum rq_zero_method zero_method,
	unsigned int zero_method_compound_frequency,
	enum rq_day_count_convention curveDayCountConvention,
	rq_date curveBaseDate,
	const struct rq_yield_curve_elem* pointStart,  /* TODO This should be generic like a rq_curve_point and then extracted */
	const struct rq_yield_curve_elem* pointEnd,   /* TODO This should be generic like a rq_curve_point and then extracted */
	rq_date forDate);

double 
rq_curve_interpolation_spline_get_discount_factor(const struct rq_yield_curve_elem* fixedDFs,
												  unsigned int numDFs,
												  rq_date forDate);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
