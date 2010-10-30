/*
** rq_asset_cds_spread.c
**
** Written by Todd Cooper
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
#include "rq_asset_cds_spread.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_cds_spread = "CdsSpread";

static struct rq_asset_cds_spread *
rq_asset_cds_spread_alloc()
{
    struct rq_asset_cds_spread *asset = 
        (struct rq_asset_cds_spread *)RQ_CALLOC(1, sizeof(struct rq_asset_cds_spread));

    return asset;
}

static void 
rq_asset_cds_spread_free(struct rq_asset_cds_spread *asset_cds_spread)
{
    if (asset_cds_spread->quote_type)
       RQ_FREE((char *)asset_cds_spread->quote_type);
   RQ_FREE(asset_cds_spread);
}

void
rq_asset_cds_spread_freefunc(void *asset_data)
{
	rq_asset_cds_spread_free((struct rq_asset_cds_spread *)asset_data);
}

RQ_EXPORT rq_asset_t
rq_asset_cds_spread_build(
    const char *asset_id,
    const char *quote_type,
    const struct rq_term *frequency
    )
{
    struct rq_asset_cds_spread *asset_cds_spread = rq_asset_cds_spread_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_cds_spread, 
        asset_id,
        asset_cds_spread,
        rq_asset_cds_spread_freefunc
        );

    asset_cds_spread->quote_type = (const char *)RQ_STRDUP(quote_type);
    memcpy(&asset_cds_spread->frequency, frequency, sizeof(struct rq_term));

    return asset;
}

RQ_EXPORT const char *
rq_asset_cds_spread_get_quote_type(const rq_asset_t asset)
{
    struct rq_asset_cds_spread *asset_cds_spread = 
        (struct rq_asset_cds_spread *)_rq_asset_get_asset_type_data(asset);
    return asset_cds_spread->quote_type;
}

RQ_EXPORT const struct rq_term *
rq_asset_cds_spread_get_frequency(const rq_asset_t asset)
{
    struct rq_asset_cds_spread *asset_cds_spread = 
        (struct rq_asset_cds_spread *)_rq_asset_get_asset_type_data(asset);
    return &asset_cds_spread->frequency;
}

RQ_EXPORT short
rq_asset_is_cds_spread(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_cds_spread);
}
