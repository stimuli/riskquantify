/**
 * \file rq_pricing_bjerksund_stensland.h
 * \author Brett Hutley
 *
 * \brief The Bjerksund-Stensland american option approximation
 * Adapted from "The Complete Guide to Option Pricing Formulas" by
 * Espen Gaarder Haug
 */
/*
** rq_pricing_bjerksund_stensland.h
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
#ifndef rq_pricing_bjerksund_stensland_h
#define rq_pricing_bjerksund_stensland_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** This function can be used to price american options on stocks, futures and currencies.
 */
RQ_EXPORT double 
rq_pricing_bjerksund_stensland(
    int call, /**< non-zero if call option, otherwise put */
    double S, /**< asset price */
    double X, /**< strike price */
    double r, /**< domestic interest rate */
    double rf, /**< foreign interest rate */
    double v, /**< volatility */
    double T /**< time to expiry in years */
    );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
