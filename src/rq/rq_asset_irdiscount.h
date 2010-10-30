/**
 * \file rq_asset_irdiscount.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_irdiscount object encapsulates the properties
 * of an interest rate discounting instrument like a treasury bill.
 */
/*
** rq_asset_irdiscount.h
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
#ifndef rq_asset_irdiscount_h
#define rq_asset_irdiscount_h

#include "rq_config.h"
#include "rq_term.h"
#include "rq_enum.h"
#include "rq_asset.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_irdiscount {
    const char *ccy_code;
    struct rq_term start_tenor;
    enum rq_day_type start_tenor_type;
    struct rq_term tenor;
    long maturity_date;
    enum rq_day_count_convention day_count_convention;
    enum rq_date_roll_convention date_roll_convention;
	enum rq_zero_method zero_method;
	long zero_method_compound_frequency;
};

/**
 * The asset type ID for a Bank Bill asset
 */
extern const char * const rq_asset_type_id_irdiscount;

/**
 * build a new rq_asset_irdiscount structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_irdiscount_build(
    const char *asset_id,
    const char *ccy,
    const struct rq_term *start_tenor,
    enum rq_day_type start_tenor_type,
    const struct rq_term *tenor,
    long maturity_date,
    enum rq_day_count_convention day_count_convention,
    enum rq_date_roll_convention date_roll_convention,
	enum rq_zero_method zero_method,
	long zero_method_compound_frequency
    );


RQ_EXPORT const char *rq_asset_irdiscount_get_ccy_code(const rq_asset_t a);
RQ_EXPORT enum rq_day_count_convention rq_asset_irdiscount_get_day_count_convention(const rq_asset_t a);

/* return start tenor & day type */
RQ_EXPORT const struct rq_term *rq_asset_irdiscount_get_start_tenor(const rq_asset_t a);
RQ_EXPORT enum rq_day_type rq_asset_irdiscount_get_start_tenor_type(const rq_asset_t a);

RQ_EXPORT const struct rq_term *rq_asset_irdiscount_get_tenor(const rq_asset_t a);
RQ_EXPORT long rq_asset_irdiscount_get_maturity_date(const rq_asset_t a);
RQ_EXPORT enum rq_date_roll_convention rq_asset_irdiscount_get_date_roll_convention(const rq_asset_t asset);

RQ_EXPORT enum rq_zero_method rq_asset_irdiscount_get_zero_method(const rq_asset_t a);
RQ_EXPORT long rq_asset_irdiscount_get_zero_method_compounding_frequency(const rq_asset_t a);


/**
 * Return true if the asset is an interest rate discount instrument.
 */
RQ_EXPORT short rq_asset_is_irdiscount(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
