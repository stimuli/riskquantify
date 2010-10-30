/*
** rq_vol_surface.c
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
#include "rq_vol_surface.h"
#include "rq_interpolate.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static unsigned short s_default_max_curves = 256;


RQ_EXPORT rq_vol_surface_t
rq_vol_surface_alloc(const char *termstruct_id)
{
    struct rq_vol_surface *s = (struct rq_vol_surface *)RQ_CALLOC(1, sizeof(struct rq_vol_surface));

    s->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_VOL_SURFACE;
    s->termstruct.termstruct_id = RQ_STRDUP(termstruct_id);

    s->num_curves = 0;
    s->max_curves = s_default_max_curves;

    s->is_strike_vol = 0;
    s->deltas = (double *)RQ_CALLOC(s_default_max_curves, sizeof(double));
    s->vol_curves = (rq_vol_curve_t *)RQ_CALLOC(s_default_max_curves, sizeof(rq_vol_curve_t));

    return s;
}

RQ_EXPORT void 
rq_vol_surface_free(rq_vol_surface_t vol_surface)
{
    unsigned short i = 0;

    rq_termstruct_clear(&vol_surface->termstruct);

    if (vol_surface->vol_curves)
    {
        while (i < vol_surface->num_curves)
        {
            if (vol_surface->vol_curves[i])
                rq_vol_curve_free(vol_surface->vol_curves[i]);

            i++;
        }

       RQ_FREE(vol_surface->vol_curves);
       RQ_FREE(vol_surface->deltas);
    }
    
   RQ_FREE(vol_surface);
}

RQ_EXPORT const char *
rq_vol_surface_get_termstruct_id(const rq_vol_surface_t vol_surface)
{
    return vol_surface->termstruct.termstruct_id;
}

RQ_EXPORT const char *
rq_vol_surface_get_underlying_asset_id(const rq_vol_surface_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_vol_surface_set_underlying_asset_id(rq_vol_surface_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT void 
rq_vol_surface_add(rq_vol_surface_t vol_surface, double delta_or_strike, rq_vol_curve_t c)
{
    int i = 0;

    while (i < vol_surface->num_curves)
    {
        if (vol_surface->deltas[i] == delta_or_strike)
        {
            if (vol_surface->vol_curves[i])
                rq_vol_curve_free(vol_surface->vol_curves[i]);

            vol_surface->vol_curves[i] = c;
            return;
        }
        else if (vol_surface->deltas[i] > delta_or_strike)
        {
            int j = vol_surface->num_curves;

            /* move all surfaces up... */
            for ( ; j > i; j--)
            {
                vol_surface->deltas[j] = vol_surface->deltas[j-1];
                vol_surface->vol_curves[j] = vol_surface->vol_curves[j-1];
            }
            
            vol_surface->deltas[i] = delta_or_strike;
            vol_surface->vol_curves[i] = c;

            vol_surface->num_curves++;
            return;
        }
        i++;
    }

    /* add at the end */
    vol_surface->deltas[vol_surface->num_curves] = delta_or_strike;
    vol_surface->vol_curves[vol_surface->num_curves] = c;
    vol_surface->num_curves++;
}


RQ_EXPORT int
rq_vol_surface_get_volatility(
    const rq_vol_surface_t vol_surface,
    double delta_or_strike,
    rq_date date,
    double *vol
    )
{
    int failed = 1;
    int i = 0;

    *vol = 0.0;

    /* look for delta in surface */
    while (i < vol_surface->num_curves)
    {
        if (vol_surface->deltas[i] == delta_or_strike)
        {
            /* get the vol from the curve */
            failed = rq_vol_curve_get_volatility(
                vol_surface->vol_curves[i],
                date,
                vol
                );
            break;
        }
        else if (delta_or_strike < vol_surface->deltas[i])
        {
            /* if this is the first just get the vol from the curve */
            if (i == 0)
            {
                failed = rq_vol_curve_get_volatility(
                    vol_surface->vol_curves[i],
                    date,
                    vol
                    );
            }
            else
            {
                double v1, v2;
                rq_vol_curve_get_volatility(
                    vol_surface->vol_curves[i-1],
                    date,
                    &v1
                    );
                rq_vol_curve_get_volatility(
                    vol_surface->vol_curves[i],
                    date,
                    &v2
                    );
                *vol = rq_interpolate_linear(
                    delta_or_strike,
                    vol_surface->deltas[i-1],
                    v1,
                    vol_surface->deltas[i],
                    v2
                    );
                failed = 0;
            }
            break;
        }

        i++;
    }

    if (failed && vol_surface->num_curves)
    {
        /* get the vol from the LAST curve */
        failed = rq_vol_curve_get_volatility(
            vol_surface->vol_curves[vol_surface->num_curves-1],
            date,
            vol
            );
    }

    return failed;
}

RQ_EXPORT void
rq_vol_surface_set_volatility(
    rq_vol_surface_t vol_surface,
    double delta_or_strike,
    rq_date date,
    double vol
    )
{
    int i = 0;

    while (i < vol_surface->num_curves)
    {
        if (vol_surface->deltas[i] == delta_or_strike)
        {
            if (!vol_surface->vol_curves[i])
                vol_surface->vol_curves[i] = rq_vol_curve_alloc();
            rq_vol_curve_add(vol_surface->vol_curves[i], date, vol);
            return;
        }
        else if (vol_surface->deltas[i] > delta_or_strike)
        {
            int j = vol_surface->num_curves;

            /* move all surfaces up... */
            for ( ; j > i; j--)
            {
                vol_surface->deltas[j] = vol_surface->deltas[j-1];
                vol_surface->vol_curves[j] = vol_surface->vol_curves[j-1];
            }
            
            vol_surface->deltas[i] = delta_or_strike;
            vol_surface->vol_curves[i] = rq_vol_curve_alloc();

            rq_vol_curve_add(vol_surface->vol_curves[i], date, vol);

            vol_surface->num_curves++;
            return;
        }
        i++;
    }

    /* add at the end */
    vol_surface->deltas[vol_surface->num_curves] = delta_or_strike;
    vol_surface->vol_curves[vol_surface->num_curves] = rq_vol_curve_alloc();
    rq_vol_curve_add(vol_surface->vol_curves[vol_surface->num_curves], date, vol);
    vol_surface->num_curves++;
}

RQ_EXPORT rq_vol_surface_t
rq_vol_surface_clone(
    const rq_vol_surface_t vol_surface
    )
{
    struct rq_vol_surface *c = (struct rq_vol_surface *)RQ_CALLOC(1, sizeof(struct rq_vol_surface));

    int i;

    c->termstruct.termstruct_id = RQ_STRDUP(vol_surface->termstruct.termstruct_id);
    if (vol_surface->termstruct.underlying_asset_id)
        c->termstruct.underlying_asset_id = RQ_STRDUP(vol_surface->termstruct.underlying_asset_id);

    c->num_curves = vol_surface->num_curves;
    c->max_curves = vol_surface->max_curves;

    c->is_strike_vol = vol_surface->is_strike_vol;
    c->deltas = (double *)RQ_CALLOC(vol_surface->max_curves, sizeof(double));
    c->vol_curves = (rq_vol_curve_t *)RQ_CALLOC(vol_surface->max_curves, sizeof(rq_vol_curve_t));

    for (i = 0; i < vol_surface->num_curves; i++)
    {
        c->deltas[i] = vol_surface->deltas[i];
        c->vol_curves[i] = rq_vol_curve_clone(vol_surface->vol_curves[i]);
    }

    return c;
}

RQ_EXPORT unsigned 
rq_vol_surface_count(const rq_vol_surface_t vol_surface)
{
    return vol_surface->num_curves;
}

RQ_EXPORT const rq_vol_curve_t 
rq_vol_surface_element_at(const rq_vol_surface_t vol_surface, unsigned offset)
{
    return vol_surface->vol_curves[offset];
}

RQ_EXPORT double 
rq_vol_surface_delta_at(const rq_vol_surface_t vol_surface, unsigned offset)
{
    return vol_surface->deltas[offset];
}

RQ_EXPORT int
rq_vol_surface_is_null(rq_vol_surface_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT unsigned short
rq_vol_surface_is_strike_vol(rq_vol_surface_t vol_surface)
{
    return vol_surface->is_strike_vol;
}

RQ_EXPORT void
rq_vol_surface_set_is_strike_vol(rq_vol_surface_t s, unsigned short is_strike_vol)
{
    s->is_strike_vol = is_strike_vol;
}
