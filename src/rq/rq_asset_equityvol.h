/**
 * \file rq_asset_equityvol.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_equityvol object encapsulates volatility on
 * equity assets.
 */
/*
** rq_asset_equityvol.h
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
#ifndef rq_asset_equityvol_h
#define rq_asset_equityvol_h

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
struct rq_asset_equityvol {
    const char *equity_code;
    double delta_or_strike;
    unsigned short is_strike_vol;
};

extern const char * const rq_asset_type_id_equityvol;

/**
 * build a new rq_asset_equityvol structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_equityvol_build(
    const char *asset_id,
    const char *ccy,
    double delta_or_strike,
    unsigned short is_strike_vol
    );

RQ_EXPORT rq_asset_t
rq_asset_equityvol_find(
    const rq_asset_mgr_t asset_mgr, 
    const char *equity_code
    );

RQ_EXPORT const char *rq_asset_equityvol_get_ccy_code(const rq_asset_t asset);
RQ_EXPORT double rq_asset_equityvol_get_delta_or_strike(const rq_asset_t asset);
/**
 * Using is_strike_vol as a flag to indicate vols are
 * in terms of fixed strike instead of delta.
 * Future extension to use enum instead of unsigned short.
 */
RQ_EXPORT unsigned short rq_asset_equityvol_is_strike_vol(const rq_asset_t asset);
RQ_EXPORT unsigned short rq_asset_equityvol_is_delta_vol(const rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
