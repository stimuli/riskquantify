/*
** rq_object_builder.c
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
#include "rq_object_builder.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

RQ_EXPORT int 
rq_object_builder_is_null(rq_object_builder_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_object_builder_t 
_rq_object_builder_alloc(
    const char *type_name,
    void *derived_data,
    void (*free_func)(void *),
    void * (*build_func)(void *, rq_stream_t, rq_object_schema_mgr_t, const char *, const char *)
    )
{
    struct rq_object_builder *b = (struct rq_object_builder *)
        RQ_MALLOC(sizeof(struct rq_object_builder));

    b->type_name = RQ_STRDUP(type_name);
    b->derived_data = derived_data;
    b->free_func = free_func;
    b->build_func = build_func;

    return b;
}

RQ_EXPORT void 
rq_object_builder_free(rq_object_builder_t b)
{
    if (b->type_name)
        RQ_FREE((char *)b->type_name);
    (*b->free_func)(b->derived_data);
    RQ_FREE(b);
}

RQ_EXPORT const char *
rq_object_builder_get_type_name(rq_object_builder_t b)
{
    return b->type_name;
}

RQ_EXPORT void *
rq_object_builder_build(
    rq_object_builder_t b, 
    rq_stream_t stream, 
    rq_object_schema_mgr_t schema_mgr, 
    const char *path,
    const char *type_name
    )
{
    return (*b->build_func)(
        b->derived_data,
        stream,
        schema_mgr,
        path,
        type_name
        );
}
