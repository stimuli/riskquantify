/*
** rq_pricing_helper.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include "rq_pricing_helper.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT enum rq_pricing_helper_result
rq_pricing_helper_value_trade(
    double *value,
    const void *trade,
    struct rq_pricing_adapter *pricing_adapter,
    rq_system_t system,
    rq_market_t market,
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
	const char *pricing_ccy,
    const char *pricing_context
    )
{
    enum rq_pricing_helper_result result = RQ_PRICING_HELPER_RESULT_FAILED;
    rq_market_requirements_t market_requirements = rq_market_requirements_alloc();

    rq_bootstrap_config_mgr_t bootstrap_config_mgr = rq_system_get_bootstrap_config_mgr(system);
    rq_termstruct_mapping_mgr_t termstruct_mapping_mgr = rq_system_get_termstruct_mapping_mgr(system);

    struct rq_pricing_request pricing_request;
    struct rq_pricing_result *pricing_result;
    rq_termstruct_cache_t termstruct_cache;

    int termstruct_type;

    pricing_request.system = system;
    pricing_request.value_date = rq_market_get_market_date(market);
    pricing_request.results_requested = 0;
    pricing_request.trade_details = (void *)trade;
    pricing_request.pricing_context = pricing_context;
    pricing_request.market_transition_cache = NULL;
    pricing_request.trade_transition_cache = NULL;
    pricing_request.pricing_currency = pricing_ccy;
    pricing_request.application_data = NULL;

    if (pricing_adapter->alloc_market_transition_cache)
        pricing_request.market_transition_cache = (*pricing_adapter->alloc_market_transition_cache)(&pricing_request);
    
    if (pricing_adapter->alloc_trade_transition_cache)
        pricing_request.trade_transition_cache = (*pricing_adapter->alloc_trade_transition_cache)(&pricing_request);

    rq_market_requirements_clear(market_requirements);

    /* discover the term structure dependencies for the trade */
    (*pricing_adapter->get_market_requirements)(
        &pricing_request,
        market_requirements
        );

    termstruct_cache = rq_termstruct_cache_alloc();

    for (termstruct_type = 0; termstruct_type < RQ_TERMSTRUCT_TYPE_MAX_ENUM; termstruct_type++)
    {
        unsigned market_req_size = rq_market_requirements_termstruct_size(
            market_requirements,
            (enum rq_termstruct_type)termstruct_type
            );
        unsigned i;
        for (i = 0; i < market_req_size; i++)
        {
            struct rq_termstruct_req *req = rq_market_requirements_termstruct_get_at(
                market_requirements,
                (enum rq_termstruct_type)termstruct_type,
                i
                );

            rq_termstruct_mapping_t mapping = rq_termstruct_mapping_mgr_find(
                termstruct_mapping_mgr,
                req->asset_id,
                req->termstruct_group_id
                );
            if (mapping)
            {
                const char *curve_id = mapping->curve_id;
                rq_bootstrap_config_t bootstrap_config = rq_bootstrap_config_mgr_find(
                    bootstrap_config_mgr,
                    curve_id,
                    (enum rq_termstruct_type)termstruct_type
                    );
                if (bootstrap_config)
                {
                    void *curve = rq_bootstrap_adapter_mgr_build(
                        bootstrap_adapter_mgr,
                        (enum rq_termstruct_type)termstruct_type,
                        bootstrap_config->bootstrap_method_id,
                        curve_id,
                        system,
                        market
                        );
                    if (curve)
                    {
                        struct rq_termstruct_specification *req_ts = rq_termstruct_cache_add_requested_term_structure(
                            termstruct_cache,
                            (enum rq_termstruct_type)termstruct_type,
                            req->termstruct_group_id,
                            req->asset_id,
                            0
                            );

                        req_ts->termstruct = curve;
                    }
                }
            }
        }
    }

    pricing_result = rq_pricing_result_alloc();

    pricing_request.results_requested = RQ_PRICING_RESULTS_VALUE;

    if ((pricing_adapter->get_pricing_results)(&pricing_request, pricing_result) && 
        pricing_result->results_returned & RQ_PRICING_RESULTS_VALUE)
    {
        *value = pricing_result->value;
        result = RQ_PRICING_HELPER_RESULT_SUCCESS;
    }

    if (pricing_adapter->free_pricing_result_data && pricing_result->results_need_freeing)
        (*pricing_adapter->free_pricing_result_data)(pricing_result);

    rq_pricing_result_free(pricing_result);

    rq_termstruct_cache_free(termstruct_cache);

    rq_market_requirements_free(market_requirements);

    if (pricing_adapter->free_market_transition_cache && pricing_request.market_transition_cache)
        (*pricing_adapter->free_market_transition_cache)(pricing_request.market_transition_cache);

    if (pricing_adapter->free_trade_transition_cache && pricing_request.trade_transition_cache)
        (*pricing_adapter->free_trade_transition_cache)(pricing_request.trade_transition_cache);

	return result;
}

