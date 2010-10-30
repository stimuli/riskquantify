/*
** rq_bootstrap_yield_curve_spread.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include "rq_bootstrap_yield_curve_spread.h"
#include "rq_bootstrap_yield_curve.h"
#include "rq_bootstrap_yield_curve_day_count.h"
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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

extern const char * const rq_bootstrap_adapter_yield_curve_spread_id;

/* #define DEBUG_BOOTSTRAP */


void rq_build_net_yield_curve_spreads( 
	const rq_bootstrap_config_t config,
	const rq_system_t system,
    const rq_market_t market,
	const rq_rate_mgr_t rate_mgr,
	rq_date *spread_curve_dates,
    double *spread_curve_spreads,
	const char **base_yield_curve_id,
	rq_bootstrap_config_t *base_yield_curve_config,
	unsigned *spread_i,
	short initValues
	)
{
	unsigned i;
	const char* underlying_config_id = NULL;
	rq_bootstrap_config_t underlying_config = NULL;
	const char *underlying_bootstrap_method = NULL;

	if (!config) return;

	/* Put all the spread values and dates into arrays. */
	(*spread_i) = 0;
	for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(config); i++)
	{
		const char *rate_id = rq_bootstrap_config_get_rate_class_id_at(config, i);
		const rq_rate_t rate = rq_rate_mgr_find(
			rate_mgr, 
			rate_id
			);
		if (rate)
		{
			/* find the asset */
			double spread = rq_rate_get_value(rate);
			rq_date value_date = rq_rate_get_value_date(rate);

			if ( initValues )
			{
				spread_curve_dates[*spread_i] = value_date;
				spread_curve_spreads[*spread_i] = spread;
			}
			else
			{
				/* It is assumed that all levels of layered spreads will share common dates */
				if ( value_date == spread_curve_dates[*spread_i] )
					spread_curve_spreads[*spread_i] += spread;
			}

			(*spread_i)++;
		}
		else
		{
			/* printf("Couldn't find rate\n"); */
		}
	}

	underlying_config_id = rq_bootstrap_config_get_curve_id1(config);
	if (underlying_config_id)
    {
        underlying_config = rq_bootstrap_config_mgr_find(
            rq_system_get_bootstrap_config_mgr(system), 
            underlying_config_id, 
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE
            ); 
        if (!underlying_config)
            return;
    }
    else
        return;

	underlying_bootstrap_method = rq_bootstrap_config_get_bootstrap_method_id(underlying_config);
	if ( strcmp( underlying_bootstrap_method, rq_bootstrap_adapter_yield_curve_spread_id ) == 0 )
	{
		/* Layered yield curve spread */
		rq_build_net_yield_curve_spreads( 
			underlying_config,
			system,
			market,
			rate_mgr,
			spread_curve_dates,
			spread_curve_spreads,
			base_yield_curve_id,
			base_yield_curve_config,
			spread_i,
			0
			);
	}
	else
	{
		*base_yield_curve_id = underlying_config_id;
		*base_yield_curve_config = underlying_config;
	}
}

								
RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_spread(
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
    const rq_calendar_t cal = rq_calendar_mgr_get(rq_system_get_calendar_mgr(system), curve_id);
    const rq_bootstrap_config_t config = rq_bootstrap_config_mgr_find(rq_system_get_bootstrap_config_mgr(system), curve_id, RQ_TERMSTRUCT_TYPE_YIELD_CURVE); 

    rq_bootstrap_config_t yc_config = NULL;
    const char *yc_curve_id = NULL;

    rq_date last_date_bootstrapped = 0;
    double last_swap_rate = 0.0;
    rq_date last_swap_date = 0;
    double last_swap_period = 0.0;

    unsigned spread_i = 0;
    rq_date spread_curve_dates[RQ_YIELD_CURVE_MAX_FACTORS];
    double spread_curve_spreads[RQ_YIELD_CURVE_MAX_FACTORS];
    rq_calendar_t cals[2];
    unsigned short num_cals = 0;

	rq_asset_t lastAsset = NULL;
	rq_rate_t lastRate = NULL;
    double lastFutureDF = 0;

    /* 
       Add one to the number of discount factors because we are
       going to create a discount factor of 1.0  at the zero days
       point 
    */
    /* int max_factors = (config->num_rate_class_ids + 1) * 2; */

    rq_yield_curve_t ts;
    const char *underlying_asset_id = NULL;

    if (!config)
        return NULL;

    if (cal)
    {
        cals[0] = cal;
        num_cals++;
    }

	rq_build_net_yield_curve_spreads( 
			config,
			system,
			market,
			rate_mgr,
			spread_curve_dates,
			spread_curve_spreads,
			&yc_curve_id,
			&yc_config,
			&spread_i,
			1
			);

	if ( !yc_curve_id || !yc_config )
		return NULL;

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
        rq_yield_curve_set_underlying_asset_id(ts, underlying_asset_id);


    /* Now bootstrap the curve, adding the spreads on the rates from the yield curve. */
    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(yc_config); i++)
    {
        const char *rate_id = rq_bootstrap_config_get_rate_class_id_at(yc_config, i);
        const rq_rate_t origRate = rq_rate_mgr_find(
            rate_mgr, 
            rate_id
            );		

        if (origRate)
        {
			rq_rate_t spreadRate = rq_rate_clone( origRate );

            /* find the asset */
			const char *asset_id = rq_rate_get_asset_id(spreadRate);
            double r = rq_rate_get_value(spreadRate);
            const rq_asset_t asset = rq_asset_mgr_get(
                asset_mgr, 
                asset_id
                );
            rq_date value_date = rq_rate_get_value_date(spreadRate);
            unsigned j;

            if (rate_adjust_factors && rate_adjust_factors[i] != 0.0)
                r *= rate_adjust_factors[i];

            for (j = 0; j < spread_i; j++)
            {
                if (spread_curve_dates[j] == value_date)
                {
                    r += spread_curve_spreads[j];
                    break;
                }
                else if (spread_curve_dates[j] > value_date)
                {
                    if (j > 0)
                    {
                        double s = rq_interpolate_linear(
                            (double)value_date,
                            (double)spread_curve_dates[j-1],
                            spread_curve_spreads[j-1],
                            (double)spread_curve_dates[j],
                            spread_curve_spreads[j]
                            );
                        r += s;
                    }
                    else
                    {
                        double s = rq_interpolate_linear(
                            (double)value_date,
                            (double)date,
                            0.0,
                            (double)spread_curve_dates[j],
                            spread_curve_spreads[j]
                            );
                        r += s;
                    }
                    break;
                }
            }

			rq_rate_set_value( spreadRate, r );


            if (asset)
            {
                if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_ccy))
                {
					double df = bootstrapAssetCcy(asset, spreadRate, &last_date_bootstrapped);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irovernight))
                {
                    double df = bootstrapAssetIROvernight(asset, spreadRate, &last_date_bootstrapped);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irdiscount))
                {
                    double df = bootstrapAssetIRDiscount(asset, spreadRate, &last_date_bootstrapped, cals, num_cals, ts);
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irfuture))
                {
                    double df;
                    switch(config->billFuture_method)
                    {
                    case RQ_BILL_FUTURE_TENOR:
                        df = bootstrapAssetIRFutureTenor(asset, spreadRate, &last_date_bootstrapped, ts, cals, num_cals);
                        break;
                    case RQ_BILL_FUTURE_STRIP:
                    default:
                        df = bootstrapAssetIRFutureStrip(asset, spreadRate, &last_date_bootstrapped, ts, cals, num_cals, lastRate, lastAsset, lastFutureDF);
                        lastFutureDF = df;
                        break;
                    }
                    setDiscountFactor(ts, last_date_bootstrapped, df);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irswap))
                {
                    // Spline swap interpolation not available from spreads yet.
                    processSwapsDCF(asset, spreadRate, &last_date_bootstrapped, ts, cals, num_cals, rq_system_get_calendar_mgr(system), &last_swap_date, &last_swap_rate, &last_swap_period, RQ_SWAP_DAY_COUNT_SIMPLE, rq_bootstrap_config_get_swapParInterpolation_method(config), 0, 0, 0);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irbond))
                {
                    bootstrapAssetIRBond(asset, spreadRate, &last_date_bootstrapped, ts, cals, num_cals);
                }
                else if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_credit_spread))
                {
                    double df = bootstrapAssetCreditSpread(asset, spreadRate, &last_date_bootstrapped);
                    rq_yield_curve_set_discount_factor(ts, last_date_bootstrapped, df);
                }
				lastAsset = asset;

                if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                    break; /* break out of for loop, we've bootstrapped enough */
            }
            else
            {
                /* printf("Couldn't find asset\n"); */
            }
			if ( lastRate ) 
				rq_rate_free( lastRate );
			lastRate = spreadRate;
        }
        else
        {
            /* printf("Couldn't find rate\n"); */
        }
    }

	if ( lastRate ) 
		rq_rate_free( lastRate );

#ifdef DEBUG_BOOTSTRAP
    fprintf(stdout, "\n\n");
    /* fclose(fh); */
#endif

    return ts;
}
