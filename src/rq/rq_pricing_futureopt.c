/*
** rq_pricing_futureopt.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
#include "rq_config.h"
#include <math.h>
#include "rq_pricing_normdist.h"
#include "rq_pricing_futureopt.h"

RQ_EXPORT double 
rq_pricing_futureopt(
    int call,
    double F,     /*  futures price */
    double X,     /*  exercise price */
    double r_dom, /*  continuously compounded rate */
    double sigma, /*  volatility */
    double tau_e, /*  time to expiry (in years) */
    double tau_d  /*  time to delivery (in years) */
    )
{
    double m = (call ? 1.0 : -1.0);
    double sigma_sqr = sigma * sigma;
    double time_sqrt = sqrt(tau_e);
    double d1 = (log(F / X) + 0.5 * sigma_sqr * tau_e) / (sigma * time_sqrt);
    double d2 = d1 - sigma * time_sqrt * m;
    d1 *= m;

    return exp(-r_dom * tau_d) * 
        ((F * rq_pricing_cumul_norm_dist(d1) * m) +
         (X * rq_pricing_cumul_norm_dist(d2) * -m));
}
