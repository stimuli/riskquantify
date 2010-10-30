/*
** rq_pricing_lookback.c
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
#include <math.h>
#include "rq_pricing_normdist.h"
#include "rq_pricing_lookback.h"

/* NOTE: at the moment this function only handles call options! */
RQ_EXPORT double 
rq_pricing_lookback(
    short call,
    double S,
    double Smin,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d
    )
{
    double sigma_sqr = sigma * sigma;
    double tau_e_sqrt = sqrt(tau_e);
    double a1 = (log(S / Smin) + (r_dom - r_for + sigma_sqr / 2.0) * tau_e) /
        (sigma * tau_e_sqrt);
    double a2 = a1 - sigma * tau_e_sqrt;
    double a3 = (log(S / Smin) + ((-r_dom + r_for) * tau_d) + ((sigma_sqr / 2.0) * tau_e)) / 
        (sigma * tau_e_sqrt);
    double Y1 = 2.0 * (r_dom - r_for - sigma_sqr / 2.0) * log(S / Smin) / 
        sigma_sqr;
    return S * exp(-r_for * tau_d) * rq_pricing_cumul_norm_dist(a1) -
        S * exp(-r_for * tau_d) * (sigma_sqr / (2.0 * (r_dom - r_for))) * 
        rq_pricing_cumul_norm_dist(-a1) -
        Smin * exp(-r_dom * tau_d) * 
        (rq_pricing_cumul_norm_dist(a2) - 
         (sigma_sqr / (2*(r_dom - r_for))) * 
         exp(Y1) * rq_pricing_cumul_norm_dist(-a3));
}

RQ_EXPORT double 
rq_pricing_lookback_floating_strike(
    short call, 
    double S, 
    double SMin, 
    double SMax, 
    double T, 
    double r, 
    double b, 
    double v
    )
{
    double a1;
    double a2;
    double m;
    double ret = 0.0;
    
    if (call)
        m = SMin;
    else
        m = SMax;
    
    a1 = (log(S / m) + (b + (v * v) / 2) * T) / (v * sqrt(T));
    a2 = a1 - v * sqrt(T);

    if (call)
        ret = S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(a1) - m * exp(-r * T) * rq_pricing_cumul_norm_dist(a2) + 
            exp(-r * T) * (v * v) / (2 * b) * S * (pow((S / m), (-2 * b / (v * v))) * rq_pricing_cumul_norm_dist(-a1 + 2 * b / v * sqrt(T)) - exp(b * T) * rq_pricing_cumul_norm_dist(-a1));
    else
        ret = m * exp(-r * T) * rq_pricing_cumul_norm_dist(-a2) - S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(-a1) + 
            exp(-r * T) * (v * v) / (2 * b) * S * (pow(-(S / m), (-2 * b / (v * v))) * rq_pricing_cumul_norm_dist(a1 - 2 * b / v * sqrt(T)) + exp(b * T) * rq_pricing_cumul_norm_dist(a1));

    return ret;
}

RQ_EXPORT double 
rq_pricing_lookback_fixed_strike(
    short call, 
    double S, 
    double SMin, 
    double SMax, 
    double X,
    double T, 
    double r, 
    double b, 
    double v
    )
{
    double d1;
    double d2;
    double e1;
    double e2;
    double m;
    double ret = 0.0;
    
    if (call)
        m = SMax;
    else
        m = SMin;
    
    d1 = (log(S / X) + (b + (v * v) / 2) * T) / (v * sqrt(T));
    d2 = d1 - v * sqrt(T);
    e1 = (log(S / m) + (b + (v * v) / 2) * T) / (v * sqrt(T));
    e2 = e1 - v * sqrt(T);
    
    if (call && X > m)
        ret = S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(d1) - X * exp(-r * T) * rq_pricing_cumul_norm_dist(d2) 
            + S * exp(-r * T) * (v * v) / (2 * b) * (pow(-(S / X), (-2 * b / (v * v))) * rq_pricing_cumul_norm_dist(d1 - 2 * b / v * sqrt(T)) + exp(b * T) * rq_pricing_cumul_norm_dist(d1));
    else if (call && X <= m)
        ret = exp(-r * T) * (m - X) + S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(e1) - exp(-r * T) * m * rq_pricing_cumul_norm_dist(e2) 
            + S * exp(-r * T) * (v * v) / (2 * b) * (pow(-(S / m), (-2 * b / (v * v))) * rq_pricing_cumul_norm_dist(e1 - 2 * b / v * sqrt(T)) + exp(b * T) * rq_pricing_cumul_norm_dist(e1));
    else if (!call && X < m)
        ret = -S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(-d1) + X * exp(-r * T) * rq_pricing_cumul_norm_dist(-d1 + v * sqrt(T)) 
            + S * exp(-r * T) * (v * v) / (2 * b) * (pow((S / X), (-2 * b / (v * v))) * rq_pricing_cumul_norm_dist(-d1 + 2 * b / v * sqrt(T)) - exp(b * T) * rq_pricing_cumul_norm_dist(-d1));
    else /* if (!call && X >= m) */
        ret = exp(-r * T) * (X - m) - S * exp((b - r) * T) * rq_pricing_cumul_norm_dist(-e1) + exp(-r * T) * m * rq_pricing_cumul_norm_dist(-e1 + v * sqrt(T)) 
            + exp(-r * T) * (v * v) / (2 * b) * S * (pow((S / m), (-2 * b / (v * v))) * rq_pricing_cumul_norm_dist(-e1 + 2 * b / v * sqrt(T)) - exp(b * T) * rq_pricing_cumul_norm_dist(-e1));

    return ret;
}

RQ_EXPORT double
rq_pricing_lookback_partial_time_floating_strike(
    short call, 
    double S, 
    double SMin, 
    double SMax, 
    double t1,
    double T2, 
    double r, 
    double b, 
    double v, 
    double lambda
    )
{
    double d1;
    double d2;
    double e1;
    double e2;
    double f1;
    double f2;
    double g1;
    double g2;
    double m;
    double part1;
    double part2;
    double part3;
    
    if (call)
        m = SMin;
    else
        m = SMax;
    
    d1 = (log(S / m) + (b + (v * v) / 2) * T2) / (v * sqrt(T2));
    d2 = d1 - v * sqrt(T2);
    e1 = (b + (v * v) / 2) * (T2 - t1) / (v * sqrt(T2 - t1));
    e2 = e1 - v * sqrt(T2 - t1);
    f1 = (log(S / m) + (b + (v * v) / 2) * t1) / (v * sqrt(t1));
    f2 = f1 - v * sqrt(t1);
    g1 = log(lambda) / (v * sqrt(T2));
    g2 = log(lambda) / (v * sqrt(T2 - t1));

    if (call)
    {
        part1 = S * exp((b - r) * T2) * rq_pricing_cumul_norm_dist(d1 - g1) - lambda * m * exp(-r * T2) * rq_pricing_cumul_norm_dist(d2 - g1);
        part2 = exp(-r * T2) * (v * v) / (2 * b) * lambda * S * 
            (pow((S / m), (-2 * b / (v * v))) * rq_pricing_cumul_bivar_norm_dist(-f1 + 2 * b * sqrt(t1) / v, -d1 + 2 * b * sqrt(T2) / v - g1, sqrt(t1 / T2)) 
             - exp(b * T2) * pow(lambda, (2 * b / (v * v))) * rq_pricing_cumul_bivar_norm_dist(-d1 - g1, e1 + g2, -sqrt(1 - t1 / T2))) 
            + S * exp((b - r) * T2) * rq_pricing_cumul_bivar_norm_dist(-d1 + g1, e1 - g2, -sqrt(1 - t1 / T2));
        part3 = exp(-r * T2) * lambda * m * rq_pricing_cumul_bivar_norm_dist(-f2, d2 - g1, -sqrt(t1 / T2)) 
            - exp(-b * (T2 - t1)) * exp((b - r) * T2) * (1 + (v * v) / (2 * b)) * lambda * S * rq_pricing_cumul_norm_dist(e2 - g2) * rq_pricing_cumul_norm_dist(-f1);
    }
    else
    {
        part1 = lambda * m * exp(-r * T2) * rq_pricing_cumul_norm_dist(-d2 + g1) - S * exp((b - r) * T2) * rq_pricing_cumul_norm_dist(-d1 + g1);
        part2 = -exp(-r * T2) * (v * v) / (2 * b) * lambda * S * 
            (pow((S / m), (-2 * b / (v * v))) * rq_pricing_cumul_bivar_norm_dist(f1 - 2 * b * sqrt(t1) / v, d1 - 2 * b * sqrt(T2) / v + g1, sqrt(t1 / T2)) 
             - exp(b * T2) * pow(lambda, (2 * b / (v * v))) * rq_pricing_cumul_bivar_norm_dist(d1 + g1, -e1 - g2, -sqrt(1 - t1 / T2))) 
            - S * exp((b - r) * T2) * rq_pricing_cumul_bivar_norm_dist(d1 - g1, -e1 + g2, -sqrt(1 - t1 / T2));
        part3 = -exp(-r * T2) * lambda * m * rq_pricing_cumul_bivar_norm_dist(f2, -d2 + g1, -sqrt(t1 / T2)) 
            + exp(-b * (T2 - t1)) * exp((b - r) * T2) * (1 + (v * v) / (2 * b)) * lambda * S * rq_pricing_cumul_norm_dist(-e2 + g2) * rq_pricing_cumul_norm_dist(f1);
    }
    
    return part1 + part2 + part3;
}

RQ_EXPORT double 
rq_pricing_lookback_partial_time_fixed_strike(
    short call, 
    double S, 
    double X, 
    double t1,
    double T2, 
    double r, 
    double b, 
    double v
    )
{
    double d1;
    double d2;
    double e1;
    double e2;
    double f1;
    double f2;
    double ret = 0.0;

    d1 = (log(S / X) + (b + (v * v) / 2) * T2) / (v * sqrt(T2));
    d2 = d1 - v * sqrt(T2);
    e1 = ((b + (v * v) / 2) * (T2 - t1)) / (v * sqrt(T2 - t1));
    e2 = e1 - v * sqrt(T2 - t1);
    f1 = (log(S / X) + (b + (v * v) / 2) * t1) / (v * sqrt(t1));
    f2 = f1 - v * sqrt(t1);

    if (call)
        ret = S * exp((b - r) * T2) * rq_pricing_cumul_norm_dist(d1) - exp(-r * T2) * X * rq_pricing_cumul_norm_dist(d2) + S * exp(-r * T2) * (v * v) / (2 * b) * (pow(-(S / X), (-2 * b / (v * v))) * rq_pricing_cumul_bivar_norm_dist(d1 - 2 * b * sqrt(T2) / v, -f1 + 2 * b * sqrt(t1) / v, -sqrt(t1 / T2)) + exp(b * T2) * rq_pricing_cumul_bivar_norm_dist(e1, d1, sqrt(1 - t1 / T2))) - S * exp((b - r) * T2) * rq_pricing_cumul_bivar_norm_dist(-e1, d1, -sqrt(1 - t1 / T2)) - X * exp(-r * T2) * rq_pricing_cumul_bivar_norm_dist(f2, -d2, -sqrt(t1 / T2)) + exp(-b * (T2 - t1)) * (1 - (v * v) / (2 * b)) * S * exp((b - r) * T2) * rq_pricing_cumul_norm_dist(f1) * rq_pricing_cumul_norm_dist(-e2);
    else
        ret = X * exp(-r * T2) * rq_pricing_cumul_norm_dist(-d2) - S * exp((b - r) * T2) * rq_pricing_cumul_norm_dist(-d1) + S * exp(-r * T2) * (v * v) / (2 * b) * (pow((S / X), (-2 * b / (v * v))) * rq_pricing_cumul_bivar_norm_dist(-d1 + 2 * b * sqrt(T2) / v, f1 - 2 * b * sqrt(t1) / v, -sqrt(t1 / T2)) - exp(b * T2) * rq_pricing_cumul_bivar_norm_dist(-e1, -d1, sqrt(1 - t1 / T2))) + S * exp((b - r) * T2) * rq_pricing_cumul_bivar_norm_dist(e1, -d1, -sqrt(1 - t1 / T2)) + X * exp(-r * T2) * rq_pricing_cumul_bivar_norm_dist(-f2, d2, -sqrt(t1 / T2)) - exp(-b * (T2 - t1)) * (1 - (v * v) / (2 * b)) * S * exp((b - r) * T2) * rq_pricing_cumul_norm_dist(-f1) * rq_pricing_cumul_norm_dist(e2);

    return ret;
}
