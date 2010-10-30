/*
** rq_asset_floating_rate_index.c
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
#include "rq_asset_floating_rate_index.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_floating_rate_index = "FloatingRateIndex";

static struct rq_asset_floating_rate_index *
rq_asset_floating_rate_index_alloc()
{
    struct rq_asset_floating_rate_index *asset = 
        (struct rq_asset_floating_rate_index *)RQ_CALLOC(1, sizeof(struct rq_asset_floating_rate_index));
    return asset;
}

static void 
rq_asset_floating_rate_index_free(void *p)
{
    struct rq_asset_floating_rate_index *asset_floating_rate_index = 
        (struct rq_asset_floating_rate_index *)p;
    if (asset_floating_rate_index->ccy_code)
       RQ_FREE((char *)asset_floating_rate_index->ccy_code);
   RQ_FREE(asset_floating_rate_index);
}


RQ_EXPORT rq_asset_t
rq_asset_floating_rate_index_build(
    const char *asset_id,
    const char *ccy_code
    )
{
    struct rq_asset_floating_rate_index *asset_floating_rate_index = rq_asset_floating_rate_index_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_floating_rate_index, 
        asset_id,
        asset_floating_rate_index,
        rq_asset_floating_rate_index_free
        );

    asset_floating_rate_index->ccy_code = (const char *)RQ_STRDUP(ccy_code);

	return asset;
}


RQ_EXPORT const char *
rq_asset_floating_rate_index_get_ccy_code(const rq_asset_t a)
{
    struct rq_asset_floating_rate_index *fri = (struct rq_asset_floating_rate_index *)_rq_asset_get_asset_type_data(a);
    return fri->ccy_code;
}

RQ_EXPORT short
rq_asset_is_floating_rate_index(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_floating_rate_index);
}
