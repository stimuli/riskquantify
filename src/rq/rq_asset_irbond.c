/*
** rq_asset_irbond.c
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
#include "rq_asset_irbond.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_irbond = "IrBond";

static struct rq_asset_irbond *
rq_asset_irbond_alloc()
{
    struct rq_asset_irbond *cp = 
        (struct rq_asset_irbond *)RQ_CALLOC(1, sizeof(struct rq_asset_irbond));

    return cp;
}

static void
rq_asset_irbond_free(struct rq_asset_irbond *asset_irbond)
{
    if (asset_irbond->ccy_code)
        RQ_FREE((char *)asset_irbond->ccy_code);
    RQ_FREE(asset_irbond);
}

void
rq_asset_irbond_freefunc(void *asset_data)
{
	rq_asset_irbond_free((struct rq_asset_irbond *)asset_data);
}

RQ_EXPORT rq_asset_t 
rq_asset_irbond_build(
    const char *asset_id,
    const char *ccy_code,
    rq_date mat_date,
    const struct rq_term *frequency,
    enum rq_day_count_convention day_count_convention,
    enum rq_date_roll_convention date_roll_convention
    )
{
    struct rq_asset_irbond *asset_irbond = rq_asset_irbond_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_irbond, 
        asset_id,
        asset_irbond,
        rq_asset_irbond_freefunc
        );

    asset_irbond->ccy_code = (const char *)RQ_STRDUP(ccy_code);
    asset_irbond->mat_date = mat_date;
    memcpy(&asset_irbond->frequency, frequency, sizeof(struct rq_term));

    asset_irbond->day_count_convention = day_count_convention;
    asset_irbond->date_roll_convention = date_roll_convention;

    return asset;
}


RQ_EXPORT const char *
rq_asset_irbond_get_ccy_code(const rq_asset_t asset)
{
    struct rq_asset_irbond *asset_irbond = 
        (struct rq_asset_irbond *)_rq_asset_get_asset_type_data(asset);
    return asset_irbond->ccy_code;
}

RQ_EXPORT rq_date
rq_asset_irbond_get_maturity_date(const rq_asset_t asset)
{
    struct rq_asset_irbond *asset_irbond = 
        (struct rq_asset_irbond *)_rq_asset_get_asset_type_data(asset);
    return asset_irbond->mat_date;
}

RQ_EXPORT const struct rq_term *
rq_asset_irbond_get_frequency(const rq_asset_t asset)
{
    struct rq_asset_irbond *asset_irbond = 
        (struct rq_asset_irbond *)_rq_asset_get_asset_type_data(asset);
    return &asset_irbond->frequency;
}

RQ_EXPORT enum rq_day_count_convention 
rq_asset_irbond_get_day_count_convention(const rq_asset_t asset)
{
    struct rq_asset_irbond *asset_irbond = 
        (struct rq_asset_irbond *)_rq_asset_get_asset_type_data(asset);
    return asset_irbond->day_count_convention;
}

RQ_EXPORT enum rq_date_roll_convention 
rq_asset_irbond_get_date_roll_convention(const rq_asset_t asset)
{
    struct rq_asset_irbond *asset_irbond = 
        (struct rq_asset_irbond *)_rq_asset_get_asset_type_data(asset);
    return asset_irbond->date_roll_convention;
}

RQ_EXPORT short
rq_asset_is_irbond(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irbond);
}
