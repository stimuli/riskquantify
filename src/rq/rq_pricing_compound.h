/**
 * \file rq_pricing_compound.h
 * \author Brett Hutley
 *
 * \brief Pricing routines for compound options
 */
/*
** rq_pricing_compound.h
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
#ifndef rq_pricing_compound_h
#define rq_pricing_compound_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** This function prices an option on an option */
RQ_EXPORT double 
rq_pricing_compound(
    int outer_call, /**< non-zero if the compound option is a call */
    int inner_call, /**< non-zero if the contained option is a call */
    double S, /**< the asset price */
    double X_outer, /**< the compound option strike price */
    double X_inner, /**< the contained option strike price */
    double T_outer, /**< the time from today to the compound option expiry in years */
    double T_inner, /**< the time from today to the contained option expiry in years */
    double r, /**< the domestic interest rate, continuously compounded */
    double rf, /**< the foreign interest rate, continuously compounded */
    double v /**< the volatility */
    );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
