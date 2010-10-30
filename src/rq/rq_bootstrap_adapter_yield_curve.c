/*
** rq_bootstrap_adapter_yield_curve.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#include "rq_bootstrap_adapter_yield_curve.h"
#include "rq_bootstrap_yield_curve_day_count.h"
#include "rq_bootstrap_yield_curve.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT const char * const rq_bootstrap_adapter_yield_curve_id = "BootstrapYieldCurve";

void *
bootstrap_yield_curve(
    rq_bootstrap_adapter_t bootstrap_adapter,
    const char *curve_id, 
    const rq_system_t system,
    rq_market_t market
    )
{
    rq_yield_curve_t yield_curve = rq_bootstrap_yield_curve_day_count(
        curve_id,
        system,
        market,
        NULL,
        0,
        0,
		rq_bootstrap_adaptor_get_debug_filename(bootstrap_adapter)
        );
    if (yield_curve)
        rq_yield_curve_mgr_add(
            rq_market_get_yield_curve_mgr(market),
            yield_curve
            );
    return yield_curve;
}

RQ_EXPORT rq_bootstrap_adapter_t
rq_bootstrap_adapter_yield_curve_alloc()
{
    return 
        _rq_bootstrap_adapter_alloc(
            rq_bootstrap_adapter_yield_curve_id,
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE,
            bootstrap_yield_curve
            );
}

