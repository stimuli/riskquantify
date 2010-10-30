/*
** rq_pricing_swaption.c
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
#include "rq_pricing_swaption.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <math.h>

RQ_EXPORT double 
rq_pricing_swaption(
    short call,
    double t1, 
    double m, 
    double F, 
    double X, 
    double T,
    double r, 
    double v
    )
{
    double d1;
    double d2;
    double ret = 0.0;
    
    d1 = (log(F / X) + (v * v) / 2 * T) / (v * sqrt(T));
    d2 = d1 - v * sqrt(T);

    if (call) /* Payer swaption */
        ret = ((1 - 1 / pow((1 + F / m), (t1 * m))) / F) * exp(-r * T) * (F * rq_pricing_cumul_norm_dist(d1) - X * rq_pricing_cumul_norm_dist(d2));
    else /* Receiver swaption */
        ret = ((1 - 1 / pow((1 + F / m), (t1 * m))) / F) * exp(-r * T) * (X * rq_pricing_cumul_norm_dist(-d2) - F * rq_pricing_cumul_norm_dist(-d1));

    return ret;
}
