/**
 * \file rq_bootstrap_yield_curve_cubic_spline.h
 * \author Joey Nicotra
 *
 * \brief A combination of general bootstrapping with cubic splines
 */
/*
** rq_bootstrap_yield_curve_cubic_spline.h
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
#ifndef rq_bootstrap_yield_curve_cubic_spline_h
#define rq_bootstrap_yield_curve_cubic_spline_h

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


void
processSwaps(rq_asset_list_t swapAssetsList,	/**< the swap asset set to contribute to the curve */
			 double* swapRates,		/**< all the swap rates to contribute to the curve */
			 rq_date* lastDateBootstrapped,	/**< last date of the shortend already bootstrapped */
			 rq_yield_curve_t ts,		/**< termstructure (with bootstrapped short end */
			 const rq_calendar_t* cals,	/**< calendars */
			 unsigned short num_cals	/**< number of calendars */
			 );

RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_cubic_spline(
    const char *curve_id,
    const rq_system_t system,
    const rq_market_t market,
    double *rate_adjust_factors, /**< if non-NULL, the factors to multiply the rate by before bootstrapping. This is an array of doubles the size of the number of bootstrap configs. */
    rq_date last_date_to_bootstrap /**< if non-zero, the last date to bootstrap on the yield curve */
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
