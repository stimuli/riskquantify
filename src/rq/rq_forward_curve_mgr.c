/*
** rq_forward_curve_mgr.c
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
#include "rq_forward_curve_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_forward_curve_mgr_t 
rq_forward_curve_mgr_alloc()
{
    struct rq_forward_curve_mgr *m = 
        (struct rq_forward_curve_mgr *)RQ_MALLOC(sizeof(struct rq_forward_curve_mgr));
    m->tree = rq_tree_rb_alloc((void (*)(void *))rq_forward_curve_free, (int (*)(const void *, const void *))strcmp);
    return m;
}

RQ_EXPORT rq_forward_curve_mgr_t 
rq_forward_curve_mgr_clone(rq_forward_curve_mgr_t forward_curve_mgr)
{
    struct rq_forward_curve_mgr *m = 
        (struct rq_forward_curve_mgr *)RQ_MALLOC(sizeof(struct rq_forward_curve_mgr));
    m->tree = rq_tree_rb_clone(
        forward_curve_mgr->tree, 
		(const void *(*)(const void *))rq_forward_curve_get_curve_id,
        (void *(*)(const void *))rq_forward_curve_clone
        );

    return m;
}


RQ_EXPORT void 
rq_forward_curve_mgr_clear(rq_forward_curve_mgr_t forward_curve_mgr)
{
    rq_tree_rb_clear(forward_curve_mgr->tree);
}

RQ_EXPORT void 
rq_forward_curve_mgr_free(rq_forward_curve_mgr_t forward_curve_mgr)
{
    rq_tree_rb_free(forward_curve_mgr->tree);
    RQ_FREE(forward_curve_mgr);
}

RQ_EXPORT void 
rq_forward_curve_mgr_add(rq_forward_curve_mgr_t forward_curve_mgr, rq_forward_curve_t c)
{
    rq_tree_rb_add(forward_curve_mgr->tree, (void *)rq_forward_curve_get_curve_id(c), c);
}

RQ_EXPORT rq_forward_curve_t
rq_forward_curve_mgr_get(const rq_forward_curve_mgr_t forward_curve_mgr, const char *ccypair_asset_id)
{
    return (rq_forward_curve_t) rq_tree_rb_find(forward_curve_mgr->tree, (void *)ccypair_asset_id);
}

RQ_EXPORT int
rq_forward_curve_mgr_is_null(rq_forward_curve_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_forward_curve_mgr_iterator_t 
rq_forward_curve_mgr_iterator_alloc()
{
    rq_forward_curve_mgr_iterator_t fcmi = (rq_forward_curve_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_forward_curve_mgr_iterator));
    fcmi->forward_curve_it = rq_tree_rb_iterator_alloc();
    return fcmi;
}

RQ_EXPORT void 
rq_forward_curve_mgr_iterator_free(rq_forward_curve_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->forward_curve_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_forward_curve_mgr_begin(rq_forward_curve_mgr_t forward_curve_mgr, rq_forward_curve_mgr_iterator_t it)
{
    rq_tree_rb_begin(forward_curve_mgr->tree, it->forward_curve_it);
}

RQ_EXPORT int 
rq_forward_curve_mgr_at_end(rq_forward_curve_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->forward_curve_it);
}

RQ_EXPORT void 
rq_forward_curve_mgr_next(rq_forward_curve_mgr_iterator_t it)
{
    rq_tree_rb_next(it->forward_curve_it);
}

RQ_EXPORT rq_forward_curve_t 
rq_forward_curve_mgr_iterator_deref(rq_forward_curve_mgr_iterator_t i)
{
    return (rq_forward_curve_t) rq_tree_rb_iterator_deref(i->forward_curve_it);
}
