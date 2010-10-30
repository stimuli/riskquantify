/*
** rq_asset_future.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#include "rq_asset_future.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_future = "Future";


void
rq_asset_future_freefunc(void *d)
{
    struct rq_asset_future *c = (struct rq_asset_future *)d;
	if (c->long_name) free((char *)c->long_name);
    if (c->type) free((char *)c->type);
    if (c->grade) free((char *)c->grade);
    if (c->location) free((char *)c->location);
    if (c->location_center) free((char *)c->location_center);
	
    if (c->currency) free((char *)c->currency);
    if (c->unit) free((char *)c->unit);

    free(c);
}

RQ_EXPORT rq_asset_t
rq_asset_future_build(
    const char *asset_id,
    const char *currency,
	const char *long_name,
    const char *type,
    const char *grade,
    const char *location,
	const char *location_center,
    const char *unit,
    double unit_multiplier
    )
{
    struct rq_asset_future *c = (struct rq_asset_future *)calloc(
        1, 
        sizeof(struct rq_asset_future)
        );
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_future, 
        asset_id,
        c,
        rq_asset_future_freefunc
        );

    if (long_name) c->long_name = strdup(long_name);
    if (type) c->type = strdup(type);
    if (grade) c->grade = strdup(grade);
    if (location) c->location = strdup(location);
    if (location_center) c->location_center = strdup(location_center);
    if (currency) c->currency = strdup(currency);
    if (unit) c->unit = strdup(unit);
    c->unit_multiplier = unit_multiplier;

    return asset;
}

RQ_EXPORT short
rq_asset_is_future(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_future);
}

RQ_EXPORT const char *
rq_asset_future_get_long_name(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->long_name;
}

RQ_EXPORT const char *
rq_asset_future_get_type(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->type;
}

RQ_EXPORT const char *
rq_asset_future_get_grade(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->grade;
}

RQ_EXPORT const char *
rq_asset_future_get_location(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->location;
}

RQ_EXPORT const char *
rq_asset_future_get_location_center(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->location_center;
}

RQ_EXPORT const char *
rq_asset_future_get_currency(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->currency;
}

RQ_EXPORT const char *
rq_asset_future_get_unit(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->unit;
}

RQ_EXPORT double 
rq_asset_future_get_unit_multiplier(rq_asset_t asset)
{
    struct rq_asset_future *c = (struct rq_asset_future *)_rq_asset_get_asset_type_data(asset);
    return c->unit_multiplier;
}
