/*
** rq_asset_irdiscount.c
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
#include "rq_asset_irdiscount.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_irdiscount = "IRDiscount";

static struct rq_asset_irdiscount *
rq_asset_irdiscount_alloc()
{
    struct rq_asset_irdiscount *asset = 
        (struct rq_asset_irdiscount *)RQ_CALLOC(1, sizeof(struct rq_asset_irdiscount));

    return asset;
}

static void 
rq_asset_irdiscount_free(struct rq_asset_irdiscount *asset_irdiscount)
{
    if (asset_irdiscount->ccy_code)
        RQ_FREE((char *)asset_irdiscount->ccy_code);
    RQ_FREE(asset_irdiscount);
}

void
rq_asset_irdiscount_freefunc(void *asset_data)
{
	rq_asset_irdiscount_free((struct rq_asset_irdiscount *)asset_data);
}

// left for backward compatibility, does not support effective start nor date roll convention
rq_date
rq_asset_irdiscount_get_value_date(struct rq_asset *asset, rq_date from_date)
{
    struct rq_asset_irdiscount *asset_irdiscount = 
        _rq_asset_get_asset_type_data(asset);
    return rq_date_add_term(from_date, &asset_irdiscount->tenor);
}

RQ_EXPORT rq_asset_t
rq_asset_irdiscount_build(
    const char *asset_id,
    const char *ccy_code,
    const struct rq_term *start_tenor,
    enum rq_day_type start_tenor_type,
    const struct rq_term *tenor,
    long maturity_date,
    enum rq_day_count_convention day_count_convention,
    enum rq_date_roll_convention date_roll_convention,
	enum rq_zero_method zero_method,
	long zero_method_compound_frequency
    )
{
    struct rq_asset_irdiscount *asset_irdiscount = rq_asset_irdiscount_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_irdiscount, 
        asset_id,
        asset_irdiscount,
        rq_asset_irdiscount_freefunc
        );

    asset->get_value_date_func = rq_asset_irdiscount_get_value_date;

    asset_irdiscount->ccy_code = (const char *)RQ_STRDUP(ccy_code);
    asset_irdiscount->day_count_convention = day_count_convention;
    asset_irdiscount->maturity_date = maturity_date;

    memcpy(&asset_irdiscount->start_tenor, start_tenor, sizeof(struct rq_term));
    memcpy(&asset_irdiscount->tenor, tenor, sizeof(struct rq_term));

    asset_irdiscount->start_tenor_type = start_tenor_type;
    asset_irdiscount->date_roll_convention = date_roll_convention;
	asset_irdiscount->zero_method = zero_method;
    asset_irdiscount->zero_method_compound_frequency = zero_method_compound_frequency;

    return asset;
}

RQ_EXPORT const char *
rq_asset_irdiscount_get_ccy_code(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return bb->ccy_code;
}

RQ_EXPORT enum rq_day_count_convention 
rq_asset_irdiscount_get_day_count_convention(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return bb->day_count_convention;
}

RQ_EXPORT const struct rq_term *
rq_asset_irdiscount_get_start_tenor(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return &bb->start_tenor;
}

RQ_EXPORT enum rq_day_type rq_asset_irdiscount_get_start_tenor_type(const rq_asset_t a)
{
    const struct rq_asset_irdiscount *asset = 
        (const struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return asset->start_tenor_type;
}

RQ_EXPORT const struct rq_term *
rq_asset_irdiscount_get_tenor(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return &bb->tenor;
}

RQ_EXPORT enum rq_date_roll_convention
rq_asset_irdiscount_get_date_roll_convention(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return bb->date_roll_convention;
}

RQ_EXPORT long 
rq_asset_irdiscount_get_maturity_date(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return bb->maturity_date;
}

RQ_EXPORT short
rq_asset_is_irdiscount(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irdiscount);
}

RQ_EXPORT enum rq_zero_method
rq_asset_irdiscount_get_zero_method(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return bb->zero_method;
}

RQ_EXPORT long
rq_asset_irdiscount_get_zero_method_compounding_frequency(const rq_asset_t a)
{
    struct rq_asset_irdiscount *bb = (struct rq_asset_irdiscount *)_rq_asset_get_asset_type_data(a);
    return bb->zero_method_compound_frequency;
}

