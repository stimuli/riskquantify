/**
 * \file rq_asset_irfuture.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_irfuture object encapsulates the properties of
 * an exchange traded interest rate future
 */
/*
** rq_asset_irfuture.h
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
#ifndef rq_asset_irfuture_h
#define rq_asset_irfuture_h

#include "rq_config.h"
#include "rq_enum.h"
#include "rq_term.h"
#include "rq_date.h"
#include "rq_asset.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_irfuture {
    const char *ccy_code;
    rq_date last_trading_date;
    rq_date settlement_date;
    struct rq_term tenor;
    enum rq_day_count_convention day_count_convention;
    enum rq_ir_future_quote_convention quote_convention;
};

extern const char * const rq_asset_type_id_irfuture;

/**
 * build a new rq_asset_irfuture structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_irfuture_build(
    const char *asset_id,
    const char *ccy,
    rq_date lastTradingDate,
    rq_date settlementDate,
    const struct rq_term *tenor,
    enum rq_day_count_convention day_count_convention,
    enum rq_ir_future_quote_convention quote_convention
    );

RQ_EXPORT const char *rq_asset_irfuture_get_ccy_code(const rq_asset_t asset);
RQ_EXPORT rq_date rq_asset_irfuture_get_last_trading_date(const rq_asset_t asset);
RQ_EXPORT rq_date rq_asset_irfuture_get_settlement_date(const rq_asset_t asset);
RQ_EXPORT rq_date rq_asset_irfuture_get_maturity_date(const rq_asset_t asset);
RQ_EXPORT const struct rq_term *rq_asset_irfuture_get_tenor(const rq_asset_t asset);
RQ_EXPORT enum rq_day_count_convention rq_asset_irfuture_get_day_count_convention(const rq_asset_t asset);
RQ_EXPORT enum rq_ir_future_quote_convention rq_asset_irfuture_get_quote_convention(const rq_asset_t asset);

/**
 * Return true if the asset is an interest rate future.
 */
RQ_EXPORT short rq_asset_is_irfuture(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
