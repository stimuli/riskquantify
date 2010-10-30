/*
** rq_rate_mgr.c
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
#include "rq_rate_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT int
rq_rate_mgr_is_null(rq_rate_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_rate_mgr_t 
rq_rate_mgr_alloc()
{
    rq_rate_mgr_t rate_mgr = (rq_rate_mgr_t)RQ_MALLOC(sizeof(struct rq_rate_mgr));
    rate_mgr->rates = rq_tree_rb_alloc((void (*)(void *))rq_rate_free, (int (*)(const void *, const void *))strcmp);
	rate_mgr->perturbation_mgr = NULL;

    return rate_mgr;
}

RQ_EXPORT void 
rq_rate_mgr_copy(rq_rate_mgr_t rate_mgr_dst, rq_rate_mgr_t rate_mgr_src)
{
    rq_tree_rb_copy(rate_mgr_dst->rates, rate_mgr_src->rates, (const void *(*)(const void *))rq_rate_get_rate_class_id, (void *(*)(const void *))rq_rate_clone);
	rate_mgr_dst->perturbation_mgr = rate_mgr_src->perturbation_mgr;
}

RQ_EXPORT rq_rate_mgr_t 
rq_rate_mgr_clone(rq_rate_mgr_t rate_mgr)
{
    rq_rate_mgr_t rmgr = (rq_rate_mgr_t)RQ_MALLOC(sizeof(struct rq_rate_mgr));

    rmgr->rates = rq_tree_rb_clone(rate_mgr->rates, (const void *(*)(const void *))rq_rate_get_rate_class_id, (void *(*)(const void *))rq_rate_clone);
	rmgr->perturbation_mgr = rate_mgr->perturbation_mgr;

    return rmgr;
}

RQ_EXPORT void 
rq_rate_mgr_free(rq_rate_mgr_t rate_mgr)
{
    rq_tree_rb_free(rate_mgr->rates);
    RQ_FREE(rate_mgr);
}

RQ_EXPORT void 
rq_rate_mgr_add(rq_rate_mgr_t rate_mgr, rq_rate_t rate)
{
    rq_tree_rb_add(rate_mgr->rates, (void *)rq_rate_get_rate_class_id(rate), rate);
	rate->perturbation_mgr = rate_mgr->perturbation_mgr;
}

RQ_EXPORT rq_rate_t 
rq_rate_mgr_find(const rq_rate_mgr_t rate_mgr, const char *rate_class_id)
{
    return (rq_rate_t) rq_tree_rb_find(rate_mgr->rates, (void *)rate_class_id);
}

RQ_EXPORT rq_rate_mgr_iterator_t 
rq_rate_mgr_iterator_alloc()
{
    rq_rate_mgr_iterator_t ycmi = (rq_rate_mgr_iterator_t)RQ_MALLOC(sizeof(struct rq_rate_mgr_iterator));
    ycmi->rate_it = rq_tree_rb_iterator_alloc();
    return ycmi;
}

RQ_EXPORT void 
rq_rate_mgr_iterator_free(rq_rate_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->rate_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_rate_mgr_begin(rq_rate_mgr_t ycmgr, rq_rate_mgr_iterator_t it)
{
    rq_tree_rb_begin(ycmgr->rates, it->rate_it);
}

RQ_EXPORT int 
rq_rate_mgr_at_end(rq_rate_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->rate_it);
}

RQ_EXPORT void 
rq_rate_mgr_next(rq_rate_mgr_iterator_t it)
{
    rq_tree_rb_next(it->rate_it);
}

RQ_EXPORT rq_rate_t 
rq_rate_mgr_iterator_deref(rq_rate_mgr_iterator_t i)
{
    return (rq_rate_t) rq_tree_rb_iterator_deref(i->rate_it);
}

RQ_EXPORT void 
rq_rate_mgr_clear(rq_rate_mgr_t rate_mgr)
{
    rq_tree_rb_clear(rate_mgr->rates);
}

RQ_EXPORT void 
rq_rate_mgr_set_perturbation_mgr(const rq_rate_mgr_t rate_mgr, rq_perturbation_mgr_t perturbation_mgr)
{
	rate_mgr->perturbation_mgr = perturbation_mgr;
	rq_rate_mgr_sync_perturbation_mgr(rate_mgr);
}

RQ_EXPORT rq_perturbation_mgr_t 
rq_rate_mgr_get_perturbation_mgr(const rq_rate_mgr_t rate_mgr)
{
	return rate_mgr->perturbation_mgr;
}

RQ_EXPORT void 
rq_rate_mgr_sync_perturbation_mgr(rq_rate_mgr_t rate_mgr)
{
	int r;
	rq_rate_mgr_iterator_t rate_mgr_it = rq_rate_mgr_iterator_alloc();
	rq_rate_mgr_begin(rate_mgr, rate_mgr_it);
	for (r = 0; !rq_rate_mgr_at_end(rate_mgr_it); r++)
	{
		rq_rate_t rate = rq_rate_mgr_iterator_deref(rate_mgr_it);
		rate->perturbation_mgr = rate_mgr->perturbation_mgr;
		rq_rate_mgr_next(rate_mgr_it);
	}
	rq_rate_mgr_iterator_free(rate_mgr_it);
}
