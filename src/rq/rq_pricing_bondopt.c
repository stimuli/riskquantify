/*
** rq_pricing_bondopt.c
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
#include "rq_config.h"
#include <math.h>
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_bondopt.h"

RQ_EXPORT double 
rq_pricing_bond_opt(
    short call,
    double B, 
    double X, 
    double r_dom, 
    double sigma, 
    double tau_e,
    double tau_d,
    short num_coupons,
    double *coupon_times, 
    double *coupon_amounts,
    double *coupon_r
    )
{
    short i;

    /* discount the bond value by all coupons paid before option expiry */
    for (i = 0; i < num_coupons; i++)
        if (coupon_times[i] <= tau_e) 
            B -= coupon_amounts[i] * exp(-coupon_r[i] * coupon_times[i]);

    return rq_pricing_blackscholes(call, B, X, r_dom, 0.0, sigma, tau_e, tau_d);
}
