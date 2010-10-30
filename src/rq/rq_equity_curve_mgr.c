/*
** rq_equity_curve_mgr.c
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
#include "rq_equity_curve_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>

static void
free_func(void *p)
{
    rq_equity_curve_free((rq_equity_curve_t)p);
}

RQ_EXPORT rq_equity_curve_mgr_t
rq_equity_curve_mgr_alloc()
{
    struct rq_equity_curve_mgr *m = (struct rq_equity_curve_mgr *)RQ_MALLOC(sizeof(struct rq_equity_curve_mgr));
    m->curves = rq_tree_rb_alloc(free_func, (int (*)(const void *, const void *))strcmp);

    return m;
}

RQ_EXPORT void  
rq_equity_curve_mgr_free(rq_equity_curve_mgr_t m)
{
    rq_tree_rb_free(m->curves);
    RQ_FREE(m);
}

RQ_EXPORT rq_equity_curve_mgr_t 
rq_equity_curve_mgr_clone(rq_equity_curve_mgr_t m)
{
    rq_equity_curve_mgr_t ecmgr = (rq_equity_curve_mgr_t)RQ_MALLOC(sizeof(struct rq_equity_curve_mgr));
	ecmgr->curves = rq_tree_rb_clone(m->curves, (const void *(*)(const void *))rq_equity_curve_get_termstruct_id, (void *(*)(const void *))rq_equity_curve_clone);

    return ecmgr;
}

RQ_EXPORT void 
rq_equity_curve_mgr_clear(rq_equity_curve_mgr_t m)
{
    rq_tree_rb_clear(m->curves);
}

RQ_EXPORT int
rq_equity_curve_mgr_is_null(rq_equity_curve_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_equity_curve_mgr_add(rq_equity_curve_mgr_t m, rq_equity_curve_t equity_curve)
{
    rq_tree_rb_add(m->curves, rq_equity_curve_get_termstruct_id(equity_curve), equity_curve);
}

RQ_EXPORT rq_equity_curve_t
rq_equity_curve_mgr_get(rq_equity_curve_mgr_t m, const char *termstruct_id)
{
    return (rq_equity_curve_t) rq_tree_rb_find(m->curves, termstruct_id);
}

RQ_EXPORT rq_equity_curve_mgr_iterator_t 
rq_equity_curve_mgr_iterator_alloc()
{
    rq_equity_curve_mgr_iterator_t ecmi = (rq_equity_curve_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_equity_curve_mgr_iterator));
    ecmi->curve_it = rq_tree_rb_iterator_alloc();
    return ecmi;
}

RQ_EXPORT void 
rq_equity_curve_mgr_iterator_free(rq_equity_curve_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->curve_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_equity_curve_mgr_begin(rq_equity_curve_mgr_t m, rq_equity_curve_mgr_iterator_t it)
{
    rq_tree_rb_begin(m->curves, it->curve_it);
}

RQ_EXPORT int 
rq_equity_curve_mgr_at_end(rq_equity_curve_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->curve_it);
}

RQ_EXPORT void 
rq_equity_curve_mgr_next(rq_equity_curve_mgr_iterator_t it)
{
    rq_tree_rb_next(it->curve_it);
}

RQ_EXPORT rq_equity_curve_t 
rq_equity_curve_mgr_iterator_deref(rq_equity_curve_mgr_iterator_t i)
{
    return (rq_equity_curve_t) rq_tree_rb_iterator_deref(i->curve_it);
}
