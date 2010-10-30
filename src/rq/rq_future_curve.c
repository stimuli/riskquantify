/*
** rq_future_curve.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#include "rq_future_curve.h"
#include "rq_interpolate.h"
#include <stdlib.h>
#include <string.h>


static unsigned int s_initial_max_future_prices = 20;

static struct rq_future_curve *
rq_future_curve_alloc(unsigned int default_size)
{
    struct rq_future_curve *fc = 
        (struct rq_future_curve *)RQ_CALLOC(1, sizeof(struct rq_future_curve));
    fc->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_FUTURE_CURVE;

    fc->num_future_prices = 0;

    if (default_size < s_initial_max_future_prices)
        default_size = s_initial_max_future_prices;

    fc->max_future_prices = default_size;
    fc->future_prices = (struct rq_future_price *)RQ_CALLOC(default_size, sizeof(struct rq_future_price));

    return fc;
}

RQ_EXPORT void 
rq_future_curve_free(rq_future_curve_t future_curve)
{
    rq_termstruct_clear(&future_curve->termstruct);

/*     if (future_curve->ccypair_asset_id) */
/*         RQ_FREE((char *)future_curve->ccypair_asset_id); */

    if (future_curve->future_prices)
        RQ_FREE(future_curve->future_prices);

    RQ_FREE(future_curve);
}

RQ_EXPORT rq_future_curve_t 
rq_future_curve_build(const char *curve_id, const char *ccypair_asset_id)
{
    struct rq_future_curve *fc = rq_future_curve_alloc(0);

    fc->termstruct.termstruct_id = RQ_STRDUP(curve_id);

    rq_termstruct_set_underlying_asset_id(&fc->termstruct, ccypair_asset_id);
    /* fc->ccypair_asset_id = RQ_STRDUP(ccypair_asset_id); */

    return fc;
}

RQ_EXPORT const char *
rq_future_curve_get_curve_id(const rq_future_curve_t future_curve)
{
    return future_curve->termstruct.termstruct_id;
}

RQ_EXPORT const char *
rq_future_curve_get_asset_id(const rq_future_curve_t future_curve)
{
    /* return future_curve->ccypair_asset_id; */
    return future_curve->termstruct.underlying_asset_id;
}

RQ_EXPORT const char *
rq_future_curve_get_underlying_asset_id(const rq_future_curve_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_future_curve_set_underlying_asset_id(rq_future_curve_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT void 
rq_future_curve_set_price(rq_future_curve_t future_curve, const char* asset_id, double price, int quote)
{
	unsigned int i=0;

	if (future_curve->num_future_prices == future_curve->max_future_prices)
	{
		unsigned int new_max_future_prices = future_curve->max_future_prices * 2;
		future_curve->future_prices = 
			(struct rq_future_price *)realloc(
			future_curve->future_prices,
			new_max_future_prices * sizeof(struct rq_future_price)
			);
		future_curve->max_future_prices = new_max_future_prices;
	}

	for (i = 0; i < future_curve->num_future_prices; i++)
	{
		if (!strcmp(future_curve->future_prices[i].asset_id,asset_id))
			break;
	}

	if (i==future_curve->num_future_prices)
		future_curve->num_future_prices++;

	// if not found then insert at end anyway
	future_curve->future_prices[i].asset_id = RQ_STRDUP(asset_id);
	future_curve->future_prices[i].price=price;
	future_curve->future_prices[i].quote=quote;
	
}

RQ_EXPORT int 
rq_future_curve_get_future_price(
    const rq_future_curve_t future_curve,
    const char* asset_id,
    struct rq_future_price *price  /**< The output parameter, the returned future price */
   )
{
	int failed = 1;
	unsigned int i;
    for (i = 0; i < future_curve->num_future_prices; i++)
    {
        if (!strcmp(future_curve->future_prices[i].asset_id,asset_id))
        {
            *price = future_curve->future_prices[i];

            failed = 0;
			break;
        }
    }

    return failed;
}

RQ_EXPORT int 
rq_future_curve_get_price(
    const rq_future_curve_t future_curve,
    const char* asset_id,
    double *price  /**< The output parameter, the returned future price */
    )
{
	int failed = 1;
    unsigned int i;

    for (i = 0; i < future_curve->num_future_prices; i++)
    {
        if (!strcmp(future_curve->future_prices[i].asset_id,asset_id))
        {
            *price = future_curve->future_prices[i].price;

            failed = 0;
			break;
        }
    }

    return failed;
}

RQ_EXPORT int 
rq_future_curve_get_cost_of_carry(
    const rq_future_curve_t future_curve,
    const char* asset_id1,
    const char* asset_id2,
    double *price  /**< The output parameter, the returned future price */
    )
{
    int failed = 1;
	double t, fromprice, toprice;

	if (!strcmp(asset_id1,asset_id2))
	{
		(*price) = 0.0;
		return 0;
	}

	if (rq_future_curve_get_price(future_curve, asset_id1, &fromprice))
		return failed;

	if (rq_future_curve_get_price(future_curve, asset_id2, &toprice))
		return failed;

	t = 30.0 / 365.0; // this is wrong, calc from the maturity_dates

	(*price) = log(toprice / fromprice ) / t;
	return 0;
}

RQ_EXPORT rq_future_curve_t 
rq_future_curve_clone(const rq_future_curve_t future_curve)
{
    //rq_future_curve_t c = rq_future_curve_build(future_curve->termstruct.termstruct_id, future_curve->termstruct.underlying_asset_id);

    rq_future_curve_t c = rq_future_curve_alloc(future_curve->num_future_prices);
    c->termstruct.termstruct_id = RQ_STRDUP(future_curve->termstruct.termstruct_id);
    rq_termstruct_set_underlying_asset_id(&c->termstruct, future_curve->termstruct.underlying_asset_id);

    while (c->num_future_prices < future_curve->num_future_prices)
    {
        c->future_prices[c->num_future_prices] = future_curve->future_prices[c->num_future_prices];
        c->num_future_prices++;
    }

    return c;
}

RQ_EXPORT unsigned int 
rq_future_curve_size(const rq_future_curve_t future_curve)
{
    return future_curve->num_future_prices;
}

RQ_EXPORT const struct rq_future_price *
rq_future_curve_element_at(
    const rq_future_curve_t future_curve,
    unsigned int offset
    )
{
    return &future_curve->future_prices[offset];
}


RQ_EXPORT double *
rq_future_curve_price_at(
    rq_future_curve_t future_curve,
    unsigned int offset
    )
{
    return &future_curve->future_prices[offset].price;
}

RQ_EXPORT int
rq_future_curve_is_null(rq_future_curve_t obj)
{
    return (obj == NULL);
}
