/*
** rq_cds_curve.c
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
#include "rq_cds_curve.h"
#include "rq_interpolate.h"
#include "rq_defs.h"

#include <stdlib.h>
#include <string.h>
#include <math.h>

static unsigned int s_max_spreads = RQ_CDS_CURVE_MAX_SPREADS;


static struct rq_cds_curve *
rq_cds_curve_alloc(unsigned int alloc_size)
{
    /* use RQ_CALLOC to zero the structure */
    struct rq_cds_curve *ts = (struct rq_cds_curve *)RQ_CALLOC(1, sizeof(struct rq_cds_curve));
    /* if (!ts) error(); */

    ts->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_CDS_SPREAD_CURVE;

	if (alloc_size)
	    ts->max_elements = alloc_size;
	else
	    ts->max_elements = s_max_spreads;

    ts->spreads = (struct rq_cds_curve_elem *)RQ_CALLOC(ts->max_elements, sizeof(struct rq_cds_curve_elem));
	ts->num_elements = 0;
    
    /* defaults */
	ts->frequency = 0.25;
	ts->recovery_rate = 0.0;
	ts->interpolation_method = RQ_INTERPOLATION_LINEAR_DISCOUNT_FACTOR;
	ts->alpha = NULL;
	ts->psi = NULL;

    return ts;
}

RQ_EXPORT rq_cds_curve_t 
rq_cds_curve_build(
    const char *curve_id
	)
{
    struct rq_cds_curve *ts = rq_cds_curve_alloc(0);

    ts->termstruct.termstruct_id = RQ_STRDUP(curve_id);

    return ts;
}

RQ_EXPORT void 
rq_cds_curve_free(rq_cds_curve_t ts)
{
    rq_termstruct_clear(&ts->termstruct);
    RQ_FREE(ts->spreads);
    RQ_FREE(ts->alpha);
    RQ_FREE(ts->psi);
    RQ_FREE(ts);
}

RQ_EXPORT const char *
rq_cds_curve_get_termstruct_id(const rq_cds_curve_t ts)
{
    return ts->termstruct.termstruct_id;
}

RQ_EXPORT const char *
rq_cds_curve_get_underlying_asset_id(const rq_cds_curve_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_cds_curve_set_underlying_asset_id(rq_cds_curve_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT double 
rq_cds_curve_get_recovery_rate(const rq_cds_curve_t yc)
{
	return yc->recovery_rate;
}

RQ_EXPORT void
rq_cds_curve_set_recovery_rate(rq_cds_curve_t yc, double recovery_rate)
{
	yc->recovery_rate = recovery_rate;
}

RQ_EXPORT double 
rq_cds_curve_get_frequency(const rq_cds_curve_t yc)
{
	return yc->frequency;
}

RQ_EXPORT void
rq_cds_curve_set_frequency(rq_cds_curve_t crv, double frequency)
{
	crv->frequency = frequency;
}

RQ_EXPORT double 
rq_cds_curve_get_survival_rate(const rq_cds_curve_t ts, double term)
{
    unsigned int i = 0;

    if (!ts || ts->num_elements == 0)
        return 0.0;

	if (term <= 0.0)
		return 1.0;		/* default probability of survival is definate  */

    /**
     * \todo Change this algorithm so that it does a bisection search
     */
	/* TODO use double compare */
    while (i < ts->num_elements)
    {
        if (ts->spreads[i].term >= term)
        {
            /* 
               If this is the first element or the dates match
               return this discount factor
            */
            if (ts->spreads[i].term == term || i == 0)
                return ts->spreads[i].survival_rate;

            return rq_interpolate_linear(
                (double)term,
                (double)ts->spreads[i-1].term,
                ts->spreads[i-1].survival_rate,
                (double)ts->spreads[i].term,
                ts->spreads[i].survival_rate
                );
        }
        i++;
    }

    return ts->spreads[ts->num_elements-1].survival_rate;
}

RQ_EXPORT void 
rq_cds_curve_set_survival_rate(rq_cds_curve_t ts, double term, double spread)
{
    /* grow the array if necessary */
    if (ts->num_elements == ts->max_elements)
    {
        unsigned int new_max_spreads = ts->max_elements * 2;
        ts->spreads = 
            (struct rq_cds_curve_elem *) realloc(
            ts->spreads, 
            new_max_spreads * sizeof(struct rq_cds_curve_elem)
            );
        ts->max_elements = new_max_spreads;
    }

    /* if this is the first spread or the date is greater
       than the last date in the array...
     */
    if (ts->num_elements == 0 || ts->spreads[ts->num_elements-1].term < term)
    {
        ts->spreads[ts->num_elements].term = term;
        ts->spreads[ts->num_elements].survival_rate = spread;
        ts->num_elements++;
    }
    else
    {
        /** \todo: 
         * Think about replacing the following with a bisection search
         */
        unsigned int i = 0;
        while (i < ts->num_elements)
        {
            if (ts->spreads[i].term >= term)
            {
                if (ts->spreads[i].term > term)
                {
                    /* insert the element in here... */
                    unsigned int j = ts->num_elements;
                    while (j > i)
                    {
                        memcpy(&ts->spreads[j], &ts->spreads[j-1], sizeof(struct rq_cds_curve_elem));

                        j--;
                    }

                    ts->spreads[i].term = term;
                    ts->num_elements++;
                }

                /* 
                   OK, the following will be executed if the dates are equal
                   OR if we've done an insert.
                */

                ts->spreads[i].survival_rate = spread;

                break; /* and exit the loop */
            }

            i++;
        }
    }

}

RQ_EXPORT rq_cds_curve_t 
rq_cds_curve_clone(const rq_cds_curve_t ts)
{
    unsigned int i;
    struct rq_cds_curve *ts_clone = rq_cds_curve_alloc(ts->max_elements);

    ts_clone->termstruct.termstruct_id = RQ_STRDUP(ts->termstruct.termstruct_id);

    if (ts->termstruct.underlying_asset_id)
        ts_clone->termstruct.underlying_asset_id = RQ_STRDUP(ts->termstruct.underlying_asset_id);

    ts_clone->frequency = ts->frequency;
    ts_clone->recovery_rate = ts->recovery_rate;
    ts_clone->num_elements = ts->num_elements;

    for (i = 0; i < ts->num_elements; i++)
    {
        ts_clone->spreads[i].term = ts->spreads[i].term;
        ts_clone->spreads[i].survival_rate = ts->spreads[i].survival_rate;
    }

    ts_clone->interpolation_method = ts->interpolation_method;

    return ts_clone;
}

RQ_EXPORT double 
rq_cds_curve_get_longest_term(const rq_cds_curve_t ts)
{
    if (ts->num_elements > 0)
        return ts->spreads[ts->num_elements-1].term;
    return 0;
}

RQ_EXPORT unsigned int 
rq_cds_curve_size(const rq_cds_curve_t ts)
{
    return ts->num_elements;
}

RQ_EXPORT struct rq_cds_curve_elem *
rq_cds_curve_element_at(
    const rq_cds_curve_t ts,
    unsigned int offset
    )
{
    return &ts->spreads[offset];
}

RQ_EXPORT int
rq_cds_curve_is_null(rq_cds_curve_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT double 
rq_cds_curve_elem_get_term(const struct rq_cds_curve_elem *e)
{
    return e->term;
}

RQ_EXPORT double 
rq_cds_curve_elem_get_survival_rate(const struct rq_cds_curve_elem *e)
{
    return e->survival_rate;
}
