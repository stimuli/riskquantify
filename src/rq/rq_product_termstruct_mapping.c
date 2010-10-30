/*
** rq_product_termstruct_mapping.c
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
#include "rq_product_termstruct_mapping.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_product_termstruct_mapping_t
rq_product_termstruct_mapping_alloc()
{
    struct rq_product_termstruct_mapping *m = (struct rq_product_termstruct_mapping *)RQ_MALLOC(sizeof(struct rq_product_termstruct_mapping));
    m->product_type = NULL;
    m->asset_termstruct_mapping_mgr = rq_asset_termstruct_mapping_mgr_alloc();

    return m;
}

RQ_EXPORT void  
rq_product_termstruct_mapping_free(rq_product_termstruct_mapping_t m)
{
    if (m->product_type)
        RQ_FREE((char *)m->product_type);
    rq_asset_termstruct_mapping_mgr_free(m->asset_termstruct_mapping_mgr);
}

RQ_EXPORT int
rq_product_termstruct_mapping_is_null(rq_product_termstruct_mapping_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT const char *
rq_product_termstruct_mapping_get_product_type(rq_product_termstruct_mapping_t m)
{
    return m->product_type;
}

RQ_EXPORT void 
rq_product_termstruct_mapping_set_product_type(rq_product_termstruct_mapping_t m, const char *product_type)
{
    if (m->product_type)
       RQ_FREE((char *)m->product_type);
    m->product_type = (const char *)RQ_STRDUP(product_type);
}

RQ_EXPORT rq_asset_termstruct_mapping_mgr_t 
rq_product_termstruct_mapping_get_asset_termstruct_mgr(rq_product_termstruct_mapping_t m)
{
    return m->asset_termstruct_mapping_mgr;
}
