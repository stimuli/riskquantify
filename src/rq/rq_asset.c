/*
** rq_asset.c
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
#include "rq_asset.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_asset_t 
_rq_asset_alloc(const char *asset_type_id, const char *asset_id, void *asset_type_data, void (*free_func)(void *asset_type_data))
{
    struct rq_asset *a = (struct rq_asset *)RQ_CALLOC(1, sizeof(struct rq_asset));
    a->asset_id = (const char *)RQ_STRDUP(asset_id);
    a->asset_type_id = (const char *)RQ_STRDUP(asset_type_id);

    a->asset_type_data = asset_type_data;
    a->free_func = free_func;

    return a;
}

RQ_EXPORT void 
rq_asset_free(rq_asset_t asset)
{
    if (asset->free_func)
        (*asset->free_func)(asset->asset_type_data);

    if (asset->asset_id)
       RQ_FREE((char *)asset->asset_id);
    if (asset->asset_type_id)
       RQ_FREE((char *)asset->asset_type_id);
}

RQ_EXPORT const char * 
rq_asset_get_asset_id(const rq_asset_t asset)
{
	if ( !asset ) return "";
	else return asset->asset_id;
}

RQ_EXPORT const char * 
rq_asset_get_asset_type_id(const rq_asset_t asset)
{
    return asset->asset_type_id;
}

RQ_EXPORT void * 
_rq_asset_get_asset_type_data(const rq_asset_t asset)
{
    return asset->asset_type_data;
}

RQ_EXPORT int 
rq_asset_is_null(rq_asset_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_date 
rq_asset_get_value_date(const rq_asset_t asset, rq_date from_date)
{
    rq_date value_date = 0;

    if (asset->get_value_date_func)
        value_date = (*asset->get_value_date_func)(asset, from_date);

    return value_date;
}
