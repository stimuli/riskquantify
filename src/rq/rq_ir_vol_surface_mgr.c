/*
** rq_ir_vol_surface_mgr.c
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
#include "rq_ir_vol_surface_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_ir_vol_surface_mgr_t 
rq_ir_vol_surface_mgr_alloc()
{
    rq_ir_vol_surface_mgr_t vsmgr = 
        (rq_ir_vol_surface_mgr_t)RQ_MALLOC(sizeof(struct rq_ir_vol_surface_mgr));
    vsmgr->ir_vol_surfaces = rq_tree_rb_alloc((void (*)(void *))rq_ir_vol_surface_free, (int (*)(const void *, const void *))strcmp);

    return vsmgr;
}

RQ_EXPORT rq_ir_vol_surface_mgr_t 
rq_ir_vol_surface_mgr_clone(rq_ir_vol_surface_mgr_t mgr)
{
    rq_ir_vol_surface_mgr_t vsmgr = 
        (rq_ir_vol_surface_mgr_t)RQ_MALLOC(sizeof(struct rq_ir_vol_surface_mgr));

    vsmgr->ir_vol_surfaces = rq_tree_rb_clone(mgr->ir_vol_surfaces, (const void * (*)(const void *))rq_ir_vol_surface_get_termstruct_id, (void * (*)(const void *))rq_ir_vol_surface_clone);

    return vsmgr;
}


RQ_EXPORT void 
rq_ir_vol_surface_mgr_free(rq_ir_vol_surface_mgr_t vsm)
{
    rq_tree_rb_free(vsm->ir_vol_surfaces);
    RQ_FREE(vsm);
}

RQ_EXPORT void 
rq_ir_vol_surface_mgr_clear(rq_ir_vol_surface_mgr_t vsm)
{
    rq_tree_rb_clear(vsm->ir_vol_surfaces);
}

/**
 * Add a new vol surface to the manager.
 */
RQ_EXPORT void 
rq_ir_vol_surface_mgr_add(
    rq_ir_vol_surface_mgr_t vsm,
    rq_ir_vol_surface_t ir_vol_surface
    )
{
    rq_tree_rb_add(vsm->ir_vol_surfaces, (void *)rq_ir_vol_surface_get_termstruct_id(ir_vol_surface), ir_vol_surface);
}

/**
 * Get a vol surface from the manager by the asset id.
 */
RQ_EXPORT rq_ir_vol_surface_t
rq_ir_vol_surface_mgr_get(
    const rq_ir_vol_surface_mgr_t vsm,
    const char *termstruct_id
    )
{
    return (rq_ir_vol_surface_t) rq_tree_rb_find(vsm->ir_vol_surfaces, termstruct_id);
}


RQ_EXPORT int
rq_ir_vol_surface_mgr_is_null(rq_ir_vol_surface_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_ir_vol_surface_mgr_iterator_t 
rq_ir_vol_surface_mgr_iterator_alloc()
{
    rq_ir_vol_surface_mgr_iterator_t it = (rq_ir_vol_surface_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_ir_vol_surface_mgr_iterator));
    
    it->ir_vol_surface_it = rq_tree_rb_iterator_alloc();

    return it;
}

RQ_EXPORT void 
rq_ir_vol_surface_mgr_begin(rq_ir_vol_surface_mgr_t ir_vol_surface_mgr, rq_ir_vol_surface_mgr_iterator_t it)
{
    rq_tree_rb_begin(ir_vol_surface_mgr->ir_vol_surfaces, it->ir_vol_surface_it);
}

RQ_EXPORT int 
rq_ir_vol_surface_mgr_at_end(rq_ir_vol_surface_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->ir_vol_surface_it);
}

RQ_EXPORT void 
rq_ir_vol_surface_mgr_next(rq_ir_vol_surface_mgr_iterator_t it)
{
    rq_tree_rb_next(it->ir_vol_surface_it);
}

RQ_EXPORT void 
rq_ir_vol_surface_mgr_iterator_free(rq_ir_vol_surface_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->ir_vol_surface_it);
    RQ_FREE(it);
}

RQ_EXPORT rq_ir_vol_surface_t 
rq_ir_vol_surface_mgr_iterator_deref(rq_ir_vol_surface_mgr_iterator_t it)
{
    return (rq_ir_vol_surface_t)rq_tree_rb_iterator_deref(it->ir_vol_surface_it);
}
