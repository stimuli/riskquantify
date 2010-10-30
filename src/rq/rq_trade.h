/**
 * \file rq_trade.h
 * \author Brett Hutley
 *
 * \brief The rq_trade object encapsulates a financial trade.
 */
/*
** rq_trade.h
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
#ifndef rq_trade_h
#define rq_trade_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_enum.h"
#include "rq_trade_status.h"
#include "rq_product.h"
#include "rq_array.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_trade {
    int trd_id;
    const char *trd_key;
    const char *trd_insert_time;
    const char *trd_usr_id;
    const char *trd_tst_code;
    const char *trd_tty_code;
    const char *trd_bys_code;
    const char *trd_cpt_key;
    const char *trd_bok_code;
    const char *trd_data;
} *rq_trade_t;


/** Test whether the rq_trade is NULL */
RQ_EXPORT int rq_trade_is_null(rq_trade_t obj);

/**
 * Allocate a new trade
 */
RQ_EXPORT rq_trade_t rq_trade_alloc();

/**
 * Free an allocated trade
 */
RQ_EXPORT void rq_trade_free(rq_trade_t trade);

/**
 * Get a trade's ID
 */
RQ_EXPORT rq_id rq_trade_get_id(rq_trade_t trade);

/**
 * Set a trade's ID
 */
RQ_EXPORT void rq_trade_set_id(rq_trade_t trade, rq_id id);

/**
 * Get the trade date of a trade
 */
RQ_EXPORT rq_date rq_trade_get_trade_date(rq_trade_t trade);

/**
 * Set the trade date of a trade
 */
RQ_EXPORT void rq_trade_set_trade_date(rq_trade_t trade, rq_date trade_date);

/**
 * Get the status of a trade
 */
RQ_EXPORT rq_trade_status rq_trade_get_trade_status(rq_trade_t trade);

/**
 * Set the status of a trade
 */
RQ_EXPORT void rq_trade_set_trade_status(rq_trade_t trade, rq_trade_status trade_status);

/**
 * Get the number of product managed by the trade
 */
RQ_EXPORT unsigned int rq_trade_get_product_count(rq_trade_t trade);

/**
 * Get the product at a particular zero-based offset
 */
RQ_EXPORT rq_product_t rq_trade_get_product_at(rq_trade_t trade, unsigned int offset);

/**
 * Add a product to the list of products managed by this trade
 */
RQ_EXPORT void rq_trade_add_product(rq_trade_t trade, rq_product_t product);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
