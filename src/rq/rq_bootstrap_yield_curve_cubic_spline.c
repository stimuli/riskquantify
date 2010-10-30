/*
** rq_bootstrap_yield_curve_cubic_spline.c
**
** Written by Joey Nicotra
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
#include "rq_bootstrap_yield_curve_cubic_spline.h"
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

/* #define DEBUG_BOOTSTRAP */ /* defining this gives a helpful printout of the assets, points and attributes that make up the yc */
#ifdef DEBUG_BOOTSTRAP
#define DEBUG_BOOTSTRAP_FH
#define debugFile "debugRqYC.log"
#endif

/*
	METHOD
	Curve Building:
	1. Bootstrap all points upto the first swap asset. This includes cash and futures rates.
	2. If interpolation is needed prior to the first swap asset, the interpolation method specified
		would be used. If the interpolation method is SPLINE_DISCOUNT_FACTOR, then
		only the supplied df points are inputed into the spline.
	3. Retrieve all swap assets
	4. Generate roll dates for the first swap
	5. Calculate par rates for all points prior to the first swap rate. If this involves interpolation
		then this would be the same as 2. above
	6. Load all derived par rates prior prior to the last bootstrapped date into the cubic spline
	7. Load all discrete swap rates into the cubic spline.
	8. Call the natural cubic spline
	9. Retrieve all cubic spline interpolated swap par rates from the first to the last swap asset.
	10. Strap each interpolated swap par rate to the rest of the points (prior to the first swap rate)
		in the curve.
TODO	11. Due to performance issues, retrieve the first CACHE_X_YEARS years of spline interpolated points
		from the curve and place them in the yield curve cache.

	Post Curve Building:
TODO	1. Upon a call to retrieve a discount factor from the curve, if the point is within CACHE_X_YEARS,
		then the discount factor will be returned, otherwise the interpolation method specified will be
		used.
	2. If the interpolation method used is SPLINE_DISCOUNT_FACTOR and the requested point is outside
		the cache, then only the original asset points with the interpolated swap par rates wll be used
		as inputs into the spline.
		
	Assumptions (TODO):
	1. Swap assets have the same compounding frequency
			Possible solution could be to make a spline call upto the last swap asset that has the 
			same tenor as the previous, as the current code stands.  Then with all following swap assets
			with a different tenor a new spline call is made.
	2. Implied (generated swap par rates) all fall on similar days
	3. Paramatrise the cache size
	4. Support only for cash deposits, futures and swaps
	5. Assets are sorted, whereby the swaps are ordered as well after cash deposits/futures
*/

void
processSwaps(rq_asset_list_t swapAssetsList, double* swapRates, rq_date* lastDateBootstrapped,
			 rq_yield_curve_t ts, const rq_calendar_t* cals, unsigned short num_cals)
{
	double splineX[RQ_MAX_SPLINE_POINTS];
	double splineY[RQ_MAX_SPLINE_POINTS];
	double splineY2[RQ_MAX_SPLINE_POINTS];
	unsigned int splineInputNum;
	unsigned int i=0;
	unsigned int j = 0;
	unsigned int numGeneratedIntermediateDates = 0;
	unsigned int lastIntermediateImpliedSwapRate = 0;
	unsigned int numRollDates = 0;
    rq_date rollDates[RQ_MAX_ROLL_DATES];
    double discount_factors[RQ_MAX_ROLL_DATES];
	double outParRate = 0.0;
	double df = 0.0;
	rq_asset_t assetSwap = NULL;
	unsigned int maxSwapNum = rq_asset_list_size(swapAssetsList);
    double periodLength = 0.0;
	rq_date swapStartDateStrapped = 0;
	rq_date swapLastDateStrapped = 0;

	if (rq_asset_list_size(swapAssetsList) > 0)
	{
		assetSwap = rq_asset_list_get_at(swapAssetsList, 0);
	}
	else
	{
		return;
	}

	periodLength = rq_term_get_annualized_period(rq_asset_irswap_get_frequency(assetSwap));

	swapStartDateStrapped = rq_date_roll_get_date(
        ts->from_date,
        rq_asset_irswap_get_tenor(assetSwap),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(assetSwap),
        cals,
        num_cals);

	numRollDates = rq_date_roll_generate_dates(
        rollDates,
        RQ_MAX_ROLL_DATES,
        ts->from_date,
        swapStartDateStrapped,
        rq_asset_irswap_get_frequency(assetSwap),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(assetSwap),
        cals,
        num_cals,
        RQ_DATE_ROLL_STUB_POSITION_NONE,
        15,	/* obsolete */
        NULL);
	
	splineY[0] = 0;
	splineX[0] = 0;
	splineInputNum = 1;

	numGeneratedIntermediateDates = (unsigned short)(numRollDates - 2);
	for (j = 0; j < numGeneratedIntermediateDates; j++)
	{
		if ((rollDates[j] >= *lastDateBootstrapped) && (rollDates[j] < swapStartDateStrapped))
		{
			break;
		}
		lastIntermediateImpliedSwapRate = j;
	}

	/* only dates prior to the term of the swap is needed */
    for (j = 0; j <= lastIntermediateImpliedSwapRate; j++)
    {
		/* TODO if already exists don't need to imply. */
		/* need to imply a swap rate for this point */
		if (j > 0)
		{
			double par_rate;
				par_rate = 
					rq_yield_curve_get_par_rate_period(
						ts,
						j,
						rollDates+1,
						periodLength);

			df = rq_yield_curve_get_discount_factor(ts, rollDates[j]);

			/* Step 6 Populating Spline Inputs - pre lastDateStrapped */
			splineX[splineInputNum] = rollDates[j];
			splineY[splineInputNum] = par_rate;
			splineInputNum++;
		}
		discount_factors[j] = df;
	}

	/* Step 7 Populating Spline Inputs - post lastDateStrapped */
	for (i=0; i<rq_asset_list_size(swapAssetsList); i++)
	{
		rq_asset_t asset = rq_asset_list_get_at(swapAssetsList, i);

		rq_date swapDate = rq_date_roll_get_date(
			ts->from_date,
			rq_asset_irswap_get_tenor(asset),
			RQ_ROLL_CONVENTION_NONE,
			rq_asset_irswap_get_date_roll_convention(asset),
			cals,
			num_cals);

		splineX[splineInputNum] = swapDate;
		splineY[splineInputNum] = swapRates[splineInputNum - lastIntermediateImpliedSwapRate - 1];
		splineInputNum++;
	}

	/* Step 8 cubic spline inputs now populated */
	rq_interpolate_spline(1,	/* natural */
                          splineX,		/* swapDates */
                          splineY,		/* swap rates */
                          splineInputNum,	/* number of input points */
                          0,
                          0,
                          splineY2);	/* output derivatives */

	/* get last swap asset */
	assetSwap = rq_asset_list_get_at(swapAssetsList, maxSwapNum-1);

	swapLastDateStrapped = rq_date_roll_get_date(
        ts->from_date,
        rq_asset_irswap_get_tenor(assetSwap),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(assetSwap),
        cals,
        num_cals);

	numRollDates = rq_date_roll_generate_dates(
        rollDates,
        RQ_MAX_ROLL_DATES,
        ts->from_date,
        swapLastDateStrapped,
        rq_asset_irswap_get_frequency(assetSwap),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(assetSwap),
        cals,
        num_cals,
        RQ_DATE_ROLL_STUB_POSITION_NONE,
        15,	/* obsolete */
        NULL);

	/* Big Big assumption, all swaps roll on same dates */
	for ( j = lastIntermediateImpliedSwapRate; rollDates[j] < swapLastDateStrapped; j++)
	{
		outParRate = rq_interpolate_splint(rollDates[j],
							splineX,
							splineY,
							splineY2,
							splineInputNum);

		// add the discount factor retrieved from the spline interpolated par rate
		df = rq_discount_factor_from_par_rate(
			outParRate * periodLength,
			j,
			discount_factors);

		setDiscountFactor(ts, rollDates[j], df);
		discount_factors[j] = df;
	}
}

RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_cubic_spline(
    const char *curve_id,
    const rq_system_t system,
    const rq_market_t market,
    double *rate_adjust_factors,
    rq_date last_date_to_bootstrap
    )
{
    unsigned int i;
    rq_date date = rq_market_get_market_date(market);
    const rq_rate_mgr_t rate_mgr = rq_market_get_rate_mgr(market);
    const rq_asset_mgr_t asset_mgr = rq_system_get_asset_mgr(system); 
    rq_calendar_t cal = rq_calendar_mgr_get(rq_system_get_calendar_mgr(system), curve_id);
    const rq_bootstrap_config_t config = rq_bootstrap_config_mgr_find(rq_system_get_bootstrap_config_mgr(system), curve_id, RQ_TERMSTRUCT_TYPE_YIELD_CURVE); 

    rq_date last_date_bootstrapped = 0;
    double last_swap_rate = 0.0;
    rq_date last_swap_date = 0;
    unsigned last_num_coupons_per_year = 0;
    rq_calendar_t cals[2];
    unsigned short num_cals = 0;	
    const char *underlying_asset_id = NULL;
	double r=0;
	rq_asset_t lastFutureAsset = NULL;
	rq_rate_t lastFutureRate = NULL;
    double lastFutureDF = 0;

	unsigned int swapAssetNum = 0;
	rq_asset_list_t swapAssets = rq_asset_list_alloc();
	double swapRates[RQ_MAX_SWAP_RATES];

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

    if (cal)
    {
        cals[num_cals] = cal;
        num_cals++;
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

	/* need to sort the rate classes by tenor, to ensure points are
     * bootstrapped in correct order 
     */
    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(config); i++)
    {
        const rq_rate_t rate = rq_rate_mgr_find(
            rate_mgr, 
            rq_bootstrap_config_get_rate_class_id_at(config, i)
            );
        if (rate)
        {
            /* find the asset */
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
                if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irovernight))
                {
                    double df = bootstrapAssetIROvernight(asset, rate, &last_date_bootstrapped);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irdiscount))
                {
                    double df = bootstrapAssetIRDiscount(asset, rate, &last_date_bootstrapped, cals, num_cals, ts);
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
                        lastFutureDF = df;
                        lastFutureRate = rate;
                        break;
                    }
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                    lastFutureAsset = asset;
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irswap))
				{
					rq_asset_list_push_back(swapAssets, asset);
					swapRates[swapAssetNum++] = rq_rate_get_value(rate);
				}

				if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                    break; /* enough bootstrapping, now get out of here, you bootstrapping cowboy. */
			}
            else
            {
                /* printf("Couldn't find asset\n"); */
            }
		}
        else
        {
            /* printf("Couldn't find rate\n"); */
        }
    }
	
	/* We've set discount factors upto the first swap rate, and from
	 that point we have swapRates. Time to bootstrap the not so clean
	 long end of the boot where the cow dung lies.
    */
	if ((rq_asset_list_size(swapAssets)) > 0 && (swapAssetNum > 0))
	{
		processSwaps(swapAssets, swapRates, &last_date_bootstrapped, ts, cals, num_cals);
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
