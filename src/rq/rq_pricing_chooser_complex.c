/*
** rq_pricing_chooser_complex.c
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
#include "rq_pricing_chooser_complex.h"
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


/* Complex chooser options */
RQ_EXPORT double 
rq_pricing_chooser_complex(
    double S, 
    double Xc, 
    double Xp, 
    double r, 
    double rf, 
    double v,
    double T, 
    double Tc, 
    double Tp 
    )
{
    double b = r - rf;
    double v_sqr = v * v;
    double d1;
    double d2;
    double y1;
    double y2;
    double rho1;
    double rho2;
    double Sv = S;
    double tc_minus_t = Tc - T;
    double tp_minus_t = Tp - T;
    double ci = rq_pricing_blackscholes_gen(1, Sv, Xc, tc_minus_t, r, b, v);
    double Pi = rq_pricing_blackscholes_gen(0, Sv, Xp, tp_minus_t, r, b, v);
    double dc = rq_pricing_blackscholes_delta(1, Sv, Xc, r, b, v, tc_minus_t, tc_minus_t);
    double dp = rq_pricing_blackscholes_delta(0, Sv, Xp, r, b, v, tp_minus_t, tp_minus_t);
    double yi = ci - Pi;
    double di = dc - dp;
    double epsilon = 0.001;
    /* Newton-Raphson skeprocess */
    while (fabs(yi) > epsilon)
    {
        Sv = Sv - (yi) / di;
        ci = rq_pricing_blackscholes_gen(1, Sv, Xc, tc_minus_t, r, b, v);
        Pi = rq_pricing_blackscholes_gen(0, Sv, Xp, tp_minus_t, r, b, v);
        dc = rq_pricing_blackscholes_delta(1, Sv, Xc, r, b, v, tc_minus_t, tc_minus_t);
        dp = rq_pricing_blackscholes_delta(0, Sv, Xp, r, b, v, tp_minus_t, tp_minus_t);
        yi = ci - Pi;
        di = dc - dp;
    }

    d1 = (log(S / Sv) + (b + v_sqr / 2) * T) / (v * sqrt(T));
    d2 = d1 - v * sqrt(T);
    y1 = (log(S / Xc) + (b + v_sqr / 2) * Tc) / (v * sqrt(Tc));
    y2 = (log(S / Xp) + (b + v_sqr / 2) * Tp) / (v * sqrt(Tp));
    rho1 = sqrt(T / Tc);
    rho2 = sqrt(T / Tp);
    
    return S * exp((b - r) * Tc) * rq_pricing_cumul_bivar_norm_dist(d1, y1, rho1) - Xc * exp(-r * Tc) * rq_pricing_cumul_bivar_norm_dist(d2, y1 - v * sqrt(Tc), rho1) - S * exp((b - r) * Tp) * rq_pricing_cumul_bivar_norm_dist(-d1, -y2, rho2) + Xp * exp(-r * Tp) * rq_pricing_cumul_bivar_norm_dist(-d2, -y2 + v * sqrt(Tp), rho2);
}

