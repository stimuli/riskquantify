/**
 * \file rq_pricing_blackscholes_french.h
 * \author Brett Hutley
 *
 * \brief French (1984) adjusted Black-Scholes model for trading day
 * volatility.  Taken from the book "The Complete Guide to Option
 * Pricing Formulas" by Espen Gaarder Haug.
 */
/*
** rq_pricing_blackscholes_french.h
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
#ifndef rq_pricing_blackscholes_french_h
#define rq_pricing_blackscholes_french_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double
rq_pricing_blackscholes_french(
    short call, 
    double S, 
    double X, 
    double T, 
    double t1,
    double r, 
    double b, 
    double v
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
