/**
 * @file
 *
 * This file contains helper functions for pricing trades
 */
/*
** rq_pricing_helper.h
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
#ifndef rq_pricing_helper_h
#define rq_pricing_helper_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_pricing_adapter.h"
#include "rq_system.h"
#include "rq_market.h"
#include "rq_bootstrap_adapter_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

enum rq_pricing_helper_result {
    RQ_PRICING_HELPER_RESULT_SUCCESS,
    RQ_PRICING_HELPER_RESULT_FAILED
};

RQ_EXPORT enum rq_pricing_helper_result 
rq_pricing_helper_value_trade(
    double *value,
    const void *trade,
    struct rq_pricing_adapter *pricing_adapter,
    rq_system_t system,
    rq_market_t market,
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
	const char *pricing_ccy,
    const char *pricing_context
    );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
