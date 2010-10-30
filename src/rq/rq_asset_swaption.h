/**
 * @file
 *
 * swaption-based asset
 */
/*
** rq_asset_swaption.h
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
#ifndef rq_asset_swaption_h
#define rq_asset_swaption_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_asset.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private asset_swaption structure that gets added to the
 * rq_asset structure.
 */
struct rq_asset_swaption {
    const char *id;
    rq_term_t option_tenor;
    rq_term_t swap_tenor;
    rq_term_t frequency;
    const char *calendar;
    enum rq_day_count_convention day_count;
    enum rq_date_roll_convention date_roll_convention;
    enum rq_roll_convention roll_convention;
	enum rq_payer_receiver payer_receiver;
	double coupon;
	const char* benchmark;
	const char* currency;
	double notional;
	double spread;
};

extern const char * const rq_asset_type_id_swaption;

/**
 * Allocate a new asset_swaption
 */
RQ_EXPORT rq_asset_t
rq_asset_swaption_build(
	const char* asset_id,
    rq_term_t option_tenor,
    rq_term_t swap_tenor,
    rq_term_t frequency,
    const char *calendar,
    enum rq_day_count_convention day_count,
    enum rq_date_roll_convention date_roll_convention,
    enum rq_roll_convention roll_convention,
	enum rq_payer_receiver payer_receiver,
	double coupon,
	const char* benchmark,
	const char* currency,
	double notional,
	double spread
    );

RQ_EXPORT short
rq_asset_is_swaption(rq_asset_t asset);

RQ_EXPORT rq_term_t
rq_asset_swaption_get_option_tenor(rq_asset_t asset);

RQ_EXPORT rq_term_t
rq_asset_swaption_get_swap_tenor(rq_asset_t asset);

RQ_EXPORT rq_term_t
rq_asset_swaption_get_frequency(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_swaption_get_calendar(rq_asset_t asset);

RQ_EXPORT enum rq_day_count_convention 
rq_asset_swaption_get_day_count_convention(rq_asset_t asset);

RQ_EXPORT enum rq_date_roll_convention
rq_asset_swaption_get_date_roll_convention(rq_asset_t asset);

RQ_EXPORT enum rq_roll_convention 
rq_asset_swaption_get_roll_convention(rq_asset_t asset);

RQ_EXPORT enum rq_payer_receiver
rq_asset_swaption_get_payer_receiver(rq_asset_t asset);

RQ_EXPORT double 
rq_asset_swaption_get_coupon(rq_asset_t asset);

RQ_EXPORT const char* 
rq_asset_swaption_get_benchmark(rq_asset_t asset);

RQ_EXPORT const char* 
rq_asset_swaption_get_currency(rq_asset_t asset);

RQ_EXPORT double
rq_asset_swaption_get_notional(rq_asset_t asset);

RQ_EXPORT double
rq_asset_swaption_get_spread(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
