/**
 * \file rq_forward_rate_imply.h
 * \author Brett Hutley
 *
 * \brief A routine for implying forward rates from yield curves
 */
/*
** rq_forward_rate_imply.h
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
#ifndef rq_forward_rate_imply_h
#define rq_forward_rate_imply_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_yield_curve.h"
#include "rq_asset.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double
rq_forward_rate_imply_from_yield_curves(
    rq_date date,
    const rq_asset_t asset_ccypair,
    double spot_rate,
    const rq_yield_curve_t yield_curve_1,
    const rq_yield_curve_t yield_curve_2
    );

RQ_EXPORT double 
rq_forward_rate_imply_direct_rate_from_yield_curves(
    rq_date date, 
    const rq_asset_t asset_ccypair,
    double spot_rate,
    const rq_yield_curve_t yield_curve_1, 
    const rq_yield_curve_t yield_curve_2,
    const char *ccy_from,
    const char *ccy_to
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
