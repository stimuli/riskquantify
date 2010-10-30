/*
** rq_bootstrap_adapter_spread_curve_simple.c
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
#include "rq_bootstrap_adapter_spread_curve_simple.h"
#include "rq_bootstrap_spread_curve_simple.h"
#include "rq_asset_ccypair.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_bootstrap_adapter_spread_curve_simple_id = "BootstrapSpreadCurveSimple";

void *
bootstrap_spread_curve_simple(
    rq_bootstrap_adapter_t bootstrap_adapter,
    const char *curve_id, 
    const rq_system_t system,
    rq_market_t market
    )
{
    rq_bootstrap_config_t bootstrap_config = 
        rq_bootstrap_config_mgr_find(
            rq_system_get_bootstrap_config_mgr(system),
            curve_id,
            RQ_TERMSTRUCT_TYPE_SPREAD_CURVE
            );
    if (bootstrap_config)
    {
        rq_spread_curve_t spread_curve = rq_bootstrap_spread_curve_simple(
            rq_market_get_market_date(market),
            rq_market_get_rate_mgr(market),
            bootstrap_config,
            rq_system_get_asset_mgr(system),
            NULL,
            0
            );
        if (spread_curve)
        {
            rq_spread_curve_mgr_add(
                rq_market_get_spread_curve_mgr(market),
                spread_curve
                );
        }
        return spread_curve;
    }

    return NULL;
}

rq_bootstrap_adapter_t
rq_bootstrap_adapter_spread_curve_simple_alloc()
{
    return 
        _rq_bootstrap_adapter_alloc(
            rq_bootstrap_adapter_spread_curve_simple_id,
            RQ_TERMSTRUCT_TYPE_SPREAD_CURVE,
            bootstrap_spread_curve_simple
            );
}

