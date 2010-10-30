/*
** rq_product_fx_single_leg.c
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
#include "rq_product_fx_single_leg.h"
#include <stdlib.h>
#include <string.h>

/* -- globals ----------------------------------------------------- */
const char * const rq_product_type_fx_single_leg = "fxSingleLeg";

/* -- code -------------------------------------------------------- */
static void
fx_single_leg_free(void *p)
{
    struct rq_fx_leg *fx_leg = (struct rq_fx_leg *)p;
    rq_fx_leg_free(fx_leg);
}

RQ_EXPORT rq_product_t
rq_product_fx_single_leg_alloc()
{
    struct rq_fx_leg *fx_leg = rq_fx_leg_alloc();
    rq_product_t product = _rq_product_alloc(
        rq_product_type_fx_single_leg,
        fx_leg,
        fx_single_leg_free
        );

    return product;
}

RQ_EXPORT struct rq_fx_leg *
rq_product_fx_single_leg_get_fx_leg(rq_product_t product)
{
    return (struct rq_fx_leg *)_rq_product_get_data(product);
}

