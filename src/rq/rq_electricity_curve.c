/*
** rq_electricity_curve.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2006-2008 Brett Hutley
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA 
*/
#include "rq_electricity_curve.h"
#include "rq_math.h"
#include <stdlib.h>
#include <string.h>

void
free_hourly_prices_for_day(void *p)
{
    struct hourly_prices_for_day *h = (struct hourly_prices_for_day *)p;
    RQ_FREE(h);
}

int
cmp_hourly_prices_for_day(const void *p1, const void *p2)
{
    rq_date d1 = (rq_date)p1;
    rq_date d2 = (rq_date)p2;

    return rq_date_diff(d1, d2);
}

RQ_EXPORT rq_electricity_curve_t 
rq_electricity_curve_alloc()
{
    rq_electricity_curve_t curve = (rq_electricity_curve_t)RQ_CALLOC(
        1, 
        sizeof(struct rq_electricity_curve)
        );

    curve->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_ELECTRICITY_CURVE;
    curve->price_curve = rq_tree_rb_alloc(
        free_hourly_prices_for_day,
        cmp_hourly_prices_for_day
        );

    return curve;
}

RQ_EXPORT void 
rq_electricity_curve_free(rq_electricity_curve_t curve)
{
    rq_termstruct_clear(&curve->termstruct);
    rq_tree_rb_free(curve->price_curve);
    RQ_FREE(curve);
}

RQ_EXPORT void 
rq_electricity_curve_add_price(rq_electricity_curve_t curve, rq_date date, unsigned char hour, double price)
{
    struct hourly_prices_for_day *h = 
        (struct hourly_prices_for_day *)rq_tree_rb_find(curve->price_curve, (const void *)date);
    if (!h)
    {
        int i;

        h = (struct hourly_prices_for_day *)malloc(sizeof(struct hourly_prices_for_day));
        h->date = date;
        for (i = 0; i < 25; i++)
            h->prices[i] = rq_math_get_nan();

        rq_tree_rb_add(curve->price_curve, (const void *)date, h);
    }

    h->prices[hour % 25] = price;
}

RQ_EXPORT double 
rq_electricity_curve_get_price(rq_electricity_curve_t curve, rq_date date, unsigned char hour)
{
    struct hourly_prices_for_day *h = 
        (struct hourly_prices_for_day *)rq_tree_rb_find(
            curve->price_curve, 
            (const void *)date
            );
    if (h)
        return h->prices[hour % 25];
    return rq_math_get_nan();
}


RQ_EXPORT rq_iterator_t
rq_electricity_curve_get_iterator(rq_electricity_curve_t curve)
{
    return rq_tree_rb_get_iterator(curve->price_curve);
}
