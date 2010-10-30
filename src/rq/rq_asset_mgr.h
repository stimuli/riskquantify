/**
 * \file rq_asset_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_asset_mgr is a manager object for assets.
 */
/*
** rq_asset_mgr.h
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
#ifndef rq_asset_mgr_h
#define rq_asset_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_asset.h"
#include "rq_string_list.h"
#include "rq_asset_list.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/**
 * This structure implements a node in the asset tree.
 */
struct rq_asset_mgr_node {
    const char *asset_type_id;
    rq_tree_rb_t asset_type_tree;
};

typedef struct rq_asset_mgr {
    rq_tree_rb_t asset_type_tree;
    rq_tree_rb_t asset_tree;
    rq_string_list_t asset_ranking;
} *rq_asset_mgr_t;


/** Test whether the rq_asset_mgr is NULL */
RQ_EXPORT int rq_asset_mgr_is_null(rq_asset_mgr_t obj);

/** Allocate a new asset manager object.
 */
RQ_EXPORT rq_asset_mgr_t rq_asset_mgr_alloc();

/** Free an allocated asset manager object.
 */
RQ_EXPORT void rq_asset_mgr_free(rq_asset_mgr_t mgr);

/** Free the assets managed by the asset manager.
 */
RQ_EXPORT void rq_asset_mgr_clear(rq_asset_mgr_t mgr);

/** Add a new asset to the asset manager.
 */
RQ_EXPORT void
rq_asset_mgr_add(
    rq_asset_mgr_t asset_mgr,
    rq_asset_t asset
    );

/** Get an asset from the asset manager by asset ID.
 */
RQ_EXPORT rq_asset_t
rq_asset_mgr_get(
    const rq_asset_mgr_t asset_mgr,
    const char *asset_id
    );

/** Iterate through the asset manager to find the type-specific asset data.
 */
RQ_EXPORT rq_asset_t
rq_asset_mgr_find(
    const rq_asset_mgr_t asset_mgr,
    const char *asset_type_id,
    int (*cmp)(const void *, const void *), 
    void *cmp_data
    );

/** Return a list of asset types.
 */
RQ_EXPORT rq_string_list_t
rq_asset_mgr_get_asset_type_list(const rq_asset_mgr_t asset_mgr);

/** Return a list of assets of the specific type.
 */
RQ_EXPORT rq_asset_list_t
rq_asset_mgr_list_assets_for_type(
    const rq_asset_mgr_t asset_mgr,
    const char *asset_type_id
    );

/** Add an asset id to ranked asset list.
 */
RQ_EXPORT void
rq_asset_mgr_add_ranking_list(rq_asset_mgr_t asset_mgr, const char *asset_ids);

/** Return a list of ranked assets.
 */
RQ_EXPORT rq_string_list_t
rq_asset_mgr_get_ranking_list(const rq_asset_mgr_t asset_mgr);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
