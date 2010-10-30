/*
** rq_bootstrap_adapter_yield_curve_composite.c
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
#include "rq_bootstrap_adapter_yield_curve_composite.h"
#include "rq_bootstrap_yield_curve_composite.h"
#include "rq_error.h"
#include <stdlib.h>
#include <string.h>


const char * const rq_bootstrap_adapter_yield_curve_composite_id = "BootstrapYieldCurveComposite";

void *
bootstrap_yield_curve_composite(
    rq_bootstrap_adapter_t bootstrap_adapter,
    const char *curve_id, 
    const rq_system_t system,
    rq_market_t market
    )
{
    rq_yield_curve_t yield_curve = rq_bootstrap_yield_curve_composite(
        curve_id,
        system,
        market
        );
    if (yield_curve)
        rq_yield_curve_mgr_add(
            rq_market_get_yield_curve_mgr(market),
            yield_curve
            );
    return yield_curve;
}

static rq_error_code
get_bootstrap_dependency_list(struct rq_bootstrap_adapter *bootstrap_adapter, const char *curve_id, const rq_system_t system, rq_market_t market, rq_bootstrap_dependency_list_t bdl)
{
    rq_error_code status = RQ_FAILED;
    const rq_bootstrap_config_t config = 
        rq_bootstrap_config_mgr_find(
            rq_system_get_bootstrap_config_mgr(system), 
            curve_id, 
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE
            ); 
    if (config && config->curve_id1 && config->curve_id2)
    {
        rq_bootstrap_dependency_list_add(
            bdl,
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE,
            config->curve_id1
            );
        rq_bootstrap_dependency_list_add(
            bdl,
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE,
            config->curve_id2
            );
        status = RQ_OK;
    }
    return status;
}

rq_bootstrap_adapter_t
rq_bootstrap_adapter_yield_curve_composite_alloc()
{
    rq_bootstrap_adapter_t bootstrap_adapter =  
        _rq_bootstrap_adapter_alloc(
            rq_bootstrap_adapter_yield_curve_composite_id,
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE,
            bootstrap_yield_curve_composite
            );
    bootstrap_adapter->get_bootstrap_dependency_list = get_bootstrap_dependency_list;
    return bootstrap_adapter;
}

