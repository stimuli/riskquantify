/*
** rq_pricing_chooser_simple.c
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
#include "rq_pricing_chooser_simple.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Simple chooser options */
RQ_EXPORT double 
rq_pricing_chooser_simple(
    double S, 
    double X, 
    double t1, 
    double t2, 
    double r, 
    double b, 
    double v
    )
{
    double log_S_over_X = log(S / X);
    double v_sqr = v * v;

    double d = (log_S_over_X + (b + v_sqr / 2.0) * t2) / (v * sqrt(t2));
    double y = (log_S_over_X + b * t2 + v_sqr * t1 / 2.0) / (v * sqrt(t1));
  
    return S * exp((b - r) * t2) * rq_pricing_cumul_norm_dist(d) - X * exp(-r * t2) * rq_pricing_cumul_norm_dist(d - v * sqrt(t2)) 
        - S * exp((b - r) * t2) * rq_pricing_cumul_norm_dist(-y) + X * exp(-r * t2) * rq_pricing_cumul_norm_dist(-y + v * sqrt(t1));
}
