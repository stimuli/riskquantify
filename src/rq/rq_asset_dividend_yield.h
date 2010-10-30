/**
 * \file rq_asset_dividend_yield.h
 * \author Todd Cooper
 *
 * \brief The rq_asset_dividend_yield object encapsulates the dividend 
 * payment properties of the of an equity as a continuously compounded yield. 
 */
/*
** rq_asset_dividend_yield.h
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
#ifndef rq__asset_dividend_yield_h
#define rq__asset_dividend_yield_h

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
struct rq_asset_dividend_yield {
    const char *equity_code;
	const char *discrete_dividend_currency;
};

/**
 * The asset type ID for a Dividend Yield asset
 */
extern const char * const rq_asset_type_id_dividend_yield;

/**
 * build a new rq_asset_dividend_yield structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_dividend_yield_build(
    const char *asset_id,
    const char *equity_code,
	const char *discrete_dividend_currency
    );


/**
 * Find a particular asset_equity by equity code in the asset manager.
 */
/*
RQ_EXPORT rq_asset_t rq_asset_equity_find(const rq_asset_mgr_t asset_mgr, const char *equity_code);
*/

/**
 * Get the code for the equity asset.
 */
RQ_EXPORT const char *rq_asset_dividend_yield_get_equity_code(const rq_asset_t a);

/**
 * Get the currency code for this dividend payment
 */
RQ_EXPORT const char *rq_asset_dividend_yield_get_discrete_dividend_currency(const rq_asset_t a);

/**
 * Return true if the asset is an overnight interest rate.
 */
RQ_EXPORT short rq_asset_is_dividend_yield(rq_asset_t asset);

RQ_EXPORT rq_asset_t rq_asset_dividend_yield_get_asset(const rq_asset_mgr_t asset_mgr, const char *asset_id);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
