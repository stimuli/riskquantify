/**
 * \file rq_asset_credit_spread.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_credit_spread is an asset that encapsulates a
 * spread over an index like LIBOR.
 */
/*
** rq_asset_credit_spread.h
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
#ifndef rq_asset_credit_spread_h
#define rq_asset_credit_spread_h

#include "rq_config.h"
#include "rq_enum.h"
#include "rq_asset.h"
#include "rq_asset_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_credit_spread {
    const char *industry_code;
    const char *credit_rating;
};

/**
 * The asset type ID for a credit spread asset
 */
extern const char * const rq_asset_type_id_credit_spread;

/**
 * build a new rq_asset_credit_spread structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_credit_spread_build(
    const char *asset_id,
    const char *industry_code,
    const char *credit_rating
    );

RQ_EXPORT rq_asset_t rq_asset_credit_spread_find(const rq_asset_mgr_t asset_mgr, const char *industry_code, const char *credit_rating);

RQ_EXPORT const char *rq_asset_credit_spread_get_industry_code(const rq_asset_t asset);

RQ_EXPORT const char *rq_asset_credit_spread_get_credit_rating(const rq_asset_t asset);

/**
 * Return true if the asset is a credit spread.
 */
RQ_EXPORT short rq_asset_is_credit_spread(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
