/*!
 * \file rq_asset.h
 * \brief Assets are things that have value. Base assets have
 * intrinsic value, like gold, cash, or oil. Similar asset classes
 * have similar characteristics.
 *
 * \author Brett Hutley
 */
/*
** rq_asset.h
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
#ifndef rq_asset_h
#define rq_asset_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the asset */
typedef struct rq_asset {
    const char *asset_id;
    const char *asset_type_id;

    /* the following data needs to be initialized by the specific asset type builder */
    void *asset_type_data; /**< asset type specific data */
    void (*free_func)(void *asset_type_data); /**< pointer to function that frees the asset type specific data */

    rq_date (*get_value_date_func)(struct rq_asset *, rq_date from_date);
} *rq_asset_t;


/** Test whether the rq_asset is NULL */
RQ_EXPORT int rq_asset_is_null(rq_asset_t obj);

/** Allocate a new rq_asset structure WITHOUT the asset type specific data.
 * This should only be called by the rq_asset_* implementation.
 * In general, users of rq_assets should call rq_asset_*_build in the 
 * appropriate rq_asset_* file.
 *
 * @param asset_type_id the asset type ID
 * @param asset_id an ID to assign to the newly created asset.
 * @return the newly created asset structure. Caller is responsible for freeing.
 */
RQ_EXPORT rq_asset_t _rq_asset_alloc(const char *asset_type_id, const char *asset_id, void *asset_type_data, void (*free_func)(void *asset_type_data));

/** Free an rq_asset structure. 
 * This will call the asset type specific free 
 * routine and thus clean up all the memory.
 */
RQ_EXPORT void rq_asset_free(rq_asset_t a);

/** Get the asset id of an asset.
 */
RQ_EXPORT const char *rq_asset_get_asset_id(const rq_asset_t asset);

/** Get the asset type id of an asset.
 */
RQ_EXPORT const char *rq_asset_get_asset_type_id(const rq_asset_t asset);

/** Get the asset-type specific data.
 */
RQ_EXPORT void * _rq_asset_get_asset_type_data(const rq_asset_t asset);

/** Get the value date of an asset, if the asset supports this function.
 *
 */
RQ_EXPORT rq_date rq_asset_get_value_date(const rq_asset_t asset, rq_date from_date);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

