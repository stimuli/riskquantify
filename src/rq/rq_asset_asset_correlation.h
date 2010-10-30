/**
 * \file rq_asset_asset_correlation.h
 * \author Hendra
 *
 * \brief The rq_asset_asset_correlation_ object encapsulates the assets correlation.
 */
/*
** rq_asset_asset_correlation.h
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
#ifndef rq_asset_asset_correlation_h
#define rq_asset_asset_correlation_h

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
struct rq_asset_asset_correlation {
    const char* assetId;
    const char* asset1;
    const char* asset2;
};

/**
 * The asset type ID for a Dividend Yield asset
 */
extern const char* const rq_asset_type_id_asset_correlation;

/**
 * build a new rq_asset_asset_correlation structure.
 *
 * @return the allocated structure. The caller is responsible for freeing this object.
 */
RQ_EXPORT rq_asset_t
rq_asset_asset_correlation_build(
    const char* assetId,
    const char* asset1,
    const char* asset2
    );

/**
 * Get the index name associated with this asset correlation.
 */
RQ_EXPORT const char* rq_asset_asset_correlation_get_assetId(const rq_asset_t asset);

/**
 * Get asset1 associated with this asset correlation.
 */
RQ_EXPORT const char* rq_asset_asset_correlation_get_asset1(const rq_asset_t asset);

/**
 * Get asset2 associated with this asset correlation.
 */
RQ_EXPORT const char* rq_asset_asset_correlation_get_asset2(const rq_asset_t asset);

/**
 * Find if there is a correlation between asset1 and asset2.
 */
RQ_EXPORT rq_asset_t rq_asset_asset_correlation_find(const rq_asset_mgr_t asset_mgr, const char* asset1, const char* asset2);

/**
 * Return true if the asset is an asset correlation structure.
 */
RQ_EXPORT short rq_asset_is_asset_correlation(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif
#endif
