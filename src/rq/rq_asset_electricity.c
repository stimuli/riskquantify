/*
** rq_asset_electricity.c
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
#include "rq_asset_electricity.h"
#include "rq_asset_mgr.h"
#include "rq_date.h"
#include "rq_array.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_electricity = "Electricity";

static struct rq_asset_electricity *
rq_asset_electricity_alloc()
{
    struct rq_asset_electricity *asset = 
        (struct rq_asset_electricity *)RQ_CALLOC(1, sizeof(struct rq_asset_electricity));

    return asset;
}

static void 
rq_asset_electricity_free(struct rq_asset_electricity *asset_electricity)
{
    if (asset_electricity->exchange)
    	RQ_FREE((char *)asset_electricity->exchange);

    if (asset_electricity->load_type)
    	RQ_FREE((char *)asset_electricity->load_type);

	RQ_FREE(asset_electricity);
}

void
rq_asset_electricity_freefunc(void *asset_data)
{
	rq_asset_electricity_free((struct rq_asset_electricity *)asset_data);
}

RQ_EXPORT rq_asset_t
rq_asset_electricity_build(const char *asset_id, const char *exchange, const char *load_type)
{
    struct rq_asset_electricity *asset_electricity = rq_asset_electricity_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_electricity, 
        asset_id,
        asset_electricity,
        rq_asset_electricity_freefunc
        );

    asset_electricity->exchange = (const char *)RQ_STRDUP(exchange);

    return asset;
}

RQ_EXPORT const char *
rq_asset_electricity_get_exchange(const rq_asset_t a)
{
    const struct rq_asset_electricity *asset_electricity = _rq_asset_get_asset_type_data(a);
    return asset_electricity->exchange;
}

RQ_EXPORT const char *
rq_asset_electricity_get_load_type(const rq_asset_t a)
{
    const struct rq_asset_electricity *asset_electricity = _rq_asset_get_asset_type_data(a);
    return asset_electricity->load_type;
}

RQ_EXPORT short
rq_asset_is_electricity(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_electricity);
}
