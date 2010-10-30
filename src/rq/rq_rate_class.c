/*
** rq_rate_class.c
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
#include "rq_rate_class.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_rate_class_t
_rq_rate_class_alloc(const char *id, const char *type_id, void *type_data, void (*free_func)(void *))
{
    struct rq_rate_class *r = (struct rq_rate_class *)
        RQ_MALLOC(sizeof(struct rq_rate_class));
    r->id = (const char *)RQ_STRDUP(id);
    r->type_id = (const char *)RQ_STRDUP(type_id);
    r->type_data = type_data;
    r->free_func = free_func;

    return r;
}

RQ_EXPORT void  
rq_rate_class_free(rq_rate_class_t r)
{
    RQ_FREE((char *)r->id);
    RQ_FREE((char *)r->type_id);
    if (r->free_func && r->type_data)
        (*r->free_func)(r->type_data);
    RQ_FREE(r);
}

RQ_EXPORT int
rq_rate_class_is_null(rq_rate_class_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT const char *
rq_rate_class_get_id(rq_rate_class_t r)
{
    return r->id;
}

RQ_EXPORT const char *
rq_rate_class_get_type_id(rq_rate_class_t r)
{
    return r->type_id;
}

RQ_EXPORT void *
rq_rate_class_get_type_data(rq_rate_class_t r)
{
    return r->type_data;
}
