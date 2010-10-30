/**
 * \file rq_asset_equity.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_equity object encapsulates the properties of
 * equity assets.
 */
/*
** rq_asset_equity.h
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
#ifndef rq_asset_equity_h
#define rq_asset_equity_h

#include "rq_asset.h"
#include "rq_asset_mgr.h"
#include "rq_date.h"
#include "rq_array.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_equity_dividend {
    rq_date date;
    double amount;
};

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_equity {
    const char *equity_code;
    const char *issuer_id;
    const char *currency;
    struct rq_term dividend_frequency;
    rq_date last_dividend_payment_date;
    rq_array_t known_dividends;
};

/**
 * The asset type ID for a Equity type asset
 */
extern const char * const rq_asset_type_id_equity;

/**
 * build a new rq_asset_equity structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_equity_build(
    const char *asset_id,
    const char *equity_code, /**< The code for this equity */
    const char *issuer_id, /**< The issuing company */
    const char *currency /**< The currency the equity is traded in */
    );

/**
 * Find a particular asset_equity by equity code in the asset manager.
 */
RQ_EXPORT rq_asset_t rq_asset_equity_find(const rq_asset_mgr_t asset_mgr, const char *equity_code);

/**
 * Get the code for the equity asset.
 */
RQ_EXPORT const char *rq_asset_equity_get_equity_code(const rq_asset_t a);

/**
 * Get the issuer ID for the equity
 */
RQ_EXPORT const char *rq_asset_equity_get_issuer_id(const rq_asset_t a);

/**
 * Get the currency associated with an equity
 */
RQ_EXPORT const char *rq_asset_equity_get_currency(const rq_asset_t a);

/**
 * Get the frequency of the dividend payments
 */
RQ_EXPORT const struct rq_term *rq_asset_equity_get_dividend_frequency(const rq_asset_t a);

/**
 * Set the frequency of the dividend payments
 */
RQ_EXPORT void rq_asset_equity_set_dividend_frequency(rq_asset_t a, const struct rq_term *freq);

/**
 * Get the last dividend payment date. Used if there are no known dividends, but a dividend yield.
 */
RQ_EXPORT rq_date rq_asset_equity_get_last_dividend_payment_date(const rq_asset_t a);

/**
 * Set the last dividend payment date. Used if there are no known dividends
 * but a dividend yield
 */
RQ_EXPORT void rq_asset_equity_set_last_dividend_payment_date(rq_asset_t a, rq_date date);

/**
 * Add a known dividend amount to the list
 */
RQ_EXPORT void rq_asset_equity_add_known_dividend(rq_asset_t a, rq_date date, double div_amount);

/**
 * Return true if the asset is an equity.
 */
RQ_EXPORT short rq_asset_is_equity(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
