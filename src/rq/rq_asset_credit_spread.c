/*
** rq_asset_credit_spread.c
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
#include "rq_asset_credit_spread.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_credit_spread = "CreditSpread";


static struct rq_asset_credit_spread *
rq_asset_credit_spread_alloc()
{
    struct rq_asset_credit_spread *cp = 
        (struct rq_asset_credit_spread *)RQ_MALLOC(sizeof(struct rq_asset_credit_spread));

    cp->industry_code = NULL;
    cp->credit_rating = NULL;

    return cp;
}

static void
rq_asset_credit_spread_free(struct rq_asset_credit_spread *asset_credit_spread)
{
    if (asset_credit_spread->industry_code)
       RQ_FREE((char *)asset_credit_spread->industry_code);
    if (asset_credit_spread->credit_rating)
       RQ_FREE((char *)asset_credit_spread->credit_rating);
   RQ_FREE(asset_credit_spread);
}

RQ_EXPORT void
rq_asset_credit_spread_freefunc(void *asset_data)
{
	rq_asset_credit_spread_free((struct rq_asset_credit_spread *)asset_data);
}

RQ_EXPORT rq_asset_t
rq_asset_credit_spread_build(
    const char *asset_id,
    const char *industry_code,
    const char *credit_rating
    )
{
    struct rq_asset_credit_spread *asset_credit_spread = rq_asset_credit_spread_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_credit_spread, 
        asset_id,
        asset_credit_spread,
        rq_asset_credit_spread_freefunc
        );

    asset_credit_spread->industry_code = (const char *)RQ_STRDUP(industry_code);
    asset_credit_spread->credit_rating = (const char *)RQ_STRDUP(credit_rating);

    return asset;
}

static int
rq_asset_credit_spread_cmp(const void *node_data, const void *cr_data)
{
    const struct rq_asset_credit_spread *credit_spread = (const struct rq_asset_credit_spread *)node_data;
    const char **cr = (const char **)cr_data;

    if (!strcmp(credit_spread->industry_code, cr[0]) && !strcmp(credit_spread->credit_rating, cr[1]))
        return 1;
    return 0;
}

RQ_EXPORT rq_asset_t
rq_asset_credit_spread_find(const rq_asset_mgr_t asset_mgr, const char *industry_code, const char *credit_rating)
{
    const char *cr[2];

    cr[0] = industry_code;
    cr[1] = credit_rating;

    return rq_asset_mgr_find(
        asset_mgr, 
        rq_asset_type_id_credit_spread, 
        rq_asset_credit_spread_cmp, 
        (void *)cr
        );
}

RQ_EXPORT const char *
rq_asset_credit_spread_get_industry_code(const rq_asset_t asset)
{
    struct rq_asset_credit_spread *asset_credit_spread = 
        (struct rq_asset_credit_spread *)_rq_asset_get_asset_type_data(asset);
    return asset_credit_spread->industry_code;
}

RQ_EXPORT const char *
rq_asset_credit_spread_get_credit_rating(const rq_asset_t asset)
{
    struct rq_asset_credit_spread *asset_credit_spread = 
        (struct rq_asset_credit_spread *)_rq_asset_get_asset_type_data(asset);
    return asset_credit_spread->credit_rating;
}

RQ_EXPORT short
rq_asset_is_credit_spread(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_credit_spread);
}
