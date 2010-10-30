/**
 * \file rq_bootstrap_equity_curve_simple.h
 * \author Brett Hutley
 *
 * \brief A bootstrap function for equity curves
 */
/*
** rq_bootstrap_equity_curve_simple.h
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
#ifndef rq_bootstrap_equity_curve_simple_h
#define rq_bootstrap_equity_curve_simple_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_system.h"
#include "rq_market.h"
#include "rq_equity_curve.h"
#include "rq_asset_dividend_yield.h"
#include "rq_asset_equity.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/**
 * This function bootstraps the equity curve specified by the curve_id
 * parameter, by looking up a bootstrap configuration for that equity
 * curve in the bootstrap config manager, and then using the
 * appropriate rates contained in the rate manager in the market. The
 * constructed equity curve is NOT added to the market automatically.
 */
RQ_EXPORT rq_equity_curve_t
rq_bootstrap_equity_curve_simple(
    const char *curve_id,
    const rq_system_t system,
    const rq_market_t market
    );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
