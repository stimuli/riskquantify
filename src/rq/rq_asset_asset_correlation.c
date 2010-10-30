/*
** rq_asset_asset_correlation.c
**
** Written by Hendra
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
#include "rq_asset_asset_correlation.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char* const rq_asset_type_id_asset_correlation = "AssetCorrelation";

static struct rq_asset_asset_correlation* 
rq_asset_asset_correlation_alloc()
{
    struct rq_asset_asset_correlation* asset = 
        (struct rq_asset_asset_correlation*)RQ_CALLOC(1, sizeof(struct rq_asset_asset_correlation));

    return asset;
}

static void 
rq_asset_asset_correlation_free(struct rq_asset_asset_correlation* asset_asset_correlation)
{
    if (asset_asset_correlation->assetId)
       RQ_FREE((char*)asset_asset_correlation->assetId);
    if (asset_asset_correlation->asset1)
       RQ_FREE((char*)asset_asset_correlation->asset1);
    if (asset_asset_correlation->asset2)
       RQ_FREE((char*)asset_asset_correlation->asset2);
    RQ_FREE(asset_asset_correlation);
}

void 
rq_asset_asset_correlation_freefunc(void* asset_data)
{
    rq_asset_asset_correlation_free((struct rq_asset_asset_correlation*)asset_data);
}

RQ_EXPORT rq_asset_t 
rq_asset_asset_correlation_build(
    const char* asset_id,
    const char* asset1,
    const char* asset2
    )
{
    struct rq_asset_asset_correlation* ac = rq_asset_asset_correlation_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_asset_correlation, 
        asset_id,
        ac,
        rq_asset_asset_correlation_freefunc
        );

    ac->assetId = RQ_STRDUP(asset_id);
    ac->asset1 = RQ_STRDUP(asset1);
    ac->asset2 = RQ_STRDUP(asset2);

    return asset;
}

RQ_EXPORT rq_asset_t 
rq_asset_asset_correlation_find(const rq_asset_mgr_t asset_mgr, const char* asset1, const char* asset2)
{
    int i;
    rq_asset_t asset = NULL;
    struct rq_asset_asset_correlation* ac = NULL;

    rq_asset_list_t assets_list = rq_asset_mgr_list_assets_for_type(asset_mgr, 
        rq_asset_type_id_asset_correlation);
    int list_size = rq_asset_list_size(assets_list);
    for (i=0; i<list_size; i++)
    {
        asset = rq_asset_list_get_at(assets_list, i);
        ac = (struct rq_asset_asset_correlation*) asset->asset_type_data;
        if ((strcmp(ac->asset1, asset1) == 0 && strcmp(ac->asset2, asset2) == 0) || 
            (strcmp(ac->asset1, asset2) == 0 && strcmp(ac->asset2, asset1) == 0))
        {
            /* found it! */
            rq_asset_list_free(assets_list);
            return asset;
        }
    }

    /* Not found! */
    rq_asset_list_free(assets_list);
	return NULL;
}

RQ_EXPORT const char* 
rq_asset_asset_correlation_get_assetId(const rq_asset_t asset)
{
    struct rq_asset_asset_correlation *asset_asset_correlation = 
        (struct rq_asset_asset_correlation *)_rq_asset_get_asset_type_data(asset);
    return asset_asset_correlation->assetId;
}

RQ_EXPORT const char* rq_asset_asset_correlation_get_asset1(const rq_asset_t asset)
{
    struct rq_asset_asset_correlation *asset_asset_correlation = 
        (struct rq_asset_asset_correlation *)_rq_asset_get_asset_type_data(asset);
    return asset_asset_correlation->asset1;
}

RQ_EXPORT const char* rq_asset_asset_correlation_get_asset2(const rq_asset_t asset)
{
    struct rq_asset_asset_correlation *asset_asset_correlation = 
        (struct rq_asset_asset_correlation *)_rq_asset_get_asset_type_data(asset);
    return asset_asset_correlation->asset2;
}

RQ_EXPORT short 
rq_asset_is_asset_correlation(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_asset_correlation);
}
