/*
** rq_asset_termstruct_mapping_mgr.c
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
#include "rq_asset_termstruct_mapping_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>

static void
free_func(void *p)
{
    rq_asset_termstruct_mapping_free((rq_asset_termstruct_mapping_t)p);
}

RQ_EXPORT rq_asset_termstruct_mapping_mgr_t
rq_asset_termstruct_mapping_mgr_alloc()
{
    int i;
    struct rq_asset_termstruct_mapping_mgr *m = (struct rq_asset_termstruct_mapping_mgr *)
        RQ_MALLOC(sizeof(struct rq_asset_termstruct_mapping_mgr));
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        m->mapping_lists[i] = rq_tree_rb_alloc(free_func, (int (*)(const void *, const void *))strcmp);

    return m;
}

RQ_EXPORT void  
rq_asset_termstruct_mapping_mgr_free(rq_asset_termstruct_mapping_mgr_t m)
{
    int i;

    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        rq_tree_rb_free(m->mapping_lists[i]);

    RQ_FREE(m);
}

RQ_EXPORT int
rq_asset_termstruct_mapping_mgr_is_null(rq_asset_termstruct_mapping_mgr_t obj)
{
    return obj == NULL;
}

RQ_EXPORT void 
rq_asset_termstruct_mapping_mgr_add(rq_asset_termstruct_mapping_mgr_t m, rq_asset_termstruct_mapping_t mapping)
{
    rq_tree_rb_t tree = m->mapping_lists[rq_asset_termstruct_mapping_get_termstruct_type(mapping)];
    rq_tree_rb_add(tree, rq_asset_termstruct_mapping_get_asset_id(mapping), mapping);
}

RQ_EXPORT rq_asset_termstruct_mapping_t 
rq_asset_termstruct_mapping_mgr_get(
    rq_asset_termstruct_mapping_mgr_t m,
    enum rq_termstruct_type termstruct_type,
    const char *asset_id
    )
{
    rq_tree_rb_t tree = m->mapping_lists[termstruct_type];
    return (rq_asset_termstruct_mapping_t)rq_tree_rb_find(tree, asset_id);
}
