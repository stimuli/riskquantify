/*
** rq_product_handler.c
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
#include "rq_product_handler.h"
#include <stdlib.h>
#include <string.h>


/* -- code -------------------------------------------------------- */
RQ_EXPORT rq_product_handler_t 
rq_product_handler_alloc(const char *product_type)
{
    struct rq_product_handler *product_handler = 
        (struct rq_product_handler *)RQ_CALLOC(1, sizeof(struct rq_product_handler));
    product_handler->product_type = RQ_STRDUP(product_type);

    return product_handler;
}

RQ_EXPORT void 
rq_product_handler_free(rq_product_handler_t handler)
{
    RQ_FREE((char *)handler->product_type);
    RQ_FREE(handler);
}

RQ_EXPORT const char *
rq_product_handler_get_product_type(const rq_product_handler_t handler)
{
    return handler->product_type;
}

RQ_EXPORT int
rq_product_handler_is_null(rq_product_handler_t obj)
{
    return (obj == NULL);
}
