/*
** rq_asset_mgr.c
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
#include "rq_asset_mgr.h"
#include "rq_asset_ccypair.h"
#include "rq_defs.h"
#include "rq_tree_rb.h"
#include "rq_tokenizer.h"
#include <stdlib.h>
#include <string.h>

/* -- structures -------------------------------------------------- */

/* -- external hidden prototypes ---------------------------------- */
void rq_asset_list_push_back(rq_asset_list_t asset_list, rq_asset_t asset);

/* -- code -------------------------------------------------------- */
RQ_EXPORT void
rq_asset_mgr_node_free(void *p)
{
    struct rq_asset_mgr_node *n = (struct rq_asset_mgr_node *)p;
    RQ_FREE((char *)n->asset_type_id);
    rq_tree_rb_free(n->asset_type_tree);
    RQ_FREE(n);
}

RQ_EXPORT struct rq_asset_mgr_node *
rq_asset_mgr_node_alloc(const char *asset_type_id)
{
    struct rq_asset_mgr_node *n = (struct rq_asset_mgr_node *)RQ_MALLOC(sizeof(struct rq_asset_mgr_node));
    n->asset_type_id = RQ_STRDUP(asset_type_id);
    n->asset_type_tree = rq_tree_rb_alloc((void (*)(void *))rq_asset_free, (int (*)(const void *, const void *))strcmp);
    return n;
}

RQ_EXPORT void
rq_asset_mgr_register_asset_type(rq_asset_mgr_t asset_mgr, const char *asset_type_id, int (*cmp_func)(const void *, const void *))
{
    struct rq_asset_mgr_node *n = (struct rq_asset_mgr_node *)rq_tree_rb_find(asset_mgr->asset_type_tree, (void *)asset_type_id);
    if (!n)
    {
        n = (struct rq_asset_mgr_node *)RQ_MALLOC(sizeof(struct rq_asset_mgr_node));
        n->asset_type_id = RQ_STRDUP(asset_type_id);
        n->asset_type_tree = rq_tree_rb_alloc((void (*)(void *))rq_asset_free, cmp_func);
        rq_tree_rb_add(asset_mgr->asset_type_tree, (void *)n->asset_type_id, n);
    }
}

RQ_EXPORT rq_asset_mgr_t 
rq_asset_mgr_alloc()
{
    struct rq_asset_mgr *asset_mgr = (struct rq_asset_mgr *)RQ_MALLOC(sizeof(struct rq_asset_mgr));
    asset_mgr->asset_type_tree = rq_tree_rb_alloc(rq_asset_mgr_node_free, (int (*)(const void *, const void *))strcmp);
    asset_mgr->asset_tree = rq_tree_rb_alloc(NULL, (int (*)(const void *, const void *))strcmp);
    asset_mgr->asset_ranking = rq_string_list_alloc();
	return asset_mgr;
}

RQ_EXPORT void 
rq_asset_mgr_free(rq_asset_mgr_t asset_mgr)
{
    rq_tree_rb_free(asset_mgr->asset_type_tree);
    rq_tree_rb_free(asset_mgr->asset_tree);
    rq_string_list_free(asset_mgr->asset_ranking);
    RQ_FREE(asset_mgr);
}

RQ_EXPORT void 
rq_asset_mgr_clear(rq_asset_mgr_t asset_mgr)
{
    rq_tree_rb_clear(asset_mgr->asset_type_tree);
    rq_tree_rb_clear(asset_mgr->asset_tree);
    rq_array_clear(asset_mgr->asset_ranking->strings);
}

RQ_EXPORT void
rq_asset_mgr_add(
    rq_asset_mgr_t asset_mgr,
    rq_asset_t asset
    )
{
    const char *asset_id = rq_asset_get_asset_id(asset);
    const char *asset_type_id = rq_asset_get_asset_type_id(asset);
    struct rq_asset_mgr_node *n = (struct rq_asset_mgr_node *)rq_tree_rb_find(asset_mgr->asset_type_tree, (void *)asset_type_id);
    if (!n)
    {
        n = rq_asset_mgr_node_alloc(asset_type_id);
        rq_tree_rb_add(asset_mgr->asset_type_tree, (void *)n->asset_type_id, n);
    }
    rq_tree_rb_add(n->asset_type_tree, (void *)asset_id, asset);
    rq_tree_rb_add(asset_mgr->asset_tree, (void *)asset_id, asset);
}

RQ_EXPORT rq_asset_t
rq_asset_mgr_get(
    const rq_asset_mgr_t asset_mgr,
    const char *asset_id
    )
{
    return (rq_asset_t)rq_tree_rb_find(asset_mgr->asset_tree, (void *)asset_id);
}

RQ_EXPORT rq_asset_t
rq_asset_mgr_find(
    const rq_asset_mgr_t asset_mgr,
    const char *asset_type_id,
    int (*cmp)(const void *, const void *), 
    void *cmp_data
    )
{
    struct rq_asset_mgr_node *n = rq_tree_rb_find(asset_mgr->asset_type_tree, (void *)asset_type_id);
    rq_asset_t asset = NULL;
    
    if (n)
    {
        asset = 
            (rq_asset_t)rq_tree_rb_locate(n->asset_type_tree, cmp, cmp_data);
    }

    return asset;
}

RQ_EXPORT int
rq_asset_mgr_is_null(rq_asset_mgr_t obj)
{
    return obj == NULL;
}

void
_rq_asset_mgr_list_assets_for_type(void *user_data, void *node_data)
{
    rq_asset_list_push_back((rq_asset_list_t)user_data, (rq_asset_t)node_data);
}

RQ_EXPORT rq_string_list_t
rq_asset_mgr_get_asset_type_list(const rq_asset_mgr_t mgr)
{
    rq_tree_rb_iterator_t tree_it = rq_tree_rb_iterator_alloc();
    rq_string_list_t asset_types = rq_string_list_alloc();
    rq_tree_rb_begin(mgr->asset_type_tree, tree_it);
    while (!rq_tree_rb_at_end(tree_it))
    {
        struct rq_asset_mgr_node *n = (struct rq_asset_mgr_node *)
            rq_tree_rb_iterator_deref(tree_it);
        rq_string_list_add(asset_types, n->asset_type_id);
        rq_tree_rb_next(tree_it);
    }
    rq_tree_rb_iterator_free(tree_it);
    return asset_types;
}

RQ_EXPORT rq_asset_list_t
rq_asset_mgr_list_assets_for_type(
    const rq_asset_mgr_t mgr,
    const char *asset_type_id
    )
{
    rq_asset_list_t asset_list = rq_asset_list_alloc();
    void *t = rq_tree_rb_find(mgr->asset_type_tree, asset_type_id);
    if (t)
    {
        struct rq_asset_mgr_node *mgr_node = (struct rq_asset_mgr_node *)t;
        rq_tree_rb_traverse_inorder(
            mgr_node->asset_type_tree, 
            asset_list,
            _rq_asset_mgr_list_assets_for_type
            );
    }

    return asset_list;
}

RQ_EXPORT void
rq_asset_mgr_add_ranking_list(rq_asset_mgr_t asset_mgr, const char *asset_ids)
{
    if (asset_ids && asset_ids[0])
    {
        char ccy[256];
        while (!RQ_IS_NULL(rq_tokenizer_get_token(&asset_ids, ccy, 256, ",", " ", "\"")))
        {
            if (strlen(ccy))
            {
                rq_string_list_add(asset_mgr->asset_ranking, ccy);
            }
        }
    }
    else
    {
        rq_string_list_add(asset_mgr->asset_ranking, "USD");
    }
}

RQ_EXPORT rq_string_list_t
rq_asset_mgr_get_ranking_list(const rq_asset_mgr_t asset_mgr)
{
    return asset_mgr->asset_ranking;
}
