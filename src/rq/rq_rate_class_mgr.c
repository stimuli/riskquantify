/*
** rq_rate_class_mgr.c
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
#include "rq_rate_class_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


void
_rq_rate_class_mgr_free(void *p)
{
    rq_rate_class_free((rq_rate_class_t)p);
}

void
_rq_rate_class_mgr_node_free(void *p)
{
    struct rq_rate_class_mgr_node *n = (struct rq_rate_class_mgr_node *)p;
    RQ_FREE((char *)n->type_id);
    rq_tree_rb_free(n->rate_classes);
}

RQ_EXPORT rq_rate_class_mgr_t
rq_rate_class_mgr_alloc()
{
    struct rq_rate_class_mgr *r = (struct rq_rate_class_mgr *)RQ_MALLOC(sizeof(struct rq_rate_class_mgr));
    r->rate_classes = rq_tree_rb_alloc(_rq_rate_class_mgr_free, (int (*)(const void *, const void *))strcmp);
    r->rate_class_types = rq_tree_rb_alloc(_rq_rate_class_mgr_node_free, (int (*)(const void *, const void *))strcmp);

    return r;
}

RQ_EXPORT void  
rq_rate_class_mgr_free(rq_rate_class_mgr_t r)
{
    /* free this first as the r->rate_classes tree owns the objects */
    rq_tree_rb_free(r->rate_class_types); 

    rq_tree_rb_free(r->rate_classes);
}

RQ_EXPORT int
rq_rate_class_mgr_is_null(rq_rate_class_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_rate_class_mgr_add(rq_rate_class_mgr_t r, rq_rate_class_t rate_class)
{
    struct rq_rate_class_mgr_node *n;
    rq_tree_rb_add(r->rate_classes, rq_rate_class_get_id(rate_class), rate_class);
    n = (struct rq_rate_class_mgr_node *)rq_tree_rb_find(r->rate_class_types, rq_rate_class_get_type_id(rate_class));
    if (!n)
    {
        n = (struct rq_rate_class_mgr_node *)RQ_MALLOC(sizeof(struct rq_rate_class_mgr_node));
        n->type_id = RQ_STRDUP(rq_rate_class_get_type_id(rate_class));
        n->rate_classes = rq_tree_rb_alloc(NULL, (int (*)(const void *, const void *))strcmp);
        rq_tree_rb_add(r->rate_class_types, n->type_id, n);
    }

    rq_tree_rb_add(n->rate_classes, rq_rate_class_get_id(rate_class), rate_class);
}

RQ_EXPORT rq_rate_class_t 
rq_rate_class_mgr_find(rq_rate_class_mgr_t r, const char *id)
{
    return (rq_rate_class_t) rq_tree_rb_find(r->rate_classes, id);
}
