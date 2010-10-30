/*
** rq_asset_irvol.c
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
#include "rq_asset_irvol.h"
#include "rq_asset_mgr.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_irvol = "IrVol";

static struct rq_asset_irvol *
rq_asset_irvol_alloc()
{
    /* use RQ_CALLOC to zero out the memory */
    struct rq_asset_irvol *cp = 
        (struct rq_asset_irvol *)RQ_CALLOC(1, sizeof(struct rq_asset_irvol));

    return cp;
}

static void
rq_asset_irvol_free(struct rq_asset_irvol *asset_irvol)
{
    if (asset_irvol->underlying_asset_id)
       RQ_FREE((char *)asset_irvol->underlying_asset_id);

   RQ_FREE(asset_irvol);
}

void
rq_asset_irvol_freefunc(void *asset_data)
{
	rq_asset_irvol_free((struct rq_asset_irvol *)asset_data);
}

static int
rq_asset_irvol_cmp(const void *node_data, const void *underlying_asset_id)
{
    const struct rq_asset_irvol *irvol = (const struct rq_asset_irvol *)node_data;
    const char *asset_id = (const char *)underlying_asset_id;

    if (!strcmp(irvol->underlying_asset_id, asset_id))
        return 1;
    return 0;
}

RQ_EXPORT rq_asset_t
rq_asset_irvol_build(
    const char *asset_id,
    const char *underlying_asset_id,
	enum rq_rate_type quote_type,
	rq_date tenor_date,
	const rq_term_t underlying_tenor,
	rq_date underlying_date
    )
{
    struct rq_asset_irvol *asset_irvol = rq_asset_irvol_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_irvol, 
        asset_id,
        asset_irvol,
        rq_asset_irvol_freefunc
        );

    asset_irvol->underlying_asset_id = (const char *)RQ_STRDUP(underlying_asset_id);
	asset_irvol->quote_type = quote_type;
	asset_irvol->tenor_date = tenor_date;
	rq_term_copy( &(asset_irvol->underlying_tenor), underlying_tenor);
	asset_irvol->underlying_date = underlying_date;

    return asset;
}

RQ_EXPORT rq_asset_t
rq_asset_irvol_find(const rq_asset_mgr_t asset_mgr, const char *underlying_asset_id)
{
    return rq_asset_mgr_find(
        asset_mgr, 
        rq_asset_type_id_irvol, 
        rq_asset_irvol_cmp, 
        (void *)underlying_asset_id
        );
}


RQ_EXPORT const char *
rq_asset_irvol_get_underlying_asset_id(const rq_asset_t asset)
{
    struct rq_asset_irvol *asset_irvol = 
        (struct rq_asset_irvol *)_rq_asset_get_asset_type_data(asset);
    return asset_irvol->underlying_asset_id;
}

RQ_EXPORT enum rq_rate_type 
rq_asset_irvol_get_quote_type(const rq_asset_t asset)
{
    struct rq_asset_irvol *asset_irvol = 
        (struct rq_asset_irvol *)_rq_asset_get_asset_type_data(asset);
    return asset_irvol->quote_type;
}

RQ_EXPORT rq_date
rq_asset_irvol_get_tenor_date(const rq_asset_t asset)
{
    struct rq_asset_irvol *asset_irvol = 
        (struct rq_asset_irvol *)_rq_asset_get_asset_type_data(asset);
    return asset_irvol->tenor_date;
}

RQ_EXPORT const rq_term_t
rq_asset_irvol_get_underlying_tenor(const rq_asset_t asset)
{
    struct rq_asset_irvol *asset_irvol = 
        (struct rq_asset_irvol *)_rq_asset_get_asset_type_data(asset);
    return &(asset_irvol->underlying_tenor);
}

RQ_EXPORT rq_date
rq_asset_irvol_get_underlying_date(const rq_asset_t asset)
{
    struct rq_asset_irvol *asset_irvol = 
        (struct rq_asset_irvol *)_rq_asset_get_asset_type_data(asset);
    return asset_irvol->underlying_date;
}
