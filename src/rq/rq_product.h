/**
 * \file rq_product.h
 * \author Brett Hutley
 *
 * \brief The rq_product object encapsulates the data relating to a
 * specific financial product.
 */
/*
** rq_product.h
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
#ifndef rq_product_h
#define rq_product_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
typedef struct rq_product {
    const char *product_type;
    void *product_data;
    void (*product_free)(void *product_data);
} *rq_product_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_product is NULL */
RQ_EXPORT int rq_product_is_null(rq_product_t obj);

/**
 * Allocate a new product. This should only be called by the derived
 * product allocation function.
 */
RQ_EXPORT rq_product_t _rq_product_alloc(const char *product_type, void *data, void (*free_func)(void *data));

/**
 * Free an allocated product
 */
RQ_EXPORT void rq_product_free(rq_product_t product);

/**
 * Get the product type of this product
 */
RQ_EXPORT const char *rq_product_get_product_type(rq_product_t product);

/**
 * Get the private data associated with this product
 */
RQ_EXPORT void *_rq_product_get_data(rq_product_t product);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
