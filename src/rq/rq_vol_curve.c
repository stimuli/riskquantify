/*
** rq_vol_curve.c
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
#include "rq_vol_curve.h"
#include "rq_interpolate.h"
#include <stdlib.h>
#include <string.h>

static unsigned int s_initial_max_vols = 20;

RQ_EXPORT rq_vol_curve_t
rq_vol_curve_alloc()
{
    struct rq_vol_curve *c = (struct rq_vol_curve *)RQ_MALLOC(sizeof(struct rq_vol_curve));
    c->num_vols = 0;
    c->max_vols = s_initial_max_vols;
    c->vols = (struct rq_volatility *)RQ_MALLOC(s_initial_max_vols * sizeof(struct rq_volatility));

    return c;
}

RQ_EXPORT void 
rq_vol_curve_free(rq_vol_curve_t vol_curve)
{
    if (vol_curve->vols)
        RQ_FREE(vol_curve->vols);
    RQ_FREE(vol_curve);
}

RQ_EXPORT void
rq_vol_curve_add(rq_vol_curve_t vol_curve, rq_date date, double vol)
{
    /*
      Check to see if I need to grow the array
    */
    if (vol_curve->num_vols == vol_curve->max_vols)
    {
        unsigned int new_max_vols = 2 * vol_curve->max_vols;
        vol_curve->vols = (struct rq_volatility *)realloc(vol_curve->vols, new_max_vols * sizeof(struct rq_volatility));
        vol_curve->max_vols = new_max_vols;
    }

    /* use bisection to find the insertion point */
    if (vol_curve->num_vols == 0)
    {
        vol_curve->vols[vol_curve->num_vols].date = date;
        vol_curve->vols[vol_curve->num_vols].vol = vol;
        vol_curve->num_vols++;
    }
    else
    {
        /* check last node first */
        if (vol_curve->vols[vol_curve->num_vols-1].date < date)
        {
            vol_curve->vols[vol_curve->num_vols].date = date;
            vol_curve->vols[vol_curve->num_vols].vol = vol;
            vol_curve->num_vols++;
        }
        else if (vol_curve->vols[0].date > date)
		{
			int i;
            for (i = vol_curve->num_vols; i > 0; i--)
                memcpy(&(vol_curve->vols[i]), &(vol_curve->vols[i-1]), sizeof(struct rq_volatility));
            vol_curve->vols[0].date = date;
            vol_curve->vols[0].vol = vol;
			vol_curve->num_vols++;
		}
		else
        {
            int hp = vol_curve->num_vols;
            int lp = 1;
            int i;
            int ix;
            do
            {
                int ip = lp + ((hp - lp) >> 1);
                ix = ip-1;
                if (vol_curve->vols[ix].date > date)
                    hp = ip;
                else if (vol_curve->vols[ix].date < date)
                    lp = ip;
                else
                {
                    /* found date */
                    vol_curve->vols[ix].vol = vol;
                    return;
                }
            }
            while (hp - lp > 1);

            /* move everything from hp up one */
            ix = hp - 1;
            for (i = vol_curve->num_vols; i > ix; i--)
                memcpy(&(vol_curve->vols[i]), &(vol_curve->vols[i-1]), sizeof(struct rq_volatility));
            vol_curve->vols[ix].date = date;
            vol_curve->vols[ix].vol = vol;
			vol_curve->num_vols++;
        }
    }
}

RQ_EXPORT int 
rq_vol_curve_get_volatility(
    const rq_vol_curve_t vol_curve,
    rq_date date,
    double *vol
    )
{
    int ok = 1;
    unsigned int i;

    for (i = 0; i < vol_curve->num_vols; i++)
    {
        if (vol_curve->vols[i].date > date)
        {
            if (i == 0)
                *vol = vol_curve->vols[i].vol;
            else
            {
                *vol = rq_interpolate_linear(
                    date,
                    vol_curve->vols[i-1].date,
                    vol_curve->vols[i-1].vol,
                    vol_curve->vols[i].date,
                    vol_curve->vols[i].vol
                    );
            }

            ok = 0;
            break;
        }
    }

    if (ok != 0 && vol_curve->num_vols > 0)
    {
        *vol = vol_curve->vols[vol_curve->num_vols-1].vol;
        ok = 0;
    }

    return ok;
}

RQ_EXPORT rq_vol_curve_t
rq_vol_curve_clone(const rq_vol_curve_t vol_curve)
{
    struct rq_vol_curve *c = (struct rq_vol_curve *)RQ_CALLOC(1, sizeof(struct rq_vol_curve));
    c->num_vols = 0;
    c->max_vols = vol_curve->max_vols;
    c->vols = (struct rq_volatility *)RQ_MALLOC(vol_curve->max_vols * sizeof(struct rq_volatility));

    while (c->num_vols < vol_curve->num_vols)
    {
        c->vols[c->num_vols] = vol_curve->vols[c->num_vols];
        c->num_vols++;
    }

    return c;
}

RQ_EXPORT unsigned 
rq_vol_curve_count(const rq_vol_curve_t vol_curve)
{
    return vol_curve->num_vols;
}

RQ_EXPORT struct rq_volatility *
rq_vol_curve_element_at(const rq_vol_curve_t vol_curve, unsigned offset)
{
    return &vol_curve->vols[offset];
}

RQ_EXPORT int
rq_vol_curve_is_null(rq_vol_curve_t obj)
{
    return (obj == NULL);
}

#ifdef RQ_TEST_VOL_CURVE

int
main()
{
    int i;
    rq_vol_curve_t vol_curve = rq_vol_curve_alloc();

    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 6, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 5, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 2, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 3, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 4, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 1, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 9, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 12, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 10, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 11, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 8, 2003), 0.12);
    rq_vol_curve_add(vol_curve, rq_date_from_dmy(15, 7, 2003), 0.12);

    for (i = 0; i < rq_vol_curve_count(vol_curve); i++)
    {
        struct rq_volatility *vol = rq_vol_curve_element_at(vol_curve, i);

        printf("%d\n", rq_date_get_month(vol->date));
    }

    rq_vol_curve_free(vol_curve);
}

#endif
