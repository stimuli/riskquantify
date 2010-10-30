/*
** rq_asset_correlation.c
**
** Written by Hendra
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
#include "rq_asset_correlation.h"
#include "rq_asset_asset_correlation.h"
#include <stdlib.h>
#include <string.h>

#define asset_correlation_seperator "/"

char* rq_asset_correlation_build_assetKey(const char* asset1, const char* asset2)
{
    char* assetIds = (char*)RQ_MALLOC(strlen(asset1) 
        + strlen(asset2) + strlen(asset_correlation_seperator) + 1);
    strcpy(assetIds, asset1);
    strcat(assetIds, asset_correlation_seperator);
    strcat(assetIds, asset2);
    return assetIds;
}

RQ_EXPORT rq_asset_correlation_t 
rq_asset_correlation_build(
    const char* asset1,
    const char* asset2,
    double correlation
    )
{
    struct rq_asset_correlation* r = (struct rq_asset_correlation *)RQ_MALLOC(sizeof(struct rq_asset_correlation));

    r->key = rq_asset_correlation_build_assetKey(asset1, asset2);
    r->correlation = correlation;

    return r;
}

RQ_EXPORT void 
rq_asset_correlation_free(rq_asset_correlation_t r)
{
    RQ_FREE((char*)r->key);
    RQ_FREE(r);
}

RQ_EXPORT rq_asset_correlation_t 
rq_asset_correlation_clone(const rq_asset_correlation_t r)
{
    struct rq_asset_correlation *c = (struct rq_asset_correlation *)RQ_MALLOC(sizeof(struct rq_asset_correlation));

    c->key = RQ_STRDUP(r->key);
    c->correlation = r->correlation;

    return c;
}

RQ_EXPORT const char*
rq_asset_correlation_get_key(const rq_asset_correlation_t r)
{
    return r->key;
}

RQ_EXPORT double
rq_asset_correlation_get_correlation(const rq_asset_correlation_t r)
{
    return r->correlation;
}

RQ_EXPORT int
rq_asset_correlation_is_null(rq_asset_correlation_t obj)
{
    return (obj == NULL);
}
