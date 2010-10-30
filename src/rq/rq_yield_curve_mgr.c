/*
** rq_yield_curve_mgr.c
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
#include "rq_yield_curve_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_yield_curve_mgr_t 
rq_yield_curve_mgr_alloc()
{
    rq_yield_curve_mgr_t ycmgr = (rq_yield_curve_mgr_t)RQ_MALLOC(sizeof(struct rq_yield_curve_mgr));
	ycmgr->yield_curves = rq_tree_rb_alloc((void (*)(void *))rq_yield_curve_free, (int (*)(const void *, const void *))strcmp);

    return ycmgr;
}

RQ_EXPORT rq_yield_curve_mgr_t 
rq_yield_curve_mgr_clone(rq_yield_curve_mgr_t m)
{
    rq_yield_curve_mgr_t ycmgr = (rq_yield_curve_mgr_t)RQ_MALLOC(sizeof(struct rq_yield_curve_mgr));
	ycmgr->yield_curves = rq_tree_rb_clone(m->yield_curves, (const void *(*)(const void *))rq_yield_curve_get_curve_id, (void *(*)(const void *))rq_yield_curve_clone);

    return ycmgr;
}

RQ_EXPORT void 
rq_yield_curve_mgr_clear(rq_yield_curve_mgr_t mgr)
{
    rq_tree_rb_clear(mgr->yield_curves);
}


RQ_EXPORT void 
rq_yield_curve_mgr_free(rq_yield_curve_mgr_t mgr)
{
    rq_tree_rb_free(mgr->yield_curves);
    RQ_FREE(mgr);
}

RQ_EXPORT void
rq_yield_curve_mgr_add(rq_yield_curve_mgr_t mgr, rq_yield_curve_t ts)
{
    rq_tree_rb_add(mgr->yield_curves, (void *)rq_yield_curve_get_curve_id(ts), ts);
}


RQ_EXPORT rq_yield_curve_t 
rq_yield_curve_mgr_get(rq_yield_curve_mgr_t mgr, const char *asset_id)
{
    return (rq_yield_curve_t) rq_tree_rb_find(mgr->yield_curves, asset_id);
}

RQ_EXPORT int
rq_yield_curve_mgr_is_null(rq_yield_curve_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_yield_curve_mgr_iterator_t 
rq_yield_curve_mgr_iterator_alloc()
{
    rq_yield_curve_mgr_iterator_t ycmi = (rq_yield_curve_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_yield_curve_mgr_iterator));

    ycmi->yield_curve_it = rq_tree_rb_iterator_alloc();
    return ycmi;
}

RQ_EXPORT void 
rq_yield_curve_mgr_iterator_free(rq_yield_curve_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->yield_curve_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_yield_curve_mgr_begin(rq_yield_curve_mgr_t ycmgr, rq_yield_curve_mgr_iterator_t it)
{
    rq_tree_rb_begin(ycmgr->yield_curves, it->yield_curve_it);
}

RQ_EXPORT int 
rq_yield_curve_mgr_at_end(rq_yield_curve_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->yield_curve_it);
}

RQ_EXPORT void 
rq_yield_curve_mgr_next(rq_yield_curve_mgr_iterator_t it)
{
    rq_tree_rb_next(it->yield_curve_it);
}

RQ_EXPORT rq_yield_curve_t 
rq_yield_curve_mgr_iterator_deref(rq_yield_curve_mgr_iterator_t i)
{
    return (rq_yield_curve_t) rq_tree_rb_iterator_deref(i->yield_curve_it);
}

