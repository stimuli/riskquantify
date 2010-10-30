/**
 * \file rq_asset_ccy.h
 * \author Brett Hutley
 *
 * \brief rq_asset_ccy objects are assets that embody the
 * characteristics of a currency.
 */
/*
** rq_asset_ccy.h
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
#ifndef rq_asset_ccy_h
#define rq_asset_ccy_h

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
struct rq_asset_ccy {
    const char *ccy_code;
    unsigned short days_per_year;
    unsigned short spot_days;
};


/**
 * The asset type ID for a Currency type asset
 */
extern const char * const rq_asset_type_id_ccy;

/**
 * build a new rq_asset_ccy structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_ccy_build(
    const char *asset_id,
    const char *ccy_code, /**< The currency code for this currency */
    unsigned short days_per_year,
    unsigned short spot_days
    );

/**
 * Find a particular asset_ccy by currency code in the asset manager.
 */
RQ_EXPORT rq_asset_t rq_asset_ccy_find(const rq_asset_mgr_t asset_mgr, const char *ccy_code);

/**
 * Get the currency code for the currency asset.
 */
RQ_EXPORT const char *rq_asset_ccy_get_ccy_code(const rq_asset_t a);

/**
 * Get the default days per year for assets in this currency.
 */
RQ_EXPORT unsigned short rq_asset_ccy_get_days_per_year(const rq_asset_t a);

/**
 * Return true if the asset is a currency.
 */
RQ_EXPORT short rq_asset_is_ccy(rq_asset_t asset);

/**
 * Get the default spot days for this currency.
 */
RQ_EXPORT unsigned short rq_asset_ccy_get_spot_days(const rq_asset_t a);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
