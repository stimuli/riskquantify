/*
** rq_pricing_barone_adesi_whaley.c
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
#include "rq_pricing_barone_adesi_whaley.h"
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* American call */
static double 
rq_pricing_barone_adesi_whaley_amer_call(
    double S, 
    double X, 
    double T,
    double r, 
    double b, 
    double v
    )
{
    double ret;

    if (b >= r)
        ret = rq_pricing_blackscholes_gen(1, S, X, T, r, b, v);
    else
    {
        double v_sqr = v * v;
        double n = 2.0 * b / v_sqr;
        double m = 2.0 * r / v_sqr;
        double q2u = (-(n - 1) + sqrt(pow((n - 1), 2) + 4 * m)) / 2.0;
        double Su = X / (1 - 1 / q2u);
        double h2 = -(b * T + 2.0 * v * sqrt(T)) * X / (Su - X);
        double Si = X + (Su - X) * (1 - exp(h2));
        double K = 2 * r / (v_sqr * (1 - exp(-r * T)));
        double d1 = (log(Si / X) + (b + v_sqr / 2) * T) / (v * sqrt(T));
        double Q2 = (-(n - 1.0) + sqrt(pow((n - 1.0), 2.0) + 4.0 * K)) / 2.0;
        double LHS = Si - X;
        double RHS = rq_pricing_blackscholes_gen(1, Si, X, T, r, b, v) + (1 - exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1)) * Si / Q2;
        double bi = exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1) * (1.0 - 1.0 / Q2) + (1.0 - exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1) / (v * sqrt(T))) / Q2;
        double E = 0.000001;

    
        /* Newton Raphson algorithm for finding critical price Si */
        while (fabs(LHS - RHS) / X > E)
        {
            Si = (X + RHS - bi * Si) / (1 - bi);
            d1 = (log(Si / X) + (b + (v * v) / 2) * T) / (v * sqrt(T));
            LHS = Si - X;
            RHS = rq_pricing_blackscholes_gen(1, Si, X, T, r, b, v) + (1 - exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1)) * Si / Q2;
            bi = exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1) * (1.0 - 1.0 / Q2) + (1.0 - exp((b - r) * T) * rq_pricing_norm_dist(d1) / (v * sqrt(T))) / Q2;
        }

        if (S < Si)
        {
            double a2;

            Q2 = (-(n - 1) + sqrt(pow((n - 1), 2) + 4 * K)) / 2;
            a2 = (Si / Q2) * (1 - exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1));
            ret = rq_pricing_blackscholes_gen(1, S, X, T, r, b, v) + a2 * pow((S / Si), Q2);
        }
        else
            ret = S - X;
    }

    return ret;
}

/* Newton Raphson algorithm to solve for the critical commodity price
   for a Put 
*/
static double 
Kp(
    double X, 
    double T, 
    double r, 
    double b, 
    double v
    )
{
    double v_sqr = v * v;
    double n = 2 * b / v_sqr;
    double m = 2 * r / v_sqr;
    double q1u = (-(n - 1) - sqrt(pow((n - 1), 2) + 4.0 * m)) / 2.0;
    double Su = X / (1.0 - 1.0 / q1u);
    double h1 = (b * T - 2.0 * v * sqrt(T)) * X / (X - Su);
    double Si = Su + (X - Su) * exp(h1);
    double K = 2.0 * r / (v_sqr * (1.0 - exp(-r * T)));
    double d1 = (log(Si / X) + (b + v_sqr / 2.0) * T) / (v * sqrt(T));
    double Q1 = (-(n - 1) - sqrt(pow((n - 1), 2.0) + 4.0 * K)) / 2.0;
    double LHS = X - Si;
    double RHS = rq_pricing_blackscholes_gen(0, Si, X, T, r, b, v) - (1.0 - exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1)) * Si / Q1;
    double bi = -exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1) * (1.0 - 1.0 / Q1) - (1.0 + exp((b - r) * T) * rq_pricing_norm_dist(-d1) / (v * sqrt(T))) / Q1;
    double E = 0.000001;
    
    /* Newton Raphson algorithm for finding critical price Si */
    while (fabs(LHS - RHS) / X > E)
    {
        Si = (X - RHS + bi * Si) / (1.0 + bi);
        d1 = (log(Si / X) + (b + v_sqr / 2.0) * T) / (v * sqrt(T));
        LHS = X - Si;
        RHS = rq_pricing_blackscholes_gen(0, Si, X, T, r, b, v) - (1.0 - exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1)) * Si / Q1;
        bi = -exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1) * (1.0 - 1.0 / Q1) - (1.0 + exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1) / (v * sqrt(T))) / Q1;
    }

    return Si;
}


/* American put */
static double 
rq_pricing_barone_adesi_whaley_amer_put(
    double S, 
    double X, 
    double T, 
    double r, 
    double b, 
    double v
    )
{
    double ret = 0.0;
    double Sk = Kp(X, T, r, b, v);

    if (S > Sk)
    {
        double v_sqr = v * v;
        double n = 2.0 * b / v_sqr;
        double K = 2.0 * r / (v_sqr * (1.0 - exp(-r * T)));
        double d1 = (log(Sk / X) + (b + v_sqr / 2.0) * T) / (v * sqrt(T));
        double Q1 = (-(n - 1) - sqrt(pow((n - 1), 2) + 4.0 * K)) / 2.0;
        double a1 = -(Sk / Q1) * (1.0 - exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1));
        ret = rq_pricing_blackscholes_gen(0, S, X, T, r, b, v) + a1 * pow((S / Sk), Q1);
    }
    else
        ret = X - S;

    return ret;
}

/* The Barone-Adesi and Whaley (1987) American approximation */
RQ_EXPORT double 
rq_pricing_barone_adesi_whaley(
    short call,
    double S, 
    double X, 
    double r, 
    double rf, 
    double v,
    double T 
    )
{
    double ret = 0.0;
    double b = r - rf;

    if (call)
        ret = rq_pricing_barone_adesi_whaley_amer_call(S, X, T, r, b, v);
    else 
        ret = rq_pricing_barone_adesi_whaley_amer_put(S, X, T, r, b, v);

    return ret;
}
