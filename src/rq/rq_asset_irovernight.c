/*
** rq_asset_irovernight.c
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
#include "rq_asset_irovernight.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_irovernight = "IROvernight";

static struct rq_asset_irovernight *
rq_asset_irovernight_alloc()
{
    struct rq_asset_irovernight *asset = 
        (struct rq_asset_irovernight *)RQ_CALLOC(1, sizeof(struct rq_asset_irovernight));

    return asset;
}

static void 
rq_asset_irovernight_free(struct rq_asset_irovernight *asset_irovernight)
{
    if (asset_irovernight->ccy_code)
       RQ_FREE((char *)asset_irovernight->ccy_code);
   RQ_FREE(asset_irovernight);
}

void
rq_asset_irovernight_freefunc(void *asset_data)
{
	rq_asset_irovernight_free((struct rq_asset_irovernight *)asset_data);
}

RQ_EXPORT rq_asset_t
rq_asset_irovernight_build(
    const char *asset_id,
    const char *ccy_code,
    enum rq_day_count_convention day_count_convention
    )
{
    struct rq_asset_irovernight *asset_irovernight = rq_asset_irovernight_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_irovernight, 
        asset_id,
        asset_irovernight,
        rq_asset_irovernight_freefunc
        );

    asset_irovernight->ccy_code = (const char *)RQ_STRDUP(ccy_code);
    asset_irovernight->day_count_convention = day_count_convention;

    return asset;
}

RQ_EXPORT const char *
rq_asset_irovernight_get_ccy_code(const rq_asset_t a)
{
    struct rq_asset_irovernight *bb = (struct rq_asset_irovernight *)_rq_asset_get_asset_type_data(a);
    return bb->ccy_code;
}

RQ_EXPORT enum rq_day_count_convention 
rq_asset_irovernight_get_day_count_convention(const rq_asset_t a)
{
    struct rq_asset_irovernight *bb = (struct rq_asset_irovernight *)_rq_asset_get_asset_type_data(a);
    return bb->day_count_convention;
}

RQ_EXPORT short
rq_asset_is_irovernight(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irovernight);
}
