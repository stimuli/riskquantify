/*
** rq_external_termstruct_mgr.c
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
#include "rq_external_termstruct_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


static void
_rq_external_termstruct_free(void *p)
{
    rq_external_termstruct_t et = (rq_external_termstruct_t)p;
    if (et->free_func)
    {
        (*et->free_func)(et);
    }
    else
    {
        free((char *)et->id);
        free(et);
    }
}

static void *
_rq_external_termstruct_get_id(void *p)
{
    rq_external_termstruct_t et = (rq_external_termstruct_t)p;
    return (void *)et->id;
}

static void *
_rq_external_termstruct_clone(void *p)
{
    rq_external_termstruct_t et_clone = NULL;
    const rq_external_termstruct_t et = (rq_external_termstruct_t)p;
    if (et->clone_func)
    {
        et_clone = (*et->clone_func)(et);
    }
    else
    {
        et_clone = (rq_external_termstruct_t) malloc(sizeof(struct rq_external_termstruct));
        et_clone->id = strdup(et->id);
        et_clone->external_termstruct = (*et->clone_func)(et->external_termstruct);
        et_clone->free_func = et->free_func;
        et_clone->clone_func = et->clone_func;
    }
    return et_clone;
}

RQ_EXPORT rq_external_termstruct_mgr_t
rq_external_termstruct_mgr_alloc()
{
    struct rq_external_termstruct_mgr *etm = (struct rq_external_termstruct_mgr *)malloc(sizeof(struct rq_external_termstruct_mgr));
	etm->tree = rq_tree_rb_alloc(_rq_external_termstruct_free, (int (*)(const void *, const void *))strcmp);

    return etm;
}

RQ_EXPORT void  
rq_external_termstruct_mgr_free(rq_external_termstruct_mgr_t etm)
{
    rq_tree_rb_free(etm->tree);
    RQ_FREE(etm);
}

RQ_EXPORT void 
rq_external_termstruct_mgr_clear(rq_external_termstruct_mgr_t m)
{
	rq_tree_rb_clear(m->tree);
}

RQ_EXPORT int
rq_external_termstruct_mgr_is_null(rq_external_termstruct_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_external_termstruct_mgr_t 
rq_external_termstruct_mgr_clone(const rq_external_termstruct_mgr_t etm)
{
    struct rq_external_termstruct_mgr *etm_clone = (struct rq_external_termstruct_mgr *)malloc(sizeof(struct rq_external_termstruct_mgr));
	etm_clone->tree = rq_tree_rb_clone(etm->tree, (const void *(*)(const void *))_rq_external_termstruct_get_id, (void *(*)(const void *))_rq_external_termstruct_clone);

    return etm_clone;
}

/*
RQ_EXPORT void 
rq_external_termstruct_mgr_add(rq_external_termstruct_mgr_t etm, const char *id, void *external_termstruct, void (*free_func)(void *), void *(*clone_func)(void *))
{
    rq_external_termstruct_t et = (rq_external_termstruct_t)malloc(sizeof(struct rq_external_termstruct));
    et->id = (const char *)strdup(id);
    et->external_termstruct = external_termstruct;
    et->free_func = free_func;
    et->clone_func = clone_func;
    rq_tree_rb_add(etm->tree, et->id, et);
}
*/

RQ_EXPORT void
rq_external_termstruct_mgr_add(rq_external_termstruct_mgr_t et_mgr, rq_external_termstruct_t et)
{
    rq_tree_rb_add(et_mgr->tree, et->id, et);
}

RQ_EXPORT void *
rq_external_termstruct_mgr_get(rq_external_termstruct_mgr_t etm, const char *id)
{
    return rq_tree_rb_find(etm->tree, id);
}
