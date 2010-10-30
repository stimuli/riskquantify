/*
** rq_asset_irfuture.c
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
#include "rq_asset_irfuture.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_irfuture = "IRFuture";

static struct rq_asset_irfuture *
rq_asset_irfuture_alloc()
{
    struct rq_asset_irfuture *cp = 
        (struct rq_asset_irfuture *)RQ_CALLOC(1, sizeof(struct rq_asset_irfuture));

    return cp;
}

static void
rq_asset_irfuture_free(struct rq_asset_irfuture *asset_irfuture)
{
    if (asset_irfuture->ccy_code)
       RQ_FREE((char *)asset_irfuture->ccy_code);
   RQ_FREE(asset_irfuture);
}

void
rq_asset_irfuture_freefunc(void *asset_data)
{
	rq_asset_irfuture_free((struct rq_asset_irfuture *)asset_data);
}

RQ_EXPORT rq_asset_t 
rq_asset_irfuture_build(
    const char *asset_id,
    const char *ccy_code,
    rq_date lastTradingDate,
    rq_date settlementDate,
    const struct rq_term *tenor,
    enum rq_day_count_convention dayCountConv,
    enum rq_ir_future_quote_convention quoteConvention
    )
{
    struct rq_asset_irfuture *asset_irfuture = rq_asset_irfuture_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_irfuture, 
        asset_id,
        asset_irfuture,
        rq_asset_irfuture_freefunc
        );

    asset_irfuture->ccy_code = (const char *)RQ_STRDUP(ccy_code);
    asset_irfuture->settlement_date = settlementDate;
    memcpy(&asset_irfuture->tenor, tenor, sizeof(struct rq_term));

    asset_irfuture->day_count_convention = dayCountConv;
    asset_irfuture->quote_convention = quoteConvention;

    return asset;
}


RQ_EXPORT const char *
rq_asset_irfuture_get_ccy_code(const rq_asset_t asset)
{
    struct rq_asset_irfuture *asset_irfuture = 
        (struct rq_asset_irfuture *)_rq_asset_get_asset_type_data(asset);
    return asset_irfuture->ccy_code;
}

RQ_EXPORT const struct rq_term *
rq_asset_irfuture_get_tenor(const rq_asset_t asset)
{
    struct rq_asset_irfuture *asset_irfuture = 
        (struct rq_asset_irfuture *)_rq_asset_get_asset_type_data(asset);
    return &asset_irfuture->tenor;
}

RQ_EXPORT rq_date 
rq_asset_irfuture_get_last_trading_date(const rq_asset_t asset)
{
    struct rq_asset_irfuture *asset_irfuture = 
        (struct rq_asset_irfuture *)_rq_asset_get_asset_type_data(asset);
    return asset_irfuture->last_trading_date;
}

RQ_EXPORT rq_date 
rq_asset_irfuture_get_settlement_date(const rq_asset_t asset)
{
    struct rq_asset_irfuture *asset_irfuture = 
        (struct rq_asset_irfuture *)_rq_asset_get_asset_type_data(asset);
    return asset_irfuture->settlement_date;
}

/* Return the expiry of the underlying bill/bond. */
RQ_EXPORT rq_date 
rq_asset_irfuture_get_maturity_date(const rq_asset_t asset)
{
    const struct rq_asset_irfuture *asset_irfuture = 
        (const struct rq_asset_irfuture *)_rq_asset_get_asset_type_data(asset);
    return rq_date_add_term(asset_irfuture->settlement_date, &asset_irfuture->tenor);
}

RQ_EXPORT enum rq_day_count_convention 
rq_asset_irfuture_get_day_count_convention(const rq_asset_t asset)
{
    struct rq_asset_irfuture *asset_irfuture = 
        (struct rq_asset_irfuture *)_rq_asset_get_asset_type_data(asset);
    return asset_irfuture->day_count_convention;
}

RQ_EXPORT enum rq_ir_future_quote_convention 
rq_asset_irfuture_get_quote_convention(const rq_asset_t asset)
{
    struct rq_asset_irfuture *asset_irfuture = 
        (struct rq_asset_irfuture *)_rq_asset_get_asset_type_data(asset);
    return asset_irfuture->quote_convention;
}

RQ_EXPORT short
rq_asset_is_irfuture(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_irfuture);
}
