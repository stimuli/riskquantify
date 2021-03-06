/*
** rq_bootstrap_adapter_forward_curve_bbi.c
**
** Written by Hendra
**
** Copyright (C) 2008 Brett Hutley
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
#include "rq_bootstrap_adapter_forward_curve_bbi.h"
#include "rq_bootstrap_forward_curve_bbi.h"
#include "rq_error.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_bootstrap_adapter_forward_curve_bbi_id = "BootstrapForwardCurveBBI";

void *
bootstrap_forward_curve_bbi(
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
            RQ_TERMSTRUCT_TYPE_FORWARD_CURVE
            );

    if (bootstrap_config)
    {
        rq_forward_curve_t forward_curve = rq_bootstrap_forward_curve_bbi(
            rq_market_get_market_date(market),
            rq_market_get_rate_mgr(market),
            bootstrap_config,
            system,
            NULL,
            0
            );

        if (forward_curve)
        {
            rq_forward_curve_mgr_add(
                rq_market_get_forward_curve_mgr(market),
                forward_curve
                );
        }
        return forward_curve;
    }

    return NULL;
}

static rq_error_code
get_bootstrap_dependency_list(struct rq_bootstrap_adapter *bootstrap_adapter, const char *curve_id, const rq_system_t system, rq_market_t market, rq_bootstrap_dependency_list_t bdl)
{
    rq_error_code status = RQ_FAILED;
    const rq_bootstrap_config_t config = rq_bootstrap_config_mgr_find(
            rq_system_get_bootstrap_config_mgr(system), 
            curve_id, 
            RQ_TERMSTRUCT_TYPE_FORWARD_CURVE
            );

    if (config && config->curve_id1)
    {
        rq_bootstrap_dependency_list_add(
            bdl,
            RQ_TERMSTRUCT_TYPE_FORWARD_CURVE,
            config->curve_id1
            );
        status = RQ_OK;
    }
    return status;
}

rq_bootstrap_adapter_t
rq_bootstrap_adapter_forward_curve_bbi_alloc()
{
    rq_bootstrap_adapter_t bootstrap_adapter = _rq_bootstrap_adapter_alloc(
        rq_bootstrap_adapter_forward_curve_bbi_id,
        RQ_TERMSTRUCT_TYPE_FORWARD_CURVE,
        bootstrap_forward_curve_bbi
        );
    bootstrap_adapter->get_bootstrap_dependency_list = get_bootstrap_dependency_list;
    return bootstrap_adapter;
}
