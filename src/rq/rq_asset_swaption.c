/*
** rq_asset_swaption.c
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
#include "rq_asset_swaption.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_swaption = "swaption";


void
rq_asset_swaption_freefunc(void *d)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)d;
    if (c->option_tenor) rq_term_free(c->option_tenor);
    if (c->swap_tenor) rq_term_free(c->swap_tenor);
    if (c->frequency) rq_term_free(c->frequency);
    if (c->calendar) free((char *)c->calendar);
	if (c->benchmark) free((char *)c->benchmark);
	if (c->currency) free((char *)c->currency);

    free(c);
}

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
    )
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)calloc(
        1, 
        sizeof(struct rq_asset_swaption)
        );
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_swaption, 
        asset_id,
        c,
        rq_asset_swaption_freefunc
        );

    if (option_tenor) c->option_tenor = rq_term_clone(option_tenor);
    if (swap_tenor) c->swap_tenor = rq_term_clone(swap_tenor);
    if (frequency) c->frequency = rq_term_clone(frequency);
    if (calendar) c->calendar = strdup(calendar);
	if (benchmark) c->benchmark = strdup(benchmark);
	if (currency) c->currency = strdup(currency);
   
	c->day_count = day_count;
    c->date_roll_convention = date_roll_convention;
    c->roll_convention = roll_convention;
	c->coupon = coupon;
	c->notional = notional;
	c->spread = spread;
	c->payer_receiver = payer_receiver;

    return asset;
}

RQ_EXPORT short
rq_asset_is_swaption(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_swaption);
}

RQ_EXPORT rq_term_t
rq_asset_swaption_get_option_tenor(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->option_tenor;
}

RQ_EXPORT rq_term_t
rq_asset_swaption_get_swap_tenor(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->swap_tenor;
}

RQ_EXPORT rq_term_t
rq_asset_swaption_get_frequency(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->frequency;
}

RQ_EXPORT const char *
rq_asset_swaption_get_calendar(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->calendar;
}

RQ_EXPORT enum rq_day_count_convention 
rq_asset_swaption_get_day_count_convention(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->day_count;
}

RQ_EXPORT enum rq_date_roll_convention 
rq_asset_swaption_get_date_roll_convention(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->date_roll_convention;
}

RQ_EXPORT enum rq_roll_convention
rq_asset_swaption_get_roll_convention(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->roll_convention;
}

RQ_EXPORT enum rq_payer_receiver
rq_asset_swaption_get_payer_receiver(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
	return c->payer_receiver;
}

RQ_EXPORT double
rq_asset_swaption_get_coupon(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->coupon;
}

RQ_EXPORT const char*
rq_asset_swaption_get_benchmark(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
	return c->benchmark;
}

RQ_EXPORT const char *
rq_asset_swaption_get_currency(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->currency;
}

RQ_EXPORT double
rq_asset_swaption_get_notional(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->notional;
}

RQ_EXPORT double
rq_asset_swaption_get_spread(rq_asset_t asset)
{
    struct rq_asset_swaption *c = (struct rq_asset_swaption *)_rq_asset_get_asset_type_data(asset);
    return c->spread;
}
