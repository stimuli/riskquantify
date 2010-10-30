/*
** rq_bootstrap_equity_curve_simple.c
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
#include "rq_bootstrap_equity_curve_simple.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_equity_curve_t
rq_bootstrap_equity_curve_simple(
    const char *curve_id,
    const rq_system_t system,
    const rq_market_t market
    )
{
    unsigned int i;
    rq_date date = rq_market_get_market_date(market);
    const rq_rate_mgr_t rate_mgr = rq_market_get_rate_mgr(market);
    const rq_asset_mgr_t asset_mgr = rq_system_get_asset_mgr(system);
    /* const rq_calendar_t cal = rq_calendar_mgr_get(rq_system_get_calendar_mgr(system), curve_id); */
    const rq_bootstrap_config_t config = rq_bootstrap_config_mgr_find(rq_system_get_bootstrap_config_mgr(system), curve_id, RQ_TERMSTRUCT_TYPE_EQUITY_CURVE); 

    rq_equity_curve_t ec;
    const char *underlying_asset_id = NULL;

	unsigned int gotDividendYieldCurve = 0;

    if (!config)
        return NULL;

    ec = rq_equity_curve_build(
        rq_bootstrap_config_get_curve_id(config),
        date
        );
    underlying_asset_id = rq_bootstrap_config_get_asset_id(config);
    if (underlying_asset_id)
        rq_equity_curve_set_underlying_asset_id(ec, underlying_asset_id);

#ifdef DEBUG_BOOTSTRAP
    fprintf(stdout, "Building %s\n", rq_bootstrap_config_get_asset_id(config));
#endif

    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(config); i++)
    {
        const rq_rate_t rate = rq_rate_mgr_find(
            rate_mgr, 
            rq_bootstrap_config_get_rate_class_id_at(config, i)
            );
        if (rate)
        {
            double r = rq_rate_get_value(rate);
            switch (rq_rate_get_rate_type(rate))
            {
                case RQ_RATE_TYPE_PRICE:
                    rq_equity_curve_set_stock_price(ec, r);
                    break;

                case RQ_RATE_TYPE_DIVIDEND:
                {
                    rq_date value_date = rq_rate_get_value_date(rate);
					const char *assetId = rq_rate_get_asset_id(rate);
					rq_asset_t dyAsset = rq_asset_dividend_yield_get_asset(asset_mgr, assetId);
					const char *dc = rq_asset_dividend_yield_get_discrete_dividend_currency(dyAsset);
					rq_dividend_t dividend;
					if (!strcmp(dc, "") || !dc)
					{
						rq_asset_t eqAsset = rq_asset_equity_find(asset_mgr, underlying_asset_id);
						const char *eq_ccy = rq_asset_equity_get_currency(eqAsset);
						dividend = rq_dividend_build(value_date, r, eq_ccy);
					}
					else
					{
						dividend = rq_dividend_build(value_date, r, dc);
					}                    
                    rq_equity_curve_add_dividend(ec, dividend);
                }
                break;

                case RQ_RATE_TYPE_DIVIDEND_YIELD:
                    rq_equity_curve_set_dividend_yield(ec, r);
                    break;

				case RQ_RATE_TYPE_DIVIDEND_YIELD_CURVE:
				{					
					rq_date value_date = rq_rate_get_value_date(rate);
					rq_equity_curve_add_to_dividend_yield(ec, value_date, r);
					gotDividendYieldCurve = 1;
				}
				break;

            } /* switch (rate_type) */
        } /* if (!IS_NULL(rq_rate)) */
        else
        {
            /* printf("Couldn't find rate\n"); */
        }
	}
	if (gotDividendYieldCurve == 1)
	{
		rq_equity_curve_dividend_yield_curve_find_date_bounds(ec);
	}

#ifdef DEBUG_BOOTSTRAP
    fprintf(stdout, "\n\n");
    /* fclose(fh); */
#endif

    return ec;
}
