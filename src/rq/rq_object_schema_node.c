/*
** rq_object_schema_node.c
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
#include "rq_object_schema_node.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

RQ_EXPORT int 
rq_object_schema_node_is_null(rq_object_schema_node_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_object_schema_node_t 
rq_object_schema_node_alloc(
    const char *property_name, 
    const char *type_name
    )
{
    struct rq_object_schema_node *n = (struct rq_object_schema_node *)
        RQ_MALLOC(sizeof(struct rq_object_schema_node));

    n->property_name = RQ_STRDUP(property_name);
    n->type_name = RQ_STRDUP(type_name);

    return n;
}

RQ_EXPORT void 
rq_object_schema_node_free(rq_object_schema_node_t n)
{
    if (n->property_name)
        RQ_FREE((char *)n->property_name);
    if (n->type_name)
        RQ_FREE((char *)n->type_name);
    RQ_FREE(n);
}

RQ_EXPORT const char *
rq_object_schema_node_get_type_name(rq_object_schema_node_t n)
{
    return n->type_name;
}

RQ_EXPORT const char *
rq_object_schema_node_get_property_name(rq_object_schema_node_t n)
{
    return n->property_name;
}

