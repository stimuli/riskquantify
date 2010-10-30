/*
** rq_calendar_mgr.c
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
#include "rq_calendar_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>

static void
rq_calendar_mgr_cal_free(void *c)
{
    rq_calendar_free((rq_calendar_t)c);
}

RQ_EXPORT rq_calendar_mgr_t
rq_calendar_mgr_alloc()
{
    struct rq_calendar_mgr *calendar_mgr = 
        (struct rq_calendar_mgr *)RQ_MALLOC(sizeof(struct rq_calendar_mgr));
    calendar_mgr->calendars = rq_tree_rb_alloc(rq_calendar_mgr_cal_free, (int (*)(const void *, const void *))strcmp);

    return calendar_mgr;
}

RQ_EXPORT void 
rq_calendar_mgr_free(rq_calendar_mgr_t calendar_mgr)
{
    rq_tree_rb_free(calendar_mgr->calendars);
}

RQ_EXPORT void 
rq_calendar_mgr_add(rq_calendar_mgr_t calendar_mgr, rq_calendar_t cal)
{
    rq_tree_rb_add(calendar_mgr->calendars, rq_calendar_get_id(cal), cal);
}

RQ_EXPORT rq_calendar_t
rq_calendar_mgr_get(const rq_calendar_mgr_t calendar_mgr, const char *id)
{
    return (rq_calendar_t)rq_tree_rb_find(calendar_mgr->calendars, id);
}

RQ_EXPORT int
rq_calendar_mgr_is_null(rq_calendar_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_calendar_mgr_clear(rq_calendar_mgr_t calendar_mgr)
{
    rq_tree_rb_clear(calendar_mgr->calendars);
}

RQ_EXPORT rq_iterator_t 
rq_calendar_mgr_get_iterator(rq_calendar_mgr_t m)
{
    return rq_tree_rb_get_iterator(m->calendars);
}

