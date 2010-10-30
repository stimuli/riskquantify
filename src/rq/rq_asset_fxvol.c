/*
** rq_asset_fxvol.c
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
#include "rq_asset_fxvol.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_fxvol = "FxVol";

static struct rq_asset_fxvol *
rq_asset_fxvol_alloc()
{
    /* use RQ_CALLOC to zero out the memory */
    struct rq_asset_fxvol *cp = 
        (struct rq_asset_fxvol *)RQ_CALLOC(1, sizeof(struct rq_asset_fxvol));

    return cp;
}

static void
rq_asset_fxvol_free(struct rq_asset_fxvol *asset_fxvol)
{
    if (asset_fxvol->ccy_code_1)
       RQ_FREE((char *)asset_fxvol->ccy_code_1);
    if (asset_fxvol->ccy_code_2)
       RQ_FREE((char *)asset_fxvol->ccy_code_2);
   RQ_FREE(asset_fxvol);
}

void
rq_asset_fxvol_freefunc(void *asset_data)
{
	rq_asset_fxvol_free((struct rq_asset_fxvol *)asset_data);
}

static int
rq_asset_fxvol_cmp(const void *node_data, const void *ccys_data)
{
    const struct rq_asset_fxvol *fxvol = (const struct rq_asset_fxvol *)node_data;
    const char **ccys = (const char **)ccys_data;

    if ((!strcmp(fxvol->ccy_code_1, ccys[0]) && !strcmp(fxvol->ccy_code_2, ccys[1])) ||
        (!strcmp(fxvol->ccy_code_1, ccys[1]) && !strcmp(fxvol->ccy_code_2, ccys[0])))
        return 1;
    return 0;
}

RQ_EXPORT rq_asset_t
rq_asset_fxvol_build(
    const char *asset_id,
    const char *ccy_code_1,
    const char *ccy_code_2,
    double delta_or_strike,
    unsigned short is_strike_vol
    )
{
    struct rq_asset_fxvol *asset_fxvol = rq_asset_fxvol_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_fxvol, 
        asset_id,
        asset_fxvol,
        rq_asset_fxvol_freefunc
        );

    asset_fxvol->ccy_code_1 = (const char *)RQ_STRDUP(ccy_code_1);
    asset_fxvol->ccy_code_2 = (const char *)RQ_STRDUP(ccy_code_2);
    asset_fxvol->delta_or_strike = delta_or_strike;
    asset_fxvol->is_strike_vol = is_strike_vol;

    return asset;
}

RQ_EXPORT rq_asset_t
rq_asset_fxvol_find(const rq_asset_mgr_t asset_mgr, const char *ccy_code_1, const char *ccy_code_2)
{
    const char *ccys[2];

    ccys[0] = ccy_code_1;
    ccys[1] = ccy_code_2;

    return rq_asset_mgr_find(
        asset_mgr, 
        rq_asset_type_id_fxvol, 
        rq_asset_fxvol_cmp, 
        (void *)ccys
        );
}


RQ_EXPORT const char *
rq_asset_fxvol_get_ccy_code_1(const rq_asset_t asset)
{
    struct rq_asset_fxvol *asset_fxvol = 
        (struct rq_asset_fxvol *)_rq_asset_get_asset_type_data(asset);
    return asset_fxvol->ccy_code_1;
}

RQ_EXPORT const char *
rq_asset_fxvol_get_ccy_code_2(const rq_asset_t asset)
{
    struct rq_asset_fxvol *asset_fxvol = 
        (struct rq_asset_fxvol *)_rq_asset_get_asset_type_data(asset);
    return asset_fxvol->ccy_code_2;
}

RQ_EXPORT double
rq_asset_fxvol_get_delta_or_strike(const rq_asset_t asset)
{
    struct rq_asset_fxvol *asset_fxvol = 
        (struct rq_asset_fxvol *)_rq_asset_get_asset_type_data(asset);
    return asset_fxvol->delta_or_strike;
}

RQ_EXPORT unsigned short
rq_asset_fxvol_is_strike_vol(const rq_asset_t asset)
{
    struct rq_asset_fxvol *asset_fxvol = 
        (struct rq_asset_fxvol *)_rq_asset_get_asset_type_data(asset);
    return asset_fxvol->is_strike_vol;
}

RQ_EXPORT unsigned short
rq_asset_fxvol_is_delta_vol(const rq_asset_t asset)
{
    struct rq_asset_fxvol *asset_fxvol = 
        (struct rq_asset_fxvol *)_rq_asset_get_asset_type_data(asset);
    return !asset_fxvol->is_strike_vol;
}
