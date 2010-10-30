/**
 * \file rq_bootstrap_yield_curve_composite.h
 * \author Brett Hutley
 *
 * \brief A function to bootstrap a yield curve from a yield curve
 *  and spread points.
 */
/*
** rq_bootstrap_yield_curve_composite.h
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
#ifndef rq_bootstrap_yield_curve_composite_h
#define rq_bootstrap_yield_curve_composite_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_yield_curve.h"
#include "rq_asset_mgr.h"
#include "rq_rate_mgr.h"
#include "rq_bootstrap_config.h"
#include "rq_interpolate.h"
#include "rq_system.h"
#include "rq_market.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- prototypes -------------------------------------------------- */

RQ_EXPORT rq_yield_curve_t
rq_bootstrap_yield_curve_composite(
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
