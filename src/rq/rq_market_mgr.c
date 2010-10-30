/*
** rq_market_mgr.c
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
#include "rq_market_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>

static void
_rq_market_mgr_free_func(void *p)
{
    rq_market_free((rq_market_t)p);
}

static int
_rq_market_mgr_cmp_func(const void *p1, const void *p2)
{
    rq_date d1 = (rq_date)p1;
    rq_date d2 = (rq_date)p2;
    return d1 - d2;
}

RQ_EXPORT rq_market_mgr_t 
rq_market_mgr_alloc()
{
    struct rq_market_mgr *market_mgr = 
        (struct rq_market_mgr *)RQ_MALLOC(sizeof(market_mgr));
    market_mgr->markets = rq_tree_rb_alloc(
        _rq_market_mgr_free_func,
        _rq_market_mgr_cmp_func
        );
    return market_mgr;
}

RQ_EXPORT void 
rq_market_mgr_free(rq_market_mgr_t market_mgr)
{
    rq_tree_rb_free(market_mgr->markets);
    RQ_FREE(market_mgr);
}

RQ_EXPORT void 
rq_market_mgr_add(rq_market_mgr_t market_mgr, rq_market_t market)
{
    rq_tree_rb_add(market_mgr->markets, (const void *)rq_market_get_market_date(market), market);
}

RQ_EXPORT rq_market_t 
rq_market_mgr_get(rq_market_mgr_t market_mgr, rq_date market_date)
{
    return (rq_market_t) rq_tree_rb_find(market_mgr->markets, (const void *)market_date);
}

RQ_EXPORT int
rq_market_mgr_is_null(rq_market_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_market_mgr_iterator_t 
rq_market_mgr_iterator_alloc()
{
    rq_market_mgr_iterator_t mmi = (rq_market_mgr_iterator_t)RQ_MALLOC(sizeof(struct rq_market_mgr_iterator));
    mmi->market_it = rq_tree_rb_iterator_alloc();
    return mmi;
}

RQ_EXPORT void 
rq_market_mgr_iterator_free(rq_market_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->market_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_market_mgr_begin(rq_market_mgr_t market_mgr, rq_market_mgr_iterator_t it)
{
    rq_tree_rb_begin(market_mgr->markets, it->market_it);
}

RQ_EXPORT int
rq_market_mgr_at_end(rq_market_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->market_it);
}

RQ_EXPORT void
rq_market_mgr_next(rq_market_mgr_iterator_t it)
{
    rq_tree_rb_next(it->market_it);
}

RQ_EXPORT rq_market_t
rq_market_mgr_iterator_deref(rq_market_mgr_iterator_t it)
{
    return (rq_market_t) rq_tree_rb_iterator_deref(it->market_it);
}
