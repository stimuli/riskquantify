/**
 * \file rq_product_fx_single_leg.h
 * \author Brett Hutley
 *
 * \brief Implements an FX transaction
 */
/*
** rq_product_fx_single_leg.h
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
#ifndef rq_product_fx_single_leg_h
#define rq_product_fx_single_leg_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_product.h"
#include "rq_fx_leg.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- globals ----------------------------------------------------- */
extern const char * const rq_product_type_fx_single_leg;


/* -- prototypes -------------------------------------------------- */
/**
 * Allocate a new product of type fx_single_leg
 */
RQ_EXPORT rq_product_t rq_product_fx_single_leg_alloc();

/**
 * Get a pointer to the fx leg structure managed by this product
 */
RQ_EXPORT struct rq_fx_leg *rq_product_fx_single_leg_get_fx_leg(rq_product_t product);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
