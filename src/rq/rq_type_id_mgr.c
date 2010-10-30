/*
** rq_type_id_mgr.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#include "rq_type_id_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>
#include <assert.h>


/* -- code -------------------------------------------------------- */
static void
type_id_free(void *p)
{
    struct rq_type_id *type_id = p;
    if (type_id->name)
        RQ_FREE((char *)type_id->name);
    RQ_FREE(type_id);
}

static int
id_cmp(const void *p1, const void *p2)
{
    long id1 = (long)p1;
    long id2 = (long)p2;

    return id1 - id2;
}

RQ_EXPORT rq_type_id_mgr_t
rq_type_id_mgr_alloc(enum rq_id_type id_type)
{
    struct rq_type_id_mgr *tim = (struct rq_type_id_mgr *)
        RQ_MALLOC(sizeof(struct rq_type_id_mgr));
    tim->id_tree = rq_tree_rb_alloc(
        type_id_free,
        id_cmp
        );
    tim->name_tree = rq_tree_rb_alloc(
        NULL,
        (int (*)(const void *, const void *))strcmp
        );
    tim->next_id = 1;
    tim->id_type = id_type;
    return tim;
}

RQ_EXPORT void  
rq_type_id_mgr_free(rq_type_id_mgr_t tim)
{
    rq_tree_rb_free(tim->id_tree);
    rq_tree_rb_free(tim->name_tree);
    RQ_FREE(tim);
}

RQ_EXPORT int
rq_type_id_mgr_is_null(rq_type_id_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_boolean
rq_type_id_mgr_add(rq_type_id_mgr_t tim, const char *name, long id)
{
    struct rq_type_id *ti = (struct rq_type_id *)rq_tree_rb_find(
        tim->name_tree,
        name
        );

    assert(tim->id_type == RQ_ID_TYPE_USER_DEFINED);

    if (!ti)
    {
        ti = (struct rq_type_id *)rq_tree_rb_find(
            tim->id_tree,
            (const void *)id
            );
        if (!ti)
        {
            ti = (struct rq_type_id *)RQ_MALLOC(sizeof(struct rq_type_id));
            ti->name = RQ_STRDUP(name);
            ti->id = tim->next_id;
            rq_tree_rb_add(tim->name_tree, ti->name, ti);
            rq_tree_rb_add(tim->id_tree, (const void *)ti->id, ti);

            return RQ_TRUE;
        }
    }

    return RQ_FALSE;
}

RQ_EXPORT long 
rq_type_id_mgr_add_type(rq_type_id_mgr_t tim, const char *name)
{
    struct rq_type_id *ti = (struct rq_type_id *)rq_tree_rb_find(
        tim->name_tree,
        name
        );
    long id;
    if (ti)
        id = ti->id;
    else
    {
        assert(tim->id_type != RQ_ID_TYPE_USER_DEFINED);

        ti = (struct rq_type_id *)RQ_MALLOC(sizeof(struct rq_type_id));
        ti->name = RQ_STRDUP(name);
        id = ti->id = tim->next_id;
        
        switch (tim->id_type)
        {
            case RQ_ID_TYPE_LONG:
                tim->next_id++;
                break;

            case RQ_ID_TYPE_BITMAP:
                tim->next_id <<= 1;
                break;
        }

        rq_tree_rb_add(tim->name_tree, ti->name, ti);
        rq_tree_rb_add(tim->id_tree, (const void *)ti->id, ti);
    }

    return id;
}

RQ_EXPORT long 
rq_type_id_mgr_get_id_for_type(rq_type_id_mgr_t tim, const char *name)
{
    struct rq_type_id *ti = (struct rq_type_id *)rq_tree_rb_find(
        tim->name_tree,
        name
        );
    return (ti ? ti->id : 0);
}

RQ_EXPORT const char *
rq_type_id_mgr_get_type_for_id(rq_type_id_mgr_t tim, long id)
{
    struct rq_type_id *ti = (struct rq_type_id *)rq_tree_rb_find(
        tim->id_tree,
        (const void *)id
        );
    return (ti ? ti->name : NULL);
}

