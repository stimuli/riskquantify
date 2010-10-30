/*
** rq_product_handler_mgr.c
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
#include "rq_product_handler_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


/* -- code -------------------------------------------------------- */
RQ_EXPORT rq_product_handler_mgr_t 
rq_product_handler_mgr_alloc()
{
    rq_product_handler_mgr_t phm = (rq_product_handler_mgr_t)RQ_MALLOC(
        sizeof(struct rq_product_handler_mgr)
        );
    phm->product_handlers = rq_tree_rb_alloc((void (*)(void *))rq_product_handler_free, (int (*)(const void *, const void *))strcmp);
	return phm;
}

RQ_EXPORT void 
rq_product_handler_mgr_free(rq_product_handler_mgr_t product_handler_mgr)
{
    rq_tree_rb_free(product_handler_mgr->product_handlers);
    RQ_FREE(product_handler_mgr);
}

RQ_EXPORT void 
rq_product_handler_mgr_add(rq_product_handler_mgr_t product_handler_mgr, rq_product_handler_t product_handler)
{
    rq_tree_rb_add(product_handler_mgr->product_handlers, rq_product_handler_get_product_type(product_handler), product_handler);
}

RQ_EXPORT rq_product_handler_t 
rq_product_handler_mgr_get(rq_product_handler_mgr_t product_handler_mgr, const char *product_type)
{
    return (rq_product_handler_t) rq_tree_rb_find(product_handler_mgr->product_handlers, product_type);
}

RQ_EXPORT int
rq_product_handler_mgr_is_null(rq_product_handler_mgr_t obj)
{
    return (obj == NULL);
}
