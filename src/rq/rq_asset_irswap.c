/*
** rq_asset_irswap.c
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
#include "rq_asset_irswap.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_irswap = "IRSwap";

static struct rq_asset_irswap *
rq_asset_irswap_alloc()
{
    struct rq_asset_irswap *cp = 
        (struct rq_asset_irswap *)RQ_CALLOC(1, sizeof(struct rq_asset_irswap));

    return cp;
}

static void
rq_asset_irswap_free(struct rq_asset_irswap *asset_irswap)
{
    if (asset_irswap->ccy_code)
       RQ_FREE((char *)asset_irswap->ccy_code);
   RQ_FREE(asset_irswap);
}

void
rq_asset_irswap_freefunc(void *asset_data)
{
	rq_asset_irswap_free((struct rq_asset_irswap *)asset_data);
}

RQ_EXPORT rq_asset_t 
rq_asset_irswap_build(
    const char *asset_id,
    const char *ccy_code,
	const char *additional_calendar,
    const struct rq_term *start_tenor,
    enum rq_day_type start_tenor_type,
    const struct rq_term *tenor,
    const struct rq_term *frequency,
    enum rq_day_count_convention day_count_convention,
    enum rq_date_roll_convention date_roll_convention
    )
{
    struct rq_asset_irswap *asset_irswap = rq_asset_irswap_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_irswap, 
        asset_id,
        asset_irswap,
        rq_asset_irswap_freefunc
        );

    asset_irswap->ccy_code = (const char *)RQ_STRDUP(ccy_code);
	if(additional_calendar && strlen(additional_calendar) > 0)
		asset_irswap->additional_calendar = (const char *)RQ_STRDUP(additional_calendar);
    else
		asset_irswap->additional_calendar = 0;
	memcpy(&asset_irswap->start_tenor, start_tenor, sizeof(struct rq_term));
    memcpy(&asset_irswap->tenor, tenor, sizeof(struct rq_term));
    memcpy(&asset_irswap->frequency, frequency, sizeof(struct rq_term));

    asset_irswap->start_tenor_type = start_tenor_type;

    asset_irswap->day_count_convention = day_count_convention;
    asset_irswap->date_roll_convention = date_roll_convention;

    return asset;
}

RQ_EXPORT const char *
rq_asset_irswap_get_additional_calendar(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return asset_irswap->additional_calendar;
}

RQ_EXPORT const char *
rq_asset_irswap_get_ccy_code(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return asset_irswap->ccy_code;
}

RQ_EXPORT const struct rq_term *
rq_asset_irswap_get_start_tenor(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return &asset_irswap->start_tenor;
}

RQ_EXPORT enum rq_day_type rq_asset_irswap_get_start_tenor_type(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return asset_irswap->start_tenor_type;
}

RQ_EXPORT const struct rq_term *
rq_asset_irswap_get_tenor(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return &asset_irswap->tenor;
}

RQ_EXPORT const struct rq_term *
rq_asset_irswap_get_frequency(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return &asset_irswap->frequency;
}

RQ_EXPORT enum rq_day_count_convention 
rq_asset_irswap_get_day_count_convention(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return asset_irswap->day_count_convention;
}

RQ_EXPORT enum rq_date_roll_convention
rq_asset_irswap_get_date_roll_convention(const rq_asset_t asset)
{
    const struct rq_asset_irswap *asset_irswap = 
        (const struct rq_asset_irswap *)_rq_asset_get_asset_type_data(asset);
    return asset_irswap->date_roll_convention;
}

RQ_EXPORT short
rq_asset_is_irswap(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irswap);
}
