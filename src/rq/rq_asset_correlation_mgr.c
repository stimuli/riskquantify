/*
** rq_asset_correlation_mgr.c
**
** Written by Hendra
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
#include "rq_asset_correlation_mgr.h"
#include "rq_asset_asset_correlation.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


/* -- code -------------------------------------------------------- */

RQ_EXPORT rq_asset_correlation_mgr_t 
rq_asset_correlation_mgr_clone(rq_asset_correlation_mgr_t m)
{
    struct rq_asset_correlation_mgr* cm = (struct rq_asset_correlation_mgr *)RQ_MALLOC(sizeof(struct rq_asset_correlation_mgr));
    cm->tree = rq_tree_rb_clone(m->tree, (const void *(*)(const void *))rq_asset_correlation_get_key, (void *(*)(const void *))rq_asset_correlation_clone);

    return cm;
}

RQ_EXPORT rq_asset_correlation_mgr_t 
rq_asset_correlation_mgr_alloc()
{
    struct rq_asset_correlation_mgr* mgr = (struct rq_asset_correlation_mgr *)RQ_CALLOC(1, sizeof(struct rq_asset_correlation_mgr));
    mgr->tree = rq_tree_rb_alloc(
        (void (*)(void *))rq_asset_correlation_free, 
        (int (*)(const void *, const void *))strcmp
        );
    return mgr;
}

RQ_EXPORT void 
rq_asset_correlation_mgr_free(rq_asset_correlation_mgr_t m)
{
    rq_tree_rb_free(m->tree);
    RQ_FREE(m);
}

RQ_EXPORT void 
rq_asset_correlation_mgr_clear(rq_asset_correlation_mgr_t m)
{
    rq_tree_rb_clear(m->tree);
}

RQ_EXPORT int 
rq_asset_correlation_mgr_get_correlation(
    rq_asset_correlation_mgr_t m, 
    const char* asset1,
    const char* asset2,
    double* correlation
    )
{
    char* key = rq_asset_correlation_build_assetKey(asset1, asset2);
    rq_asset_correlation_t er = rq_tree_rb_find(m->tree, key);
    RQ_FREE(key);

    if (er != NULL)
    {
        *correlation = rq_asset_correlation_get_correlation(er);
        return 0;
    }

    key = rq_asset_correlation_build_assetKey(asset2, asset1);
    er = rq_tree_rb_find(m->tree, key);
    RQ_FREE(key);

    if (er != NULL)
    {
        *correlation = rq_asset_correlation_get_correlation(er);
        return 0;
    }
    else
    {
        *correlation = 0.0;
        return 1;
    }
}

RQ_EXPORT void 
rq_asset_correlation_mgr_add(
    rq_asset_correlation_mgr_t m, 
    const char* asset1,
    const char* asset2,
    double correlation
    )
{
    rq_asset_correlation_t er = rq_asset_correlation_build(asset1, asset2, correlation);
    rq_tree_rb_add(m->tree, (void *)rq_asset_correlation_get_key(er), er);
}

RQ_EXPORT int 
rq_asset_correlation_mgr_is_null(rq_asset_correlation_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_asset_correlation_mgr_iterator_t 
rq_asset_correlation_mgr_iterator_alloc()
{
    rq_asset_correlation_mgr_iterator_t acmi = (rq_asset_correlation_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_asset_correlation_mgr_iterator));
    acmi->asset_correlation_it = rq_tree_rb_iterator_alloc();
    return acmi;
}

RQ_EXPORT void 
rq_asset_correlation_mgr_iterator_free(rq_asset_correlation_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->asset_correlation_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_asset_correlation_mgr_begin(rq_asset_correlation_mgr_t m, rq_asset_correlation_mgr_iterator_t it)
{
    rq_tree_rb_begin(m->tree, it->asset_correlation_it);
}

RQ_EXPORT int 
rq_asset_correlation_mgr_at_end(rq_asset_correlation_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->asset_correlation_it);
}

RQ_EXPORT void 
rq_asset_correlation_mgr_next(rq_asset_correlation_mgr_iterator_t it)
{
    rq_tree_rb_next(it->asset_correlation_it);
}

RQ_EXPORT rq_asset_correlation_t 
rq_asset_correlation_mgr_iterator_deref(rq_asset_correlation_mgr_iterator_t i)
{
    return (rq_asset_correlation_t) rq_tree_rb_iterator_deref(i->asset_correlation_it);
}
