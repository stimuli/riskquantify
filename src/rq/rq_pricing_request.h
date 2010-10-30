/**
 * \file rq_pricing_request.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_request structure is passed to a pricing
 * adapter in order to make a pricing request.
 */
/*
** rq_pricing_request.h
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
#ifndef rq_pricing_request_h
#define rq_pricing_request_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_product.h"
#include "rq_system.h"
#include "rq_market.h"
#include "rq_termstruct_cache.h"
#include "rq_string_set.h"
#include "rq_named_variant_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- structs ----------------------------------------------------- */
struct rq_pricing_request {
	rq_system_t system;
	rq_market_t market;
    rq_date value_date; /**< The value date to price to */
    unsigned long results_requested;
    rq_string_set_t named_values_requested; /**< can request results by name using this */
    rq_string_set_t pricing_values_requested; /**< can request pricing results by name using this */
    rq_string_set_t tracing_values_requested; /**< can request tracing results by name using this */
    rq_named_variant_mgr_t pricing_parameters; /**< can supply additional parameters by name in this structure */
    const char *pricing_currency;
    void *application_data; /* assigned by application */
	void *price_adapter_cache; /* created when pricing adapter is created */
	void *trade_details;
	void *calibration_details;
    void *reference_product;
    void *trade_transition_cache; /**< a cache to hold information for when the trade gets transitioned to different states because of market rates or time */
    void *market_transition_cache; /**< a cache to store market rates for when an interpolated rate needs to be used */
    const char *pricing_context;
    rq_termstruct_cache_t termstruct_cache;
};

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
