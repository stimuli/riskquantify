/*
** rq_pricing_time_switch.c
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
#include "rq_pricing_time_switch.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


RQ_EXPORT double 
rq_pricing_time_switch(
    short call, 
    double S, 
    double X, 
    double a, 
    double T, 
    int m,
    double dt, 
    double r, 
    double b, 
    double v
    )
{
    double sum = 0;
    int i;
    int n = (int)(T / dt);
    int Z = (call ? 1 : -1);
    double half_b_minus_vsqr = (b - (v * v) / 2.0);
    double log_S_over_X = log(S / X);
    
    for (i = 1; i <= n; i++)
    {
        double d = 
            (log_S_over_X + half_b_minus_vsqr * i * dt) / 
            (v * sqrt(i * dt));
        sum = sum + rq_pricing_cumul_norm_dist(Z * d) * dt;
    }

    return a * exp(-r * T) * sum + dt * a * exp(-r * T) * m;
}
