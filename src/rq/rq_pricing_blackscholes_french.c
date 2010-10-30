/*
** rq_pricing_blackscholes_french.c
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
#include "rq_pricing_blackscholes_french.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* French (1984) adjusted Black and Scholes model for trading day volatility */
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
    )
{
    double ret = 0.0;
    
    double d1 = (log(S / X) + b * T + pow(v, 2) / 2 * t1) / (v * sqrt(t1));
    double d2 = d1 - v * sqrt(t1);
  
    if (call)
        ret = S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1) - X * exp(-r * T) * rq_pricing_cumul_norm_dist(d2);
    else 
        ret = X * exp(-r * T) * rq_pricing_cumul_norm_dist(-d2) - S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1);

    return ret;
}
