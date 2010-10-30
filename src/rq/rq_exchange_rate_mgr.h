/**
 * \file rq_exchange_rate_mgr.h
 * \author Brett Hutley
 *
 * \brief rq_exchange_rate_mgr provides a manager class for exchange
 * rate objects
 */
/*
** rq_exchange_rate_mgr.h
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
#ifndef rq_exchange_rate_mgr_h
#define rq_exchange_rate_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_forward_curve_mgr.h"
#include "rq_asset_mgr.h"
#include "rq_exchange_rate.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- defines ----------------------------------------------------- */
#define MAXCCYCODELEN 3
#define MAXCCYPAIRLEN 6

/* -- structs ----------------------------------------------------- */

/**
 * This structure defines how we work out cross rates. For example,
 * say we want the GBP/JPY exchange rate, but we don't have the
 * forward curve for this. What we do is have a list of currencies
 * that we try and cross through. If USD is the first in the list,
 * we try and grab the GBP/USD rate and the JPY/USD rate to imply
 * the GBP/JPY rate.
 */
struct rq_exchange_rate_cross_thru_node {
    struct rq_exchange_rate_cross_thru_node *next;
    char ccy_code[MAXCCYCODELEN+1];
};

typedef struct rq_exchange_rate_mgr {
    rq_tree_rb_t tree;

    struct rq_exchange_rate_cross_thru_node *cross_thru_node;
} *rq_exchange_rate_mgr_t;

typedef struct rq_exchange_rate_mgr_iterator {
    rq_tree_rb_iterator_t exchange_rate_it;
} * rq_exchange_rate_mgr_iterator_t;


/** Test whether the rq_exchange_rate_mgr is NULL */
RQ_EXPORT int rq_exchange_rate_mgr_is_null(rq_exchange_rate_mgr_t obj);

/**
 * Allocate a new exchange rate manager
 */
RQ_EXPORT rq_exchange_rate_mgr_t rq_exchange_rate_mgr_alloc();

/**
 * Make a deep copy of the exchange rate manager
 */
RQ_EXPORT rq_exchange_rate_mgr_t rq_exchange_rate_mgr_clone(rq_exchange_rate_mgr_t ermgr);

/**
 * Free the exchange rate manager
 */
RQ_EXPORT void rq_exchange_rate_mgr_free(rq_exchange_rate_mgr_t exchange_rate_mgr);

/**
 * Free the exchange rates managed by the exchange rate manager.
 * This also frees the cross thru list.
 */
RQ_EXPORT void rq_exchange_rate_mgr_clear(rq_exchange_rate_mgr_t exchange_rate_mgr);

/**
 * This function, if successful, returns the exchange rate that converts
 * the "from" currency amount into the "to" currency amount.
 */
RQ_EXPORT int 
rq_exchange_rate_mgr_get_or_imply(
    rq_exchange_rate_mgr_t exchange_rate_mgr, 
    rq_forward_curve_mgr_t forward_curve_mgr,
    rq_asset_mgr_t asset_mgr,
    const char *ccy_code_from,
    const char *ccy_code_to,
    rq_date date,
    double *exchange_rate,
    short cache_result
    );

/**
 * Add an exchange rate to the manager
 */
RQ_EXPORT void
rq_exchange_rate_mgr_add(
    rq_exchange_rate_mgr_t exchange_rate_mgr, 
    const char *ccy_code_from,
    const char *ccy_code_to,
    double exchange_rate
    );

/**
 * Get an exchange rate from the manager
 */
RQ_EXPORT double
rq_exchange_rate_mgr_get(
    rq_exchange_rate_mgr_t exchange_rate_mgr, 
    const char *ccy_code_from,
    const char *ccy_code_to
    );

/**
 * This function APPENDS a cross-through currency to the
 * list of currencies the exchange rate manager will use
 * if a cross rate is unable to be determined directly.
 */
RQ_EXPORT void
rq_exchange_rate_mgr_add_cross_thru_ccy_code(
    rq_exchange_rate_mgr_t exchange_rate_mgr, 
    const char *ccy_code
    );

/**
 * This function parse thru a comma delimited list 
 * of cross-through currencies and append them
 * to the exchange rate manager.
 */
RQ_EXPORT void
rq_exchange_rate_mgr_add_cross_thru_ccy_codes(
    rq_exchange_rate_mgr_t m, 
    const char *ccy_codes
    );

/**
 * Allocate an iterator that can iterator over the list of exchange
 * rates
 */
RQ_EXPORT rq_exchange_rate_mgr_iterator_t rq_exchange_rate_mgr_iterator_alloc();

/**
 * Free an allocated exchange rate iterator
 */
RQ_EXPORT void rq_exchange_rate_mgr_iterator_free(rq_exchange_rate_mgr_iterator_t it);

/**
 * Initialize an iterator to the start of the list of exchange rates
 */
RQ_EXPORT void rq_exchange_rate_mgr_begin(rq_exchange_rate_mgr_t tree, rq_exchange_rate_mgr_iterator_t it);

/**
 * Test whether the iterator has passed the end of the list of exchange
 * rates
 */
RQ_EXPORT int rq_exchange_rate_mgr_at_end(rq_exchange_rate_mgr_iterator_t it);

/**
 * Move to the next exchange rate in the list of exchange rates
 */
RQ_EXPORT void rq_exchange_rate_mgr_next(rq_exchange_rate_mgr_iterator_t i);

/**
 * Return the exchange rate pointed to by the iterator
 */
RQ_EXPORT rq_exchange_rate_t rq_exchange_rate_mgr_iterator_deref(rq_exchange_rate_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
