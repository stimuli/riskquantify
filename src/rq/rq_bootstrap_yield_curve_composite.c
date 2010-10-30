/*
** rq_bootstrap_yield_curve_composite.c
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
#include "rq_bootstrap_yield_curve_composite.h"
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

/* #define DEBUG_BOOTSTRAP */

RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_composite(
    const char *curve_id,
    const rq_system_t system,
    const rq_market_t market
    )
{
    rq_yield_curve_t yc = NULL;
    const rq_bootstrap_config_t config = 
        rq_bootstrap_config_mgr_find(
            rq_system_get_bootstrap_config_mgr(system), 
            curve_id, 
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE
            ); 
    if (config && config->curve_id1 && config->curve_id2)
    {
        rq_yield_curve_mgr_t ycmgr = rq_market_get_yield_curve_mgr(market);
        rq_yield_curve_t base_curve = rq_yield_curve_mgr_get(
            ycmgr,
            config->curve_id1
            );
        rq_yield_curve_t spread_curve = rq_yield_curve_mgr_get(
            ycmgr,
            config->curve_id2
            );

        if (base_curve && spread_curve)
        {
            const char *underlying_asset_id = NULL;

            yc = rq_yield_curve_init(
                rq_bootstrap_config_get_curve_id(config),
				rq_bootstrap_config_get_interpolation_method(config),
				rq_bootstrap_config_get_extrapolation_method_start(config),
				rq_bootstrap_config_get_extrapolation_method_end(config),
				rq_bootstrap_config_get_zero_method(config),
				rq_bootstrap_config_get_zero_method_compound_frequency(config),
				rq_bootstrap_config_get_default_day_count_convention(config),
                rq_market_get_market_date(market)
                );

            underlying_asset_id = rq_bootstrap_config_get_asset_id(config);
            if (underlying_asset_id)
            {
                rq_asset_t a = rq_asset_mgr_get(
                    rq_system_get_asset_mgr(system), 
                    underlying_asset_id
                    );

                rq_yield_curve_set_underlying_asset_id(yc, underlying_asset_id);

                /* Set the default day count convention to be ACT/360 if the 
                   underlying currency has a 360 day basis.
                */
                if (a && !strcmp(rq_asset_get_asset_type_id(a), rq_asset_type_id_ccy))
                {
                    if (rq_asset_ccy_get_days_per_year(a) == 360)
                        rq_yield_curve_set_default_day_count_convention(
                            yc,
                            RQ_DAY_COUNT_ACTUAL_360
                            );
                }
            }

            rq_yield_curve_set_yield_curve_type(
                yc,
                RQ_YIELD_CURVE_TYPE_COMPOSITE
                );

            rq_yield_curve_set_base_curve(yc, base_curve);
            rq_yield_curve_set_spread_curve(yc, spread_curve);
        }
    }
            
    return yc;
}
