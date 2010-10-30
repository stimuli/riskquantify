/*
** rq_termstruct_mapping_mgr.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include "rq_termstruct_mapping_mgr.h"
#include <stdlib.h>
#include <string.h>

/*
 * The main tree contains the following nodes. These nodes contain
 * another tree that is keyed on the termstruct group id.
 */
struct rq_mapping_mgr_node {
    const char *asset_id;
    rq_tree_rb_t asset_mappings;
};

static struct rq_mapping_mgr_node *
rq_mapping_mgr_node_alloc(const char *asset_id)
{
    struct rq_mapping_mgr_node *n = (struct rq_mapping_mgr_node *)RQ_MALLOC(sizeof(struct rq_mapping_mgr_node));
    n->asset_id = RQ_STRDUP(asset_id);
    n->asset_mappings = rq_tree_rb_alloc(
        (void (*)(void *))rq_termstruct_mapping_free,
        (int (*)(const void *, const void *))strcmp
        );
    return n;
}

static void
rq_mapping_mgr_node_free(struct rq_mapping_mgr_node *n)
{
    rq_tree_rb_free(n->asset_mappings);
    RQ_FREE((char *)n->asset_id);
    RQ_FREE(n);
}

static void
rq_mapping_mgr_node_add(
    struct rq_mapping_mgr_node *n,
    rq_termstruct_mapping_t mapping
    )
{
    rq_tree_rb_add(n->asset_mappings, mapping->termstruct_group_id, mapping);
}

static rq_termstruct_mapping_t
rq_mapping_mgr_node_find(
    struct rq_mapping_mgr_node *n,
    const char *termstruct_group_id
    )
{
    return (rq_termstruct_mapping_t)rq_tree_rb_find(n->asset_mappings, termstruct_group_id);
}

/** Test whether the rq_termstruct_mapping_mgr is NULL */
RQ_EXPORT int 
rq_termstruct_mapping_mgr_is_null(rq_termstruct_mapping_mgr_t obj)
{
    return obj == NULL;
}

/** Allocate a new termstruct_mapping_mgr
 */
RQ_EXPORT rq_termstruct_mapping_mgr_t 
rq_termstruct_mapping_mgr_alloc()
{
    rq_termstruct_mapping_mgr_t mapping_mgr = (rq_termstruct_mapping_mgr_t)
        RQ_MALLOC(sizeof(struct rq_termstruct_mapping_mgr));
    mapping_mgr->termstruct_mappings = rq_tree_rb_alloc(
        (void (*)(void *))rq_mapping_mgr_node_free,
        (int (*)(const void *, const void *))strcmp
        );
    return mapping_mgr;
}

/** Free an object of type rq_termstruct_mapping_mgr
 */
RQ_EXPORT void 
rq_termstruct_mapping_mgr_free(rq_termstruct_mapping_mgr_t termstruct_mapping_mgr)
{
    rq_tree_rb_free(termstruct_mapping_mgr->termstruct_mappings);
    RQ_FREE(termstruct_mapping_mgr);
}

RQ_EXPORT void 
rq_termstruct_mapping_mgr_add(rq_termstruct_mapping_mgr_t termstruct_mapping_mgr, rq_termstruct_mapping_t termstruct_mapping)
{
    struct rq_mapping_mgr_node *n = (struct rq_mapping_mgr_node *)
        rq_tree_rb_find(termstruct_mapping_mgr->termstruct_mappings, termstruct_mapping->asset_id);
    if (!n)
    {
        n = rq_mapping_mgr_node_alloc(termstruct_mapping->asset_id);
        rq_tree_rb_add(termstruct_mapping_mgr->termstruct_mappings, n->asset_id, n);
    }

    rq_mapping_mgr_node_add(n, termstruct_mapping);
}

RQ_EXPORT rq_termstruct_mapping_t
rq_termstruct_mapping_mgr_find(rq_termstruct_mapping_mgr_t termstruct_mapping_mgr, const char *asset_id, const char *termstruct_group_id)
{
    rq_termstruct_mapping_t termstruct_mapping = NULL;
    struct rq_mapping_mgr_node *mn = (struct rq_mapping_mgr_node *)
        rq_tree_rb_find(termstruct_mapping_mgr->termstruct_mappings, asset_id);
    if (mn)
        termstruct_mapping = rq_mapping_mgr_node_find(mn, termstruct_group_id);

    return termstruct_mapping;
}
