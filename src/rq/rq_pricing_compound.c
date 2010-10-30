/*
** rq_pricing_compound.c
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
#include "rq_pricing_compound.h"
#include "rq_pricing_normdist.h"
#include "rq_pricing_blackscholes.h"
#include <stdlib.h>
#include <math.h>

/* Calculation of critical price options on options */
static double 
critical_value(
    int call, 
    double X_inner, 
    double X_outer, 
    double T,
    double r, 
    double b, 
    double v
    )
{
    double Si;
    double ci;
    double di;
    double epsilon;
    
    Si = X_inner;
    ci = rq_pricing_blackscholes_gen(call, Si, X_inner, T, r, b, v);
    di = rq_pricing_blackscholes_delta(call, Si, X_inner, r, b, v, T, T);
    epsilon = 0.000001;
    /* Newton-Raphson algorithm */
    while (fabs(ci - X_outer) > epsilon)
    {
        Si = Si - (ci - X_outer) / di;
        ci = rq_pricing_blackscholes_gen(call, Si, X_inner, T, r, b, v);
        di = rq_pricing_blackscholes_delta(call, Si, X_inner, r, b, v, T, T);
    }

    return Si;
}

RQ_EXPORT double 
rq_pricing_compound(
    int outer_call,
    int inner_call,
    double S, 
    double X_outer, 
    double X_inner, 
    double T_outer,
    double T_inner, 
    double r, 
    double rf, 
    double v
    )
{
    double y1;
    double y2;
    double z1;
    double z2;
    double I;
    double rho;
    double b = r - rf;

    double ret = 0.0;
    
    I = critical_value(inner_call, X_inner, X_outer, T_inner - T_outer, r, b, v);
    
    rho = sqrt(T_outer / T_inner);
    y1 = (log(S / I) + (b + (v * v) / 2) * T_outer) / (v * sqrt(T_outer));
    y2 = y1 - v * sqrt(T_outer);
    z1 = (log(S / X_inner) + (b + (v * v) / 2) * T_inner) / (v * sqrt(T_inner));
    z2 = z1 - v * sqrt(T_inner);

    if (outer_call && inner_call)
        ret = S * exp((b - r) * T_inner) * rq_pricing_cumul_bivar_norm_dist(z1, y1, rho) - X_inner * exp(-r * T_inner) * rq_pricing_cumul_bivar_norm_dist(z2, y2, rho) - X_outer * exp(-r * T_outer) * rq_pricing_cumul_norm_dist(y2);
    else if (!outer_call && inner_call)
        ret = X_inner * exp(-r * T_inner) * rq_pricing_cumul_bivar_norm_dist(z2, -y2, -rho) - S * exp((b - r) * T_inner) * rq_pricing_cumul_bivar_norm_dist(z1, -y1, -rho) + X_outer * exp(-r * T_outer) * rq_pricing_cumul_norm_dist(-y2);
    else if (outer_call && !inner_call)
        ret = X_inner * exp(-r * T_inner) * rq_pricing_cumul_bivar_norm_dist(-z2, -y2, rho) - S * exp((b - r) * T_inner) * rq_pricing_cumul_bivar_norm_dist(-z1, -y1, rho) - X_outer * exp(-r * T_outer) * rq_pricing_cumul_norm_dist(-y2);
    else /* !outer_call && !inner_call */
        ret = S * exp((b - r) * T_inner) * rq_pricing_cumul_bivar_norm_dist(-z1, y1, -rho) - X_inner * exp(-r * T_inner) * rq_pricing_cumul_bivar_norm_dist(-z2, y2, -rho) + exp(-r * T_outer) * X_outer * rq_pricing_cumul_norm_dist(y2);

    return ret;
}

