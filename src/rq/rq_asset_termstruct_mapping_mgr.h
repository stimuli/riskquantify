/**
 * \file rq_asset_termstruct_mapping_mgr.h
 * \author Brett Hutley
 *
 * \brief A manager for asset to termstructure mapping objects
 */
/*
** rq_asset_termstruct_mapping_mgr.h
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
#ifndef rq_asset_termstruct_mapping_mgr_h
#define rq_asset_termstruct_mapping_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_enum.h"
#include "rq_asset_termstruct_mapping.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The asset_termstruct_mapping_mgr structure.
 */
typedef struct rq_asset_termstruct_mapping_mgr {
    rq_tree_rb_t mapping_lists[RQ_TERMSTRUCT_TYPE_MAX_ENUM];
} *rq_asset_termstruct_mapping_mgr_t;


/** Test whether the rq_asset_termstruct_mapping_mgr is NULL */
RQ_EXPORT int rq_asset_termstruct_mapping_mgr_is_null(rq_asset_termstruct_mapping_mgr_t obj);

/**
 * Allocate a new asset_termstruct_mapping_mgr
 */
RQ_EXPORT rq_asset_termstruct_mapping_mgr_t rq_asset_termstruct_mapping_mgr_alloc();

/**
 * Free an object of type rq_asset_termstruct_mapping_mgr
 */
RQ_EXPORT void rq_asset_termstruct_mapping_mgr_free(rq_asset_termstruct_mapping_mgr_t asset_termstruct_mapping_mgr);

/**
 * Add an asset termstructure mapping object to the manager
 */
RQ_EXPORT void rq_asset_termstruct_mapping_mgr_add(rq_asset_termstruct_mapping_mgr_t asset_termstruct_mapping_mgr, rq_asset_termstruct_mapping_t mapping);

/**
 * Get an asset termstructure mapping object from the manager
 */
RQ_EXPORT rq_asset_termstruct_mapping_t 
rq_asset_termstruct_mapping_mgr_get(
    rq_asset_termstruct_mapping_mgr_t asset_termstruct_mapping_mgr,
    enum rq_termstruct_type termstruct_type,
    const char *asset_id
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
