/**
 * \file rq_asset_termstruct_mapping.h
 * \author Brett Hutley
 *
 * \brief This object represents a mapping between an asset and a
 * termstructure.
 */
/*
** rq_asset_termstruct_mapping.h
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
#ifndef rq_asset_termstruct_mapping_h
#define rq_asset_termstruct_mapping_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the rq_asset_termstruct_mapping */
typedef struct rq_asset_termstruct_mapping {
    const char *asset_id;
    enum rq_termstruct_type termstruct_type;
    const char *termstruct_id;
} *rq_asset_termstruct_mapping_t;


/** Test whether the rq_asset_termstruct_mapping is NULL */
RQ_EXPORT int rq_asset_termstruct_mapping_is_null(rq_asset_termstruct_mapping_t obj);

/**
 * Allocate a new asset_termstruct_mapping
 */
RQ_EXPORT rq_asset_termstruct_mapping_t rq_asset_termstruct_mapping_alloc();

/**
 * Free an object of type rq_asset_termstruct_mapping
 */
RQ_EXPORT void rq_asset_termstruct_mapping_free(rq_asset_termstruct_mapping_t asset_termstruct_mapping);

/**
 * Set the asset id in the asset termstructure mapping
 */
RQ_EXPORT void rq_asset_termstruct_mapping_set_asset_id(rq_asset_termstruct_mapping_t asset_termstruct_mapping, const char *asset_id);

/**
 * Get the asset id in the asset termstructure mapping
 */
RQ_EXPORT const char *rq_asset_termstruct_mapping_get_asset_id(rq_asset_termstruct_mapping_t asset_termstruct_mapping);

/**
 * Set the term structure type in the asset termstructure mapping
 */
RQ_EXPORT void rq_asset_termstruct_mapping_set_termstruct_type(rq_asset_termstruct_mapping_t asset_termstruct_mapping, enum rq_termstruct_type termstruct_type);

/**
 * Get the term structure type in the asset termstructure mapping
 */
RQ_EXPORT enum rq_termstruct_type rq_asset_termstruct_mapping_get_termstruct_type(rq_asset_termstruct_mapping_t asset_termstruct_mapping);

/**
 * Set the term structure id in the asset term structure mapping
 */
RQ_EXPORT void rq_asset_termstruct_mapping_set_termstruct_id(rq_asset_termstruct_mapping_t asset_termstruct_mapping, const char *termstruct_id);

/**
 * Get the term structure id in the asset term structure mapping
 */
RQ_EXPORT const char *rq_asset_termstruct_mapping_get_termstruct_id(rq_asset_termstruct_mapping_t asset_termstruct_mapping);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
