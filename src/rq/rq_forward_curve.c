/*
** rq_forward_curve.c
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
#include <math.h>
#include "rq_forward_curve.h"
#include "rq_interpolate.h"
#include <stdlib.h>
#include <string.h>


static unsigned int s_initial_max_forward_rates = 20;

static struct rq_forward_curve *
rq_forward_curve_alloc(unsigned int default_size)
{
    struct rq_forward_curve *fc = 
        (struct rq_forward_curve *)RQ_CALLOC(1, sizeof(struct rq_forward_curve));
    fc->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_FORWARD_CURVE;

    fc->num_forward_rates = 0;

    if (default_size < s_initial_max_forward_rates)
        default_size = s_initial_max_forward_rates;

    fc->max_forward_rates = default_size;
    fc->forward_rates = (struct rq_forward_rate *)RQ_CALLOC(default_size, sizeof(struct rq_forward_rate));

    return fc;
}

RQ_EXPORT void 
rq_forward_curve_free(rq_forward_curve_t forward_curve)
{
    rq_termstruct_clear(&forward_curve->termstruct);

/*     if (forward_curve->ccypair_asset_id) */
/*         RQ_FREE((char *)forward_curve->ccypair_asset_id); */

    if (forward_curve->forward_rates)
        RQ_FREE(forward_curve->forward_rates);

    RQ_FREE(forward_curve);
}

RQ_EXPORT rq_forward_curve_t 
rq_forward_curve_build(const char *curve_id, const char *ccypair_asset_id)
{
    struct rq_forward_curve *fc = rq_forward_curve_alloc(0);

    fc->termstruct.termstruct_id = RQ_STRDUP(curve_id);

    rq_termstruct_set_underlying_asset_id(&fc->termstruct, ccypair_asset_id);
    /* fc->ccypair_asset_id = RQ_STRDUP(ccypair_asset_id); */

    return fc;
}

RQ_EXPORT const char *
rq_forward_curve_get_curve_id(const rq_forward_curve_t forward_curve)
{
    return forward_curve->termstruct.termstruct_id;
}

RQ_EXPORT const char *
rq_forward_curve_get_asset_id(const rq_forward_curve_t forward_curve)
{
    /* return forward_curve->ccypair_asset_id; */
    return forward_curve->termstruct.underlying_asset_id;
}

RQ_EXPORT const char *
rq_forward_curve_get_underlying_asset_id(const rq_forward_curve_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_forward_curve_set_underlying_asset_id(rq_forward_curve_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT void 
rq_forward_curve_set_rate(rq_forward_curve_t forward_curve, rq_date date, double rate, int quote)
{
    if (forward_curve->num_forward_rates == forward_curve->max_forward_rates)
    {
        unsigned int new_max_forward_rates = forward_curve->max_forward_rates * 2;
        forward_curve->forward_rates = 
            (struct rq_forward_rate *)realloc(
                forward_curve->forward_rates,
                new_max_forward_rates * sizeof(struct rq_forward_rate)
                );
        forward_curve->max_forward_rates = new_max_forward_rates;
    }

    /* if this is the first rate or the date is greater
       than the last date in the array...
     */
    if (forward_curve->num_forward_rates == 0 || forward_curve->forward_rates[forward_curve->num_forward_rates-1].date < date)
    {
        forward_curve->forward_rates[forward_curve->num_forward_rates].date = date;
        forward_curve->forward_rates[forward_curve->num_forward_rates].rate = rate;
        forward_curve->forward_rates[forward_curve->num_forward_rates].quote = quote;
        forward_curve->num_forward_rates++;
    }
    else
    {
        /** \todo: 
         * Think about replacing the following with a bisection search
         */
        unsigned int i = 0;
        while (i < forward_curve->num_forward_rates)
        {
            if (forward_curve->forward_rates[i].date >= date)
            {
                if (forward_curve->forward_rates[i].date > date)
                {
                    /* insert the element in here... */
                    unsigned int j = forward_curve->num_forward_rates;
                    while (j > i)
                    {
                        memcpy(&forward_curve->forward_rates[j], &forward_curve->forward_rates[j-1], sizeof(struct rq_forward_rate));

                        j--;
                    }

                    forward_curve->forward_rates[i].date = date;
                    forward_curve->num_forward_rates++;
                }

                /* 
                   OK, the following will be executed if the dates are equal
                   OR if we've done an insert.
                */

                forward_curve->forward_rates[i].rate = rate;
                forward_curve->forward_rates[i].quote = quote;

                break; /* and exit the loop */
            }

            i++;
        }
    }
}

RQ_EXPORT int 
rq_forward_curve_get_forward_rate(
    const rq_forward_curve_t forward_curve,
    rq_date date,
    struct rq_forward_rate *rate  /**< The output parameter, the returned forward rate */
   )
{
	int failed = 1;
    unsigned int i;

    for (i = 0; i < forward_curve->num_forward_rates; i++)
    {
        if (forward_curve->forward_rates[i].date > date)
        {
            if (i == 0)
                *rate = forward_curve->forward_rates[i];
            else
            {
                *rate = forward_curve->forward_rates[i-1];
                rate->date = date;
                rate->rate = rq_interpolate_linear(
                    date,
                    forward_curve->forward_rates[i-1].date,
                    forward_curve->forward_rates[i-1].rate,
                    forward_curve->forward_rates[i].date,
                    forward_curve->forward_rates[i].rate
                    );
            }

            failed = 0;
			break;
        }
    }

    if (failed && forward_curve->num_forward_rates > 0)
    {
        *rate = forward_curve->forward_rates[forward_curve->num_forward_rates-1];
        failed = 0;
    }

    return failed;
}

RQ_EXPORT int 
rq_forward_curve_get_rate(
    const rq_forward_curve_t forward_curve,
    rq_date date,
    double *rate  /**< The output parameter, the returned forward rate */
    )
{
    int failed = 1;
    unsigned int i;

    for (i = 0; i < forward_curve->num_forward_rates; i++)
    {
        if (forward_curve->forward_rates[i].date > date)
        {
            if (i == 0)
                *rate = forward_curve->forward_rates[i].rate;
            else
            {
                *rate = rq_interpolate_linear(
                    date,
                    forward_curve->forward_rates[i-1].date,
                    forward_curve->forward_rates[i-1].rate,
                    forward_curve->forward_rates[i].date,
                    forward_curve->forward_rates[i].rate
                    );
            }

            failed = 0;
			break;
        }
    }

    if (failed && forward_curve->num_forward_rates > 0)
    {
        *rate = forward_curve->forward_rates[forward_curve->num_forward_rates-1].rate;
        failed = 0;
    }

    return failed;
}

RQ_EXPORT int 
rq_forward_curve_get_cost_of_carry(
    const rq_forward_curve_t forward_curve,
    rq_date from_date,
    rq_date to_date,
    double *rate  /**< The output parameter, the returned forward rate */
    )
{
    int failed = 1;
	double t, fromRate, toRate;

	if (from_date > to_date)
		return failed;

	if (from_date == to_date)
	{
		(*rate) = 0.0;
		return 0;
	}

	if (rq_forward_curve_get_rate(forward_curve, from_date, &fromRate))
		return failed;

	if (rq_forward_curve_get_rate(forward_curve, to_date, &toRate))
		return failed;

	t = (to_date - from_date) / 365.0;

	(*rate) = log(toRate / fromRate ) / t;
	return 0;
}

RQ_EXPORT rq_forward_curve_t 
rq_forward_curve_clone(const rq_forward_curve_t forward_curve)
{
    rq_forward_curve_t c = rq_forward_curve_alloc(forward_curve->num_forward_rates);
    c->termstruct.termstruct_id = RQ_STRDUP(forward_curve->termstruct.termstruct_id);
    rq_termstruct_set_underlying_asset_id(&c->termstruct, forward_curve->termstruct.underlying_asset_id);

    while (c->num_forward_rates < forward_curve->num_forward_rates)
    {
        c->forward_rates[c->num_forward_rates] = forward_curve->forward_rates[c->num_forward_rates];
        c->num_forward_rates++;
    }

    return c;
}

RQ_EXPORT unsigned int 
rq_forward_curve_size(const rq_forward_curve_t forward_curve)
{
    return forward_curve->num_forward_rates;
}

RQ_EXPORT const struct rq_forward_rate *
rq_forward_curve_element_at(
    const rq_forward_curve_t forward_curve,
    unsigned int offset
    )
{
    return &forward_curve->forward_rates[offset];
}


RQ_EXPORT double *
rq_forward_curve_rate_at(
    rq_forward_curve_t forward_curve,
    unsigned int offset
    )
{
    return &forward_curve->forward_rates[offset].rate;
}

RQ_EXPORT int
rq_forward_curve_is_null(rq_forward_curve_t obj)
{
    return (obj == NULL);
}
