/**
 * @file
 *
 * The rq_pricing_adapter structure allows us to provide a standard interface
 * to pricing models.
 */
/*
** rq_pricing_adapter.h
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
#ifndef rq_pricing_adapter_h
#define rq_pricing_adapter_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_enum.h"
#include "rq_product.h"
#include "rq_system.h"
#include "rq_market.h"
#include "rq_pricing_request.h"
#include "rq_pricing_result.h"
#include "rq_assetflow_list.h"
#include "rq_market_requirements.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- structs ----------------------------------------------------- */

struct rq_pricing_adapter {
	const char *(*get_pricing_adapter_id)();

    /* called when the pricing adapter is registered and deregistered */
	void *(*alloc_pricing_adapter_cache)();
	void (*free_pricing_adapter_cache)(void *cache);

    /*
      The trade transition cache is used to store information
      needed when the trade is transitioned to various states.
      ie Barriers are knocked in or out.
      The trade transition cache is cleared on every path in a
      monte carlo simulation.
    */
    void *(*alloc_trade_transition_cache)(struct rq_pricing_request *);
    void (*clear_trade_transition_cache)(void *);
    void (*free_trade_transition_cache)(void *);

    /*
      The market transition cache stores market information that 
      is needed in Monte Carlo simulations when rates need to be
      set between node dates
    */
    void *(*alloc_market_transition_cache)(struct rq_pricing_request *);
    void (*clear_market_transition_cache)(void *);
    void (*free_market_transition_cache)(void *);

	/* 
       a callback to get the market content that a pricing model needs
       to price a trade. 
    */
    void (*get_market_requirements)(struct rq_pricing_request *pricing_request, rq_market_requirements_t market_requirements); 

    short (*get_pricing_results)(struct rq_pricing_request *pricing_request, struct rq_pricing_result *pricing_result);

    void (*free_pricing_result_data)(struct rq_pricing_result *);

    void (*merge_trade_product)(struct rq_pricing_request *pricing_request);
};

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
