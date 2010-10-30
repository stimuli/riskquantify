/**
 * \file rq_bootstrap_cds_curve.h
 * \author Brett Hutley
 *
 * \brief A simple function to bootstrap a forward curve
 */
/*
** rq_bootstrap_cds_curve.h
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
#ifndef rq_bootstrap_cds_curve_h
#define rq_bootstrap_cds_curve_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_market.h"
#include "rq_system.h"
#include "rq_cds_curve.h"
#include "rq_asset_mgr.h"
#include "rq_rate_mgr.h"
#include "rq_bootstrap_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- prototypes -------------------------------------------------- */

RQ_EXPORT rq_cds_curve_t
rq_bootstrap_cds_curve(
    const rq_system_t system,
	const rq_market_t market,
    const rq_bootstrap_config_t config, 
    rq_date last_date_to_bootstrap    
    );

RQ_EXPORT void 
rq_build_net_cds_spread_rates(
	rq_date mktDate,
	const rq_system_t system,
	const rq_market_t market,
    const rq_bootstrap_config_t config, 
    rq_date last_date_to_bootstrap,
	short *isCdsSpreadSupplied,
	rq_cds_curve_t *net_cds_spread_rates,
	rq_yield_curve_t* base_yield_curve 
	);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
