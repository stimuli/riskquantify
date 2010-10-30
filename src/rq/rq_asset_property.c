/*
** rq_asset_property.c
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
#include "rq_asset_property.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_property = "Property";

static struct rq_asset_property *
rq_asset_property_alloc()
{
    struct rq_asset_property *asset = 
        (struct rq_asset_property *)RQ_CALLOC(1, sizeof(struct rq_asset_property));
    asset->property_type = RQ_ASSET_PROPERTY_TYPE_NOT_SPECIFIED;

    return asset;
}

void  
rq_asset_property_free(void *data)
{
    struct rq_asset_property *property = (struct rq_asset_property *)data;

    if (property->country_id)
        RQ_FREE((char *)property->country_id);
    if (property->region_id)
        RQ_FREE((char *)property->region_id);
    if (property->type_code)
        RQ_FREE((char *)property->type_code);

    RQ_FREE(property);
}

RQ_EXPORT rq_asset_t
rq_asset_property_build(
    const char *asset_id, 
    const char *country_id, 
    enum rq_asset_property_type property_type, 
    const char *region_id, 
    const char *type_code
    )
{
    struct rq_asset_property *property = rq_asset_property_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_property, 
        asset_id,
        property,
        rq_asset_property_free
        );

    property->country_id = (const char *)RQ_STRDUP(country_id);
    property->region_id = (const char *)RQ_STRDUP(region_id);
    property->property_type = property_type;
    property->type_code = (const char *)RQ_STRDUP(type_code);

    return asset;
}

RQ_EXPORT short
rq_asset_is_property(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_property);
}

const char *
rq_asset_property_get_country_id(rq_asset_t asset)
{
    struct rq_asset_property *property = (struct rq_asset_property *)_rq_asset_get_asset_type_data(asset);
    return property->country_id;
}

const char *
rq_asset_property_get_region_id(rq_asset_t asset)
{
    struct rq_asset_property *property = (struct rq_asset_property *)_rq_asset_get_asset_type_data(asset);
    return property->region_id;
}

const char *
rq_asset_property_get_type_code(rq_asset_t asset)
{
    struct rq_asset_property *property = (struct rq_asset_property *)_rq_asset_get_asset_type_data(asset);
    return property->type_code;
}

enum rq_asset_property_type 
rq_asset_property_get_property_type(rq_asset_t asset)
{
    struct rq_asset_property *property = (struct rq_asset_property *)_rq_asset_get_asset_type_data(asset);
    return property->property_type;
}
