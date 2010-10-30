/*
** rq_asset_equityvol.c
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
#include "rq_asset_equityvol.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_equityvol = "EquityVol";

static struct rq_asset_equityvol *
rq_asset_equityvol_alloc()
{
    /* use RQ_CALLOC to zero out the memory */
    struct rq_asset_equityvol *cp = 
        (struct rq_asset_equityvol *)RQ_CALLOC(1, sizeof(struct rq_asset_equityvol));

    return cp;
}

static void
rq_asset_equityvol_free(struct rq_asset_equityvol *asset_equityvol)
{
    if (asset_equityvol->equity_code)
       RQ_FREE((char *)asset_equityvol->equity_code);
   RQ_FREE(asset_equityvol);
}

void
rq_asset_equityvol_freefunc(void *asset_data)
{
	rq_asset_equityvol_free((struct rq_asset_equityvol *)asset_data);
}

static int
rq_asset_equityvol_cmp(const void *node_data, const void *equity_code)
{
    const struct rq_asset_equityvol *equityvol = (const struct rq_asset_equityvol *)node_data;
    const char *equity = (const char *)equity_code;

    if (!strcmp(equityvol->equity_code, equity))
        return 1;
    return 0;
}

RQ_EXPORT rq_asset_t
rq_asset_equityvol_build(
    const char *asset_id,
    const char *equity_code,
    double delta_or_strike,
    unsigned short is_strike_vol
    )
{
    struct rq_asset_equityvol *asset_equityvol = rq_asset_equityvol_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_equityvol, 
        asset_id,
        asset_equityvol,
        rq_asset_equityvol_freefunc
        );

    asset_equityvol->equity_code = (const char *)RQ_STRDUP(equity_code);
    asset_equityvol->delta_or_strike = delta_or_strike;
    asset_equityvol->is_strike_vol = is_strike_vol;

    return asset;
}

RQ_EXPORT rq_asset_t
rq_asset_equityvol_find(const rq_asset_mgr_t asset_mgr, const char *equity_code)
{
    return rq_asset_mgr_find(
        asset_mgr, 
        rq_asset_type_id_equityvol, 
        rq_asset_equityvol_cmp, 
        (void *)equity_code
        );
}


RQ_EXPORT const char *
rq_asset_equityvol_get_equity_code(const rq_asset_t asset)
{
    struct rq_asset_equityvol *asset_equityvol = 
        (struct rq_asset_equityvol *)_rq_asset_get_asset_type_data(asset);
    return asset_equityvol->equity_code;
}

RQ_EXPORT double
rq_asset_equityvol_get_delta_or_strike(const rq_asset_t asset)
{
    struct rq_asset_equityvol *asset_equityvol = 
        (struct rq_asset_equityvol *)_rq_asset_get_asset_type_data(asset);
    return asset_equityvol->delta_or_strike;
}

RQ_EXPORT unsigned short
rq_asset_equityvol_is_strike_vol(const rq_asset_t asset)
{
    struct rq_asset_equityvol *asset_equityvol = 
        (struct rq_asset_equityvol *)_rq_asset_get_asset_type_data(asset);
    return asset_equityvol->is_strike_vol;
}

RQ_EXPORT unsigned short
rq_asset_equityvol_is_delta_vol(const rq_asset_t asset)
{
    struct rq_asset_equityvol *asset_equityvol = 
        (struct rq_asset_equityvol *)_rq_asset_get_asset_type_data(asset);
    return !asset_equityvol->is_strike_vol;
}
