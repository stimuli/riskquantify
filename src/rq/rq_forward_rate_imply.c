/*
** rq_forward_rate_imply.c
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
#include "rq_forward_rate_imply.h"
#include "rq_asset_ccypair.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT double 
rq_forward_rate_imply_from_yield_curves(
    rq_date date, 
    const rq_asset_t asset_ccypair,
    double spot_rate,
    const rq_yield_curve_t yield_curve_1, 
    const rq_yield_curve_t yield_curve_2
    )
{
    double df1 = rq_yield_curve_get_discount_factor(yield_curve_1, date);
    double df2 = rq_yield_curve_get_discount_factor(yield_curve_2, date);
    int one_per_two = 0;
    double forward_rate;

    if (!strcmp(rq_asset_ccypair_get_ccy_code_1(asset_ccypair), rq_yield_curve_get_curve_id(yield_curve_1)))
    {
        if (rq_asset_ccypair_get_quote_convention(asset_ccypair) == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2)
            one_per_two = 1;
        else
            one_per_two = 0;
    }
    else
    {
        if (rq_asset_ccypair_get_quote_convention(asset_ccypair) == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2)
            one_per_two = 0;
        else
            one_per_two = 1;
    }

    if (one_per_two)
        forward_rate = spot_rate * df1 / df2;
    else
        forward_rate = spot_rate * df2 / df1;

    return forward_rate;
}

RQ_EXPORT double 
rq_forward_rate_imply_direct_rate_from_yield_curves(
    rq_date date, 
    const rq_asset_t asset_ccypair,
    double spot_rate,
    const rq_yield_curve_t yield_curve_1, 
    const rq_yield_curve_t yield_curve_2,
    const char *ccy_from,
    const char *ccy_to
    )
{
    double df1 = rq_yield_curve_get_discount_factor(yield_curve_1, date);
    double df2 = rq_yield_curve_get_discount_factor(yield_curve_2, date);
    int one_per_two = 0;
    int flip = 0;
    double forward_rate;

    if (!strcmp(rq_asset_ccypair_get_ccy_code_1(asset_ccypair), rq_yield_curve_get_curve_id(yield_curve_1)))
    {
        if (rq_asset_ccypair_get_quote_convention(asset_ccypair) == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2)
            one_per_two = 1;
        else
            one_per_two = 0;
    }
    else
    {
        if (rq_asset_ccypair_get_quote_convention(asset_ccypair) == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2)
            one_per_two = 0;
        else
            one_per_two = 1;
    }

    if (!strcmp(rq_asset_ccypair_get_ccy_code_1(asset_ccypair), ccy_from))
    {
        /* OK, we've converting using the same quote convention that's on
           the ccy pair.
        */
        if (!one_per_two)
        {
            /* if it's two per one we need to flip it. */
            flip = 1;
        }
    }
    else
    {
        /* OK, we've converting where the from_ccy os ccy 2 on the asset.
           A two_per_one rate is therefore direct rate.
        */
        if (one_per_two)
            flip = 1;
    }

    if (one_per_two)
        forward_rate = spot_rate * df1 / df2;
    else
        forward_rate = spot_rate * df2 / df1;

    if (flip && forward_rate)
        forward_rate = 1.0 / forward_rate;

    return forward_rate;
}

