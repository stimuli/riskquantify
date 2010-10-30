/**
 * \file rq_asset_cds_spread.h
 * \author Todd Cooper
 *
 * \brief The rq_asset_cds_spread object encapsulates the spread paid as a
 *  fee for the CDS protection.
 */
/*
** rq_asset_cds_spread.h
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
#ifndef rq__asset_cds_spread_h
#define rq__asset_cds_spread_h

/*
#include "rq_config.h"
#include "rq_term.h"
#include "rq_enum.h"
*/
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
struct rq_asset_cds_spread {
    const char *quote_type;
    struct rq_term frequency;
};

/**
 * The asset type ID for a Dividend Yield asset
 */
extern const char * const rq_asset_type_id_cds_spread;

/**
 * build a new rq_asset_cds_spread structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_cds_spread_build(
    const char *asset_id,
    const char *quote_type,
    const struct rq_term *frequency
    );

/**
 * Get the quote type associated with this CDS spread.
 */
RQ_EXPORT const char *rq_asset_cds_spread_get_quote_type(const rq_asset_t asset);

/**
 * Get the fee frequency associated with this CDS contract.
 */
RQ_EXPORT const struct rq_term *rq_asset_cds_spread_get_frequency(const rq_asset_t asset);

/**
 * Return true if the asset is an overnight interest rate.
 */
RQ_EXPORT short rq_asset_is_cds_spread(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
