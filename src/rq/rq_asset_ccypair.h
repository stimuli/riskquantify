/**
 * \file rq_asset_ccypair.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_ccypair object encapsulates the conventions
 * that "currency pairs" have in foreign exchange trades.
 */
/*
** rq_asset_ccypair.h
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
#ifndef rq_asset_ccypair_h
#define rq_asset_ccypair_h

#include "rq_config.h"
#include "rq_enum.h"
#include "rq_asset.h"
#include "rq_asset_mgr.h"
#include "rq_calendar_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_ccypair {
    const char *ccys;
    const char *ccy_code_1;
    const char *ccy_code_2;
    enum rq_ccypair_quote_convention quote_convention;
    unsigned short days_per_year;
    unsigned short spot_days;
};

/**
 * The asset type ID for a currency pair asset
 */
extern const char * const rq_asset_type_id_ccypair;

/**
 * build a new rq_asset_ccypair structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_ccypair_build(
    const char *asset_id,
    const char *ccy_code_1,
    const char *ccy_code_2,
    enum rq_ccypair_quote_convention quote_convention,
    unsigned int days_per_year,
    unsigned int spot_days
    );

RQ_EXPORT int rq_asset_ccypair_cmp(void *data, void *ccypair);

/**
 * finds a currency pair asset in the asset manager from
 * two currency codes of the currency pair.
 */
RQ_EXPORT rq_asset_t rq_asset_ccypair_find(const rq_asset_mgr_t asset_mgr, const char *ccy_code_1, const char *ccy_code_2);

/**
 * Gets the first currency code out of the currency pair
 */
RQ_EXPORT const char *rq_asset_ccypair_get_ccy_code_1(const rq_asset_t asset);

/**
 * Gets the second currency code out of the currency pair
 */
RQ_EXPORT const char *rq_asset_ccypair_get_ccy_code_2(const rq_asset_t asset);

/**
 * Gets the quote convention from the currency pair
 */
RQ_EXPORT enum rq_ccypair_quote_convention rq_asset_ccypair_get_quote_convention(const rq_asset_t asset);

/**
 * Gets the days per year convention from the currency pair
 */
RQ_EXPORT unsigned short rq_asset_ccypair_get_days_per_year(const rq_asset_t asset);

/**
 * Return true if the asset is a currency pair.
 */
RQ_EXPORT short rq_asset_is_ccypair(rq_asset_t asset);

/**
 * Get the default spot days for this currency pair.
 */
RQ_EXPORT unsigned short rq_asset_ccypair_get_spot_days(const rq_asset_t asset);

/**
 * Return true if the asset is opposite to expected convention (term per base / 2per1).
 */
RQ_EXPORT short
rq_asset_ccypair_is_inverted_base(const rq_asset_t asset, const char *base_ccy);

/**
 * Return true if the asset is opposite to expected convention (term per base / 2per1).
 */
RQ_EXPORT short
rq_asset_ccypair_is_inverted_term(const rq_asset_t asset, const char *term_ccy);

/**
 * Return ccy pair with expected base/term ccys.
 */
RQ_EXPORT const rq_asset_t
rq_ccy_pair_get_ccy_pair(
    const rq_asset_mgr_t asset_mgr,
    const char *ccy1,
    const char *ccy2,
    const char **base_foreign_ccy,
    const char **term_domestic_ccy);

/**
 * Get the spot date for this currency pair.
 */
RQ_EXPORT rq_date
rq_asset_ccypair_get_spot_date(
    const rq_calendar_mgr_t cal_mgr,
    const rq_asset_mgr_t asset_mgr,
    const char *ccy1,
    const char *ccy2,
    rq_date from_date
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
