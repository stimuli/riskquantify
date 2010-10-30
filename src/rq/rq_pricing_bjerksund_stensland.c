
/*
** rq_pricing_bjerksund_stensland.c
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
#include "rq_pricing_bjerksund_stensland.h"
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_normdist.h"
#include "rq_math.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static double 
phi(
    double S, 
    double T, 
    double gamma, 
    double H, 
    double I,
    double r, 
    double b, 
    double v
    )
{
    double lambda;
    double kappa;
    double d;
    
    lambda = (-r + gamma * b + 0.5 * gamma * (gamma - 1) * (v * v)) * T;
    d = -(log(S / H) + (b + (gamma - 0.5) * (v * v)) * T) / (v * sqrt(T));
    kappa = 2 * b / (v * v) + (2 * gamma - 1);
    return exp(lambda) * pow(S, gamma) * (rq_pricing_cumul_norm_dist(d) - pow(I / S, kappa) * rq_pricing_cumul_norm_dist(d - 2 * log(I / S) / (v * sqrt(T))));
}

static double
rq_pricing_bjerksund_stensland_approx(
    double S, 
    double X, 
    double T, 
    double r, 
    double b, 
    double v
    )
{
    double ret = 0.0;
    
    if (b >= r) /* Never optimal to exersice before maturity */
        ret = rq_pricing_blackscholes_gen(1, S, X, T, r, b, v);
    else
    {
        double betaA = (0.5 - b / (v * v));
		double betaB = sqrt(pow((b / (v * v) - 0.5), 2) + 2 * r / (v * v));
		double beta = betaA + betaB;
        double b_inf = beta / (beta - 1) * X;
        double B0 = MAX(X, r / (r - b) * X);
        double ht = -(b * T + 2 * v * sqrt(T)) * B0 / (b_inf - B0);
        double I = B0 + (b_inf - B0) * (1 - exp(ht));
        double alpha = (I - X) * pow(I, -beta);

        if (S >= I)
            ret = S - X;
        else
		{
			double r1 = alpha * pow(S, beta); 
			double r2 = alpha * phi(S, T, beta, I, I, r, b, v);
			double r3 = phi(S, T, 1, I, I, r, b, v);
			double r4 = phi(S, T, 1, X, I, r, b, v);
			double r5 = X * phi(S, T, 0, I, I, r, b, v);
			double r6 = X * phi(S, T, 0, X, I, r, b, v);

            ret =  r1 - r2 + r3 - r4 - r5 + r6;
		}
	}

    return ret;
}

RQ_EXPORT double 
rq_pricing_bjerksund_stensland(
    int call, 
    double S, 
    double X, 
    double r, 
    double rf, 
    double v,
    double T
    )
{
    double b = r - rf;
    if (call)
        return rq_pricing_bjerksund_stensland_approx(S, X, T, r, b, v);
    else /* Use the Bjerksund and Stensland put-call transformation */
        return rq_pricing_bjerksund_stensland_approx(X, S, T, r - b, -b, v);
}

