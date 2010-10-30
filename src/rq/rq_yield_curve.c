/*
** rq_yield_curve.c
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
#include "rq_yield_curve.h"
#include "rq_interpolate.h"
#include "rq_day_count.h"
#include "rq_rate_conversions.h"
#include "rq_defs.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static unsigned int s_max_factors = RQ_YIELD_CURVE_MAX_FACTORS;

/*
 * Allocate yield curve to contain in_max_factors number of discount factors, or s_max_factors of them, if in_max_factors number is 0
 */
static struct rq_yield_curve *
rq_yield_curve_alloc(const unsigned int in_max_factors)
{
	/* use RQ_CALLOC to zero the structure */
	struct rq_yield_curve *ts = (struct rq_yield_curve *)RQ_CALLOC(1, sizeof(struct rq_yield_curve));
	/* if (!ts) error(); */

    ts->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_YIELD_CURVE;
	if (in_max_factors != 0) /* alloc according to an existing curve */
	{
		ts->max_factors = in_max_factors;
		ts->discount_factors = (struct rq_yield_curve_elem *)RQ_CALLOC(in_max_factors, sizeof(struct rq_yield_curve_elem));
	}
	else
	{
		ts->max_factors = s_max_factors;
		ts->discount_factors = (struct rq_yield_curve_elem *)RQ_CALLOC(s_max_factors, sizeof(struct rq_yield_curve_elem));
	}
    memset(ts->factor_cache, 0, RQ_FACTOR_CACHE_SIZE * sizeof(double));
    ts->factor_cache_size = 0; /* Disabled until bootstrapping is complete. */
    ts->interpolation_method = RQ_INTERPOLATION_LINEAR_DISCOUNT_FACTOR;
	ts->zero_method = RQ_ZERO_CASH_DEPOSIT;
	ts->zero_method_compound_frequency = 1;
    ts->yield_curve_type = RQ_YIELD_CURVE_TYPE_DISCOUNTFACTOR;
    ts->base_curve = 0;
    ts->spread_curve = 0;
    ts->additive_factor = 0.0;
    ts->multiplicative_factor = 0.0;

    ts->curve_start_extrapolation_method = RQ_EXTRAPOLATION_LINEAR_DISCOUNT_FACTOR;
    ts->curve_end_extrapolation_method = RQ_EXTRAPOLATION_LINEAR_ZERO;
	
    ts->default_day_count_convention = RQ_DAY_COUNT_ACTUAL_365;

    return ts;
}

RQ_EXPORT void rq_yield_curve_cache_enable(rq_yield_curve_t ts)
{
    ts->factor_cache_size = RQ_FACTOR_CACHE_SIZE;
}

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
    )
{
    struct rq_yield_curve *ts = rq_yield_curve_alloc(0); /* create a new curve */

    ts->termstruct.termstruct_id = RQ_STRDUP(curve_id);
    ts->from_date = from_date;

	if (interpolationMethod != RQ_INTERPOLATION_INVALID)
	{
		ts->interpolation_method = interpolationMethod;
	}
	if (extrapolationMethodStart != RQ_EXTRAPOLATION_INVALID)
	{
	    ts->curve_start_extrapolation_method = extrapolationMethodStart;
	}
	if (extrapolationMethodEnd != RQ_EXTRAPOLATION_INVALID)
	{
		ts->curve_end_extrapolation_method = extrapolationMethodEnd;
	}
	if (zero_method != RQ_ZERO_INVALID)
	{
		ts->zero_method = zero_method;
	}
	ts->zero_method_compound_frequency = zero_method_compound_frequency;
	if (defaultDCC != RQ_DAY_COUNT_INVALID)
	{
	    ts->default_day_count_convention = defaultDCC;
	}

    return ts;
}

RQ_EXPORT void 
rq_yield_curve_free(rq_yield_curve_t ts)
{
    rq_termstruct_clear(&ts->termstruct);
    RQ_FREE(ts->discount_factors);
    RQ_FREE(ts);
}

RQ_EXPORT const char *
rq_yield_curve_get_curve_id(const rq_yield_curve_t ts)
{
    return ts->termstruct.termstruct_id;
}

RQ_EXPORT const char *
rq_yield_curve_get_underlying_asset_id(const rq_yield_curve_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_yield_curve_set_debug_filename(rq_yield_curve_t yc, const char *filename)
{
	yc->debug_filename = filename;
}

RQ_EXPORT void
rq_yield_curve_set_underlying_asset_id(rq_yield_curve_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT rq_date 
rq_yield_curve_get_curve_date(const rq_yield_curve_t ts)
{
    return ts->from_date;
}

static struct rq_yield_curve_elem *
rq_yield_curve_lower_bound(const rq_yield_curve_t ts, rq_date for_date)
{
	struct rq_yield_curve_elem *dfs = ts->discount_factors;

	struct rq_yield_curve_elem *begin = dfs;
	struct rq_yield_curve_elem *end = dfs + ts->num_factors;

	if (ts->num_factors == 0 || for_date <= begin->date)
	{
		return begin;
	}
	else if (for_date <= (end - 1)->date)
	{
		struct rq_yield_curve_elem *mid;

		while (1)
		{
			mid = begin + (end - begin) / 2;

			if (for_date > mid->date)
				begin = mid + 1;
			else if (for_date == mid->date || for_date > (mid - 1)->date)
				return mid;
			else
				end = mid;
		}
	}

	return end;
}

double 
rq_curve_interpolation_spline_get_discount_factor(const struct rq_yield_curve_elem* fixedDFs,
												  unsigned int numDFs,
												  rq_date forDate)
{
	double yp2[RQ_YIELD_CURVE_MAX_FACTORS+1];
	double x[RQ_YIELD_CURVE_MAX_FACTORS+1];
	double y[RQ_YIELD_CURVE_MAX_FACTORS+1];

	unsigned int i;
	for (i=0; i < RQ_YIELD_CURVE_MAX_FACTORS && i < numDFs; i++)
	{
		x[i+1] = fixedDFs[i].date;
		y[i+1] = fixedDFs[i].discount_factor;
	}

	rq_interpolate_spline(1,
						x,
						y,
						numDFs,
						0,
						0,
						yp2);

	return rq_interpolate_splint(forDate,
						x,
						y,
						yp2,
						numDFs);
}

double 
rq_curve_interpolation_get_discount_factor(
	enum rq_interpolation_method method,
	enum rq_zero_method zero_method,
	unsigned int zero_method_compound_frequency,
	enum rq_day_count_convention curveDayCountConvention,
	rq_date curveBaseDate,
	const struct rq_yield_curve_elem* pointStart,  /* TODO This should be generic like a rq_curve_point and then extracted */
	const struct rq_yield_curve_elem* pointEnd,   /* TODO This should be generic like a rq_curve_point and then extracted */
	rq_date forDate)
{
	double df = 0.0;
	switch (method)
    {
        case RQ_INTERPOLATION_LINEAR_ZERO:
        {
            double days1, days2, zero, zero1, zero2;
			days1 = pointStart->date - curveBaseDate;
            zero1 = rq_rate_discount_to_zero(
                pointStart->discount_factor,
				zero_method,
				zero_method_compound_frequency,
                rq_day_count_get_year_fraction(curveDayCountConvention, curveBaseDate, pointStart->date)
                );
            days2 = pointEnd->date - curveBaseDate; 
            zero2 = rq_rate_discount_to_zero(
                pointEnd->discount_factor,
				zero_method,
				zero_method_compound_frequency,
                rq_day_count_get_year_fraction(curveDayCountConvention, curveBaseDate, pointEnd->date)
                );
			if (days1 == 0)
			{
				zero1 = zero2;
			}
            zero = rq_interpolate_linear(
                (double)(forDate - curveBaseDate),
                days1,
				zero1,
                days2,
                zero2
                );
            df = rq_rate_zero_to_discount(
                zero,
				zero_method,
				zero_method_compound_frequency,
                rq_day_count_get_year_fraction(curveDayCountConvention, curveBaseDate, forDate)
                ); 
        }
	        break;

        case RQ_INTERPOLATION_LINEAR_DISCOUNT_FACTOR:
		{
			df = rq_interpolate_linear(
                (double)forDate,
                (double)pointStart->date,
                pointStart->discount_factor,
                (double)pointEnd->date,
                pointEnd->discount_factor
                );
		}
			break;

		case RQ_INTERPOLATION_SPLINE_DISCOUNT_FACTOR:
			break;

		case RQ_INTERPOLATION_LOG_LINEAR_DISCOUNT_FACTOR:
		{
			df = rq_interpolate_log_linear(
                (double)forDate,
                (double)pointStart->date,
                pointStart->discount_factor,
                (double)pointEnd->date,
                pointEnd->discount_factor);
		}
		break;

		case RQ_INTERPOLATION_LOG_LINEAR_ZERO:
        {
            double days1 = pointStart->date - curveBaseDate;
            double zero1 = rq_rate_discount_to_zero(
                pointStart->discount_factor,
				zero_method,
				zero_method_compound_frequency,
                rq_day_count_get_year_fraction(curveDayCountConvention, curveBaseDate, pointStart->date));
            double days2 = pointEnd->date - curveBaseDate; 
            double zero2 = rq_rate_discount_to_zero(
                pointEnd->discount_factor,
				zero_method,
				zero_method_compound_frequency,
                rq_day_count_get_year_fraction(curveDayCountConvention, curveBaseDate, pointEnd->date));
            double zero = rq_interpolate_log_linear(
                (double)(forDate - curveBaseDate),
                days1,
                zero1,
                days2,
                zero2);
            df = rq_rate_zero_to_discount(
                zero,
				zero_method,
				zero_method_compound_frequency,
                rq_day_count_get_year_fraction(curveDayCountConvention, curveBaseDate, forDate)); 
        }
	        break;

        default:
            df = rq_interpolate_linear(
                (double)forDate,
                (double)pointStart->date,
                pointStart->discount_factor,
                (double)pointEnd->date,
                pointEnd->discount_factor
                );
            break;
    }

	return df;
}

RQ_EXPORT double 
rq_yield_curve_get_discount_factor(const rq_yield_curve_t ts, rq_date for_date)
{
    double df = 1.0;
    unsigned long days;
    int apply_additive_factor;
    int apply_multiplicative_factor;
    
    if (!ts || for_date <= ts->from_date)
        return 1.0;	

	/* only apply if non zero - TA */
	apply_additive_factor = ts->additive_factor != 0.0;
	/* only apply if non zero, and not 1.0 - TA */
	apply_multiplicative_factor = (ts->multiplicative_factor != 0.0) && (ts->multiplicative_factor != 1.0);

    days = for_date - ts->from_date;
    if (days < ts->factor_cache_size && !(apply_additive_factor || apply_multiplicative_factor)
                                    && ts->factor_cache[days] != 0.0)
        //ignore cache if using additive or multiplicative factor
        return ts->factor_cache[days];	

    df = 1.0;
    if (ts->yield_curve_type == RQ_YIELD_CURVE_TYPE_DISCOUNTFACTOR)
    {		
        struct rq_yield_curve_elem* el = rq_yield_curve_lower_bound(ts, for_date);
		if (el != ts->discount_factors + ts->num_factors)
		{
            /* Interpolate */
            if (el->date >= for_date)
            {
                /* 
                   If this is the first element or the dates match
                   return this discount factor
                */
                if (el->date == for_date)
                {
                    df = el->discount_factor;
                }
                else if (el == ts->discount_factors)
                {
                    /* OK, for_date is less than than the first date on
                       the curve. We have a number of choices for how
                       to calculate the discount factor here.
                    */
                    switch (ts->curve_start_extrapolation_method)
                    {
                        case RQ_EXTRAPOLATION_LAST_ZERO:
                        {
                            double zero = rq_rate_discount_to_zero(
                                el->discount_factor,
								ts->zero_method,
								ts->zero_method_compound_frequency,
                                rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, el->date)
                                );
                            df = rq_rate_zero_to_discount(
                                zero,
								ts->zero_method,
								ts->zero_method_compound_frequency,
                                rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, for_date)
                                ); 
                        }
                        break;

                        case RQ_EXTRAPOLATION_LINEAR_ZERO:
                        {
							/* TODO JN Doesn't look right. Need an upperUpperBound to linearly extract down. days is wrong. */
                            double days1 = el->date - ts->from_date;
                            double zero = rq_rate_discount_to_zero(
                                el->discount_factor,
								ts->zero_method,
								ts->zero_method_compound_frequency,
                                rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, el->date)
                                );
                            if (ts->num_factors > 1)
                            {
                                double days2 = el->date - ts->from_date;
                                double zero2 = rq_rate_discount_to_zero(
                                    el->discount_factor,
									ts->zero_method,
									ts->zero_method_compound_frequency,
                                    rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, el->date)
                                    );
                                zero = rq_interpolate_linear(
                                    (double)(for_date - ts->from_date),
                                    days1,
                                    zero,
                                    days2,
                                    zero2
                                    );
                            }
                            df = rq_rate_zero_to_discount(
                                zero,
								ts->zero_method,
								ts->zero_method_compound_frequency,
                                rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, for_date)
                                );
                        }
                        break;

                        case RQ_EXTRAPOLATION_LINEAR_ZERO_ZERO:
                        {
                            double days2 = el->date - ts->from_date;
                            double zero2 = rq_rate_discount_to_zero(
                                el->discount_factor,
								ts->zero_method,
								ts->zero_method_compound_frequency,
                                rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, el->date)
                                );

                            /* interolate a zero rate assuming there is a zero
                               rate of 0.0 at the start of the curve.
                            */
                            double zero = rq_interpolate_linear(
                                (double)(for_date - ts->from_date),
                                0.0,
                                0.0,
                                days2,
                                zero2
                                );
                            df = rq_rate_zero_to_discount(
                                zero,
								ts->zero_method,
								ts->zero_method_compound_frequency,
                                rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, for_date)
                                );
                        }
                        break;

                        case RQ_EXTRAPOLATION_LINEAR_DISCOUNT_FACTOR:
                        default:
                            df = rq_interpolate_linear(
                                (double)for_date,
                                (double)ts->from_date,
                                1.0,
                                (double)el->date,
                                el->discount_factor
                                );
                            break;
                    }
                }
                else
                {
					struct rq_yield_curve_elem* pel = el - 1;
 					if (ts->interpolation_method == RQ_INTERPOLATION_SPLINE_DISCOUNT_FACTOR)
					{
						df = rq_curve_interpolation_spline_get_discount_factor(ts->discount_factors, ts->num_factors, for_date);
					}
					else
					{
						df = rq_curve_interpolation_get_discount_factor(ts->interpolation_method, ts->zero_method, ts->zero_method_compound_frequency,
							ts->default_day_count_convention, ts->from_date, pel, el, for_date);
					}
				}
            }
        }
		else
        {
            /* Extrapolate. */
            if (ts->num_factors > 0)
            {
                /*
                  double days = 
                  (double)ts->discount_factors[i-1].date - 
                  (double)ts->discount_factors[i-2].date;
                  double df_delta = 
                  ts->discount_factors[i-1].discount_factor - 
                  ts->discount_factors[i-2].discount_factor;
                  double df_daily_change = df_delta / days;
                  double days_forward = 
                  (double)for_date - 
                  (double)ts->discount_factors[i-1].date;
                  double df = 
                  (double)ts->discount_factors[i-1].discount_factor +
                  (df_daily_change * days_forward);
                */
                switch (ts->curve_end_extrapolation_method)
                {
                    case RQ_EXTRAPOLATION_LINEAR_ZERO:
                    case RQ_EXTRAPOLATION_LINEAR_ZERO_ZERO:
                    {
                        double zero = rq_rate_discount_to_zero(
                            ts->discount_factors[ts->num_factors-1].discount_factor,
							ts->zero_method,
							ts->zero_method_compound_frequency,
                            rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, ts->discount_factors[ts->num_factors-1].date)
                            );
                        if (ts->num_factors > 1)
                        {
                            double days1 = ts->discount_factors[ts->num_factors-1].date - ts->from_date;
                            double days2 = ts->discount_factors[ts->num_factors-2].date - ts->from_date;
                            double zero2 = rq_rate_discount_to_zero(
                                ts->discount_factors[ts->num_factors-2].discount_factor,
								ts->zero_method,
								ts->zero_method_compound_frequency,
                                rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, ts->discount_factors[ts->num_factors-2].date)
                                );
                            zero = rq_interpolate_linear(
                                (double)(for_date - ts->from_date),
                                days2,
                                zero2,
                                days1,
                                zero
                                );
                            }
                        /* df = 1.0 / (1.0 + zero * ((double)(for_date - ts->from_date)) / 365.0); */
                        df = rq_rate_zero_to_discount(
                            zero,
							ts->zero_method,
							ts->zero_method_compound_frequency,
                            rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, for_date)
                            ); 
                    }
                    break;

                    case RQ_EXTRAPOLATION_LAST_ZERO:
                    default:
                    {
                        double zero = rq_rate_discount_to_zero(
                            ts->discount_factors[ts->num_factors-1].discount_factor,
							ts->zero_method,
							ts->zero_method_compound_frequency,
                            rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, ts->discount_factors[ts->num_factors-1].date)
                            );
                        /* double zero = ((1.0 / ts->discount_factors[i-1].discount_factor) - 1.0) * (365.0 / (double)(ts->discount_factors[i-1].date - ts->from_date)); */
                        /* df = 1.0 / (1.0 + zero * (((double)(for_date - ts->from_date)) / 365.0)); */
                        df = rq_rate_zero_to_discount(
                            zero,
							ts->zero_method,
							ts->zero_method_compound_frequency,
                            rq_day_count_get_year_fraction(ts->default_day_count_convention, ts->from_date, for_date)
                            ); 
                    }
                    break;
                }
            }
            /* else if (ts->num_factors == 1)
                df = ts->discount_factors[0].discount_factor; */
            else
                df = 1.0;
        }

        if (df < 1.0)
        {
			/* only apply if non zero additive factor, or non zero & non 1 multiplicative factor - TA */
			if (apply_additive_factor || apply_multiplicative_factor)
			{
                double day_frac = rq_day_count_get_year_fraction(
					ts->default_day_count_convention, 
					ts->from_date, 
					for_date
					);

				double zero = rq_rate_discount_to_zero(
                    df,
					ts->zero_method,
					ts->zero_method_compound_frequency,
                    day_frac
                    );
			
				/* now apply any additive factors */
				if (apply_additive_factor)
					zero += ts->additive_factor;

				/* now apply any multiplicative factors */
				if (apply_multiplicative_factor)
					zero *= ts->multiplicative_factor;

                df = rq_rate_zero_to_discount(
                    zero,
					ts->zero_method,
					ts->zero_method_compound_frequency,
					day_frac
                    ); 
                /* df = 1.0 / (1.0 + (zero * (for_date - ts->from_date) / 365.0)); */
			}
		}
    
    }
    else
    { /* composite */
        double zero = 0.0;
        double day_frac;
        
        if (ts->base_curve && ts->spread_curve)
        {
			double df_base = rq_yield_curve_get_discount_factor(
				ts->base_curve,
		        for_date
		        );
		    double day_count_frac_base = rq_day_count_get_year_fraction(
				ts->default_day_count_convention,
		        ts->base_curve->from_date,
				for_date
		        );
			double base_rate = rq_rate_discount_to_zero(
		        df_base,
				ts->zero_method,
				ts->zero_method_compound_frequency,
				day_count_frac_base
				);

			double df_spread = rq_yield_curve_get_discount_factor(
				ts->spread_curve,
		        for_date
		        );
		    double day_count_frac_spread = rq_day_count_get_year_fraction(
				ts->default_day_count_convention,
		        ts->spread_curve->from_date,
				for_date
		        );
			double spread_rate = rq_rate_discount_to_zero(
		        df_spread,
				ts->zero_method,
				ts->zero_method_compound_frequency,
				day_count_frac_spread
				);


#if 0
            double spread_rate = 0.0;

            rq_date first_spread_date = rq_yield_curve_get_first_date(ts->spread_curve);
            if (first_spread_date)
            {
                if (first_spread_date > for_date)
                {
                    if (for_date > ts->from_date)
                    {
                        /* interpolate between zero and the rate at the first date. */
                        double sr2 = rq_yield_curve_get_simple_rate(
                            ts->spread_curve,
                            first_spread_date,
                            ts->default_day_count_convention
                            );
                        spread_rate = rq_interpolate_linear(
                            for_date,
                            ts->from_date,
                            0.0,
                            first_spread_date,
                            sr2
                            );
                    }
                }
                else
                {
                    spread_rate = rq_yield_curve_get_simple_rate(
                        ts->spread_curve,
                        for_date,
                        ts->default_day_count_convention
                        );
                }
            }
#endif
            zero = base_rate + spread_rate;
            
        }
        else if (ts->base_curve)
        {
			double df_base = rq_yield_curve_get_discount_factor(
				ts->base_curve,
		        for_date
		        );
		    double day_count_frac_base = rq_day_count_get_year_fraction(
				ts->default_day_count_convention,
		        ts->base_curve->from_date,
				for_date
		        );
			zero = rq_rate_discount_to_zero(
		        df_base,
				ts->zero_method,
				ts->zero_method_compound_frequency,
				day_count_frac_base
				);
        }

        /* now apply any additive factors */
		/* only apply if non zero - TA */
        if (ts->additive_factor)
            zero += ts->additive_factor;

        /* now apply any multiplicative factors */
  		/* only apply if non zero, and not 1.0 - TA */
        if (ts->multiplicative_factor && (ts->multiplicative_factor != 1.0))
            zero *= ts->multiplicative_factor;

        day_frac = rq_day_count_get_year_fraction(
            ts->default_day_count_convention,
            ts->from_date,
            for_date
            );

        df = rq_rate_zero_to_discount(
            zero,
			ts->zero_method,
			ts->zero_method_compound_frequency,
            day_frac
            );
	}

    if (days < ts->factor_cache_size && !(apply_additive_factor || apply_multiplicative_factor))
        ts->factor_cache[days] = df;

    return df;
}

RQ_EXPORT double 
rq_yield_curve_get_forward_discount_factor(const rq_yield_curve_t yield_curve, rq_date start_date, rq_date end_date)
{
    /*
      <-- df1 -->
      <-- df -------------->
                 <-- df2 -->

         df = df1 * df2

      => df2 = df / df1
    */
    double df = rq_yield_curve_get_discount_factor(yield_curve, end_date);
    double df1 = rq_yield_curve_get_discount_factor(yield_curve, start_date);

    return (df1 != 0 ? df / df1 : df);
}

RQ_EXPORT void 
rq_yield_curve_set_discount_factor(rq_yield_curve_t ts, rq_date for_date, double discount_factor)
{
    /* grow the array if necessary */
    if (ts->num_factors == ts->max_factors)
    {
        unsigned int new_max_factors = ts->max_factors * 2;
        ts->discount_factors = 
            (struct rq_yield_curve_elem *) realloc(
            ts->discount_factors, 
            new_max_factors * sizeof(struct rq_yield_curve_elem)
            );
        ts->max_factors = new_max_factors;
    }

    /* if this is the first discount factor or the date is greater
       than the last date in the array...
     */
    if (ts->num_factors == 0 || ts->discount_factors[ts->num_factors-1].date < for_date)
    {
        ts->discount_factors[ts->num_factors].date = for_date;
        ts->discount_factors[ts->num_factors].discount_factor = discount_factor;
        ts->num_factors++;
    }
    else
    {
        /** \todo: 
         * Think about replacing the following with a bisection search
         */
        unsigned int i = 0;
        while (i < ts->num_factors)
        {
            if (ts->discount_factors[i].date >= for_date)
            {
                if (ts->discount_factors[i].date > for_date)
                {
                    /* insert the element in here... */
                    unsigned int j = ts->num_factors;
                    while (j > i)
                    {
                        memcpy(&ts->discount_factors[j], &ts->discount_factors[j-1], sizeof(struct rq_yield_curve_elem));

                        j--;
                    }

                    ts->discount_factors[i].date = for_date;
                    ts->num_factors++;
                }

                /* 
                   OK, the following will be executed if the dates are equal
                   OR if we've done an insert.
                */

                ts->discount_factors[i].discount_factor = discount_factor;

                break; /* and exit the loop */
            }

            i++;
        }
    }
}

RQ_EXPORT rq_yield_curve_t 
rq_yield_curve_clone(const rq_yield_curve_t ts)
{
    unsigned int i;
    struct rq_yield_curve *ts_clone = rq_yield_curve_alloc(ts->max_factors);

    ts_clone->termstruct.termstruct_id = RQ_STRDUP(ts->termstruct.termstruct_id);
    if (ts->termstruct.underlying_asset_id)
        ts_clone->termstruct.underlying_asset_id = RQ_STRDUP(ts->termstruct.underlying_asset_id);

    ts_clone->from_date = ts->from_date;
    ts_clone->num_factors = ts->num_factors;

    for (i = 0; i < ts->num_factors; i++)
    {
        ts_clone->discount_factors[i].date = ts->discount_factors[i].date;
        ts_clone->discount_factors[i].discount_factor = ts->discount_factors[i].discount_factor;
    }

    ts_clone->interpolation_method = ts->interpolation_method;

    /* How do I clone the base and spread curves?? They will probably
     * have been cloned seperately and need to be set again!
     */
    ts_clone->yield_curve_type = ts->yield_curve_type;
    ts_clone->base_curve = 0;
    ts_clone->spread_curve = 0;

    ts_clone->additive_factor = ts->additive_factor;
    ts_clone->multiplicative_factor = ts->multiplicative_factor;

    ts_clone->curve_start_extrapolation_method = ts->curve_start_extrapolation_method;
    ts_clone->curve_end_extrapolation_method = ts->curve_end_extrapolation_method;

	ts_clone->zero_method = ts->zero_method;
	ts_clone->zero_method_compound_frequency = ts->zero_method_compound_frequency;

    ts_clone->default_day_count_convention = ts->default_day_count_convention;
    
    return ts_clone;
}

RQ_EXPORT rq_date 
rq_yield_curve_get_first_date(const rq_yield_curve_t yc)
{
    if (yc->num_factors > 0)
        return yc->discount_factors[0].date;
    return 0;
}

RQ_EXPORT rq_date 
rq_yield_curve_get_last_date(const rq_yield_curve_t ts)
{
    if (ts->num_factors > 0)
        return ts->discount_factors[ts->num_factors-1].date;
    return 0;
}

RQ_EXPORT double 
rq_yield_curve_get_simple_rate(
    const rq_yield_curve_t yield_curve,
    rq_date date,
    enum rq_day_count_convention day_count_convention
    )
{
    double df = rq_yield_curve_get_discount_factor(
        yield_curve,
        date
        );
    double day_count_frac = rq_day_count_get_year_fraction(
        day_count_convention,
        yield_curve->from_date,
        date
        );
    double rate = rq_rate_discount_to_zero(
        df,
		RQ_ZERO_SIMPLE,
		0,
        day_count_frac
        );
    return rate;
}

RQ_EXPORT double 
rq_yield_curve_get_par_rate(
    const rq_yield_curve_t yield_curve,
    unsigned int num_dates,
    rq_date *dates,
    unsigned int periods_per_year
    )
{
    double r = 0.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        double last_df = rq_yield_curve_get_discount_factor(
                yield_curve,
                dates[num_dates-1]
                );
        unsigned int i;
        for (i = 0; i < num_dates - 1; i++)
        {
            double df = rq_yield_curve_get_discount_factor(
                yield_curve,
                dates[i]
                );
            sum_df += df;
        }

        sum_df += last_df;

        r = ((1.0 - last_df) * (double)periods_per_year) / sum_df;

        /* r /= 100.0; */
    }

    return r;
}

RQ_EXPORT double 
rq_yield_curve_get_par_rate_period(
    const rq_yield_curve_t yield_curve,
    unsigned int num_dates,
    rq_date *dates,
    double period
    )
{
    double r = 0.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        double last_df = rq_yield_curve_get_discount_factor(
                yield_curve,
                dates[num_dates-1]
                );
        unsigned int i;
        for (i = 0; i < num_dates - 1; i++)
        {
            double df = rq_yield_curve_get_discount_factor(
                yield_curve,
                dates[i]
                );
            sum_df += df;
        }

        sum_df += last_df;

        r = ((1.0 - last_df) / period) / sum_df;

        /* r /= 100.0; */
    }

    return r;
}

RQ_EXPORT double 
rq_yield_curve_get_par_rate_day_count(
    const rq_yield_curve_t yield_curve,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention *day_counts
    )
{
    double r = 0.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        double year_count_frac = 1.0;

        rq_date last_date = dates[num_dates-1];
        double last_df = rq_yield_curve_get_discount_factor(
                yield_curve,
                last_date
                );

        unsigned int i;
        for (i = 1; i < num_dates - 1; i++)
        {
            double df = rq_yield_curve_get_discount_factor(
                yield_curve,
                dates[i]
                );

            year_count_frac = rq_day_count_get_year_fraction(day_counts[i], dates[i-1], dates[i]);
            sum_df += df * year_count_frac;
        }

        if (num_dates > 1)
            year_count_frac = rq_day_count_get_year_fraction(day_counts[num_dates-1], dates[num_dates-2], last_date);

        sum_df += last_df * year_count_frac;

        r = (1.0 - last_df) / sum_df;
    }

    return r;
}


RQ_EXPORT double 
rq_yield_curve_get_accrual(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    unsigned int periods_per_year
    )
{
    double r = 0.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        unsigned int i;
        for (i = 1; i < num_dates; i++)
        {
            double df = rq_yield_curve_get_forward_discount_factor(
                yield_curve,
                from_date,
                dates[i]
                );
            sum_df += df;
        }

        r = sum_df / (double)periods_per_year;
    }

    return r;
}

RQ_EXPORT double 
rq_yield_curve_get_accrual_day_count(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    )
{
    double r = 0.0;
    double year_count_frac = 1.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        unsigned int i;
        for (i = 1; i < num_dates; i++)
        {
            double df = rq_yield_curve_get_forward_discount_factor(
                yield_curve,
                from_date,
                dates[i]
                );
			year_count_frac = rq_day_count_get_year_fraction(day_count, dates[i-1], dates[i]);
            sum_df += df * year_count_frac;
        }

        r = sum_df;
    }

    return r;
}

RQ_EXPORT double 
rq_yield_curve_get_forward_par_rate(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    unsigned int periods_per_year
    )
{
    //TODO: should just merge with rq_yield_curve_get_par_rate_period
    //      rq_yield_curve_get_forward_par_rate_period(yield_curve, num_dates, dates, 1.0/periods_per_year);

    double r = 0.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        double last_df = rq_yield_curve_get_forward_discount_factor(
                yield_curve,
                from_date,
                dates[num_dates-1]
                );
        unsigned int i;
        for (i = 0; i < num_dates - 1; i++)
        {
            if (dates[i] > from_date)
            {
                double df = rq_yield_curve_get_forward_discount_factor(
                    yield_curve,
                    from_date,
                    dates[i]
                    );
                sum_df += df;
            }
        }

        sum_df += last_df;

        r = ((1.0 - last_df) * (double)periods_per_year) / sum_df;

        /* r /= 100.0; */
    }

    return r;
}

RQ_EXPORT double 
rq_yield_curve_get_forward_par_rate_period(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    double period
    )
{
    double r = 0.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        double last_df = rq_yield_curve_get_forward_discount_factor(
                yield_curve,
                from_date,
                dates[num_dates-1]
                );
        unsigned int i;
        for (i = 0; i < num_dates - 1; i++)
        {
            if (dates[i] > from_date)
            {
                double df = rq_yield_curve_get_forward_discount_factor(
                    yield_curve,
                    from_date,
                    dates[i]
                    );
                sum_df += df;
            }
        }

        sum_df += last_df;

        r = ((1.0 - last_df) / period) / sum_df;

        /* r /= 100.0; */
    }

    return r;
}

RQ_EXPORT double 
rq_yield_curve_get_forward_par_rate_day_count(
    const rq_yield_curve_t yield_curve,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    )
{
double r = 0.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        double year_count_frac = 1.0;
        double last_df = rq_yield_curve_get_forward_discount_factor(
                yield_curve,
                from_date,
                dates[num_dates-1]
                );
        unsigned int i;
        for (i = 1; i < num_dates - 1; i++)
        {
            if (dates[i] > from_date)
            {
                double df = rq_yield_curve_get_forward_discount_factor(
                    yield_curve,
                    from_date,
                    dates[i]
                    );
				year_count_frac = rq_day_count_get_year_fraction(day_count, dates[i-1], dates[i]);
                sum_df += df * year_count_frac;
            }
        }

		if (num_dates > 1)
            year_count_frac = rq_day_count_get_year_fraction(day_count, dates[num_dates-2], dates[num_dates-1]);

        sum_df += last_df * year_count_frac;

        r = (1.0 - last_df) / sum_df;
    }

    return r;
}

RQ_EXPORT double 
rq_yield_curve_get_forward_simple_rate(
    const rq_yield_curve_t yield_curve,
    rq_date start_date,
    rq_date end_date,
    enum rq_day_count_convention day_count_convention
    )
{
    /*
      effective_rate = rate / num_compounding_periods_per_year

      df = 1.0 / (1.0 + effective_rate)^num_compounding_periods_per_year
     
     */
    double fwd_df = rq_yield_curve_get_forward_discount_factor(
        yield_curve,
        start_date,
        end_date
        );
    double day_count_frac = rq_day_count_get_year_fraction(
        day_count_convention,
        start_date,
        end_date
        );
    double rate = rq_rate_discount_to_zero(
        fwd_df,
		RQ_ZERO_SIMPLE,
		0,
        day_count_frac
        );
    return rate;
}

RQ_EXPORT double 
rq_yield_curve_get_continuous_rate(
    const rq_yield_curve_t yield_curve,
    rq_date start_date,
    rq_date end_date,
    enum rq_day_count_convention day_count_convention
    )
{
    /*
      effective_rate = rate / num_compounding_periods_per_year

      df = 1.0 / (1.0 + effective_rate)^num_compounding_periods_per_year
     
     */
    double fwd_df = rq_yield_curve_get_forward_discount_factor(
        yield_curve,
        start_date,
        end_date
        );
    double day_count_frac = rq_day_count_get_year_fraction(
        day_count_convention,
        start_date,
        end_date
        );
    double rate = -1.0 * log(fwd_df) / day_count_frac;
    return rate;
}


RQ_EXPORT double 
rq_yield_curve_get_forward_effective_rate(
    const rq_yield_curve_t yield_curve,
    rq_date start_date,
    rq_date end_date
    )
{
    /*
      effective_rate = rate / num_compounding_periods_per_year

      df = 1.0 / (1.0 + effective_rate)^num_compounding_periods_per_year
     
     */
    double fwd_df = rq_yield_curve_get_forward_discount_factor(
        yield_curve,
        start_date,
        end_date
        );
    double fwd_af = 1.0 / fwd_df;
    double rate = (fwd_af - 1.0);
    return rate;
}

RQ_EXPORT unsigned int 
rq_yield_curve_size(const rq_yield_curve_t ts)
{
    return ts->num_factors;
}

RQ_EXPORT struct rq_yield_curve_elem *
rq_yield_curve_element_at(
    const rq_yield_curve_t ts,
    unsigned int offset
    )
{
    return &ts->discount_factors[offset];
}

void
rq_yield_curve_remove_element_at(
    rq_yield_curve_t ts,
    unsigned int offset
    )
{
	rq_yield_curve_remove_element_range(ts, offset, offset);
}

void
rq_yield_curve_remove_element_range(
    rq_yield_curve_t ts,
    unsigned int from_offset,
    unsigned int to_offset
    )
{
	unsigned int count = to_offset - from_offset + 1;
	unsigned int remaining = ts->num_factors - to_offset - 1;
	if (remaining > 0)
	{
		memmove(&ts->discount_factors[from_offset], &ts->discount_factors[to_offset + 1], remaining * sizeof(struct rq_yield_curve_elem));
	}
	memset(&ts->discount_factors[ts->num_factors - count], 0, count * sizeof(struct rq_yield_curve_elem));
	ts->num_factors -= count;
}

RQ_EXPORT int
rq_yield_curve_is_null(rq_yield_curve_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_date 
rq_yield_curve_elem_get_date(const struct rq_yield_curve_elem *e)
{
    return e->date;
}

RQ_EXPORT double 
rq_yield_curve_elem_get_discount_factor(const struct rq_yield_curve_elem *e)
{
    return e->discount_factor;
}


RQ_EXPORT enum rq_yield_curve_type
rq_yield_curve_get_yield_curve_type(const rq_yield_curve_t ts)
{
    return ts->yield_curve_type;
}

RQ_EXPORT void
rq_yield_curve_set_yield_curve_type(
    const rq_yield_curve_t ts,
    enum rq_yield_curve_type yield_curve_type
    )
{
    ts->yield_curve_type = yield_curve_type;
}

RQ_EXPORT rq_yield_curve_t
rq_yield_curve_get_base_curve(rq_yield_curve_t ts)
{
    return ts->base_curve;
}

RQ_EXPORT void
rq_yield_curve_set_base_curve(
    rq_yield_curve_t ts,
    rq_yield_curve_t base_curve
    )
{
    ts->base_curve = base_curve;
}

RQ_EXPORT rq_yield_curve_t
rq_yield_curve_get_spread_curve(rq_yield_curve_t ts)
{
    return ts->spread_curve;
}

RQ_EXPORT void
rq_yield_curve_set_spread_curve(
    rq_yield_curve_t ts,
    rq_yield_curve_t spread_curve
    )
{
    ts->spread_curve = spread_curve;
}

RQ_EXPORT double
rq_yield_curve_get_additive_factor(rq_yield_curve_t ts)
{
    return ts->additive_factor;
}

RQ_EXPORT void
rq_yield_curve_set_additive_factor(
    rq_yield_curve_t ts,
    double additive_factor
    )
{
    ts->additive_factor = additive_factor;
}

RQ_EXPORT double
rq_yield_curve_get_multiplicative_factor(
    rq_yield_curve_t ts
    )
{
    return ts->multiplicative_factor;
}

RQ_EXPORT void
rq_yield_curve_set_multiplicative_factor(
    rq_yield_curve_t ts,
    double multiplicative_factor
    )
{
    ts->multiplicative_factor = multiplicative_factor;
}

RQ_EXPORT void
rq_yield_curve_set_composite(
    rq_yield_curve_t yc,
    rq_yield_curve_t base_curve,
    rq_yield_curve_t spread_curve
    )
{
    yc->yield_curve_type = RQ_YIELD_CURVE_TYPE_COMPOSITE;
    yc->base_curve = base_curve;
    yc->spread_curve = spread_curve;
}

RQ_EXPORT void 
rq_yield_curve_set_default_day_count_convention(
    rq_yield_curve_t yc,
    enum rq_day_count_convention day_count
    )
{
    yc->default_day_count_convention = day_count;
}

RQ_EXPORT enum rq_day_count_convention 
rq_yield_curve_get_default_day_count_convention(rq_yield_curve_t yc)
{
    return yc->default_day_count_convention;
}
