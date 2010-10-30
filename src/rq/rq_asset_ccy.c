/*
** rq_asset_ccy.c
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
#include "rq_asset_ccy.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_ccy = "Ccy";

static struct rq_asset_ccy *
rq_asset_ccy_alloc()
{
    struct rq_asset_ccy *asset = 
        (struct rq_asset_ccy *)RQ_CALLOC(1, sizeof(struct rq_asset_ccy));

    return asset;
}

static void 
rq_asset_ccy_free(struct rq_asset_ccy *asset_ccy)
{
    if (asset_ccy->ccy_code)
       RQ_FREE((char *)asset_ccy->ccy_code);
   RQ_FREE(asset_ccy);
}

void
rq_asset_ccy_freefunc(void *asset_data)
{
	rq_asset_ccy_free((struct rq_asset_ccy *)asset_data);
}

static int
rq_asset_ccy_code_cmp(const void *node_data, const void *ccy_data)
{
    const struct rq_asset_ccy *asset_ccy = (const struct rq_asset_ccy *)node_data;
    const char *ccy = (const char *)ccy_data;

    return strcmp(asset_ccy->ccy_code, ccy);
}

RQ_EXPORT
rq_asset_t
rq_asset_ccy_build(
    const char *asset_id,
    const char *ccy_code,
    unsigned short days_per_year,
    unsigned short spot_days
    )
{
    struct rq_asset_ccy *asset_ccy = rq_asset_ccy_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_ccy, 
        asset_id,
        asset_ccy,
        rq_asset_ccy_freefunc
        );

    asset_ccy->ccy_code = (const char *)RQ_STRDUP(ccy_code);
    asset_ccy->days_per_year = days_per_year;
    asset_ccy->spot_days = spot_days;

    return asset;
}

RQ_EXPORT
rq_asset_t
rq_asset_ccy_find(const rq_asset_mgr_t asset_mgr, const char *ccy_code)
{
    return rq_asset_mgr_find(
        asset_mgr, 
        rq_asset_type_id_ccy, 
        rq_asset_ccy_code_cmp, 
        (void *)ccy_code
        );
}

RQ_EXPORT
const char *
rq_asset_ccy_get_ccy_code(const rq_asset_t a)
{
    const struct rq_asset_ccy *asset_ccy = _rq_asset_get_asset_type_data(a);
    return asset_ccy->ccy_code;
}

RQ_EXPORT
unsigned short 
rq_asset_ccy_get_days_per_year(const rq_asset_t a)
{
    const struct rq_asset_ccy *asset_ccy = _rq_asset_get_asset_type_data(a);
    return asset_ccy->days_per_year;
}

RQ_EXPORT short
rq_asset_is_ccy(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_ccy);
}

RQ_EXPORT
unsigned short 
rq_asset_ccy_get_spot_days(const rq_asset_t a)
{
    const struct rq_asset_ccy *asset_ccy = _rq_asset_get_asset_type_data(a);
    return asset_ccy->spot_days;
}
