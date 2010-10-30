/*
** rq_pricing_extreme_spread.c
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
#include "rq_pricing_extreme_spread.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <math.h>

RQ_EXPORT double 
rq_pricing_extreme_spread(
    int call,
    int extreme_spread,
    double S, 
    double S_min, 
    double S_max, 
    double t1, 
    double T, 
    double r, 
    double rf, 
    double v
    )
{
    double m;
    double Mo;
    double mu1;
    double mu;
    int kappa;
    int eta;
    double ret = 0.0;
    double b = r - rf;
    
    if (call)
        eta = 1;
    else
        eta = -1;

    if (extreme_spread)
        kappa = 1;
    else
        kappa = -1;

    if (kappa * eta == 1)
        Mo = S_max;
    else if (kappa * eta == -1)
        Mo = S_min;
        
    mu1 = b - (v * v) / 2;
    mu = mu1 + (v * v);
    m = log(Mo / S);

    if (kappa == 1) /* Extreme Spread Option */
        ret = eta * (S * exp((b - r) * T) * (1 + (v * v) / (2 * b)) * rq_pricing_cumul_norm_dist(eta * (-m + mu * T) / (v * sqrt(T))) - exp(-r * (T - t1)) * S * exp((b - r) * T) * (1 + (v * v) / (2 * b)) * rq_pricing_cumul_norm_dist(eta * (-m + mu * t1) / (v * sqrt(t1))) 
                     + exp(-r * T) * Mo * rq_pricing_cumul_norm_dist(eta * (m - mu1 * T) / (v * sqrt(T))) - exp(-r * T) * Mo * (v * v) / (2 * b) * exp(2 * mu1 * m / (v * v)) * rq_pricing_cumul_norm_dist(eta * (-m - mu1 * T) / (v * sqrt(T))) 
                     - exp(-r * T) * Mo * rq_pricing_cumul_norm_dist(eta * (m - mu1 * t1) / (v * sqrt(t1))) + exp(-r * T) * Mo * (v * v) / (2 * b) * exp(2 * mu1 * m / (v * v)) * rq_pricing_cumul_norm_dist(eta * (-m - mu1 * t1) / (v * sqrt(t1))));
    else if (kappa == -1)  /* Reverse Extreme Spread Option */
        ret = -eta * (S * exp((b - r) * T) * (1 + (v * v) / (2 * b)) * rq_pricing_cumul_norm_dist(eta * (m - mu * T) / (v * sqrt(T))) + exp(-r * T) * Mo * rq_pricing_cumul_norm_dist(eta * (-m + mu1 * T) / (v * sqrt(T))) 
                      - exp(-r * T) * Mo * (v * v) / (2 * b) * exp(2 * mu1 * m / (v * v)) * rq_pricing_cumul_norm_dist(eta * (m + mu1 * T) / (v * sqrt(T))) - S * exp((b - r) * T) * (1 + (v * v) / (2 * b)) * rq_pricing_cumul_norm_dist(eta * (-mu * (T - t1)) / (v * sqrt(T - t1))) 
                      - exp(-r * (T - t1)) * S * exp((b - r) * T) * (1 - (v * v) / (2 * b)) * rq_pricing_cumul_norm_dist(eta * (mu1 * (T - t1)) / (v * sqrt(T - t1))));

    return ret;
}
