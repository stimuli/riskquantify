/*
** rq_market.c
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
#include "rq_market.h"
#include "rq_defs.h"
#include <stdlib.h>


RQ_EXPORT rq_market_t 
rq_market_alloc(rq_date market_date)
{
    struct rq_market *m = (struct rq_market *)RQ_MALLOC(sizeof(struct rq_market));

    m->market_date = market_date;

    m->rate_mgr = rq_rate_mgr_alloc();
    m->yield_curve_mgr = rq_yield_curve_mgr_alloc();
    m->forward_curve_mgr = rq_forward_curve_mgr_alloc();
    m->future_curve_mgr = rq_future_curve_mgr_alloc();
    m->vol_surface_mgr = rq_vol_surface_mgr_alloc();
    m->ir_vol_surface_mgr = rq_ir_vol_surface_mgr_alloc();
    m->exchange_rate_mgr = rq_exchange_rate_mgr_alloc();
    m->asset_correlation_mgr = rq_asset_correlation_mgr_alloc();
    m->spot_price_mgr = rq_spot_price_mgr_alloc();
    m->equity_curve_mgr = rq_equity_curve_mgr_alloc();
    m->spread_curve_mgr = rq_spread_curve_mgr_alloc();
	m->cds_curve_mgr = rq_cds_curve_mgr_alloc();
    m->external_termstruct_mgr = rq_external_termstruct_mgr_alloc();

    return m;
}

RQ_EXPORT rq_market_t 
rq_market_clone(rq_market_t mkt)
{
    struct rq_market *m = (struct rq_market *)RQ_MALLOC(sizeof(struct rq_market));

    m->market_date = mkt->market_date;
    m->rate_mgr = rq_rate_mgr_clone(mkt->rate_mgr);
    m->yield_curve_mgr = rq_yield_curve_mgr_clone(mkt->yield_curve_mgr);
    m->forward_curve_mgr = rq_forward_curve_mgr_clone(mkt->forward_curve_mgr);
    m->future_curve_mgr = rq_future_curve_mgr_clone(mkt->future_curve_mgr);
    m->vol_surface_mgr = rq_vol_surface_mgr_clone(mkt->vol_surface_mgr);
    m->ir_vol_surface_mgr = rq_ir_vol_surface_mgr_clone(mkt->ir_vol_surface_mgr);
    m->exchange_rate_mgr = rq_exchange_rate_mgr_clone(mkt->exchange_rate_mgr);
    m->asset_correlation_mgr = rq_asset_correlation_mgr_clone(mkt->asset_correlation_mgr);
    m->spot_price_mgr = rq_spot_price_mgr_clone(mkt->spot_price_mgr);
    m->equity_curve_mgr = rq_equity_curve_mgr_clone(mkt->equity_curve_mgr);
    m->spread_curve_mgr = rq_spread_curve_mgr_clone(mkt->spread_curve_mgr);
    m->cds_curve_mgr = rq_cds_curve_mgr_clone(mkt->cds_curve_mgr);
    m->external_termstruct_mgr = rq_external_termstruct_mgr_clone(mkt->external_termstruct_mgr);

    return m;
}

RQ_EXPORT void 
rq_market_free(rq_market_t market)
{
    if (market->rate_mgr)
        rq_rate_mgr_free(market->rate_mgr);
    if (market->yield_curve_mgr)
        rq_yield_curve_mgr_free(market->yield_curve_mgr);
    if (market->forward_curve_mgr)
        rq_forward_curve_mgr_free(market->forward_curve_mgr);
    if (market->future_curve_mgr)
        rq_future_curve_mgr_free(market->future_curve_mgr);
    if (market->vol_surface_mgr)
        rq_vol_surface_mgr_free(market->vol_surface_mgr);
    if (market->ir_vol_surface_mgr)
        rq_ir_vol_surface_mgr_free(market->ir_vol_surface_mgr);
    if (market->exchange_rate_mgr)
        rq_exchange_rate_mgr_free(market->exchange_rate_mgr);
    if (market->asset_correlation_mgr)
        rq_asset_correlation_mgr_free(market->asset_correlation_mgr);
    if (market->spot_price_mgr)
        rq_spot_price_mgr_free(market->spot_price_mgr);
    if (market->equity_curve_mgr)
        rq_equity_curve_mgr_free(market->equity_curve_mgr);
    if (market->spread_curve_mgr)
        rq_spread_curve_mgr_free(market->spread_curve_mgr);
    if (market->cds_curve_mgr)
        rq_cds_curve_mgr_free(market->cds_curve_mgr);
    if (market->external_termstruct_mgr)
        rq_external_termstruct_mgr_free(market->external_termstruct_mgr);

    RQ_FREE(market);
}

RQ_EXPORT rq_date 
rq_market_get_market_date(const rq_market_t market)
{
    return market->market_date;
}

RQ_EXPORT void 
rq_market_set_market_date(rq_market_t market, rq_date market_date)
{
    market->market_date = market_date;
}

RQ_EXPORT rq_rate_mgr_t 
rq_market_get_rate_mgr(const rq_market_t market)
{
    return market->rate_mgr;
}

RQ_EXPORT rq_yield_curve_mgr_t
rq_market_get_yield_curve_mgr(const rq_market_t market)
{
    return market->yield_curve_mgr;
}

RQ_EXPORT rq_forward_curve_mgr_t 
rq_market_get_forward_curve_mgr(const rq_market_t market)
{
    return market->forward_curve_mgr;
}

RQ_EXPORT rq_future_curve_mgr_t 
rq_market_get_future_curve_mgr(const rq_market_t market)
{
    return market->future_curve_mgr;
}

RQ_EXPORT rq_vol_surface_mgr_t 
rq_market_get_vol_surface_mgr(const rq_market_t market)
{
    return market->vol_surface_mgr;
}

RQ_EXPORT rq_ir_vol_surface_mgr_t 
rq_market_get_ir_vol_surface_mgr(const rq_market_t market)
{
    return market->ir_vol_surface_mgr;
}

RQ_EXPORT rq_exchange_rate_mgr_t 
rq_market_get_exchange_rate_mgr(const rq_market_t market)
{
    return market->exchange_rate_mgr;
}

RQ_EXPORT rq_asset_correlation_mgr_t 
rq_market_get_asset_correlation_mgr(const rq_market_t market)
{
    return market->asset_correlation_mgr;
}

RQ_EXPORT rq_spot_price_mgr_t 
rq_market_get_spot_price_mgr(const rq_market_t market)
{
    return market->spot_price_mgr;
}

RQ_EXPORT rq_equity_curve_mgr_t
rq_market_get_equity_curve_mgr(const rq_market_t market)
{
    return market->equity_curve_mgr;
}

RQ_EXPORT rq_spread_curve_mgr_t
rq_market_get_spread_curve_mgr(const rq_market_t market)
{
    return market->spread_curve_mgr;
}

RQ_EXPORT rq_cds_curve_mgr_t
rq_market_get_cds_curve_mgr(const rq_market_t market)
{
    return market->cds_curve_mgr;
}

RQ_EXPORT rq_external_termstruct_mgr_t 
rq_market_get_external_termstruct_mgr(const rq_market_t market)
{
    return market->external_termstruct_mgr;
}

RQ_EXPORT void *
rq_market_get_termstruct(const rq_market_t market, enum rq_termstruct_type termstruct_type, const char *termstruct_id)
{
    void *ts = NULL;

    /* replace this switch statement with a proper adapter manager */
    switch (termstruct_type)
    {
        case RQ_TERMSTRUCT_TYPE_YIELD_CURVE:
        {
            rq_yield_curve_t yc = rq_yield_curve_mgr_get(
                market->yield_curve_mgr,
                termstruct_id
                );
            ts = yc;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_FORWARD_CURVE:
        {
            rq_forward_curve_t fc = rq_forward_curve_mgr_get(
                market->forward_curve_mgr,
                termstruct_id
                );
            ts = fc;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_FUTURE_CURVE:
        {
            rq_future_curve_t fc = rq_future_curve_mgr_get(
                market->future_curve_mgr,
                termstruct_id
                );
            ts = fc;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_VOL_SURFACE:
        {
            rq_vol_surface_t vs = rq_vol_surface_mgr_get(
                market->vol_surface_mgr,
                termstruct_id
                );
            ts = vs;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_IR_VOL_SURFACE:
        {
            rq_ir_vol_surface_t is = rq_ir_vol_surface_mgr_get(
                market->ir_vol_surface_mgr,
                termstruct_id
                );
            ts = is;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_SPREAD_CURVE:
        {
            rq_spread_curve_t sc = rq_spread_curve_mgr_get(
                market->spread_curve_mgr,
                termstruct_id
                );
            ts = sc;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_CDS_SPREAD_CURVE:
        {
            rq_cds_curve_t sc = rq_cds_curve_mgr_get(
                market->cds_curve_mgr,
                termstruct_id
                );
            ts = sc;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_EQUITY_CURVE:
        {
            rq_equity_curve_t ec = rq_equity_curve_mgr_get(
                market->equity_curve_mgr,
                termstruct_id
                );
            ts = ec;
        }
        break;

        case RQ_TERMSTRUCT_TYPE_EXTERNAL:
        {
            ts = rq_external_termstruct_mgr_get(
                market->external_termstruct_mgr,
                termstruct_id
                );
        }
        break;
    }

    return ts;
}

RQ_EXPORT int
rq_market_get_discount_factor(rq_market_t market, const char *asset_id, rq_date to_date, double *df)
{
    int ok = 1;
    rq_yield_curve_t yc = rq_yield_curve_mgr_get(market->yield_curve_mgr, asset_id);
    if (yc)
    {
        *df = rq_yield_curve_get_discount_factor(yc, to_date);
        ok = 0;
    }

    return ok;
}

RQ_EXPORT int
rq_market_get_forward_rate(
    rq_market_t market, 
    const char *asset_ccypair_id, 
    rq_date to_date,
    double *forward_rate
    )
{
    int ok = 1;
    rq_forward_curve_t forward_curve = rq_forward_curve_mgr_get(
        market->forward_curve_mgr,
        asset_ccypair_id);
    if (forward_curve)
        ok = rq_forward_curve_get_rate(forward_curve, to_date, forward_rate);

    return ok;
}

RQ_EXPORT int
rq_market_get_volatility(
    rq_market_t market, 
    const char *ccypair_asset_id, 
    double delta, 
    rq_date to_date,
    double *volatility
    )
{
    int failed = 1;
    rq_vol_surface_t vol_surface = rq_vol_surface_mgr_get(
        market->vol_surface_mgr,
        ccypair_asset_id
        );
    if (vol_surface)
        failed = rq_vol_surface_get_volatility(
            vol_surface,
            delta,
            to_date,
            volatility
            );

    return failed;
}

RQ_EXPORT int
rq_market_get_ir_volatility(
    rq_market_t market, 
    const char *asset_underlying_id, 
    double underlyingTenor, 
    rq_date to_date,
    double *volatility
    )
{
    int failed = 1;
    rq_ir_vol_surface_t vol_surface = rq_ir_vol_surface_mgr_get(
        market->ir_vol_surface_mgr,
        asset_underlying_id
        );
    if (vol_surface)
        failed = rq_ir_vol_surface_get_volatility(
            vol_surface,
            underlyingTenor,
            to_date,
            volatility
            );

    return failed;
}

RQ_EXPORT int
rq_market_is_null(rq_market_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_market_clear(rq_market_t market)
{
    rq_rate_mgr_clear(market->rate_mgr);
    rq_yield_curve_mgr_clear(market->yield_curve_mgr);
    rq_forward_curve_mgr_clear(market->forward_curve_mgr);
    rq_future_curve_mgr_clear(market->future_curve_mgr);
    rq_vol_surface_mgr_clear(market->vol_surface_mgr);
	rq_ir_vol_surface_mgr_clear(market->ir_vol_surface_mgr);
    rq_exchange_rate_mgr_clear(market->exchange_rate_mgr);
    rq_spot_price_mgr_clear(market->spot_price_mgr);
    rq_equity_curve_mgr_clear(market->equity_curve_mgr);
}

RQ_EXPORT rq_market_t
rq_market_transition_through_time(rq_market_t base_market, rq_date to_date)
{
    rq_market_t new_market = rq_market_alloc(to_date);
    rq_date from_date = rq_market_get_market_date(base_market);
    int days_diff = to_date - from_date;

    rq_rate_mgr_t base_rate_mgr = rq_market_get_rate_mgr(base_market);
    rq_rate_mgr_t new_rate_mgr = rq_market_get_rate_mgr(new_market);

    rq_rate_mgr_iterator_t rate_mgr_it = rq_rate_mgr_iterator_alloc();

    int r;

    rq_rate_mgr_begin(base_rate_mgr, rate_mgr_it);
    for (r = 0; !rq_rate_mgr_at_end(rate_mgr_it); r++)
    {
        rq_rate_t rate = rq_rate_mgr_iterator_deref(rate_mgr_it);
		rq_rate_t rate2 = rq_rate_clone(rate);

        rq_rate_set_observation_date(rate2, rq_rate_get_observation_date(rate) + days_diff);
        rq_rate_set_value_date(rate2, rq_rate_get_value_date(rate) + days_diff);

		rq_rate_mgr_add(new_rate_mgr, rate2);

        rq_rate_mgr_next(rate_mgr_it);
    }

    rq_rate_mgr_iterator_free(rate_mgr_it);

    return new_market;
}
