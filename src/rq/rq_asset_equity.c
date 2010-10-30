/*
** rq_asset_equity.c
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
#include "rq_asset_equity.h"
#include "rq_asset_mgr.h"
#include "rq_date.h"
#include "rq_array.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_equity = "Equity";

static struct rq_asset_equity *
rq_asset_equity_alloc()
{
    struct rq_asset_equity *asset = 
        (struct rq_asset_equity *)RQ_CALLOC(1, sizeof(struct rq_asset_equity));

    asset->known_dividends = rq_array_alloc(free);

    return asset;
}

static void 
rq_asset_equity_free(struct rq_asset_equity *asset_equity)
{
    if (asset_equity->equity_code)
        RQ_FREE((char *)asset_equity->equity_code);
    if (asset_equity->issuer_id)
        RQ_FREE((char *)asset_equity->issuer_id);
    if (asset_equity->currency)
        RQ_FREE((char *)asset_equity->currency);

    rq_array_free(asset_equity->known_dividends);

    RQ_FREE(asset_equity);
}

void
rq_asset_equity_freefunc(void *asset_data)
{
	rq_asset_equity_free((struct rq_asset_equity *)asset_data);
}

static int
rq_asset_equity_code_cmp(const void *node_data, const void *equity_data)
{
    const struct rq_asset_equity *asset_equity = (const struct rq_asset_equity *)node_data;
    const char *equity = (const char *)equity_data;

    return strcmp(asset_equity->equity_code, equity);
}

RQ_EXPORT rq_asset_t
rq_asset_equity_build(
    const char *asset_id,
    const char *equity_code,
    const char *issuer_id,
    const char *currency
    )
{
    struct rq_asset_equity *asset_equity = rq_asset_equity_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_equity, 
        asset_id,
        asset_equity,
        rq_asset_equity_freefunc
        );

    asset_equity->equity_code = (const char *)RQ_STRDUP(equity_code);
    asset_equity->issuer_id = (const char *)RQ_STRDUP(issuer_id);
    asset_equity->currency = (const char *)RQ_STRDUP(currency);

    return asset;
}

RQ_EXPORT rq_asset_t
rq_asset_equity_find(const rq_asset_mgr_t asset_mgr, const char *equity_code)
{
    return rq_asset_mgr_find(
        asset_mgr, 
        rq_asset_type_id_equity, 
        rq_asset_equity_code_cmp, 
        (void *)equity_code
        );
}

RQ_EXPORT const char *
rq_asset_equity_get_equity_code(const rq_asset_t a)
{
    const struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    return asset_equity->equity_code;
}

RQ_EXPORT const char *
rq_asset_equity_get_issuer_id(const rq_asset_t a)
{
    const struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    return asset_equity->issuer_id;
}

RQ_EXPORT const char *
rq_asset_equity_get_currency(const rq_asset_t a)
{
    const struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    return asset_equity->currency;
}

RQ_EXPORT const struct rq_term *
rq_asset_equity_get_dividend_frequency(const rq_asset_t a)
{
    const struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    return &asset_equity->dividend_frequency;
}

RQ_EXPORT void
rq_asset_equity_set_dividend_frequency(rq_asset_t a, const struct rq_term *freq)
{
    struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    rq_term_copy(
        &asset_equity->dividend_frequency, 
        freq
        );
}

RQ_EXPORT rq_date
rq_asset_equity_get_last_dividend_payment_date(const rq_asset_t a)
{
    const struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    return asset_equity->last_dividend_payment_date;
}

RQ_EXPORT void
rq_asset_equity_set_last_dividend_payment_date(rq_asset_t a, rq_date date)
{
    struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    asset_equity->last_dividend_payment_date = date;
}

RQ_EXPORT void
rq_asset_equity_add_known_dividend(rq_asset_t a, rq_date date, double div_amount)
{
    struct rq_asset_equity *asset_equity = _rq_asset_get_asset_type_data(a);
    struct rq_equity_dividend *div = (struct rq_equity_dividend *)RQ_MALLOC(
        sizeof(struct rq_equity_dividend)
        );
    div->date = date;
    div->amount = div_amount;

    rq_array_push_back(asset_equity->known_dividends, div);
}

RQ_EXPORT short
rq_asset_is_equity(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_equity);
}
