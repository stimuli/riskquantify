/*
** rq_pricing_extendible_maturity.c
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
#include "rq_pricing_extendible_maturity.h"
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

RQ_EXPORT double 
rq_pricing_extendible_maturity_writer(
    short call,
    double S, 
    double X1, 
    double X2, 
    double t1,
    double T2, 
    double r, 
    double b, 
    double v
    )
{
    double ret = 0.0;
    double v_sqr = v * v;
    double rho = sqrt(t1 / T2);
    double z1 = (log(S / X2) + (b + v_sqr / 2.0) * T2) / (v * sqrt(T2));
    double z2 = (log(S / X1) + (b + v_sqr / 2.0) * t1) / (v * sqrt(t1));

    if (call)
        ret = rq_pricing_blackscholes_gen(call, S, X1, t1, r, b, v) + S * exp((b - r) * T2) * rq_pricing_cumul_bivar_norm_dist(z1, -z2, -rho) - X2 * exp(-r * T2) * rq_pricing_cumul_bivar_norm_dist(z1 - sqrt(v_sqr * T2), -z2 + sqrt(v * v * t1), -rho);
    else
        ret = rq_pricing_blackscholes_gen(call, S, X1, t1, r, b, v) + X2 * exp(-r * T2) * rq_pricing_cumul_bivar_norm_dist(-z1 + sqrt(v_sqr * T2), z2 - sqrt(v_sqr * t1), -rho) - S * exp((b - r) * T2) * rq_pricing_cumul_bivar_norm_dist(-z1, z2, -rho);

    return ret;
}
