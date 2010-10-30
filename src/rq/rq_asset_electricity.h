/**
 * \file rq_asset_electricity.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_electricity object encapsulates the properties
 * of electricity assets.
 */
/*
** rq_asset_electricity.h
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
#ifndef rq_asset_electricity_h
#define rq_asset_electricity_h

#include "rq_asset.h"
#include "rq_asset_mgr.h"
#include "rq_datehour.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_electricity {
    const char *exchange; /* the electricity exchange (nordpool/france/etc) */
    const char *load_type; /* base/peak load */
    rq_datehour datehour; /* the date and hour of the delivery */
};


/**
 * The asset type ID for a electricity type asset
 */
extern const char * const rq_asset_type_id_electricity;

/**
 * build a new rq_asset_electricity structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_electricity_build(
    const char *asset_id,
    const char *exchange, /**< The exchange the electricity is traded in */
    const char *load_type
    );

/** Get the exchange associated with an electricity
 */
RQ_EXPORT const char *rq_asset_electricity_get_exchange(const rq_asset_t a);

/** Get the load type associated with an electricity
 */
RQ_EXPORT const char *rq_asset_electricity_get_load_type(const rq_asset_t a);

/**
 * Return true if the asset is an electricity type asset.
 */
RQ_EXPORT short rq_asset_is_electricity(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
