/**
 * \file rq_product_termstruct_mapping.h
 * \author Brett Hutley
 *
 * \brief Each product can have it's own mapping of assets to term
 * structures
 */
/*
** rq_product_termstruct_mapping.h
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
#ifndef rq_product_termstruct_mapping_h
#define rq_product_termstruct_mapping_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_asset_termstruct_mapping_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The product_termstruct_mapping structure.
 */
typedef struct rq_product_termstruct_mapping {
    const char *product_type;
    rq_asset_termstruct_mapping_mgr_t asset_termstruct_mapping_mgr;
} *rq_product_termstruct_mapping_t;


/** Test whether the rq_product_termstruct_mapping is NULL */
RQ_EXPORT int rq_product_termstruct_mapping_is_null(rq_product_termstruct_mapping_t obj);

/**
 * Allocate a new product_termstruct_mapping
 */
RQ_EXPORT rq_product_termstruct_mapping_t rq_product_termstruct_mapping_alloc();

/**
 * Free an object of type rq_product_termstruct_mapping
 */
RQ_EXPORT void rq_product_termstruct_mapping_free(rq_product_termstruct_mapping_t product_termstruct_mapping);

/**
 * Get the product type associated with this product to term structure mapping
 */
RQ_EXPORT const char *rq_product_termstruct_mapping_get_product_type(rq_product_termstruct_mapping_t product_termstruct_mapping);

/**
 * Set the product type associated with this product to term structure mapping
 */
RQ_EXPORT void rq_product_termstruct_mapping_set_product_type(rq_product_termstruct_mapping_t product_termstruct_mapping, const char *product_type);

/**
 * Get the asset termstructure mapping manager associated with this product
 */
RQ_EXPORT rq_asset_termstruct_mapping_mgr_t rq_product_termstruct_mapping_get_asset_termstruct_mgr(rq_product_termstruct_mapping_t product_termstruct_mapping);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
