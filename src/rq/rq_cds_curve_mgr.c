/*
** rq_cds_curve_mgr.c
**
** Written by Todd Cooper
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
#include "rq_cds_curve_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_cds_curve_mgr_t 
rq_cds_curve_mgr_alloc()
{
    struct rq_cds_curve_mgr *m = 
        (struct rq_cds_curve_mgr *)RQ_MALLOC(sizeof(struct rq_cds_curve_mgr));
    m->tree = rq_tree_rb_alloc((void (*)(void *))rq_cds_curve_free, (int (*)(const void *, const void *))strcmp);
    return m;
}

RQ_EXPORT rq_cds_curve_mgr_t 
rq_cds_curve_mgr_clone(rq_cds_curve_mgr_t cds_curve_mgr)
{
    struct rq_cds_curve_mgr *m = 
        (struct rq_cds_curve_mgr *)RQ_MALLOC(sizeof(struct rq_cds_curve_mgr));
    m->tree = rq_tree_rb_clone(
        cds_curve_mgr->tree, 
		(const void *(*)(const void *))rq_cds_curve_get_termstruct_id,
        (void *(*)(const void *))rq_cds_curve_clone
        );

    return m;
}


RQ_EXPORT void 
rq_cds_curve_mgr_clear(rq_cds_curve_mgr_t cds_curve_mgr)
{
    rq_tree_rb_clear(cds_curve_mgr->tree);
}

RQ_EXPORT void 
rq_cds_curve_mgr_free(rq_cds_curve_mgr_t cds_curve_mgr)
{
    rq_tree_rb_free(cds_curve_mgr->tree);
    RQ_FREE(cds_curve_mgr);
}

RQ_EXPORT void 
rq_cds_curve_mgr_add(rq_cds_curve_mgr_t cds_curve_mgr, rq_cds_curve_t c)
{
    rq_tree_rb_add(cds_curve_mgr->tree, (void *)rq_cds_curve_get_termstruct_id(c), c);
}

RQ_EXPORT rq_cds_curve_t
rq_cds_curve_mgr_get(const rq_cds_curve_mgr_t cds_curve_mgr, const char *ccypair_asset_id)
{
    return (rq_cds_curve_t) rq_tree_rb_find(cds_curve_mgr->tree, (void *)ccypair_asset_id);
}

RQ_EXPORT int
rq_cds_curve_mgr_is_null(rq_cds_curve_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_cds_curve_mgr_iterator_t 
rq_cds_curve_mgr_iterator_alloc()
{
    rq_cds_curve_mgr_iterator_t fcmi = (rq_cds_curve_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_cds_curve_mgr_iterator));
    fcmi->cds_curve_it = rq_tree_rb_iterator_alloc();
    return fcmi;
}

RQ_EXPORT void 
rq_cds_curve_mgr_iterator_free(rq_cds_curve_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->cds_curve_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_cds_curve_mgr_begin(rq_cds_curve_mgr_t cds_curve_mgr, rq_cds_curve_mgr_iterator_t it)
{
    rq_tree_rb_begin(cds_curve_mgr->tree, it->cds_curve_it);
}

RQ_EXPORT int 
rq_cds_curve_mgr_at_end(rq_cds_curve_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->cds_curve_it);
}

RQ_EXPORT void 
rq_cds_curve_mgr_next(rq_cds_curve_mgr_iterator_t it)
{
    rq_tree_rb_next(it->cds_curve_it);
}

RQ_EXPORT rq_cds_curve_t 
rq_cds_curve_mgr_iterator_deref(rq_cds_curve_mgr_iterator_t i)
{
    return (rq_cds_curve_t) rq_tree_rb_iterator_deref(i->cds_curve_it);
}
