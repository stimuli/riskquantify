/**
 * \file rq_bootstrap_yield_curve_day_count.h
 *
 * \brief A general bootstrapping with assets of different day count convention
 */
/*
** rq_bootstrap_yield_curve_day_count.h
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
#ifndef rq_bootstrap_yield_curve_day_count_h
#define rq_bootstrap_yield_curve_day_count_h

#include "rq_config.h"
#include "rq_date.h"
#include "rq_system.h"
#include "rq_market.h"
#include "rq_array.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_yc_day_count {
    rq_date date;
    enum rq_day_count_convention convention;
} *rq_yc_day_count_t;

typedef rq_array_t rq_yc_day_count_mgr;

double
bootstrapParDCF(const rq_asset_t asset,
              const rq_rate_t rate,
              rq_date* last_date_bootstrapped,
              rq_yield_curve_t ts,
		      const rq_calendar_t* cals,
              unsigned short num_cals,
              rq_rate_t lastRate,
              rq_asset_t lastAsset,
              double lastDF);

void
processSwapsDCF(const rq_asset_t asset,
                const rq_rate_t rate,
                rq_date* last_date_bootstrapped,
                rq_yield_curve_t ts,
                const rq_calendar_t* cals,
                unsigned short num_cals,
				rq_calendar_mgr_t,
                rq_date* last_swap_date,
                double* last_swap_rate,
                double* last_year_count_frac,
                enum rq_swapDayCount_method swapDayCount_method,
				enum rq_swapParInterpolation_method parInterpolation_method,
				const rq_asset_t *splineSwapAssets,
				const rq_rate_t *splineSwapRates,
				int numSwaps);

RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_day_count(
    const char *curve_id,
    const rq_system_t system,
    const rq_market_t market,
    double *rate_adjust_factors, /**< if non-NULL, the factors to multiply the rate by before bootstrapping. This is an array of doubles the size of the number of bootstrap configs. */
    rq_date last_date_to_bootstrap, /**< if non-zero, the last date to bootstrap on the yield curve */
    int force_backward_compatible_daycount,  /* fix swap daycount method to RQ_SWAP_DAY_COUNT_DAYCOUNT for the old adaptor "BootstrapYieldCurveDayCount" */
	const char *debug_filename
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
