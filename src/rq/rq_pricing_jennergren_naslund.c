/*
** rq_pricing_jennergren_naslund.c
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
#include "rq_pricing_jennergren_naslund.h"
#include "rq_pricing_blackscholes.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Executive stock options */
RQ_EXPORT double 
rq_pricing_jennergren_naslund(
    short call,
    double S, 
    double X, 
    double T, 
    double r, 
    double b, 
    double v, 
    double lambda
    )
{
    return exp(-lambda * T) * rq_pricing_blackscholes_gen(call, S, X, T, r, b, v);
}
