/**
 * \file rq_product_termstruct_mapping_mgr.h
 * \author Brett Hutley
 *
 * \brief This object manages the list of product to term structure
 * mapping rules
 */
/*
** rq_product_termstruct_mapping_mgr.h
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
#ifndef rq_product_termstruct_mapping_mgr_h
#define rq_product_termstruct_mapping_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_product_termstruct_mapping.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private product_termstruct_mapping_mgr structure.
 */
typedef struct rq_product_termstruct_mapping_mgr {
    rq_tree_rb_t mappings;
} *rq_product_termstruct_mapping_mgr_t;


/** Test whether the rq_product_termstruct_mapping_mgr is NULL */
RQ_EXPORT int rq_product_termstruct_mapping_mgr_is_null(rq_product_termstruct_mapping_mgr_t obj);

/**
 * Allocate a new product_termstruct_mapping_mgr
 */
RQ_EXPORT rq_product_termstruct_mapping_mgr_t rq_product_termstruct_mapping_mgr_alloc();

/**
 * Free an object of type rq_product_termstruct_mapping_mgr
 */
RQ_EXPORT void rq_product_termstruct_mapping_mgr_free(rq_product_termstruct_mapping_mgr_t product_termstruct_mapping_mgr);

/**
 * Add a mapping rule to the manager
 */
RQ_EXPORT void rq_product_termstruct_mapping_mgr_add(rq_product_termstruct_mapping_mgr_t product_termstruct_mapping_mgr, rq_product_termstruct_mapping_t mapping);

/**
 * Get the termstructure mapping rules for a particular product
 */
RQ_EXPORT rq_product_termstruct_mapping_t rq_product_termstruct_mapping_mgr_get(rq_product_termstruct_mapping_mgr_t product_termstruct_mapping_mgr, const char *product_type);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
