/*
** rq_pricing_jumpdiffusion.c
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
#include "rq_pricing_jumpdiffusion.h"
#include "rq_pricing_blackscholes.h"
#include "rq_math.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

RQ_EXPORT double 
rq_pricing_jump_diffusion(
    short call,
    double S, 
    double X, 
    double T, 
    double r, 
    double v,
    double lambda, 
    double gamma
    )
{
    double v_sqr = v * v;
    double delta = sqrt(gamma * v_sqr / lambda);
    double delta_sqr = delta * delta;
    double sum = 0.0;
    double z = sqrt(v_sqr - lambda * delta_sqr);
    double z_sqr = z * z;
    int i;

    for (i = 0; i <= 10; i++)
    {
        double vi = sqrt(z_sqr + delta_sqr * (i / T));
        sum = sum + exp(-lambda * T) * pow((lambda * T), i) / fact(i) * rq_pricing_blackscholes_gen(call, S, X, T, r, r, vi);
    }


    return sum;
}
