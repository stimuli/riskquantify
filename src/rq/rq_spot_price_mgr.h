/**
 * \file rq_spot_price_mgr.h
 * \author Brett Hutley
 *
 * \brief Manages the spot prices for equities and commodities in the
 * underlying ccy code of the asset
 */
/*
** rq_spot_price_mgr.h
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
#ifndef rq_spot_price_mgr_h
#define rq_spot_price_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_spot_price.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_spot_price_mgr_node {
    const char *asset_type_id;
    rq_tree_rb_t prices;
};

/** The spot_price_mgr structure.
 */
typedef struct rq_spot_price_mgr {
    rq_tree_rb_t asset_types;
} *rq_spot_price_mgr_t;


/** Test whether the rq_spot_price_mgr is NULL */
RQ_EXPORT int rq_spot_price_mgr_is_null(rq_spot_price_mgr_t obj);

/**
 * Allocate a new spot_price_mgr
 */
RQ_EXPORT rq_spot_price_mgr_t rq_spot_price_mgr_alloc();

/**
 * Free an object of type rq_spot_price_mgr
 */
RQ_EXPORT void rq_spot_price_mgr_free(rq_spot_price_mgr_t spot_price_mgr);

/**
 * Clear the spot price manager
 */
RQ_EXPORT void rq_spot_price_mgr_clear(rq_spot_price_mgr_t spot_price_mgr);

/**
 * Clone the spot price manager
 */
RQ_EXPORT rq_spot_price_mgr_t rq_spot_price_mgr_clone(rq_spot_price_mgr_t spot_price_mgr);

/**
 * Add a new spot price
 */
RQ_EXPORT void rq_spot_price_mgr_add(rq_spot_price_mgr_t spot_price_mgr, const char *asset_type_id, const char *asset_id, double price);

/**
 * Get a spot price for an asset
 */
RQ_EXPORT double rq_spot_price_mgr_get_price(rq_spot_price_mgr_t spot_price_mgr, const char *asset_type_id, const char *asset_id);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
