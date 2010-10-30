/*
** rq_asset_base_correlation.c
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
#include "rq_asset_base_correlation.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_base_correlation = "BaseCorrelation";

static struct rq_asset_base_correlation *
rq_asset_base_correlation_alloc()
{
    struct rq_asset_base_correlation *asset = 
        (struct rq_asset_base_correlation *)RQ_CALLOC(1, sizeof(struct rq_asset_base_correlation));

    return asset;
}

static void 
rq_asset_base_correlation_free(struct rq_asset_base_correlation *asset_base_correlation)
{
    if (asset_base_correlation->name)
       RQ_FREE((char *)asset_base_correlation->name);
   RQ_FREE(asset_base_correlation);
}

void
rq_asset_base_correlation_freefunc(void *asset_data)
{
	rq_asset_base_correlation_free((struct rq_asset_base_correlation *)asset_data);
}

RQ_EXPORT rq_asset_t
rq_asset_base_correlation_build(
    const char *asset_id,
    const char *name,
	double detachment
    )
{
    struct rq_asset_base_correlation *asset_base_correlation = rq_asset_base_correlation_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_base_correlation, 
        asset_id,
        asset_base_correlation,
        rq_asset_base_correlation_freefunc
        );

	if (name)
	    asset_base_correlation->name = (const char *)RQ_STRDUP(name);
	else
		asset_base_correlation->name = NULL;

	asset_base_correlation->detachment = detachment;

    return asset;
}

RQ_EXPORT const char *
rq_asset_base_correlation_get_name(const rq_asset_t asset)
{
    struct rq_asset_base_correlation *asset_base_correlation = 
        (struct rq_asset_base_correlation *)_rq_asset_get_asset_type_data(asset);
    return asset_base_correlation->name;
}


RQ_EXPORT double
rq_asset_base_correlation_get_detachment(const rq_asset_t asset)
{
    struct rq_asset_base_correlation *asset_base_correlation = 
        (struct rq_asset_base_correlation *)_rq_asset_get_asset_type_data(asset);
    return asset_base_correlation->detachment;
}
RQ_EXPORT short
rq_asset_is_base_correlation(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_base_correlation);
}
