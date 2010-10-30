/*
** rq_bootstrap_adapter_future_curve_simple.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#include "rq_bootstrap_adapter_future_curve_simple.h"
#include "rq_bootstrap_future_curve_simple.h"
#include "rq_asset_ccypair.h"
#include "rq_error.h"
#include <stdlib.h>
#include <string.h>


const char * const rq_bootstrap_adapter_future_curve_simple_id = "BootstrapFutureCurveSimple";

void *
bootstrap_future_curve_simple(
    rq_bootstrap_adapter_t bootstrap_adapter,
    const char *curve_id, 
    const rq_system_t system,
    rq_market_t market
    )
{
    rq_spot_price_mgr_t spot_price_mgr;

    rq_bootstrap_config_t bootstrap_config = 
        rq_bootstrap_config_mgr_find(
            rq_system_get_bootstrap_config_mgr(system),
            curve_id,
            RQ_TERMSTRUCT_TYPE_FUTURE_CURVE
            );
    if (bootstrap_config)
    {
        rq_future_curve_t future_curve = rq_bootstrap_future_curve_simple(
            rq_market_get_market_date(market),
            rq_market_get_rate_mgr(market),
            bootstrap_config,
            rq_system_get_asset_mgr(system),
            NULL,
            0
            );
        if (future_curve)
        {
            /* Add the spot price into the exchange price manager */

            if (rq_future_curve_size(future_curve) > 0)
            {
                rq_asset_mgr_t asset_mgr = rq_system_get_asset_mgr(system);
                rq_asset_t asset = rq_asset_mgr_get(asset_mgr, rq_future_curve_get_asset_id(future_curve));
                if (asset)
                {
                    if (rq_asset_is_ccypair(asset))
                    { 
                        const char *c1 = rq_asset_ccypair_get_ccy_code_1(asset);
                        const char *c2 = rq_asset_ccypair_get_ccy_code_2(asset);
                        rq_date spotDate = rq_asset_ccypair_get_spot_date(rq_system_get_calendar_mgr(system),
                                                                          asset_mgr,
                                                                          c1,
                                                                          c2,
                                                                          rq_market_get_market_date(market));
                        //if (future_price->price != 0.0)
                         double future_price;
                        if (rq_future_curve_get_price(future_curve, spotDate, &future_price) == RQ_OK)
                        {
                            rq_exchange_rate_mgr_t ermgr = rq_market_get_exchange_rate_mgr(market);
                            enum rq_ccypair_quote_convention qc = rq_asset_ccypair_get_quote_convention(asset);
                            if (qc == RQ_CCYPAIR_QUOTE_CONVENTION_2PER1)
                            {
                                /* direct */
                                rq_exchange_rate_mgr_add(
                                    ermgr,
                                    c1,
                                    c2,
                                    future_price
                                    );
                                rq_exchange_rate_mgr_add(
                                    ermgr,
                                    c2,
                                    c1,
                                    1.0 / future_price
                                    );
                            }
                            else
                            {
                                /* direct */
                                rq_exchange_rate_mgr_add(
                                    ermgr,
                                    c2,
                                    c1,
                                    future_price
                                    );
                                rq_exchange_rate_mgr_add(
                                    ermgr,
                                    c1,
                                    c2,
                                    1.0 / future_price
                                    );
                            }
                        } /* if future price is not 0 */
                    } /* if currency pair */
                    else
                    {
                        const struct rq_future_price *future_price = rq_future_curve_element_at(future_curve, 0);
                        spot_price_mgr = rq_market_get_spot_price_mgr(market);
                        rq_spot_price_mgr_add(spot_price_mgr, rq_asset_get_asset_type_id(asset), rq_asset_get_asset_id(asset), future_price->price);
                    }
                }
            }
            
            rq_future_curve_mgr_add(
                rq_market_get_future_curve_mgr(market),
                future_curve
                );
        }
        return future_curve;
    }

    return NULL;
}

rq_bootstrap_adapter_t
rq_bootstrap_adapter_future_curve_simple_alloc()
{
    return 
        _rq_bootstrap_adapter_alloc(
            rq_bootstrap_adapter_future_curve_simple_id,
            RQ_TERMSTRUCT_TYPE_FUTURE_CURVE,
            bootstrap_future_curve_simple
            );
}

