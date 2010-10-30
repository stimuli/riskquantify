/*
** rq_spread_curve.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#include "rq_spread_curve.h"
#include "rq_interpolate.h"
#include "rq_defs.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

static unsigned int s_max_spreads = RQ_SPREAD_CURVE_MAX_SPREADS;


static struct rq_spread_curve *
rq_spread_curve_alloc(unsigned int in_num_spreads)
{
    /* use RQ_CALLOC to zero the structure */
    struct rq_spread_curve *ts = (struct rq_spread_curve *)RQ_CALLOC(1, sizeof(struct rq_spread_curve));
    /* if (!ts) error(); */

	if( in_num_spreads == 0 ) 
		in_num_spreads = s_max_spreads;

    ts->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_SPREAD_CURVE;
    ts->max_spreads = in_num_spreads;
    ts->spreads = (struct rq_spread_curve_elem *)RQ_CALLOC(in_num_spreads, sizeof(struct rq_spread_curve_elem));
    ts->interpolation_method = RQ_INTERPOLATION_LINEAR_DISCOUNT_FACTOR;

    /* if (!ts->discount_factors) error(); */

    return ts;
}

RQ_EXPORT rq_spread_curve_t 
rq_spread_curve_build(
    const char *curve_id,
    rq_date from_date
    )
{
    struct rq_spread_curve *ts = rq_spread_curve_alloc(0);

    ts->termstruct.termstruct_id = RQ_STRDUP(curve_id);
    ts->from_date = from_date;

    return ts;
}

RQ_EXPORT void 
rq_spread_curve_free(rq_spread_curve_t ts)
{
    rq_termstruct_clear(&ts->termstruct);
    RQ_FREE(ts->spreads);
    RQ_FREE(ts);
}

RQ_EXPORT const char *
rq_spread_curve_get_termstruct_id(const rq_spread_curve_t ts)
{
    return ts->termstruct.termstruct_id;
}

RQ_EXPORT const char *
rq_spread_curve_get_underlying_asset_id(const rq_spread_curve_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_spread_curve_set_underlying_asset_id(rq_spread_curve_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT rq_date 
rq_spread_curve_get_curve_date(const rq_spread_curve_t ts)
{
    return ts->from_date;
}

RQ_EXPORT double 
rq_spread_curve_get_spread(const rq_spread_curve_t ts, rq_date for_date)
{
    unsigned int i = 0;

    if (!ts || for_date <= ts->from_date || ts->num_spreads == 0)
        return 0.0;

    /**
     * \todo Change this algorithm so that it does a bisection search
     */
    while (i < ts->num_spreads)
    {
        if (ts->spreads[i].date >= for_date)
        {
            /* 
               If this is the first element or the dates match
               return this discount factor
            */
            if (ts->spreads[i].date == for_date || i == 0)
                return ts->spreads[i].spread;

            return rq_interpolate_linear(
                (double)for_date,
                (double)ts->spreads[i-1].date,
                ts->spreads[i-1].spread,
                (double)ts->spreads[i].date,
                ts->spreads[i].spread
                );
        }
        i++;
    }

    return ts->spreads[ts->num_spreads-1].spread;
}

RQ_EXPORT void 
rq_spread_curve_set_spread(rq_spread_curve_t ts, rq_date for_date, double spread)
{
    /* grow the array if necessary */
    if (ts->num_spreads == ts->max_spreads)
    {
        unsigned int new_max_spreads = ts->max_spreads * 2;
        ts->spreads = 
            (struct rq_spread_curve_elem *) realloc(
            ts->spreads, 
            new_max_spreads * sizeof(struct rq_spread_curve_elem)
            );
        ts->max_spreads = new_max_spreads;
    }

    /* if this is the first spread or the date is greater
       than the last date in the array...
     */
    if (ts->num_spreads == 0 || ts->spreads[ts->num_spreads-1].date < for_date)
    {
        ts->spreads[ts->num_spreads].date = for_date;
        ts->spreads[ts->num_spreads].spread = spread;
        ts->num_spreads++;
    }
    else
    {
        /** \todo: 
         * Think about replacing the following with a bisection search
         */
        unsigned int i = 0;
        while (i < ts->num_spreads)
        {
            if (ts->spreads[i].date >= for_date)
            {
                if (ts->spreads[i].date > for_date)
                {
                    /* insert the element in here... */
                    unsigned int j = ts->num_spreads;
                    while (j > i)
                    {
                        memcpy(&ts->spreads[j], &ts->spreads[j-1], sizeof(struct rq_spread_curve_elem));

                        j--;
                    }

                    ts->spreads[i].date = for_date;
                    ts->num_spreads++;
                }

                /* 
                   OK, the following will be executed if the dates are equal
                   OR if we've done an insert.
                */

                ts->spreads[i].spread = spread;

                break; /* and exit the loop */
            }

            i++;
        }
    }

}

RQ_EXPORT rq_spread_curve_t 
rq_spread_curve_clone(const rq_spread_curve_t ts)
{
    unsigned int i;
    struct rq_spread_curve *ts_clone = rq_spread_curve_alloc(ts->max_spreads);

    ts_clone->termstruct.termstruct_id = RQ_STRDUP(ts->termstruct.termstruct_id);

    if (ts->termstruct.underlying_asset_id)
        ts_clone->termstruct.underlying_asset_id = RQ_STRDUP(ts->termstruct.underlying_asset_id);

    ts_clone->from_date = ts->from_date;
    ts_clone->num_spreads = ts->num_spreads;

    for (i = 0; i < ts->num_spreads; i++)
    {
        ts_clone->spreads[i].date = ts->spreads[i].date;
        ts_clone->spreads[i].spread = ts->spreads[i].spread;
    }

    ts_clone->interpolation_method = ts->interpolation_method;

    return ts_clone;
}

RQ_EXPORT rq_date 
rq_spread_curve_get_last_date(const rq_spread_curve_t ts)
{
    if (ts->num_spreads > 0)
        return ts->spreads[ts->num_spreads-1].date;
    return 0;
}

RQ_EXPORT unsigned int 
rq_spread_curve_size(const rq_spread_curve_t ts)
{
    return ts->num_spreads;
}

RQ_EXPORT struct rq_spread_curve_elem *
rq_spread_curve_element_at(
    const rq_spread_curve_t ts,
    unsigned int offset
    )
{
    return &ts->spreads[offset];
}

RQ_EXPORT int
rq_spread_curve_is_null(rq_spread_curve_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_date 
rq_spread_curve_elem_get_date(const struct rq_spread_curve_elem *e)
{
    return e->date;
}

RQ_EXPORT double 
rq_spread_curve_elem_get_spread(const struct rq_spread_curve_elem *e)
{
    return e->spread;
}
