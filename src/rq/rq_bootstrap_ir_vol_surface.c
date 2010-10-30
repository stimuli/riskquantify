/*
** rq_bootstrap_ir_vol_surface.c
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
#include "rq_bootstrap_ir_vol_surface.h"
#include "rq_defs.h"
#include "rq_asset_irvol.h"
#include "rq_asset_irswap.h"
#include "rq_term.h"
#include <stdlib.h>
#include <string.h>

#include <stdio.h>

RQ_EXPORT rq_ir_vol_surface_t
rq_bootstrap_ir_vol_surface(
    rq_date date, 
    const rq_rate_mgr_t rate_mgr, 
    const rq_bootstrap_config_t config, 
    const rq_asset_mgr_t asset_mgr,
    double *rate_adjust_factors,
    rq_date last_date_to_bootstrap    
    )
{
    unsigned int i;
    rq_date last_date_bootstrapped = 0;

    /* construct an empty forward curve */
    rq_ir_vol_surface_t vs = rq_ir_vol_surface_alloc(
        rq_bootstrap_config_get_curve_id(config)
        );
    const char *cfg_asset_id = rq_bootstrap_config_get_asset_id(config);
    if (cfg_asset_id)
        rq_ir_vol_surface_set_underlying_asset_id(vs, cfg_asset_id);

    /* iterate through the bootstrap_config getting asset ids */
    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(config); i++)
    {
        rq_rate_t rate = rq_rate_mgr_find(
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

            if (!asset)
				continue;

			/* thc todo handle the different pricing conventions (price vs yield). */
            if (!strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irvol))
            {
				/* get info from asset */
				const struct rq_term *underlyingTenor = rq_asset_irvol_get_underlying_tenor(asset);
				double r = rq_rate_get_value(rate);
				/* try to get underlying asset */
				const char *underlyingId = rq_asset_irvol_get_underlying_asset_id(asset);
				const rq_asset_t underlyingAsset = underlyingId ? rq_asset_mgr_get(asset_mgr, rq_asset_irvol_get_underlying_asset_id(asset)) : NULL;

				/* for swaption based vol surface get underlying term from swap asset */
				if (underlyingAsset && !strcmp(rq_asset_get_asset_type_id(underlyingAsset), rq_asset_type_id_irswap))
				{
                    underlyingTenor = rq_asset_irswap_get_tenor(underlyingAsset); 
				}

				/* calc rate and insert into vol surface */
				last_date_bootstrapped = rq_rate_get_value_date(rate);

				if (rate_adjust_factors && rate_adjust_factors[i] != 0.0)
					r *= rate_adjust_factors[i];

				rq_ir_vol_surface_set_volatility(
					vs, 
					rq_term_get_annualized_period(underlyingTenor),
					last_date_bootstrapped,
					r
					);

            }

            if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                break; /* break out of for loop, we've bootstrapped enough */

        }
    }

    return vs;
}

