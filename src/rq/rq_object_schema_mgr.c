/*
** rq_object_schema_mgr.c
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
#include "rq_object_schema_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* -- the objects to initialize in the object schema -- */
#include "rq_calendar.h"

RQ_EXPORT int 
rq_object_schema_mgr_is_null(rq_object_schema_mgr_t obj)
{
    return (obj == NULL);
}

static void
_rq_object_schema_free(void *sp)
{
    rq_object_schema_free((rq_object_schema_t)sp);
}

RQ_EXPORT rq_object_schema_mgr_t 
rq_object_schema_mgr_alloc()
{
    struct rq_object_schema_mgr *m = (struct rq_object_schema_mgr *)
        RQ_MALLOC(sizeof(struct rq_object_schema_mgr));

    m->tree = rq_tree_rb_alloc(_rq_object_schema_free, (int (*)(const void *, const void *))strcmp);

    return m;
}

RQ_EXPORT void 
rq_object_schema_mgr_free(rq_object_schema_mgr_t m)
{
    rq_tree_rb_free(m->tree);

    RQ_FREE(m);
}

RQ_EXPORT void
rq_object_schema_mgr_add(rq_object_schema_mgr_t m, rq_object_schema_t os)
{
    rq_tree_rb_add(m->tree, (void *)rq_object_schema_get_type_name(os), os);
}

RQ_EXPORT rq_object_schema_t
rq_object_schema_mgr_find(rq_object_schema_mgr_t m, const char *type_name)
{
    void *p = rq_tree_rb_find(m->tree, (void *)type_name);

    return p;
}

RQ_EXPORT void 
rq_object_schema_mgr_init_object_schemas(rq_object_schema_mgr_t schema_mgr)
{
    rq_calendar_init_object_schemas(schema_mgr);
}
