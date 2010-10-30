/**
 * \file rq_bootstrap_yield_curve.h
 * \author Joey Nicotra
 *
 * \brief common yield curve bootstrap/smoothing routines
 */
/*
** rq_bootstrap_yield_curve.h
**
** Written by Joey Nicotra
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
#ifndef rq_bootstrap_yield_curve_h
#define rq_bootstrap_yield_curve_h

#include "rq_config.h"
#include "rq_date.h"
#include "rq_system.h"
#include "rq_market.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

double bootstrapAssetCcy(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped);
double bootstrapAssetIROvernight(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped);
double bootstrapAssetIRDiscount(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped,
								const rq_calendar_t* cals, unsigned short numCals, rq_yield_curve_t ts);
void bootstrapAssetIRSwap(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped, rq_yield_curve_t ts, const rq_calendar_t* cals,
		unsigned short num_cals, rq_calendar_mgr_t,
        rq_date* last_swap_date, double* last_swap_rate, double* last_swap_period);
void bootstrapAssetIRBond(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped, rq_yield_curve_t ts, const rq_calendar_t* cals, unsigned short num_cals);
double bootstrapAssetIRFutureTenor(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped, rq_yield_curve_t ts, const rq_calendar_t* cals, unsigned short num_cals);
double bootstrapAssetIRFutureStrip(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped, rq_yield_curve_t ts,
        const rq_calendar_t* cals, unsigned short num_cals, rq_rate_t lastRate, rq_asset_t lastAsset, double lastFutureDF);
double bootstrapAssetCreditSpread(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped);

void setDiscountFactor(rq_yield_curve_t ts, rq_date for_date, double df);
void dumpYieldCurve(rq_system_t system, rq_yield_curve_t yc);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
