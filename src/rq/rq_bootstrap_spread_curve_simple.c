/*
** rq_bootstrap_spread_curve_simple.c
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
#include "rq_bootstrap_spread_curve_simple.h"
#include "rq_defs.h"
#include <stdlib.h>

RQ_EXPORT rq_spread_curve_t
rq_bootstrap_spread_curve_simple(
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
    rq_spread_curve_t cscrv = rq_spread_curve_build(
        rq_bootstrap_config_get_curve_id(config),
        date
        );
    const char *underlying_asset_id = rq_bootstrap_config_get_asset_id(config);
    if (underlying_asset_id)
        rq_spread_curve_set_underlying_asset_id(cscrv, underlying_asset_id);

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
            if (rate_adjust_factors && rate_adjust_factors[i] != 0.0)
                r *= rate_adjust_factors[i];

            last_date_bootstrapped = rq_rate_get_value_date(rate);

            rq_spread_curve_set_spread(
                cscrv, 
                last_date_bootstrapped,
                rq_rate_get_value(rate)
                );

            if (last_date_to_bootstrap > 0 && last_date_bootstrapped > last_date_to_bootstrap)
                break; /* break out of for loop, we've bootstrapped enough */
        }
    }

    if (rq_spread_curve_size(cscrv))
        return cscrv;

    rq_spread_curve_free(cscrv);
    return NULL;
}

