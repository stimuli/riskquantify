/*
** rq_product.c
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
/* -- includes ---------------------------------------------------- */
#include "rq_product.h"
#include <stdlib.h>
#include <string.h>


/* -- code -------------------------------------------------------- */
RQ_EXPORT rq_product_t 
_rq_product_alloc(const char *product_type, void *data, void (*free_func)(void *data))
{
    struct rq_product *product = (struct rq_product *)RQ_MALLOC(sizeof(struct rq_product));

    product->product_type = (const char *)RQ_STRDUP(product_type);
    product->product_data = data;
    product->product_free = free_func;

    return product;
}

RQ_EXPORT void 
rq_product_free(rq_product_t p)
{
    if (p->product_type)
        RQ_FREE((char *)p->product_type);
    if (p->product_free)
        (*p->product_free)(p->product_data);
    RQ_FREE(p);
}

RQ_EXPORT const char *
rq_product_get_product_type(rq_product_t p)
{
    return p->product_type;
}

RQ_EXPORT int
rq_product_is_null(rq_product_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void *
_rq_product_get_data(rq_product_t p)
{
    return p->product_data;
}
