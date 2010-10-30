/*
** rq_variant.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2004-2008 Brett Hutley
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
#include "rq_variant.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_variant_t
rq_variant_alloc()
{
    struct rq_variant *v = (struct rq_variant *)RQ_CALLOC(1, sizeof(struct rq_variant));
    v->type = RQ_VARIANT_TYPE_NIL;
    return v;
}

RQ_EXPORT void  
rq_variant_free(rq_variant_t v)
{
    if (v->type == RQ_VARIANT_TYPE_STRING)
        RQ_FREE((char *)v->value.str);
    RQ_FREE(v);
}

RQ_EXPORT int
rq_variant_is_null(rq_variant_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_variant_t 
rq_variant_alloc_string(const char *str)
{
    struct rq_variant *v = (struct rq_variant *)RQ_CALLOC(1, sizeof(struct rq_variant));
    v->type = RQ_VARIANT_TYPE_STRING;
    v->value.str = RQ_STRDUP(str);
    return v;
}

RQ_EXPORT rq_variant_t 
rq_variant_alloc_double(double d)
{
    struct rq_variant *v = (struct rq_variant *)RQ_CALLOC(1, sizeof(struct rq_variant));
    v->type = RQ_VARIANT_TYPE_DOUBLE;
    v->value.d = d;
    return v;
}

RQ_EXPORT rq_variant_t 
rq_variant_alloc_integer(long i)
{
    struct rq_variant *v = (struct rq_variant *)RQ_CALLOC(1, sizeof(struct rq_variant));
    v->type = RQ_VARIANT_TYPE_INTEGER;
    v->value.i = i;
    return v;
}

RQ_EXPORT rq_variant_t 
rq_variant_alloc_date(rq_date dt)
{
    struct rq_variant *v = (struct rq_variant *)RQ_CALLOC(1, sizeof(struct rq_variant));
    v->type = RQ_VARIANT_TYPE_DATE;
    v->value.date = dt;
    return v;
}

RQ_EXPORT rq_variant_t 
rq_variant_alloc_object(unsigned long obj_type, void *obj_ptr)
{
    struct rq_variant *v = (struct rq_variant *)RQ_CALLOC(1, sizeof(struct rq_variant));
    v->type = RQ_VARIANT_TYPE_OBJECT;

    v->value.object.obj_type = obj_type;
    v->value.object.obj_ptr = obj_ptr;

    return v;
}


RQ_EXPORT void 
rq_variant_init_nil(struct rq_variant *var)
{
    var->type = RQ_VARIANT_TYPE_NIL;
}

RQ_EXPORT void 
rq_variant_init_string(struct rq_variant *var, const char *str)
{
    var->type = RQ_VARIANT_TYPE_STRING;
    var->value.str = RQ_STRDUP(str);
}

RQ_EXPORT void 
rq_variant_init_double(struct rq_variant *var, double d)
{
    var->type = RQ_VARIANT_TYPE_DOUBLE;
    var->value.d = d;
}

RQ_EXPORT void 
rq_variant_init_integer(struct rq_variant *var, long i)
{
    var->type = RQ_VARIANT_TYPE_INTEGER;
    var->value.i = i;
}

RQ_EXPORT void 
rq_variant_init_date(struct rq_variant *var, rq_date dt)
{
    var->type = RQ_VARIANT_TYPE_DATE;
    var->value.date = dt;
}

RQ_EXPORT void 
rq_variant_init_object(struct rq_variant *var, unsigned long obj_type, void *obj_ptr)
{
    var->type = RQ_VARIANT_TYPE_OBJECT;
    var->value.object.obj_type = obj_type;
    var->value.object.obj_ptr = obj_ptr;
}

RQ_EXPORT enum rq_variant_type 
rq_variant_get_type(rq_variant_t v)
{
    return v->type;
}

RQ_EXPORT const char *
rq_variant_get_string(rq_variant_t v)
{
    if (v->type == RQ_VARIANT_TYPE_STRING)
        return v->value.str;
    return NULL;
}

RQ_EXPORT double 
rq_variant_get_double(rq_variant_t v)
{
    if (v->type == RQ_VARIANT_TYPE_DOUBLE)
        return v->value.d;
    return 0.0;
}

RQ_EXPORT long 
rq_variant_get_integer(rq_variant_t v)
{
    if (v->type == RQ_VARIANT_TYPE_INTEGER)
        return v->value.i;
    return 0;
}

RQ_EXPORT rq_date 
rq_variant_get_date(rq_variant_t v)
{
    if (v->type == RQ_VARIANT_TYPE_DATE)
        return v->value.date;
    return 0;
}

RQ_EXPORT const void *
rq_variant_get_object_ptr(rq_variant_t v)
{
    if (v->type == RQ_VARIANT_TYPE_OBJECT)
        return v->value.object.obj_ptr;
    return NULL;
}

RQ_EXPORT unsigned long
rq_variant_get_object_type(rq_variant_t v)
{
    if (v->type == RQ_VARIANT_TYPE_OBJECT)
        return v->value.object.obj_type;
    return 0;
}

RQ_EXPORT double 
rq_variant_get_as_double(rq_variant_t v)
{
    double ret = 0.0;
    switch (v->type)
    {
        case RQ_VARIANT_TYPE_STRING:
            ret = atof(v->value.str);
            break;
            
        case RQ_VARIANT_TYPE_DOUBLE:
            ret = v->value.d;
            break;

        case RQ_VARIANT_TYPE_INTEGER:
            ret = v->value.i;
            break;

        case RQ_VARIANT_TYPE_DATE:
            ret = v->value.date;
            break;

        case RQ_VARIANT_TYPE_OBJECT:
            ret = (double)v->value.object.obj_type;
            break;
    }

    return ret;
}

RQ_EXPORT long 
rq_variant_get_as_integer(rq_variant_t v)
{
    long ret = 0;
    switch (v->type)
    {
        case RQ_VARIANT_TYPE_STRING:
            ret = atol(v->value.str);
            break;
            
        case RQ_VARIANT_TYPE_DOUBLE:
            ret = (long)v->value.d;
            break;

        case RQ_VARIANT_TYPE_INTEGER:
            ret = v->value.i;
            break;

        case RQ_VARIANT_TYPE_DATE:
            ret = v->value.date;
            break;

        case RQ_VARIANT_TYPE_OBJECT:
            ret = (long)v->value.object.obj_type;
            break;
    }

    return ret;
}

RQ_EXPORT rq_date 
rq_variant_get_as_date(rq_variant_t v)
{
    rq_date ret = 0;
    switch (v->type)
    {
        case RQ_VARIANT_TYPE_STRING:
            ret = rq_date_parse(v->value.str, RQ_DATE_FORMAT_YMD);
            break;
            
        case RQ_VARIANT_TYPE_DOUBLE:
            ret = (long)v->value.d;
            break;

        case RQ_VARIANT_TYPE_INTEGER:
            ret = v->value.i;
            break;

        case RQ_VARIANT_TYPE_DATE:
            ret = v->value.date;
            break;
    }

    return ret;
}

RQ_EXPORT const char *
rq_variant_coerce_to_string(rq_variant_t v)
{
    const char *ret = NULL;
    switch (v->type)
    {
        case RQ_VARIANT_TYPE_STRING:
            break;
            
        case RQ_VARIANT_TYPE_DOUBLE:
        {
            char buf[256];
            snprintf(buf, 256, "%lf", v->value.d);
            v->value.str = RQ_STRDUP(buf);
            v->type = RQ_VARIANT_TYPE_STRING;
        }
        break;

        case RQ_VARIANT_TYPE_INTEGER:
        {
            char buf[256];
            snprintf(buf, 256, "%ld", v->value.i);
            v->value.str = RQ_STRDUP(buf);
            v->type = RQ_VARIANT_TYPE_STRING;
        }
        break;

        case RQ_VARIANT_TYPE_DATE:
        {
            char buf[256];
            rq_date_to_string(buf, "yyyy-mm-dd", v->value.date);
            v->value.str = RQ_STRDUP(buf);
            v->type = RQ_VARIANT_TYPE_STRING;
        }
        break;
    }

    return v->value.str;
}
