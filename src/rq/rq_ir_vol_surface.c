/*
** rq_ir_vol_surface.c
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
#include "rq_ir_vol_surface.h"
#include "rq_interpolate.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static unsigned short s_default_max_curves = 16;


RQ_EXPORT rq_ir_vol_surface_t
rq_ir_vol_surface_alloc(const char *termstruct_id)
{
    struct rq_ir_vol_surface *s = (struct rq_ir_vol_surface *)RQ_CALLOC(1, sizeof(struct rq_ir_vol_surface));

    s->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_VOL_SURFACE;
    s->termstruct.termstruct_id = RQ_STRDUP(termstruct_id);

    s->num_curves = 0;
    s->max_curves = s_default_max_curves;

    s->tenors = (double *)RQ_CALLOC(s_default_max_curves, sizeof(double));
    s->vol_curves = (rq_vol_curve_t *)RQ_CALLOC(s_default_max_curves, sizeof(rq_vol_curve_t));

    return s;
}

RQ_EXPORT void 
rq_ir_vol_surface_free(rq_ir_vol_surface_t ir_vol_surface)
{
    unsigned short i = 0;

    rq_termstruct_clear(&ir_vol_surface->termstruct);

    if (ir_vol_surface->vol_curves)
    {
        while (i < ir_vol_surface->num_curves)
        {
            if (ir_vol_surface->vol_curves[i])
                rq_vol_curve_free(ir_vol_surface->vol_curves[i]);

            i++;
        }

       RQ_FREE(ir_vol_surface->vol_curves);
       RQ_FREE(ir_vol_surface->tenors);
    }
    
   RQ_FREE(ir_vol_surface);
}

RQ_EXPORT const char *
rq_ir_vol_surface_get_termstruct_id(const rq_ir_vol_surface_t ir_vol_surface)
{
    return ir_vol_surface->termstruct.termstruct_id;
}

RQ_EXPORT const char *
rq_ir_vol_surface_get_underlying_asset_id(const rq_ir_vol_surface_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_ir_vol_surface_set_underlying_asset_id(rq_ir_vol_surface_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT void 
rq_ir_vol_surface_add(rq_ir_vol_surface_t ir_vol_surface, double underlyingTenor, rq_vol_curve_t c)
{
    int i = 0;

    while (i < ir_vol_surface->num_curves)
    {
        if (ir_vol_surface->tenors[i] == underlyingTenor)
        {
            if (ir_vol_surface->vol_curves[i])
                rq_vol_curve_free(ir_vol_surface->vol_curves[i]);

            ir_vol_surface->vol_curves[i] = c;
            return;
        }
		else if (ir_vol_surface->tenors[i] > underlyingTenor)
        {
            int j = ir_vol_surface->num_curves;

            /* move all surfaces up... */
            for ( ; j > i; j--)
            {
                ir_vol_surface->tenors[j] = ir_vol_surface->tenors[j-1];
                ir_vol_surface->vol_curves[j] = ir_vol_surface->vol_curves[j-1];
            }
            
            ir_vol_surface->tenors[i] = underlyingTenor;	/* TODO check if need to use rq_term_clone(tenor) */
            ir_vol_surface->vol_curves[i] = c;

            ir_vol_surface->num_curves++;
            return;
        }
        i++;
    }

    /* add at the end */
    ir_vol_surface->tenors[ir_vol_surface->num_curves] = underlyingTenor;	/* TODO check if need to use rq_term_clone(tenor) */
    ir_vol_surface->vol_curves[ir_vol_surface->num_curves] = c;
    ir_vol_surface->num_curves++;
}

/* TODO confirm if tenor relates to underlying (swap) maturity */
RQ_EXPORT int
rq_ir_vol_surface_get_volatility(
    const rq_ir_vol_surface_t ir_vol_surface,
    double underlyingTenor,
    rq_date optionMaturity,
    double *vol
    )
{
    int failed = 1;
    int i = 0;

    *vol = 0.0;

    /* look for delta in surface */
    while (i < ir_vol_surface->num_curves)
    {
		if (ir_vol_surface->tenors[i] == underlyingTenor)
        {
            /* get the vol from the curve */

            failed = rq_vol_curve_get_volatility(
                ir_vol_surface->vol_curves[i],
                optionMaturity,
                vol
                );
        }
		else if (ir_vol_surface->tenors[i] > underlyingTenor)
        {
            /* if this is the first just get the vol from the curve */
            if (i == 0)
            {
                failed = rq_vol_curve_get_volatility(
                    ir_vol_surface->vol_curves[i],
                    optionMaturity,
                    vol
                    );
            }
            else
            {
				/* TODO we should check the failed here */
                double v1, v2;
                rq_vol_curve_get_volatility(
                    ir_vol_surface->vol_curves[i-1],
                    optionMaturity,
                    &v1
                    );
                rq_vol_curve_get_volatility(
                    ir_vol_surface->vol_curves[i],
                    optionMaturity,
                    &v2
                    );
                *vol = rq_interpolate_linear(
                    underlyingTenor,
                    ir_vol_surface->tenors[i-1],
                    v1,
                    ir_vol_surface->tenors[i],
                    v2
                    );
                failed = 0;
            }
            break;
        }

        i++;
    }

    if (failed && ir_vol_surface->num_curves)
    {
        /* get the vol from the LAST curve */
        failed = rq_vol_curve_get_volatility(
            ir_vol_surface->vol_curves[ir_vol_surface->num_curves-1],
            optionMaturity,
            vol
            );
    }

    return failed;
}

RQ_EXPORT void
rq_ir_vol_surface_set_volatility(
    rq_ir_vol_surface_t ir_vol_surface,
    double underlyingTenor,
    rq_date optionMaturity,
    double vol
    )
{
    int i = 0;

    while (i < ir_vol_surface->num_curves)
    {
        if (ir_vol_surface->tenors[i] == underlyingTenor)
        {
            if (!ir_vol_surface->vol_curves[i])
                ir_vol_surface->vol_curves[i] = rq_vol_curve_alloc();
            rq_vol_curve_add(ir_vol_surface->vol_curves[i], optionMaturity, vol);
            return;
        }
		else if (ir_vol_surface->tenors[i] > underlyingTenor)
        {
            int j = ir_vol_surface->num_curves;

            /* move all surfaces up... */
            for ( ; j > i; j--)
            {
                ir_vol_surface->tenors[j] = ir_vol_surface->tenors[j-1];
                ir_vol_surface->vol_curves[j] = ir_vol_surface->vol_curves[j-1];
            }
            
            ir_vol_surface->tenors[i] = underlyingTenor;	/* TODO check if need to use rq_term_clone(tenor) */
            ir_vol_surface->vol_curves[i] = rq_vol_curve_alloc();

            rq_vol_curve_add(ir_vol_surface->vol_curves[i], optionMaturity, vol);

            ir_vol_surface->num_curves++;
            return;
        }
        i++;
    }

    /* add at the end */
    ir_vol_surface->tenors[ir_vol_surface->num_curves] = underlyingTenor;
    ir_vol_surface->vol_curves[ir_vol_surface->num_curves] = rq_vol_curve_alloc();
    rq_vol_curve_add(ir_vol_surface->vol_curves[ir_vol_surface->num_curves], optionMaturity, vol);
    ir_vol_surface->num_curves++;
}

RQ_EXPORT rq_ir_vol_surface_t
rq_ir_vol_surface_clone(
    const rq_ir_vol_surface_t ir_vol_surface
    )
{
    struct rq_ir_vol_surface *c = (struct rq_ir_vol_surface *)RQ_CALLOC(1, sizeof(struct rq_ir_vol_surface));

    int i;

    c->termstruct.termstruct_id = RQ_STRDUP(ir_vol_surface->termstruct.termstruct_id);
    if (ir_vol_surface->termstruct.underlying_asset_id)
        c->termstruct.underlying_asset_id = RQ_STRDUP(ir_vol_surface->termstruct.underlying_asset_id);

    c->num_curves = ir_vol_surface->num_curves;
    c->max_curves = ir_vol_surface->max_curves;

    c->tenors = (double *)RQ_CALLOC(ir_vol_surface->max_curves, sizeof(double));
    c->vol_curves = (rq_vol_curve_t *)RQ_CALLOC(ir_vol_surface->max_curves, sizeof(rq_vol_curve_t));

    for (i = 0; i < ir_vol_surface->num_curves; i++)
    {
        c->tenors[i] = ir_vol_surface->tenors[i];
        c->vol_curves[i] = rq_vol_curve_clone(ir_vol_surface->vol_curves[i]);
    }

    return c;
}

RQ_EXPORT unsigned 
rq_ir_vol_surface_count(const rq_ir_vol_surface_t ir_vol_surface)
{
    return ir_vol_surface->num_curves;
}

RQ_EXPORT const rq_vol_curve_t 
rq_ir_vol_surface_element_at(const rq_ir_vol_surface_t ir_vol_surface, unsigned offset)
{
    return ir_vol_surface->vol_curves[offset];
}

RQ_EXPORT const double
rq_ir_vol_surface_tenor_at(const rq_ir_vol_surface_t ir_vol_surface, unsigned offset)
{
    return ir_vol_surface->tenors[offset];
}

RQ_EXPORT int
rq_ir_vol_surface_is_null(rq_ir_vol_surface_t obj)
{
    return (obj == NULL);
}
