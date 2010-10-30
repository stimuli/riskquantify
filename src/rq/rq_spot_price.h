/**
 * \file rq_spot_price.h
 * \author Brett Hutley
 *
 * \brief The spot prices of an equity or commodity
 */
/*
** rq_spot_price.h
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
#ifndef rq_spot_price_h
#define rq_spot_price_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The spot_price structure.
 */
typedef struct rq_spot_price {
    const char *asset_id;
    double price;
} *rq_spot_price_t;


/** Test whether the rq_spot_price is NULL */
RQ_EXPORT int rq_spot_price_is_null(rq_spot_price_t obj);

/**
 * Allocate a new spot_price
 */
RQ_EXPORT rq_spot_price_t rq_spot_price_alloc(const char *asset_id, double spot_price);

/**
 * Free an object of type rq_spot_price
 */
RQ_EXPORT void rq_spot_price_free(rq_spot_price_t spot_price);

/**
 * Clone an object of type rq_spot_price
 */
RQ_EXPORT rq_spot_price_t rq_spot_price_clone(rq_spot_price_t spot_price);

/**
 * Get the asset ID
 */
RQ_EXPORT const char *rq_spot_price_get_asset_id(rq_spot_price_t spot_price);

/**
 * Get the price
 */
RQ_EXPORT double rq_spot_price_get_price(rq_spot_price_t spot_price);

/**
 * Set the price
 */
RQ_EXPORT void rq_spot_price_set_price(rq_spot_price_t spot_price, double price);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

