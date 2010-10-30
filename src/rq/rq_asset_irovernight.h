/**
 * \file rq_asset_irovernight.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_irovernight object encapsulates the properties
 * of an overnight interest rate.
 */
/*
** rq_asset_irovernight.h
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
#ifndef rq_asset_irovernight_h
#define rq_asset_irovernight_h

#include "rq_config.h"
#include "rq_term.h"
#include "rq_enum.h"
#include "rq_asset.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The data that gets added to the rq_asset structure. 
 */
struct rq_asset_irovernight {
    const char *ccy_code;
    enum rq_day_count_convention day_count_convention;
};

/**
 * The asset type ID for a Bank Bill asset
 */
extern const char * const rq_asset_type_id_irovernight;

/**
 * build a new rq_asset_irovernight structure.
 *
 * @return the allocated structure. The caller is responsible for
 * freeing this object.  
 */
RQ_EXPORT rq_asset_t
rq_asset_irovernight_build(
    const char *asset_id,
    const char *ccy,
    enum rq_day_count_convention day_count_convention
    );


RQ_EXPORT const char *rq_asset_irovernight_get_ccy_code(const rq_asset_t a);
RQ_EXPORT enum rq_day_count_convention rq_asset_irovernight_get_day_count_convention(const rq_asset_t a);

/**
 * Return true if the asset is an overnight interest rate.
 */
RQ_EXPORT short rq_asset_is_irovernight(rq_asset_t asset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
