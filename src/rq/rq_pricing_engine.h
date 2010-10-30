/**
 * \file rq_pricing_engine.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_engine object provides a standard interface
 * to a set of pricing adapters.
 */
/*
** rq_pricing_engine.h
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
#ifndef rq_pricing_engine_h
#define rq_pricing_engine_h

#define PA_CACHE

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_pricing_adapter.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
struct rq_pricing_engine_node {
    struct rq_pricing_engine_node *left;
    struct rq_pricing_engine_node *right;

    const char *product_id;

    void *pricing_adapter_cache;

    struct rq_pricing_adapter *pricing_adapter;
};

typedef struct rq_pricing_engine {
    struct rq_pricing_engine_node *head;
} *rq_pricing_engine_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_pricing_engine is NULL */
RQ_EXPORT int rq_pricing_engine_is_null(rq_pricing_engine_t obj);

/**
 * Allocate a new pricing engine object
 */
RQ_EXPORT rq_pricing_engine_t rq_pricing_engine_alloc();

/**
 * Free an allocated pricing engine object
 */
RQ_EXPORT void rq_pricing_engine_free(rq_pricing_engine_t pricing_engine);

/**
 * Add a pricing adapter to a pricing engine
 */
RQ_EXPORT void rq_pricing_engine_add_pricing_adapter(rq_pricing_engine_t pricing_engine, const char *product_id, struct rq_pricing_adapter *pricing_adapter);

/**
 * Perform a pricing request.
 */
RQ_EXPORT int rq_pricing_engine_get_pricing_results(rq_pricing_engine_t pricing_engine, const char *product_id, struct rq_pricing_request *pricing_request, struct rq_pricing_result *pricing_result);

/**
 * Get the pricing adapter for a given product ID
 */
RQ_EXPORT struct rq_pricing_adapter *rq_pricing_engine_get_pricing_adapter(rq_pricing_engine_t pricing_engine, const char *product_id);

/**
 * Get the pricing adapter cache for a pricing adapter
 */
RQ_EXPORT void *rq_pricing_engine_get_pricing_adapter_cache(rq_pricing_engine_t pricing_engine, const char *product_id);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
