/*
** rq_spot_price_mgr.c
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
#include "rq_spot_price_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


/* -- code -------------------------------------------------------- */
static void
spot_price_free_func(void *p)
{
    rq_spot_price_free((rq_spot_price_t)p);
}

static void
node_free_func(void *p)
{
    struct rq_spot_price_mgr_node *node = (struct rq_spot_price_mgr_node *)p;
    RQ_FREE((char *)node->asset_type_id);
    rq_tree_rb_free(node->prices);
    RQ_FREE(node);
}

RQ_EXPORT rq_spot_price_mgr_t
rq_spot_price_mgr_alloc()
{
    struct rq_spot_price_mgr *spm = (struct rq_spot_price_mgr *)RQ_MALLOC(sizeof(struct rq_spot_price_mgr));
    spm->asset_types = rq_tree_rb_alloc(node_free_func, (int (*)(const void *, const void *))strcmp);
	return spm;
}

RQ_EXPORT void
rq_spot_price_mgr_free(rq_spot_price_mgr_t spm)
{
    rq_tree_rb_free(spm->asset_types);
    RQ_FREE(spm);
}

RQ_EXPORT void 
rq_spot_price_mgr_clear(rq_spot_price_mgr_t spm)
{
   rq_tree_rb_clear(spm->asset_types);
}

static const void *
rq_spot_price_get_key(const void *p)
{
    return (void *)rq_spot_price_get_asset_id((rq_spot_price_t)p);
}

static void *
_rq_spot_price_clone(const void *p)
{
    return rq_spot_price_clone((rq_spot_price_t)p);
}

static void *
rq_spot_price_mgr_node_get_key(void *p)
{
    struct rq_spot_price_mgr_node *n = (struct rq_spot_price_mgr_node *)p;
    return (void *)n->asset_type_id;
}

static void *
rq_spot_price_mgr_node_clone(void *n)
{
    struct rq_spot_price_mgr_node *node = (struct rq_spot_price_mgr_node *)n;
    struct rq_spot_price_mgr_node *cl_node = (struct rq_spot_price_mgr_node *)
        RQ_MALLOC(sizeof(struct rq_spot_price_mgr_node));
    cl_node->asset_type_id = RQ_STRDUP(node->asset_type_id);
    cl_node->prices = rq_tree_rb_clone(
        node->prices,
        (const void *(*)(const void *))rq_spot_price_get_key,
        (void *(*)(const void *))_rq_spot_price_clone
        );
    return cl_node;
}

RQ_EXPORT rq_spot_price_mgr_t 
rq_spot_price_mgr_clone(rq_spot_price_mgr_t spm)
{
    struct rq_spot_price_mgr *spm_clone = (struct rq_spot_price_mgr *)RQ_MALLOC(sizeof(struct rq_spot_price_mgr));

    spm_clone->asset_types = rq_tree_rb_clone(
        spm->asset_types,
        (const void *(*)(const void *))rq_spot_price_mgr_node_get_key,
        (void *(*)(const void *))rq_spot_price_mgr_node_clone
        );

    return spm_clone;
}

RQ_EXPORT int
rq_spot_price_mgr_is_null(rq_spot_price_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_spot_price_mgr_add(rq_spot_price_mgr_t spm, const char *asset_type_id, const char *asset_id, double price)
{
    rq_spot_price_t spot_price;
    struct rq_spot_price_mgr_node *node = rq_tree_rb_find(
        spm->asset_types,
        asset_type_id
        );
    if (!node)
    {
        node = (struct rq_spot_price_mgr_node *)RQ_MALLOC(sizeof(struct rq_spot_price_mgr_node));
        node->asset_type_id = RQ_STRDUP(asset_type_id);
        node->prices = rq_tree_rb_alloc(spot_price_free_func, (int (*)(const void *, const void *))strcmp);
        rq_tree_rb_add(spm->asset_types, node->asset_type_id, node);
    }
    else
    {
        spot_price = rq_tree_rb_find(node->prices, asset_id);
        if (spot_price)
        {
            rq_spot_price_set_price(spot_price, price);
            return;
        }
    }

    spot_price = rq_spot_price_alloc(asset_id, price);
    rq_tree_rb_add(node->prices, rq_spot_price_get_asset_id(spot_price), spot_price);
}

RQ_EXPORT double 
rq_spot_price_mgr_get_price(rq_spot_price_mgr_t spm, const char *asset_type_id, const char *asset_id)
{
    struct rq_spot_price_mgr_node *node = rq_tree_rb_find(
        spm->asset_types,
        asset_type_id
        );
    if (node)
    {
        rq_spot_price_t spot_price;
        spot_price = rq_tree_rb_find(node->prices, asset_id);
        if (spot_price)
            return rq_spot_price_get_price(spot_price);
    }

    return 0.0;
}
