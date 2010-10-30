/*
** rq_bootstrap_yield_curve_day_count.c
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
#include "rq_bootstrap_yield_curve_day_count.h"
#include "rq_bootstrap_yield_curve.h"
#include "rq_bootstrap_config.h" 
#include "rq_asset_mgr.h"
#include "rq_asset_ccy.h"
#include "rq_asset_irovernight.h"
#include "rq_asset_irdiscount.h"
#include "rq_asset_irbond.h"
#include "rq_asset_irfuture.h"
#include "rq_asset_irswap.h"
#include "rq_asset_credit_spread.h"
#include "rq_asset_list.h"
#include "rq_date_roll.h"
#include "rq_defs.h"
#include "rq_interpolate.h"
#include "rq_day_count.h"
#include "rq_vector.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

/* #define DEBUG_BOOTSTRAP */ /* defining this gives a helpful printout of the assets, points and attributes that make up the yc */
#ifdef DEBUG_BOOTSTRAP
#define DEBUG_BOOTSTRAP_FH
#define debugFile "debugRqYC.log"
#endif

static void
_rq_yc_day_count_mgr_free_func(void *p)
{
    free(p);
}

static int
_rq_yc_day_count_mgr_cmp_func(const void *p1, const void *p2)
{
    rq_date d1 = (rq_date)p1;
    rq_date d2 = (rq_date)p2;
    return d1 - d2;
}

void
rq_yc_day_count_mgr_add(rq_yc_day_count_mgr day_count_mgr, rq_date date, enum rq_day_count_convention day_count)
{
    rq_yc_day_count_t dc = (rq_yc_day_count_t)RQ_MALLOC(sizeof(struct rq_yc_day_count));
    dc->date = date;
    dc->convention = day_count;
    rq_array_push_back(day_count_mgr, dc);
}

enum rq_day_count_convention
rq_yc_day_count_mgr_find(rq_yc_day_count_mgr day_count_mgr, rq_date date)
{
    rq_yc_day_count_t result = NULL;
    rq_yc_day_count_t p = NULL;

    int count = rq_array_size(day_count_mgr);
    int i;

    /* TODO: since sorted, can use binary search instead of linear */
    for (i = count - 1; i >= 0; --i)
    {
        rq_yc_day_count_t p = (rq_yc_day_count_t)rq_array_get_at(day_count_mgr, i);
        if (p->date == date)
            return p->convention;
        else if (date < p->date)
            result = p;
        else
            break;
    }

    if (result)
        return result->convention;
    else
        return (enum rq_day_count_convention)NULL;
}

/* TODO: remove as asset now supports start tenor offset,
         so no need for the "RQ_RATE_TYPE_PAR" fudge */
double
bootstrapParDCF(const rq_asset_t asset,
              const rq_rate_t rate,
              rq_date* last_date_bootstrapped,
              rq_yield_curve_t ts,
		      const rq_calendar_t* cals,
              unsigned short num_cals,
              rq_rate_t lastRate,
              rq_asset_t lastAsset,
              double lastDF)
{
    long fromDate = 0;
    long maturityDateUnadjusted = 0, maturityDate = 0, startDate = 0;
    double day_count_frac = 0.0, dayCountFracActual = 0.0;
	double df = 0.0, r = 0.0;
	struct rq_term year_term;

    startDate = rq_rate_get_observation_date(rate);
    maturityDate = rq_rate_get_value_date(rate);
    if (num_cals > 0) //got a curve calendar, will override what's done in rate factory
    {
        startDate = rq_date_roll_offset_term(startDate,
                                             rq_asset_irswap_get_start_tenor_type(asset),
                                             rq_asset_irswap_get_start_tenor(asset),
                                             cals,
                                             num_cals,
                                             rq_asset_irswap_get_date_roll_convention(asset));

        maturityDate = rq_date_roll_get_date(startDate, 
                                             rq_asset_irdiscount_get_tenor(asset),
			                                 RQ_ROLL_CONVENTION_NONE,
                                             rq_asset_irdiscount_get_date_roll_convention(asset),
                                             cals,
			                                 num_cals);
    }

    // this gets the unadjusted maturity date to determine if it's the 1yr point
	// and hence use simple interest instead of compounding 
    maturityDateUnadjusted = rq_date_roll_get_date(startDate,
                                                   rq_asset_irdiscount_get_tenor(asset),
                                                   RQ_ROLL_CONVENTION_NONE,
                                                   RQ_DATE_ROLL_NO_ADJUSTMENT,
                                                   cals,
				                                   num_cals);

    fromDate = ts->from_date;
    if (!RQ_IS_NULL(lastRate) && rq_rate_get_rate_type(lastRate) == RQ_RATE_TYPE_PAR)
    {
        fromDate = rq_rate_get_value_date(lastRate);
    }

    day_count_frac = rq_day_count_get_year_fraction(rq_asset_irdiscount_get_day_count_convention(asset),
                                                    fromDate,
                                                    maturityDate);

    r = rq_rate_get_value(rate);

	rq_term_fill(&year_term, 0, 0, 0, 1);

	if(rq_date_add_term(startDate, &year_term) < maturityDateUnadjusted)
    {
        df = 1.0 / pow(1.0 + r, day_count_frac);
    }
    else
    {
        df = 1.0 / (1.0 + r * day_count_frac);
    }

    if (fromDate > ts->from_date)
    {
        df *= lastDF;
    }

    *last_date_bootstrapped = maturityDate;
    return df;
}

// Setup spline arrays, then calculate second derivatives.
// Outputs : splineX splineY splineY2 splineNum
static int rq_generate_swap_splines(enum rq_swapParInterpolation_method parInterpolationMethod, const dates[], rq_yield_curve_t ts, const rq_asset_t splineSwapAssets[], const rq_rate_t splineSwapRates[],
    int numSwaps, int num_discount_factors, rq_date last_date_on_curve, rq_date start_date, double dfStart, int simple_daycount,
    double splineX[], double splineY[], double splineY2[], double year_count_frac, enum rq_day_count_convention day_count,
    const rq_calendar_t* base_cals, unsigned short base_num_cals, rq_calendar_mgr_t calendar_mgr)
{
    int j, splineNum, swap_i = 0, passed_last_date = 0;
	double df = 0.0, sum_dfs = 0.0;
    // Add each roll date and each swap rate to the spline arrays.
    for (j = 0; j < num_discount_factors+numSwaps && j < RQ_MAX_SPLINE_POINTS-1 && swap_i < numSwaps; j++)
    {
		// Note if parInterpolationMethod is RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE_SWAPS_ONLY then does not add cash rates to interpolation points.
        if (parInterpolationMethod == RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE && !passed_last_date && (dates[j+1] <= last_date_on_curve || j == 0))
        {
            // We must do this separately for each swap since they could have different tenors or roll conv
            df = rq_yield_curve_get_discount_factor(ts, dates[j+1]);
            splineX[j+1] = dates[j+1];
            if(!simple_daycount)
            {
                year_count_frac = rq_day_count_get_year_fraction(day_count, dates[j], dates[j+1]);
                splineX[j+1] = rq_day_count_get_year_fraction(day_count, start_date, (rq_date)splineX[j+1]);
            }
            sum_dfs += df * year_count_frac;
            splineY[j+1] = (1.0 - df / dfStart) / (sum_dfs / dfStart); // Par rate
        }
        else
        {
            rq_date this_start_date;
            rq_asset_t asset = splineSwapAssets[swap_i];
            int num_cals, c;
            rq_calendar_t cals[3];
            num_cals = base_num_cals;
            for(c=0; c < num_cals; c++)
                cals[c] = base_cals[c];
            if(rq_asset_irswap_get_additional_calendar(asset))
            {
                cals[num_cals] = rq_calendar_mgr_get(calendar_mgr, rq_asset_irswap_get_additional_calendar(asset));
                if(cals[num_cals])
                    num_cals++;
            }
            this_start_date = rq_date_roll_offset_term(ts->from_date,
                                                       rq_asset_irswap_get_start_tenor_type(asset),
                                                       rq_asset_irswap_get_start_tenor(asset),
                                                       cals,
                                                       num_cals,
                                                       rq_asset_irswap_get_date_roll_convention(asset));

            splineX[j+1] = rq_date_roll_get_date(
                this_start_date,
                rq_asset_irswap_get_tenor(asset),
                RQ_ROLL_CONVENTION_NONE,
                rq_asset_irswap_get_date_roll_convention(asset),
                cals,
                num_cals);
            if(!simple_daycount)
                splineX[j+1] = rq_day_count_get_year_fraction(day_count, start_date, (rq_date)splineX[j+1]);
            splineY[j+1] = rq_rate_get_value(splineSwapRates[swap_i]);
            swap_i++;
			passed_last_date = 1;
            // If this swap rate is in the spline array then ignore it.
            if(splineX[j+1] <= splineX[j])
                j--;
        }
    }
    splineNum = j;
    // Generate spline coefficients (2nd derivatives).
    rq_interpolate_spline(1,    //natural
                        splineX,        // swapDates
                        splineY,        // swap rates
                        splineNum, // number of input points
                        0,
                        0,
                        splineY2);  // output derivatives
    return splineNum;
}

/*
	METHOD
	Curve Building:
	1. Bootstrap all points upto the first swap asset. This includes cash and futures rates.
	2. For each swap asset...
	3. Generate roll dates for the first swap
	4. Calculate par rates for all points prior to the first swap rate. If this involves interpolation,
		then linear interpolate the point
	5. Strap each interpolated swap par rate to the rest of the points (prior to the first swap rate)
		in the curve.

	Assumptions (TODO):
	1. Implied (generated swap par rates) all fall on similar days
	2. Paramatrise the cache size
	3. Support only for cash deposits, futures and swaps
	4. Assets are sorted, whereby the swaps are ordered as well after cash deposits/futures
*/

void
processSwapsDCF(const rq_asset_t asset,
                const rq_rate_t rate,
                rq_date* last_date_bootstrapped,
                rq_yield_curve_t ts,
                const rq_calendar_t* base_cals,
                unsigned short base_num_cals,
				rq_calendar_mgr_t calendar_mgr,
                rq_date* last_swap_date,
                double* last_swap_rate,
                double* last_year_count_frac,
                enum rq_swapDayCount_method swapDayCount_method,
				enum rq_swapParInterpolation_method parInterpolation_method,
				const rq_asset_t *splineSwapAssets,
				const rq_rate_t *splineSwapRates,
				int numSwaps)
{
    /*
      Work out dates that the payments fall on for the swap.
      Calculate discount factors for all the dates except 
      the last date.
    */
    rq_date dates[RQ_YIELD_CURVE_MAX_FACTORS];
    double splineX[RQ_MAX_SPLINE_POINTS];
    double splineY[RQ_MAX_SPLINE_POINTS];
    double splineY2[RQ_MAX_SPLINE_POINTS];
    int splineNum;
	int num_cals;
	rq_calendar_t cals[3];
    int simple_daycount = swapDayCount_method == RQ_SWAP_DAY_COUNT_SIMPLE;
    FILE *debug_fp = 0;
	int no_interpolation;

    double r = rq_rate_get_value(rate);
    double df;

    rq_date start_date = ts->from_date; /* rq_rate_get_observation_date(rate); */
    double dfStart = 1.0;
    int num_dates;
    unsigned short num_discount_factors;
    double last_df = 1.0;
    unsigned short j;
    rq_date last_date_on_curve = rq_yield_curve_get_last_date(ts);
    double swap_period = rq_term_get_annualized_period(rq_asset_irswap_get_frequency(asset));
    double year_count_frac = swap_period;
    enum rq_day_count_convention day_count = rq_asset_irswap_get_day_count_convention(asset);
    double sum_dfs = 0.0;
    double x, x1, x2;
    rq_date lastDateStrapped;
	num_cals = base_num_cals;
	for(j=0; j < num_cals; j++)
		cals[j] = base_cals[j];
	if(rq_asset_irswap_get_additional_calendar(asset))
	{
		cals[num_cals] = rq_calendar_mgr_get(calendar_mgr, rq_asset_irswap_get_additional_calendar(asset));
		if(cals[num_cals])
			num_cals++;
	}

    start_date = rq_date_roll_offset_term(start_date,
                                          rq_asset_irswap_get_start_tenor_type(asset),
                                          rq_asset_irswap_get_start_tenor(asset),
                                          cals,
                                          num_cals,
                                          rq_asset_irswap_get_date_roll_convention(asset));

    if (start_date > ts->from_date)
        dfStart = rq_yield_curve_get_discount_factor(ts, start_date);

    lastDateStrapped = rq_date_roll_get_date(
        start_date,
        rq_asset_irswap_get_tenor(asset),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(asset),
        cals,
        num_cals
        );

    num_dates = rq_date_roll_generate_dates(
        dates,
        RQ_YIELD_CURVE_MAX_FACTORS,
        start_date,
        lastDateStrapped,
        rq_asset_irswap_get_frequency(asset),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(asset),
        cals,
        num_cals,
        RQ_DATE_ROLL_STUB_POSITION_NONE,
        15, /* allow 15 days error on date creation */
		NULL
        );

    /* ignore the first date */
    num_discount_factors = (unsigned short)(num_dates - 2);
    if(ts->debug_filename)
        debug_fp = fopen(ts->debug_filename, "a");

	if(debug_fp)
	{
		char buf[64];
		time_t ltime;
		time( &ltime );
		ctime_s(buf, 64, &ltime);
		fprintf(debug_fp, "%s : START bootstrapSwapAsset() ", buf);
		fprintf(debug_fp, "curve_id: %s asset_id: %s date: %s\n", rq_yield_curve_get_curve_id(ts), 
			rq_asset_get_asset_id(asset), rq_date_to_string(buf, "yyyymmdd", lastDateStrapped));
	}

    // If swap par rate interpolation method is cubic spline then setup spline points
    if(splineSwapRates)
    {
        splineNum = rq_generate_swap_splines(parInterpolation_method, dates, ts, splineSwapAssets, splineSwapRates, numSwaps,
			num_discount_factors, last_date_on_curve, start_date, dfStart, simple_daycount, 
			splineX, splineY, splineY2, swap_period, day_count, base_cals, base_num_cals, calendar_mgr);
        if(debug_fp)
        {
			fprintf(debug_fp, "Spline interpolation array X Y Y2 :\n");
			for(j = 1; j <= splineNum; j++)
				fprintf(debug_fp, "%d %.17lf %.17lf %.17lf\n", j, splineX[j], splineY[j], splineY2[j]);
        }
    }

	last_df = dfStart;
	// If 1 then no interpolation occurs and the swap par rate goes into each roll date.
	no_interpolation = ts->num_factors == 0;
    for (j = 0; j < num_discount_factors; j++)
    {
        if (dates[j+1] <= last_date_on_curve)
        {
			// This is wrong. The swap rate should be inserted before this.
            df = rq_yield_curve_get_discount_factor(ts, dates[j+1]);
        }
        else
        {
            /* need to imply a swap rate for this point */
            double par_rate_prev;
            double par_rate;
            rq_date date_prev;

            if (*last_swap_rate == 0.0 || *last_year_count_frac != year_count_frac)
            {
				if(sum_dfs == 0.0)
				{
					// First roll point is at ts->from_date so no par rate can be computed. Use first df point to calculate.
					df = ts->discount_factors[0].discount_factor;
					par_rate_prev = (1.0 - df) / (df * rq_day_count_get_year_fraction(day_count, ts->from_date, ts->discount_factors[0].date));
				}
				else
					par_rate_prev = (1.0 - last_df / dfStart) / (sum_dfs / dfStart);
                date_prev = dates[j];
            }
            else
            {
                par_rate_prev = *last_swap_rate;
                date_prev = *last_swap_date;
            }

            /* now linearly interpolate a new swap rate for
               the date we actually want */
            if(simple_daycount)
            {
                /* With BootstrapYieldCurveSimple each leg is assumed to have the same dcf. */
                x2 = lastDateStrapped;
                x1 = date_prev;
                x = dates[j+1];
                year_count_frac = swap_period;
            }
            else
            {
                /* This is inconsistent with the sum_df and will be raised a separate issue. For now no change to the functionality. Forge reference is: 178664 Enhanced bootstrap algorithm for swap. */
                /* Also in the forge case the example spreadsheet and spec differ, Spreadsheet uses start_date difference, spec uses relative dcf. */

                x2 = rq_day_count_get_year_fraction(day_count, start_date, lastDateStrapped);
                x1 = rq_day_count_get_year_fraction(day_count, start_date, date_prev);
                x = rq_day_count_get_year_fraction(day_count, start_date, dates[j+1]);
                year_count_frac = rq_day_count_get_year_fraction(day_count, start_date, dates[j+1])
                                - rq_day_count_get_year_fraction(day_count, start_date, dates[j]);
            }

			// ts->num_factors == 0 special case only swap points in curve so interpolation is not possible.
			// This allows us to bootstrap a curve containing only swaps.
			if(no_interpolation)
			{
				par_rate = r;
				// We also need to define dfStart or this will always be 1.
				if(start_date > ts->from_date)
				{
					dfStart = 1.0 / (1 + r * rq_day_count_get_year_fraction(day_count, ts->from_date, start_date));
					// And also add it into the curve.
					setDiscountFactor(ts, start_date, dfStart);
				}
			}
			else
				if(splineSwapRates)
					par_rate = rq_interpolate_splint(x, splineX, splineY, splineY2, splineNum);
				else
					par_rate = rq_interpolate_linear(x, x1, par_rate_prev, x2, r);

            /* now add the discount factor implied by this
               par rate to the yield curve */
            df = (dfStart - sum_dfs * par_rate) / (1 + par_rate * year_count_frac);
            setDiscountFactor(ts, dates[j+1], df);
			if(debug_fp)
			{
				char buf[16];
				fprintf(debug_fp, "interpolation %d date %s par_rate %.17lf x %.17lf df %.17lf dfStart %.17lf sum_dfs %.17lf year_count_frac %.17lf\n",
					j, rq_date_to_string(buf, "yyyymmdd", dates[j+1]), par_rate, x, df, dfStart, sum_dfs, year_count_frac);
			}
        }
        if(simple_daycount)
            sum_dfs += df * swap_period;
        else
            sum_dfs += df * rq_day_count_get_year_fraction(day_count, dates[j], dates[j+1]);
        last_df = df;
    }

    if(!simple_daycount)
        year_count_frac = rq_day_count_get_year_fraction(day_count, start_date, dates[num_discount_factors+1])
                        - rq_day_count_get_year_fraction(day_count, start_date, dates[num_discount_factors]);
    df = (dfStart - sum_dfs * r) / (1 + r * year_count_frac);

    setDiscountFactor(ts, lastDateStrapped, df);

    *last_swap_date = lastDateStrapped;
    *last_swap_rate = r;
    *last_year_count_frac = year_count_frac;
    *last_date_bootstrapped = lastDateStrapped;
	if(debug_fp)
	{
		char buf[64];
		time_t ltime;
		time( &ltime );
		ctime_s(buf, 64, &ltime);
		fprintf(debug_fp, "%s : END bootstrapSwapAsset() ", buf);
		fprintf(debug_fp, "curve_id: %s asset_id: %s\n\n",
			rq_yield_curve_get_curve_id(ts), rq_asset_get_asset_id(asset));
		fclose(debug_fp);
	}
}

RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_day_count(
    const char *curve_id,
    const rq_system_t system,
    const rq_market_t market,
    double *rate_adjust_factors,
    rq_date last_date_to_bootstrap,
    int force_backward_compatible_daycount,
	const char *debug_filename
    )
{
    unsigned int i;
    rq_date date = rq_market_get_market_date(market);
    const rq_rate_mgr_t rate_mgr = rq_market_get_rate_mgr(market);
    const rq_asset_mgr_t asset_mgr = rq_system_get_asset_mgr(system); 
    rq_calendar_t cal;
    const rq_bootstrap_config_t config = rq_bootstrap_config_mgr_find(rq_system_get_bootstrap_config_mgr(system), curve_id, RQ_TERMSTRUCT_TYPE_YIELD_CURVE); 

    rq_date last_date_bootstrapped = 0;
    double last_swap_rate = 0.0;
    rq_date last_swap_date = 0;
    double last_year_count_frac = 0.0;
    rq_calendar_t cals[2];
    unsigned short num_cals = 0;	
    const char *underlying_asset_id = NULL;
	double r=0;
	rq_asset_t lastAsset = NULL;
	rq_rate_t lastRate = NULL;
	rq_asset_t lastFutureAsset = NULL;
	rq_rate_t lastFutureRate = NULL;
	double lastDF = 0.0;
	double lastFutureDF = 0.0;
    rq_asset_t splineSwapAssets[RQ_MAX_SWAP_RATES];
    rq_rate_t splineSwapRates[RQ_MAX_SWAP_RATES];
    int numSplineSwaps = 0;
    enum rq_swapDayCount_method swapDayCount_method;
	enum rq_swapParInterpolation_method parInterp_method;

#ifdef DEBUG_BOOTSTRAP
#ifdef DEBUG_BOOTSTRAP_FH
    FILE *fh = fopen(debugFile, "a");
#else
	FILE *fh = stdout;
#endif
#endif
    rq_yield_curve_t ts;

    if (!config)
        return NULL;

    if(force_backward_compatible_daycount)
        swapDayCount_method = RQ_SWAP_DAY_COUNT_DAYCOUNT;
    else
        swapDayCount_method = rq_bootstrap_config_get_swapDayCount_method(config);

	parInterp_method = rq_bootstrap_config_get_swapParInterpolation_method(config);

    //first try curve calendar
    cal = rq_calendar_mgr_get(rq_system_get_calendar_mgr(system), curve_id);
    if (cal)
    {
        cals[num_cals++] = cal;
    }
    else
    {
        //no curve calendar, so try curve asset
        cal = rq_calendar_mgr_get(rq_system_get_calendar_mgr(system), rq_bootstrap_config_get_asset_id(config));
        if (cal)
            cals[num_cals++] = cal;
    }

    ts = rq_yield_curve_init(
        rq_bootstrap_config_get_curve_id(config),
		rq_bootstrap_config_get_interpolation_method(config),
		rq_bootstrap_config_get_extrapolation_method_start(config),
		rq_bootstrap_config_get_extrapolation_method_end(config),
		rq_bootstrap_config_get_zero_method(config),
		rq_bootstrap_config_get_zero_method_compound_frequency(config),
		rq_bootstrap_config_get_default_day_count_convention(config),
        date
        );
	rq_yield_curve_set_debug_filename(ts, debug_filename);

    underlying_asset_id = rq_bootstrap_config_get_asset_id(config);
    if (underlying_asset_id)
    {
        rq_asset_t a = rq_asset_mgr_get(asset_mgr, underlying_asset_id);

        rq_yield_curve_set_underlying_asset_id(ts, underlying_asset_id);

        /* Set the default day count convention to be ACT/360 if the 
           underlying currency has a 360 day basis.
        */
        if (a && !strcmp(rq_asset_get_asset_type_id(a), rq_asset_type_id_ccy))
        {
            if (rq_asset_ccy_get_days_per_year(a) == 360)
                rq_yield_curve_set_default_day_count_convention(
                    ts,
                    RQ_DAY_COUNT_ACTUAL_360
                    );
        }
    }

#ifdef DEBUG_BOOTSTRAP
    fprintf(fh, "Building %s curve\n", rq_bootstrap_config_get_asset_id(config));
	fflush(fh);
#endif

    // need to sort the rate classes by tenor, to ensure points are bootstrapped in correct order
    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(config); i++)
    {
        const rq_rate_t rate = rq_rate_mgr_find(
            rate_mgr, 
            rq_bootstrap_config_get_rate_class_id_at(config, i)
            );
        if (rate)
        {
            // find the asset
			const char *asset_id = rq_rate_get_asset_id(rate);
            const rq_asset_t asset = rq_asset_mgr_get(
                asset_mgr, 
                asset_id
                );

            if (rate_adjust_factors && rate_adjust_factors[i] != 0.0)
                r *= rate_adjust_factors[i];

            if (asset)
            {
#ifdef DEBUG_BOOTSTRAP
                    {
                        short d1, m1, y1;
                        short d2, m2, y2;
                        rq_date_to_dmy(rq_rate_get_observation_date(rate), &d1, &m1, &y1);
                        rq_date_to_dmy(rq_rate_get_value_date(rate), &d2, &m2, &y2);
						fprintf(fh, "\tBootstrap: \tasset=%s \trate=%f \tto_date=%d/%d/%d \tfrom_date=%d/%d/%d\n", asset_id, r, d1, m1, y1, d2, m2, y2);
						fflush(fh);
                    }
#endif
                if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_ccy))
                {
                    double df = bootstrapAssetCcy(asset, rate, &last_date_bootstrapped);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irovernight))
                {
                    double df = bootstrapAssetIROvernight(asset, rate, &last_date_bootstrapped);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irdiscount))
                {
                    double df = 0.0;
                    if (rq_rate_get_rate_type(rate) == RQ_RATE_TYPE_PAR)
                        df = bootstrapParDCF(asset, rate, &last_date_bootstrapped, ts, cals, num_cals, lastRate, lastAsset, lastDF);
                    else
                        df = bootstrapAssetIRDiscount(asset, rate, &last_date_bootstrapped, cals, num_cals, ts);
                    lastDF = df;
                    setDiscountFactor(ts, last_date_bootstrapped, df);
				}
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irfuture))
                {
                    double df;
                    switch(config->billFuture_method)
                    {
                    case RQ_BILL_FUTURE_TENOR:
                        df = bootstrapAssetIRFutureTenor(asset, rate, &last_date_bootstrapped, ts, cals, num_cals);
                        break;
                    case RQ_BILL_FUTURE_STRIP:
                    default:
                        df = bootstrapAssetIRFutureStrip(asset, rate, &last_date_bootstrapped, ts, cals, num_cals, lastFutureRate, lastFutureAsset, lastFutureDF);
                        break;
                    }
					lastDF = df;
					lastFutureDF = df;
                    lastFutureAsset = asset;
                    lastFutureRate = rate;
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irswap))
                {

					// If using spline interpolation of swap par rates then we need to know all swaps before we can do any.
                    switch(parInterp_method)
                    {
					case RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE :
					case RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE_SWAPS_ONLY :
                        if(numSplineSwaps < RQ_MAX_SWAP_RATES)
                        {
                            splineSwapAssets[numSplineSwaps] = asset;
                            splineSwapRates[numSplineSwaps] = rate;
                            numSplineSwaps++;
                        }
						break;
					case RQ_SWAP_PAR_INTERPOLATION_LINEAR:
					default :
                        processSwapsDCF(asset,
                                    rate,
                                    &last_date_bootstrapped,
                                    ts,
                                    cals,
                                    num_cals,
									rq_system_get_calendar_mgr(system),
                                    &last_swap_date,
                                    &last_swap_rate,
                                    &last_year_count_frac,
                                    swapDayCount_method,
									parInterp_method,
									0,0,0);
						break;
					}
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irbond))
                {
					bootstrapAssetIRBond(asset, rate, &last_date_bootstrapped, ts, cals, num_cals);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_credit_spread))
                {
					double df = bootstrapAssetCreditSpread(asset, rate, &last_date_bootstrapped);
                    rq_yield_curve_set_discount_factor(ts, last_date_bootstrapped, df);
                }
				lastAsset = asset;

				if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                    break; // enough bootstrapping, now get out of here, you bootstrapping cowboy.
			}
            else
            {
                // printf("Couldn't find asset\n");
            }
			lastRate = rate;
		}
        else
        {
            // printf("Couldn't find rate\n");
        }
    }
	for(i = 0; i < (unsigned int)numSplineSwaps; i++)
	{
        processSwapsDCF(splineSwapAssets[i],
                        splineSwapRates[i],
                        &last_date_bootstrapped,
                        ts,
                        cals,
                        num_cals,
                        rq_system_get_calendar_mgr(system),
                        &last_swap_date,
                        &last_swap_rate,
                        &last_year_count_frac,
                        swapDayCount_method,
						parInterp_method,
                        splineSwapAssets,
                        splineSwapRates,
                        numSplineSwaps);
		lastRate = splineSwapRates[i];
		lastAsset = splineSwapAssets[i];
	}

#ifdef DEBUG_BOOTSTRAP
	dumpYieldCurveSpline(system, ts);
	fprintf(fh, "\n\n");
#ifdef DEBUG_BOOTSTRAP_FH
    fclose(fh);
#endif
#endif

    if (rq_yield_curve_size(ts))
        return ts;

    rq_yield_curve_free(ts);
    return NULL;
}
