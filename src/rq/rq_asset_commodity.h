/**
 * @file
 *
 * Commodity-based asset
 */
/*
** rq_asset_commodity.h
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
#ifndef rq_asset_commodity_h
#define rq_asset_commodity_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_asset.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private asset_commodity structure that gets added to the
 * rq_asset structure.
 */
struct rq_asset_commodity {
    const char *long_name;
    const char *type;
    const char *grade;
    const char *location;
    const char *location_center;
    const char *currency;
    const char *unit;
    double unit_multiplier;
};

extern const char * const rq_asset_type_id_commodity;

/**
 * Allocate a new asset_commodity
 */
RQ_EXPORT rq_asset_t
rq_asset_commodity_build(
    const char *asset_id,
	const char *long_name,
    const char *type,
    const char *grade,
    const char *location,
	const char *location_center,
    const char *currency,
    const char *unit,
    double unit_multiplier
    );

RQ_EXPORT short
rq_asset_is_commodity(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_commodity_get_long_name(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_commodity_get_type(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_commodity_get_grade(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_commodity_get_location(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_commodity_get_location_center(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_commodity_get_currency(rq_asset_t asset);

RQ_EXPORT const char *
rq_asset_commodity_get_unit(rq_asset_t asset);

RQ_EXPORT double 
rq_asset_commodity_get_unit_multiplier(rq_asset_t asset);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
