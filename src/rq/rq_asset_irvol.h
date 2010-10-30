/**
 * \file rq_asset_irvol.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_irvol object encapsulates the properties of a
 * volatility on interest rates.
 */
/*
** rq_asset_irvol.h
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
#ifndef rq_asset_irvol_h
#define rq_asset_irvol_h

#include "rq_asset.h"
#include "rq_asset_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_irvol {
    const char *underlying_asset_id;
	enum rq_rate_type quote_type;
	rq_date tenor_date;
	struct rq_term underlying_tenor;
	rq_date underlying_date;
};

extern const char * const rq_asset_type_id_irvol;

/**
 * build a new rq_asset_irvol structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_irvol_build(
    const char *asset_id,
    const char *underlying_asset_id,
	enum rq_rate_type quote_type,
	rq_date tenor_date,
	const rq_term_t underlying_tenor,
	rq_date underlying_date
    );

RQ_EXPORT rq_asset_t
rq_asset_irvol_find(
    const rq_asset_mgr_t asset_mgr, 
    const char *underlying_asset_id
    );

RQ_EXPORT const char *rq_asset_irvol_get_underlying_asset_id(const rq_asset_t asset);
RQ_EXPORT enum rq_rate_type rq_asset_irvol_get_quote_type(const rq_asset_t asset);
RQ_EXPORT rq_date rq_asset_irvol_get_tenor_date(const rq_asset_t asset);
RQ_EXPORT const rq_term_t rq_asset_irvol_get_underlying_tenor(const rq_asset_t asset);
RQ_EXPORT rq_date rq_asset_irvol_get_underlying_date(const rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
