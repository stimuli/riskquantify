/*
** rq_bootstrap_adapter_equity_curve_simple.c
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
#include "rq_bootstrap_adapter_equity_curve_simple.h"
#include "rq_bootstrap_equity_curve_simple.h"
#include <stdlib.h>
#include <string.h>


const char * const rq_bootstrap_adapter_equity_curve_simple_id = "BootstrapEquityCurveSimple";

void *
bootstrap_equity_curve_simple(
    rq_bootstrap_adapter_t bootstrap_adapter,
    const char *curve_id, 
    const rq_system_t system,
    rq_market_t market
    )
{
    rq_equity_curve_t equity_curve = rq_bootstrap_equity_curve_simple(
        curve_id,
        system,
        market
        );
    if (equity_curve != NULL)
        rq_equity_curve_mgr_add(
            rq_market_get_equity_curve_mgr(market),
            equity_curve
            );
    return equity_curve;
}

rq_bootstrap_adapter_t
rq_bootstrap_adapter_equity_curve_simple_alloc()
{
    return 
        _rq_bootstrap_adapter_alloc(
            rq_bootstrap_adapter_equity_curve_simple_id,
            RQ_TERMSTRUCT_TYPE_EQUITY_CURVE,
            bootstrap_equity_curve_simple
            );
}

