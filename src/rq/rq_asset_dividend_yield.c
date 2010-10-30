/*
** rq_asset_dividend_yield.c
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
#include "rq_asset_dividend_yield.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_dividend_yield = "DividendYield";

static struct rq_asset_dividend_yield *
rq_asset_dividend_yield_alloc()
{
    struct rq_asset_dividend_yield *asset = 
        (struct rq_asset_dividend_yield *)RQ_CALLOC(1, sizeof(struct rq_asset_dividend_yield));

    return asset;
}

static void 
rq_asset_dividend_yield_free(struct rq_asset_dividend_yield *asset_dividend_yield)
{
    if (asset_dividend_yield->equity_code)
       RQ_FREE((char *)asset_dividend_yield->equity_code);
	if (asset_dividend_yield->discrete_dividend_currency)
		RQ_FREE((char *)asset_dividend_yield->discrete_dividend_currency);
   RQ_FREE(asset_dividend_yield);
}

void
rq_asset_dividend_yield_freefunc(void *asset_data)
{
	rq_asset_dividend_yield_free((struct rq_asset_dividend_yield *)asset_data);
}

RQ_EXPORT rq_asset_t
rq_asset_dividend_yield_build(
    const char *asset_id,
    const char *equity_code,
	const char *discrete_dividend_currency
    )
{
    struct rq_asset_dividend_yield *asset_dividend_yield = rq_asset_dividend_yield_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_dividend_yield, 
        asset_id,
        asset_dividend_yield,
        rq_asset_dividend_yield_freefunc
        );

    asset_dividend_yield->equity_code = (const char *)RQ_STRDUP(equity_code);
	asset_dividend_yield->discrete_dividend_currency = (const char *)RQ_STRDUP(discrete_dividend_currency);

    return asset;
}

RQ_EXPORT const char *
rq_asset_dividend_yield_get_equity_code(const rq_asset_t a)
{
    struct rq_asset_dividend_yield *bb = (struct rq_asset_dividend_yield *)_rq_asset_get_asset_type_data(a);
    return bb->equity_code;
}

RQ_EXPORT const char *
rq_asset_dividend_yield_get_discrete_dividend_currency(const rq_asset_t a)
{
    struct rq_asset_dividend_yield *bb = (struct rq_asset_dividend_yield *)_rq_asset_get_asset_type_data(a);
    return bb->discrete_dividend_currency;
}

RQ_EXPORT short
rq_asset_is_dividend_yield(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_dividend_yield);
}

RQ_EXPORT rq_asset_t
rq_asset_dividend_yield_get_asset(const rq_asset_mgr_t asset_mgr, const char *asset_id)
{
	return rq_asset_mgr_get(
			asset_mgr,
			asset_id
			);
}
