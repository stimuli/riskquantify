/**
 * \file rq_asset_list.h
 * \author Brett Hutley
 *
 * \brief A list of assets. The assets are managed by an asset
 * manager, this list is a SHALLOW copy.
 */
/*
** rq_asset_list.h
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
#ifndef rq_asset_list_h
#define rq_asset_list_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_asset.h"
#include "rq_array.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the rq_asset_list */
typedef struct rq_asset_list {
    rq_array_t assets;
} *rq_asset_list_t;


/** Test whether the rq_asset_list is NULL */
RQ_EXPORT int rq_asset_list_is_null(rq_asset_list_t obj);

/**
 * Allocate a new asset_list
 */
RQ_EXPORT rq_asset_list_t rq_asset_list_alloc();

/**
 * Free an object of type rq_asset_list
 */
RQ_EXPORT void rq_asset_list_free(rq_asset_list_t asset_list);

/**
 * Get the number of assets in the list
 */
RQ_EXPORT unsigned int rq_asset_list_size(rq_asset_list_t asset_list);

/**
 * Get the asset at a specific zero-based offset
 */
RQ_EXPORT rq_asset_t rq_asset_list_get_at(rq_asset_list_t asset_list, unsigned int offset);

RQ_EXPORT void rq_asset_list_push_back(rq_asset_list_t al, rq_asset_t asset);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
