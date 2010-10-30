/**
 * \file rq_asset_irswap.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_irswap object encapsulates the properties of an
 * interest rate swap.
 */
/*
** rq_asset_irswap.h
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
#ifndef rq_asset_irswap_h
#define rq_asset_irswap_h

#include "rq_config.h"
#include "rq_enum.h"
#include "rq_term.h"
#include "rq_asset.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_irswap {
    const char *ccy_code;
	const char *additional_calendar; /* Additional to the ccy, for payments and accruals date. */
    struct rq_term start_tenor;
    enum rq_day_type start_tenor_type;
    struct rq_term tenor;
    struct rq_term frequency;
    enum rq_day_count_convention day_count_convention;
    enum rq_date_roll_convention date_roll_convention;
};

extern const char * const rq_asset_type_id_irswap;


/**
 * build a new rq_asset_irswap structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t 
rq_asset_irswap_build(
    const char *asset_id,
    const char *ccy_code,
	const char *additional_calendar,
	const struct rq_term *start_tenor,
    enum rq_day_type start_tenor_type,
    const struct rq_term *tenor,
    const struct rq_term *frequency,
    enum rq_day_count_convention day_count_convention,
    enum rq_date_roll_convention date_roll_convention    /* business day convention */
    );

/**
 * Get the currency code associated with this swap contract.
 */
RQ_EXPORT const char *rq_asset_irswap_get_ccy_code(const rq_asset_t asset);

/**
 * Get a possible additional calendar for this swap fixed leg.
 */
RQ_EXPORT const char *rq_asset_irswap_get_additional_calendar(const rq_asset_t asset);

/**
 * Get the start tenor of this swap contract
 */
RQ_EXPORT const struct rq_term *rq_asset_irswap_get_start_tenor(const rq_asset_t asset);

/**
 * Get the start tenor day type of this swap contract
 */
RQ_EXPORT enum rq_day_type rq_asset_irswap_get_start_tenor_type(const rq_asset_t asset);

/**
 * Get the tenor of this swap contract
 */
RQ_EXPORT const struct rq_term *rq_asset_irswap_get_tenor(const rq_asset_t asset);

/**
 * Get the payment frequency associated with this swap contract
 */
RQ_EXPORT const struct rq_term *rq_asset_irswap_get_frequency(const rq_asset_t asset);

/**
 * Get the day count convention of this swap contract
 */
RQ_EXPORT enum rq_day_count_convention rq_asset_irswap_get_day_count_convention(const rq_asset_t asset);

/**
 * Get the date roll convention associated with this swap asset
 */
RQ_EXPORT enum rq_date_roll_convention rq_asset_irswap_get_date_roll_convention(const rq_asset_t asset);

/**
 * Return true if the asset is an interest rate swap.
 */
RQ_EXPORT short rq_asset_is_irswap(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
