/*
** rq_bootstrap_forward_curve_simple.c
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
#include "rq_bootstrap_forward_curve_simple.h"
#include "rq_asset_irfuture.h"
#include "rq_defs.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_forward_curve_t
rq_bootstrap_forward_curve_simple(
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
    rq_forward_curve_t fc = rq_forward_curve_build(
        rq_bootstrap_config_get_curve_id(config),
        rq_bootstrap_config_get_asset_id(config)
        );


    /* iterate through the bootstrap_config getting asset ids */
    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(config); i++)
    {
        const rq_rate_t rate = rq_rate_mgr_find(
            rate_mgr, 
            rq_bootstrap_config_get_rate_class_id_at(config, i)
            );
        if (rate)
        {
            double r = rq_rate_get_value(rate);
            rq_date last_tmp = 0;

            /* find the asset */
			const char *asset_id = rq_rate_get_asset_id(rate);
            const rq_asset_t asset = rq_asset_mgr_get(
                asset_mgr, 
                asset_id
                );
			int quote = 0;
            
            if (rate_adjust_factors && rate_adjust_factors[i] != 0.0)
                r *= rate_adjust_factors[i];

            last_date_bootstrapped = rq_rate_get_value_date(rate);

            if (asset && !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irfuture))
			{
				enum rq_ir_future_quote_convention futureQuoteConv = rq_asset_irfuture_get_quote_convention(asset);
				quote = futureQuoteConv;
			}

            rq_forward_curve_set_rate(
                fc, 
                last_date_bootstrapped,
                rq_rate_get_value(rate),
				quote 
                );

            if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                break; /* break out of for loop, we've bootstrapped enough */
        }
    }

    if (rq_forward_curve_size(fc))
        return fc;

    rq_forward_curve_free(fc);
    return NULL;
}

