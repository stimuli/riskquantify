/*
** rq_product_termstruct_mapping_mgr.c
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
#include "rq_product_termstruct_mapping_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


static void
free_func(void *p)
{
    rq_product_termstruct_mapping_free((rq_product_termstruct_mapping_t)p);
}

RQ_EXPORT rq_product_termstruct_mapping_mgr_t
rq_product_termstruct_mapping_mgr_alloc()
{
    struct rq_product_termstruct_mapping_mgr *m = (struct rq_product_termstruct_mapping_mgr *)RQ_MALLOC(sizeof(struct rq_product_termstruct_mapping_mgr));
    m->mappings = rq_tree_rb_alloc(free_func, (int (*)(const void *, const void *))strcmp);

    return m;
}

RQ_EXPORT void  
rq_product_termstruct_mapping_mgr_free(rq_product_termstruct_mapping_mgr_t m)
{
    rq_tree_rb_free(m->mappings);
    RQ_FREE(m);
}

RQ_EXPORT int
rq_product_termstruct_mapping_mgr_is_null(rq_product_termstruct_mapping_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_product_termstruct_mapping_mgr_add(rq_product_termstruct_mapping_mgr_t m, rq_product_termstruct_mapping_t mapping)
{
    rq_tree_rb_add(
        m->mappings, 
        rq_product_termstruct_mapping_get_product_type(mapping),
        mapping
        );
}

RQ_EXPORT rq_product_termstruct_mapping_t 
rq_product_termstruct_mapping_mgr_get(rq_product_termstruct_mapping_mgr_t m, const char *product_type)
{
    return (rq_product_termstruct_mapping_t) rq_tree_rb_find(m->mappings, product_type);
}
