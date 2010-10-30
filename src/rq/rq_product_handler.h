/**
 * \file rq_product_handler.h
 * \author Brett Hutley
 *
 * \brief The rq_product_handler object provides an interface into
 * functions that implement behaviour relating to a particular
 * financial product.
 */
/*
** rq_product_handler.h
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
#ifndef rq_product_handler_h
#define rq_product_handler_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_product.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
typedef struct rq_product_handler {
    const char *product_type;
} *rq_product_handler_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_product_handler is NULL */
RQ_EXPORT int rq_product_handler_is_null(rq_product_handler_t obj);

/**
 * Allocate a new product handler, giving it the specified product
 * type
 */
RQ_EXPORT rq_product_handler_t 
rq_product_handler_alloc(const char *product_type);

/**
 * Free the allocated product handler.
 */
RQ_EXPORT void rq_product_handler_free(rq_product_handler_t product_handler);

/**
 * Get the product type from the product handler
 */
RQ_EXPORT const char *rq_product_handler_get_product_type(const rq_product_handler_t product_handler);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
