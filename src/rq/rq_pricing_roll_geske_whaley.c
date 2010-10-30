/*
** rq_pricing_roll_geske_whaley.c
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
#include "rq_pricing_roll_geske_whaley.h"
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* American Calls on stocks with known dividends, Roll-Geske-Whaley */
RQ_EXPORT double
rq_pricing_roll_geske_whaley(
    double S, 
    double X, 
    double t1, /**< t1 time to dividend payout */
    double tau_e, /**< tau_e time to option expiration */
    double r, 
    double d, 
    double v
    )
{
    double Sx = S - d * exp(-r * t1);
    double I;
    double a1;
    double a2;
    double b1;
    double b2;
    double high_S;
    double low_S;
    double epsilon = 0.00001;
    double ci;
    double infinity = 100000000;

    if (d <= X * (1.0 - exp(-r * (tau_e - t1))))  /* Not optimal to exercise */
        return rq_pricing_blackscholes_gen(1, Sx, X, tau_e, r, 0, v);

    ci = rq_pricing_blackscholes_gen(1, S, X, tau_e - t1, r, 0, v);
    high_S = S;
    while ((ci - high_S - d + X) > 0 && high_S < infinity)
    {
        high_S = high_S * 2;
        ci = rq_pricing_blackscholes_gen(1, high_S, X, tau_e - t1, r, 0, v);
    }

    if (high_S > infinity)
        return rq_pricing_blackscholes_gen(1, Sx, X, tau_e, r, 0, v);
    
    low_S = 0;
    I = high_S * 0.5;
    ci = rq_pricing_blackscholes_gen(1, I, X, tau_e - t1, r, 0, v);
    
    /* Search algorithm to find the critical stock price I */
    while (fabs(ci - I - d + X) > epsilon && high_S - low_S > epsilon)
    {
        if ((ci - I - d + X) < 0)
            high_S = I;
        else
            low_S = I;

        I = (high_S + low_S) / 2;
        ci = rq_pricing_blackscholes_gen(1, I, X, tau_e - t1, r, 0, v);
    }

    a1 = (log(Sx / X) + (r + pow(v, 2) / 2) * tau_e) / (v * sqrt(tau_e));
    a2 = a1 - v * sqrt(tau_e);
    b1 = (log(Sx / I) + (r + pow(v, 2) / 2) * t1) / (v * sqrt(t1));
    b2 = b1 - v * sqrt(t1);
   
    return Sx * rq_pricing_cumul_norm_dist(b1) + Sx * rq_pricing_cumul_bivar_norm_dist(a1, -b1, -sqrt(t1 / tau_e)) - X * exp(-r * tau_e) * rq_pricing_cumul_bivar_norm_dist(a2, -b2, -sqrt(t1 / tau_e)) - (X - d) * exp(-r * t1) * rq_pricing_cumul_norm_dist(b2);
}
