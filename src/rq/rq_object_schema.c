/*
** rq_object_schema.c
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
#include "rq_object_schema.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

RQ_EXPORT int 
rq_object_schema_is_null(rq_object_schema_t obj)
{
    return (obj == NULL);
}

static void
_rq_object_schema_node_free(void *np)
{
    rq_object_schema_node_free((rq_object_schema_node_t)np);
}

RQ_EXPORT rq_object_schema_t 
rq_object_schema_alloc(
    const char *type_name,
    void *(*constructor)(),
    void (*destructor)(void *),
    void *(*cloner)(void *),
    void (*set_value_string)(void *, const char *, const char *),
    void (*set_value_integer)(void *, const char *, long),
    void (*set_value_double)(void *, const char *, double),
    void (*set_value_date)(void *, const char *, rq_date),
    void (*set_value_object)(void *, const char *, void *)
    )
{
    struct rq_object_schema *s = (struct rq_object_schema *)
        RQ_MALLOC(sizeof(struct rq_object_schema));

    s->type_name = RQ_STRDUP(type_name);
    s->constructor = constructor;
    s->destructor = destructor;
    s->cloner = cloner;
    s->set_value_string = set_value_string;
    s->set_value_integer = set_value_integer;
    s->set_value_double = set_value_double;
    s->set_value_date = set_value_date;
    s->set_value_object = set_value_object;

    s->properties = rq_tree_rb_alloc(_rq_object_schema_node_free, (int (*)(const void *, const void *))strcmp);

    return s;
}

RQ_EXPORT void 
rq_object_schema_free(rq_object_schema_t s)
{
    if (s->type_name)
       RQ_FREE((char *)s->type_name);

    rq_tree_rb_free(s->properties);

   RQ_FREE(s);
}

RQ_EXPORT const char *
rq_object_schema_get_type_name(rq_object_schema_t s)
{
    return s->type_name;
}

RQ_EXPORT void 
rq_object_schema_add_property(
    rq_object_schema_t s, 
    rq_object_schema_node_t child
    )
{
    rq_tree_rb_add(s->properties, (void *)rq_object_schema_node_get_property_name(child), child);
}

RQ_EXPORT void *
rq_object_schema_construct_object(rq_object_schema_t s)
{
    return (*s->constructor)();
}

RQ_EXPORT void 
rq_object_schema_destroy_object(rq_object_schema_t s, void *object)
{
    (*s->destructor)(object);
}

RQ_EXPORT void *
rq_object_schema_clone_object(rq_object_schema_t s, void *object)
{
    return (*s->cloner)(object);
}

RQ_EXPORT void
rq_object_schema_set_value_string(rq_object_schema_t s, void *object, const char *property_name, const char *value)
{
    if (s->set_value_string)
        (*s->set_value_string)(object, property_name, value);
}

RQ_EXPORT void
rq_object_schema_set_value_integer(rq_object_schema_t s, void *object, const char *property_name, long value)
{
    if (s->set_value_integer)
        (*s->set_value_integer)(object, property_name, value);
}

RQ_EXPORT void
rq_object_schema_set_value_double(rq_object_schema_t s, void *object, const char *property_name, double value)
{
    if (s->set_value_double)
        (*s->set_value_double)(object, property_name, value);
}

RQ_EXPORT void
rq_object_schema_set_value_date(rq_object_schema_t s, void *object, const char *property_name, rq_date value)
{
    if (s->set_value_date)
        (*s->set_value_date)(object, property_name, value);
}

RQ_EXPORT void
rq_object_schema_set_value_object(rq_object_schema_t s, void *object, const char *property_name, void *value)
{
    if (s->set_value_object)
        (*s->set_value_object)(object, property_name, value);
}

RQ_EXPORT rq_object_schema_node_t 
rq_object_schema_find_property(
    rq_object_schema_t s,
    const char *property_name
    )
{
    return (rq_object_schema_node_t)rq_tree_rb_find(s->properties, (void *)property_name);
}
