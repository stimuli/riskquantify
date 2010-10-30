/**
 * \file rq_pricing_extreme_spread.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_extreme_spread files provide pricing routines
 * for Extreme Spread options.
 */
/*
** rq_pricing_extreme_spread.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#ifndef rq_pricing_extreme_spread_h
#define rq_pricing_extreme_spread_h

#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** This function prices an extreme spread option.
 */
RQ_EXPORT double 
rq_pricing_extreme_spread(
    int call, /**< non-zero if a call option */
    int extreme_spread, /**< non-zero for extreme spread, zero for reverse extreme spread */
    double S, /**< asset price */
    double SMin, /**< minimum asset price */
    double SMax, /**< maximum asset price */
    double t1, /**< seperation point in years */
    double T, /**< time to expiry in years */
    double r, /**< domestic interest rate (continuously compounded) */
    double rf, /**< foreign interest rate (continuously compounded) */
    double v /**< volatility */
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
