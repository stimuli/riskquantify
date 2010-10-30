/*
** rq_bootstrap_yield_curve_simple.c
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
#include "rq_bootstrap_yield_curve_simple.h"
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
#include "rq_date_roll.h"
#include "rq_defs.h"
#include "rq_interpolate.h"
#include "rq_day_count.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*#define DEBUG_BOOTSTRAP */ /* defining this gives a helpful printout of the assets, points and attributes that make up the yc */
#ifdef DEBUG_BOOTSTRAP
#define DEBUG_BOOTSTRAP_FH
#define debugFile "debugRqYC.log"
#endif



RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_simple(
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
    rq_calendar_t cal;
    const rq_bootstrap_config_t config = rq_bootstrap_config_mgr_find(rq_system_get_bootstrap_config_mgr(system), curve_id, RQ_TERMSTRUCT_TYPE_YIELD_CURVE); 

    rq_date last_date_bootstrapped = 0;
    double last_swap_rate = 0.0;
    rq_date last_swap_date = 0;
    double last_swap_period = 0.0;
    rq_calendar_t cals[2];
    unsigned short num_cals = 0;
    const char *underlying_asset_id = NULL;
	double r=0;
	rq_asset_t lastFutureAsset = NULL;
	rq_rate_t lastFutureRate = NULL;
    double lastFutureDF = 0;

#ifdef DEBUG_BOOTSTRAP
#ifdef DEBUG_BOOTSTRAP_FH
    FILE *fh = fopen(debugFile, "a");
#else
	FILE *fh = stdout;
#endif
#endif

    /* 
       Add one to the number of discount factors because we are
       going to create a discount factor of 1.0  at the zero days
       point 
    */
    /* int max_factors = (config->num_rate_class_ids + 1) * 2; */

    rq_yield_curve_t ts;

    if (!config)
        return NULL;

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
//        config,
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
    fprintf(fh, "Building %s %s yield curve simple\n", curve_id, rq_bootstrap_config_get_asset_id(config));
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
            /* find the asset */
			const char *asset_id = rq_rate_get_asset_id(rate);
            const rq_asset_t asset = rq_asset_mgr_get(
                asset_mgr, 
                asset_id
                );

            /* Note these are not actually applied !! */
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
						fprintf(fh, "\tBootstrap: %i\trate_class_id=%s \tasset=%s \trate=%f \tobs_date=%d/%d/%d \tvalue_date=%d/%d/%d\n", i, rq_rate_get_rate_class_id(rate), asset_id, rq_rate_get_value(rate), d1, m1, y1, d2, m2, y2);
						fflush(fh);
                    }
#endif
                if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_ccy))
                {
                    double df = bootstrapAssetCcy(asset, rate, &last_date_bootstrapped);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
#ifdef DEBUG_BOOTSTRAP
{
    char buf[40];
    fprintf(fh, " %g %s ", df, rq_date_to_string(buf, "dd/mm/yy", last_date_bootstrapped));
}
#endif
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irovernight))
                {
                    double df = bootstrapAssetIROvernight(asset, rate, &last_date_bootstrapped);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
#ifdef DEBUG_BOOTSTRAP
{
    char buf[40];
    fprintf(fh, " %g %s ", df, rq_date_to_string(buf, "dd/mm/yy", last_date_bootstrapped));
}
#endif
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irdiscount))
                {
                    double df = bootstrapAssetIRDiscount(asset, rate, &last_date_bootstrapped, cals, num_cals, ts);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
#ifdef DEBUG_BOOTSTRAP
{
    char buf[40];
    fprintf(fh, " %g %s ", df, rq_date_to_string(buf, "dd/mm/yy", last_date_bootstrapped));
}
#endif
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
                        break;
                    }
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                    lastFutureAsset = asset;
                    lastFutureRate = rate;
#ifdef DEBUG_BOOTSTRAP
{
    char buf[40];
    fprintf(fh, " %g %s ", df, rq_date_to_string(buf, "dd/mm/yy", last_date_bootstrapped));
}
#endif
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irswap))
                {
					processSwapsDCF(asset, rate, &last_date_bootstrapped, ts, cals, num_cals, rq_system_get_calendar_mgr(system), &last_swap_date, &last_swap_rate, &last_swap_period, RQ_SWAP_DAY_COUNT_SIMPLE, rq_bootstrap_config_get_swapParInterpolation_method(config), 0, 0, 0);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irbond))
                {
					bootstrapAssetIRBond(asset, rate, &last_date_bootstrapped, ts, cals, num_cals);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_credit_spread))
                {
					double df = bootstrapAssetCreditSpread(asset, rate, &last_date_bootstrapped);
                    rq_yield_curve_set_discount_factor(ts, last_date_bootstrapped, df);
#ifdef DEBUG_BOOTSTRAP
{
    char buf[40];
    fprintf(fh, " %g %s ", df, rq_date_to_string(buf, "dd/mm/yy", last_date_bootstrapped));
}
#endif
                }

				if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                    break; /* break out of for loop, we've bootstrapped enough */
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

#ifdef DEBUG_BOOTSTRAP
/*	dumpYieldCurve(system, ts); */
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
