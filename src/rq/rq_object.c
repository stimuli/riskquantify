/*
** rq_object.c
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
/* -- includes ---------------------------------------------------- */
#include "rq_object.h"
#include "rq_object_schema.h"
#include "rq_symbol_table.h"
#include "rq_tree_rb.h"
#include "rq_array.h"
#include "rq_double_linked_list.h"
#include "rq_linked_list.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


/* Nil and true */
static struct rq_object _rq_object_nil;
static struct rq_object _rq_object_true;

rq_object_t rq_object_nil = &_rq_object_nil;
rq_object_t rq_object_true = &_rq_object_true;



/* -- prototypes -------------------------------------------------- */
static struct rq_object *_rq_object_mgr_alloc_object(struct rq_object_mgr *object_mgr, enum rq_object_type object_type);

/* -- code -------------------------------------------------------- */
RQ_EXPORT int 
rq_object_is_null(rq_object_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT int 
rq_object_is_nil(rq_object_t obj)
{
    return 
        obj == NULL ||
        obj == rq_object_nil ||
        rq_object_get_object_type(obj) == RQ_OBJECT_TYPE_NIL;
}


RQ_EXPORT rq_object_t 
rq_object_alloc(rq_object_mgr_t object_mgr)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_NIL);
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_integer(rq_object_mgr_t object_mgr, long i)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_INTEGER);
    o->value.i = i;
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_double(rq_object_mgr_t object_mgr, double d)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_DOUBLE);
    o->value.d = d;
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_string(rq_object_mgr_t object_mgr, const char *s)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_STRING);
    o->value.s = RQ_STRDUP(s);
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_date(rq_object_mgr_t object_mgr, rq_date date)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_DATE);
    o->value.date = date;
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_cons(rq_object_mgr_t object_mgr, rq_object_t car, rq_object_t cdr)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_CONS);
    o->value.cons.car = car;
    o->value.cons.cdr = cdr;
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_identifier(rq_object_mgr_t object_mgr, const char *id)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_IDENTIFIER);
    o->value.s = RQ_STRDUP(id);
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_rqobject(rq_object_mgr_t object_mgr, void *object, rq_object_schema_t object_schema)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_RQOBJECT);
    o->value.rqobject.object = object;
    o->value.rqobject.object_schema = object_schema;
    return o;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_builtin(rq_object_mgr_t object_mgr, rq_object_t (*func)(void *, rq_object_t))
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_BUILTIN);
    o->value.func = func;
    return o;
}

static int
_rq_object_tree_node_cmp(const void *p1, const void *p2)
{
    rq_object_t car1 = (rq_object_t)p1;
    rq_object_t car2 = (rq_object_t)p2;
    int ret = 0;

    switch (rq_object_get_object_type(car1))
    {
        case RQ_OBJECT_TYPE_INTEGER:
            ret = rq_object_get_integer(car1) - rq_object_coerce_integer(car2);
            break;

        case RQ_OBJECT_TYPE_DOUBLE:
        {
            double r = rq_object_get_double(car1) - rq_object_coerce_double(car2);
            ret = (r > 0 ? 1 : (r < 0 ? -1 : 0));
        }
        break;

        case RQ_OBJECT_TYPE_DATE:
            ret = rq_object_get_date(car1) - rq_object_coerce_date(car2);
            break;

        case RQ_OBJECT_TYPE_STRING:
            ret = strcmp(rq_object_get_string(car1), rq_object_coerce_string(car2));
            break;

        case RQ_OBJECT_TYPE_IDENTIFIER:
            ret = strcmp(rq_object_get_identifier(car1), rq_object_coerce_identifier(car2));
            break;
    }

    return ret;
}

RQ_EXPORT rq_object_t 
rq_object_alloc_tree(rq_object_mgr_t object_mgr)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_TREE);
    o->value.tree = rq_tree_rb_alloc(
        NULL,
        _rq_object_tree_node_cmp
        );
    return o;
}

RQ_EXPORT rq_object_t
rq_object_alloc_vector(rq_object_mgr_t object_mgr)
{
    struct rq_object *o = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, RQ_OBJECT_TYPE_VECTOR);
    o->value.vector = rq_array_alloc(NULL);
    return o;
}

RQ_EXPORT rq_object_t
rq_object_clone(rq_object_mgr_t object_mgr, rq_object_t o)
{
    struct rq_object *o2 = _rq_object_mgr_alloc_object((struct rq_object_mgr *)object_mgr, o->object_type);

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
        case RQ_OBJECT_TYPE_TRUE:
            break;

        case RQ_OBJECT_TYPE_INTEGER:
            o2->value.i = o->value.i;
            break;

        case RQ_OBJECT_TYPE_DOUBLE:
            o2->value.d = o->value.d;
            break;

        case RQ_OBJECT_TYPE_DATE:
            o2->value.date = o->value.date;
            break;

        case RQ_OBJECT_TYPE_STRING:
        case RQ_OBJECT_TYPE_IDENTIFIER:
            if (o->value.s)
                o2->value.s = RQ_STRDUP(o->value.s);
            else
                o2->value.s = NULL;
            break;

        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_ERROR:
            o2->value.cons.car = rq_object_clone(object_mgr, o->value.cons.car);
            o2->value.cons.cdr = rq_object_clone(object_mgr, o->value.cons.cdr);
            break;

        case RQ_OBJECT_TYPE_TREE:
        {
            rq_tree_rb_iterator_t tree_it = rq_tree_rb_iterator_alloc();

            o2->value.tree = rq_tree_rb_alloc(
                NULL,
                _rq_object_tree_node_cmp
                );

            rq_tree_rb_begin(o->value.tree, tree_it);
            while (!rq_tree_rb_at_end(tree_it))
            {
                rq_object_t to;
                rq_object_t c;
                to = (rq_object_t)rq_tree_rb_iterator_deref(tree_it);
                c = rq_object_clone(object_mgr, to);

                rq_tree_rb_add(o2->value.tree, c->value.cons.car, c);

                rq_tree_rb_next(tree_it);
            }

			rq_tree_rb_iterator_free(tree_it);
        }
        break;

        case RQ_OBJECT_TYPE_VECTOR:
        {
            unsigned int i;
            rq_object_t clone = o2;
            o2->value.vector = rq_array_alloc(NULL);
            for (i = 0; i < rq_object_vector_size(o2); i++)
            {
                rq_object_t src_obj = rq_object_vector_get_at(o2, i);
                rq_object_t clone_obj = rq_object_clone(object_mgr, src_obj);
                rq_object_vector_add(clone, clone_obj);
            }
        }
        break;

        case RQ_OBJECT_TYPE_RQOBJECT:
            o2->value.rqobject.object = rq_object_schema_clone_object(o->value.rqobject.object_schema, o->value.rqobject.object);
            o2->value.rqobject.object_schema = o->value.rqobject.object_schema;
            break;

        case RQ_OBJECT_TYPE_BUILTIN:
            o2->value.func = o->value.func;
            break;

        default:
            assert(0);
            break;
    }

    return o2;
}

RQ_EXPORT void 
rq_object_clear(rq_object_t o)
{
    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
        case RQ_OBJECT_TYPE_TRUE:
        case RQ_OBJECT_TYPE_INTEGER:
        case RQ_OBJECT_TYPE_DOUBLE:
        case RQ_OBJECT_TYPE_DATE:
        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_BUILTIN:
            break;

        case RQ_OBJECT_TYPE_STRING:
        case RQ_OBJECT_TYPE_IDENTIFIER:
            if (o->value.s)
               RQ_FREE((char *)o->value.s);
            break;

        case RQ_OBJECT_TYPE_TREE:
            rq_tree_rb_free(o->value.tree);
            break;

        case RQ_OBJECT_TYPE_VECTOR:
            rq_array_free(o->value.vector);
            break;

        case RQ_OBJECT_TYPE_RQOBJECT:
            rq_object_schema_destroy_object(o->value.rqobject.object_schema, o->value.rqobject.object);
            break;

        default:
            assert(0);
            break;
    }
    
    o->object_type = RQ_OBJECT_TYPE_NIL;
}

RQ_EXPORT void
rq_object_tag_hierarchy(rq_object_t o, unsigned short gc_id)
{
    o->gc_id = gc_id;

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_TREE:
        {
            rq_tree_rb_iterator_t it = rq_tree_rb_iterator_alloc(o->value.tree);
            while (!rq_tree_rb_at_end(it))
            {
                rq_object_t child_object = (rq_object_t)rq_tree_rb_iterator_deref(it);
                rq_object_tag_hierarchy(child_object, gc_id);
                rq_tree_rb_next(it);
            }
            rq_tree_rb_iterator_free(it);
        }
        break;

        case RQ_OBJECT_TYPE_VECTOR:
        {
            unsigned int i = 0;
            for ( ; i < rq_array_size(o->value.vector); i++)
            {
                rq_object_t child_object = rq_array_get_at(o->value.vector, i);
                rq_object_tag_hierarchy(child_object, gc_id);
            }
        }
        break;

        case RQ_OBJECT_TYPE_CONS:
        {
            rq_object_tag_hierarchy(o->value.cons.car, gc_id);
            rq_object_tag_hierarchy(o->value.cons.cdr, gc_id);
        }
        break;

        default:
            break;
    }
}

RQ_EXPORT enum rq_object_type 
rq_object_get_object_type(rq_object_t o)
{
    return o->object_type;
}

RQ_EXPORT int 
rq_object_get_integer(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_INTEGER);

    return o->value.i;
}

RQ_EXPORT double 
rq_object_get_double(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_DOUBLE);

    return o->value.d;
}

RQ_EXPORT const char *
rq_object_get_string(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_STRING);

    return o->value.s;
}

RQ_EXPORT rq_date 
rq_object_get_date(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_DATE);

    return o->value.date;
}

RQ_EXPORT rq_object_t 
rq_object_get_cons_car(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_CONS);

    return o->value.cons.car;
}

RQ_EXPORT rq_object_t 
rq_object_get_cons_cdr(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_CONS);

    return o->value.cons.cdr;
}

RQ_EXPORT const char *
rq_object_get_identifier(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_IDENTIFIER);

    return o->value.s;
}

RQ_EXPORT void *
rq_object_get_rqobject(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_RQOBJECT);

    return o->value.rqobject.object;
}

RQ_EXPORT rq_object_t
rq_object_call_builtin(rq_object_t o, void *interpreter, rq_object_t args)
{
    assert(o->object_type == RQ_OBJECT_TYPE_BUILTIN);

    return (*o->value.func)(interpreter, args);
}

RQ_EXPORT void 
rq_object_set_object(rq_object_mgr_t object_mgr, rq_object_t o2, rq_object_t o)
{
    rq_object_clear(o2);
    o2->object_type = o->object_type;

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
        case RQ_OBJECT_TYPE_TRUE:
            break;

        case RQ_OBJECT_TYPE_INTEGER:
            o2->value.i = o->value.i;
            break;

        case RQ_OBJECT_TYPE_DOUBLE:
            o2->value.d = o->value.d;
            break;

        case RQ_OBJECT_TYPE_DATE:
            o2->value.date = o->value.date;
            break;

        case RQ_OBJECT_TYPE_STRING:
        case RQ_OBJECT_TYPE_IDENTIFIER:
            if (o->value.s)
                o2->value.s = RQ_STRDUP(o->value.s);
            else
                o2->value.s = NULL;
            break;

        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_ERROR:
            o2->value.cons.car = rq_object_clone(object_mgr, o->value.cons.car);
            o2->value.cons.cdr = rq_object_clone(object_mgr, o->value.cons.cdr);
            break;

        case RQ_OBJECT_TYPE_TREE:
        {
            rq_tree_rb_iterator_t tree_it = rq_tree_rb_iterator_alloc();

            o2->value.tree = rq_tree_rb_alloc(
                NULL,
                _rq_object_tree_node_cmp
                );

            rq_tree_rb_begin(o->value.tree, tree_it);
            while (!rq_tree_rb_at_end(tree_it))
            {
                rq_object_t to;
                rq_object_t c;
                to = rq_tree_rb_iterator_deref(tree_it);
                c = rq_object_clone(object_mgr, to);

                rq_tree_rb_add(o2->value.tree, c->value.cons.car, c);

                rq_tree_rb_next(tree_it);
            }

			rq_tree_rb_iterator_free(tree_it);
        }
        break;

        case RQ_OBJECT_TYPE_VECTOR:
        {
            unsigned int i;
            rq_object_t clone = o2;
            o2->value.vector = rq_array_alloc(NULL);
            for (i = 0; i < rq_object_vector_size(o2); i++)
            {
                rq_object_t src_obj = rq_object_vector_get_at(o2, i);
                rq_object_t clone_obj = rq_object_clone(object_mgr, src_obj);
                rq_object_vector_add(clone, clone_obj);
            }
        }
        break;

        case RQ_OBJECT_TYPE_RQOBJECT:
            o2->value.rqobject.object = rq_object_schema_clone_object(o->value.rqobject.object_schema, o->value.rqobject.object);
            o2->value.rqobject.object_schema = o->value.rqobject.object_schema;
            break;

        case RQ_OBJECT_TYPE_BUILTIN:
            o2->value.func = o->value.func;
            break;

        default:
            assert(0);
            break;
    }
}

RQ_EXPORT void 
rq_object_set_integer(rq_object_t o, int value)
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_INTEGER;
    o->value.i = value;
}

RQ_EXPORT void
rq_object_set_double(rq_object_t o, double value)
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_DOUBLE;
    o->value.d = value;
}

RQ_EXPORT void
rq_object_set_string(rq_object_t o, const char *value)
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_STRING;
    o->value.s = RQ_STRDUP(value);
}

RQ_EXPORT void
rq_object_set_date(rq_object_t o, rq_date value)
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_DATE;
    o->value.date = value;
}

RQ_EXPORT void
rq_object_set_cons(rq_object_t o, rq_object_t car, rq_object_t cdr)
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_CONS;
    o->value.cons.car = car;
    o->value.cons.cdr = cdr;
}

RQ_EXPORT void
rq_object_set_cons_car(rq_object_t o, rq_object_t car)
{
    assert(o->object_type == RQ_OBJECT_TYPE_CONS);
    o->value.cons.car = car;
}

RQ_EXPORT void
rq_object_set_cons_cdr(rq_object_t o, rq_object_t cdr)
{
    assert(o->object_type == RQ_OBJECT_TYPE_CONS);
    o->value.cons.cdr = cdr;
}

RQ_EXPORT void
rq_object_set_identifier(rq_object_t o, const char *value)
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_IDENTIFIER;
    o->value.s = RQ_STRDUP(value);
}

RQ_EXPORT void
rq_object_set_rqobject(rq_object_t o, void *rqobject, rq_object_schema_t object_schema)
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_RQOBJECT;
    o->value.rqobject.object = rqobject;
    o->value.rqobject.object_schema = object_schema;
}

RQ_EXPORT void
rq_object_set_builtin(rq_object_t o, rq_object_t (*func)(void *, rq_object_t ))
{
    rq_object_clear(o);

    o->object_type = RQ_OBJECT_TYPE_BUILTIN;
    o->value.func = func;
}

RQ_EXPORT void *
rq_object_acquire_rqobject(rq_object_t o)
{
    void *object = o->value.rqobject.object;

    assert(o->object_type == RQ_OBJECT_TYPE_RQOBJECT);

    o->object_type = RQ_OBJECT_TYPE_NIL;

    return object;
}

RQ_EXPORT const char *
rq_object_get_rqobject_type_name(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_RQOBJECT);

    return rq_object_schema_get_type_name(o->value.rqobject.object_schema);
}

RQ_EXPORT rq_object_schema_t 
rq_object_get_rqobject_schema(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_RQOBJECT);

    return o->value.rqobject.object_schema;
}

RQ_EXPORT void
rq_object_reset(rq_object_t o)
{
    o->object_type = RQ_OBJECT_TYPE_NIL;
}

RQ_EXPORT long 
rq_object_coerce_integer(rq_object_t o)
{
    long ret = 0;

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
        case RQ_OBJECT_TYPE_TRUE:
        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_ERROR:
        case RQ_OBJECT_TYPE_RQOBJECT:
            break;

        case RQ_OBJECT_TYPE_INTEGER:
            ret = o->value.i;
            break;

        case RQ_OBJECT_TYPE_DOUBLE:
            ret = (long)o->value.d;
            break;

        case RQ_OBJECT_TYPE_DATE:
            ret = o->value.date;
            break;

        case RQ_OBJECT_TYPE_STRING:
        case RQ_OBJECT_TYPE_IDENTIFIER:
            ret = atol(o->value.s);
            break;

        default:
            assert(0);
            break;
    }

    rq_object_clear(o);
    o->object_type = RQ_OBJECT_TYPE_INTEGER;
    o->value.i = ret;

    return ret;
}

RQ_EXPORT double 
rq_object_coerce_double(rq_object_t o)
{
    double ret = 0;

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
        case RQ_OBJECT_TYPE_TRUE:
        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_ERROR:
        case RQ_OBJECT_TYPE_RQOBJECT:
            break;

        case RQ_OBJECT_TYPE_INTEGER:
            ret = o->value.i;
            break;

        case RQ_OBJECT_TYPE_DOUBLE:
            ret = o->value.d;
            break;

        case RQ_OBJECT_TYPE_DATE:
            ret = o->value.date;
            break;

        case RQ_OBJECT_TYPE_STRING:
        case RQ_OBJECT_TYPE_IDENTIFIER:
            ret = atof(o->value.s);
            break;

        default:
            assert(0);
            break;
    }

    rq_object_clear(o);
    o->object_type = RQ_OBJECT_TYPE_DOUBLE;
    o->value.d = ret;

    return ret;
}

RQ_EXPORT const char *
rq_object_coerce_string(rq_object_t o)
{
    const char *ret = NULL;

    if (o->object_type == RQ_OBJECT_TYPE_STRING)
        return o->value.s;
    
    if (o->object_type == RQ_OBJECT_TYPE_IDENTIFIER)
    {
        o->object_type = RQ_OBJECT_TYPE_STRING;
        return o->value.s;
    }

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
            ret = RQ_STRDUP("nil");
            break;

        case RQ_OBJECT_TYPE_TRUE:
            ret = RQ_STRDUP("t");
            break;

        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_ERROR:
        case RQ_OBJECT_TYPE_RQOBJECT:
            break;

        case RQ_OBJECT_TYPE_INTEGER:
        {
            char buf[256];
            sprintf(buf, "%ld", o->value.i);
            ret = RQ_STRDUP(buf);
        }
        break;

        case RQ_OBJECT_TYPE_DOUBLE:
        {
            char buf[256];
            sprintf(buf, "%lf", o->value.d);
            ret = RQ_STRDUP(buf);
        }
        break;

        case RQ_OBJECT_TYPE_DATE:
        {
            char buf[256];
            rq_date_to_string(buf, "yyyy-mm-dd", o->value.date);
            ret = RQ_STRDUP(buf);
        }
        break;

        default:
            assert(0);
            break;
    }

    if (ret)
    {
        rq_object_clear(o);
        o->object_type = RQ_OBJECT_TYPE_STRING;
        o->value.s = ret;
    }

    return ret;
}

RQ_EXPORT rq_date 
rq_object_coerce_date(rq_object_t o)
{
    rq_date ret = 0;

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_ERROR:
        case RQ_OBJECT_TYPE_RQOBJECT:
            break;

        case RQ_OBJECT_TYPE_INTEGER:
            ret = o->value.i;
            break;

        case RQ_OBJECT_TYPE_DOUBLE:
            ret = (rq_date)o->value.d;
            break;

        case RQ_OBJECT_TYPE_DATE:
            ret = o->value.date;
            break;

        case RQ_OBJECT_TYPE_STRING:
        case RQ_OBJECT_TYPE_IDENTIFIER:
            ret = rq_date_parse(o->value.s, RQ_DATE_FORMAT_YMD);
            break;

        default:
            assert(0);
            break;
    }

    rq_object_clear(o);
    o->object_type = RQ_OBJECT_TYPE_DATE;
    o->value.date = ret;

    return ret;
}

RQ_EXPORT const char *
rq_object_coerce_identifier(rq_object_t o)
{
    const char *ret = NULL;

    if (o->object_type == RQ_OBJECT_TYPE_IDENTIFIER)
        return o->value.s;
    
    if (o->object_type == RQ_OBJECT_TYPE_STRING)
    {
        o->object_type = RQ_OBJECT_TYPE_IDENTIFIER;
        return o->value.s;
    }

    switch (o->object_type)
    {
        case RQ_OBJECT_TYPE_NIL:
        case RQ_OBJECT_TYPE_CONS:
        case RQ_OBJECT_TYPE_ERROR:
        case RQ_OBJECT_TYPE_RQOBJECT:
            break;

        case RQ_OBJECT_TYPE_INTEGER:
        {
            char buf[256];
            sprintf(buf, "%ld", o->value.i);
            ret = RQ_STRDUP(buf);
        }
        break;

        case RQ_OBJECT_TYPE_DOUBLE:
        {
            char buf[256];
            sprintf(buf, "%lf", o->value.d);
            ret = RQ_STRDUP(buf);
        }
        break;

        case RQ_OBJECT_TYPE_DATE:
        {
            char buf[256];
            rq_date_to_string(buf, "yyyy-mm-dd", o->value.date);
            ret = RQ_STRDUP(buf);
        }
        break;

        default:
            assert(0);
            break;
    }

    if (ret)
    {
        rq_object_clear(o);
        o->object_type = RQ_OBJECT_TYPE_IDENTIFIER;
        o->value.s = ret;
    }

    return ret;
}

RQ_EXPORT unsigned int
rq_object_vector_size(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_VECTOR);

    return rq_array_size(o->value.vector);
}

RQ_EXPORT rq_object_t
rq_object_vector_get_at(rq_object_t o, unsigned int i)
{
    assert(o->object_type == RQ_OBJECT_TYPE_VECTOR);

    return (rq_object_t) rq_array_get_at(o->value.vector, i);
}

RQ_EXPORT void
rq_object_vector_add(rq_object_t o, rq_object_t child)
{
    assert(o->object_type == RQ_OBJECT_TYPE_VECTOR);

    rq_array_push_back(o->value.vector, child);
}

RQ_EXPORT rq_object_t 
rq_object_tree_find(rq_object_t o, rq_object_t key)
{
    rq_object_t f;

    assert(o->object_type == RQ_OBJECT_TYPE_TREE);

    f = rq_tree_rb_find(o->value.tree, key);

    if (f)
        return rq_object_get_cons_cdr(f);
    return f;
}

RQ_EXPORT rq_object_t
rq_object_tree_keys(rq_object_mgr_t object_mgr, rq_object_t o)
{
    rq_object_t v = rq_object_alloc_vector(object_mgr);
    rq_tree_rb_iterator_t tree_it = rq_tree_rb_iterator_alloc();

    assert(o->object_type == RQ_OBJECT_TYPE_TREE);

    rq_tree_rb_begin(o->value.tree, tree_it);
    while (!rq_tree_rb_at_end(tree_it))
    {
        rq_object_t cons;
        rq_object_t car;
        rq_object_t car_clone;
        cons = rq_tree_rb_iterator_deref(tree_it);
        car = rq_object_get_cons_car(cons);
        car_clone = rq_object_clone(object_mgr, car);
        rq_object_vector_add(v, car_clone);
        rq_tree_rb_next(tree_it);
    }

	rq_tree_rb_iterator_free(tree_it);

    return v;
}

RQ_EXPORT void
rq_object_tree_add(rq_object_t o, rq_object_t c)
{
    assert(o->object_type == RQ_OBJECT_TYPE_TREE);
    assert(c->object_type == RQ_OBJECT_TYPE_CONS);

    rq_tree_rb_add(o->value.tree, c->value.cons.car, c);
}

RQ_EXPORT void 
rq_object_tree_clear(rq_object_t o)
{
    assert(o->object_type == RQ_OBJECT_TYPE_TREE);

    rq_tree_rb_clear(o->value.tree);
}

RQ_EXPORT int 
rq_object_list_size(rq_object_t obj)
{
    int i = -1;

    if (rq_object_get_object_type(obj) == RQ_OBJECT_TYPE_CONS)
    {
        for (i = 0; !rq_object_is_nil(obj); i++)
            obj = rq_object_get_cons_cdr(obj);
    }

    return i;
}

/* -- object pool code -------------------------------------------- */
RQ_EXPORT struct rq_object_pool *
rq_object_pool_alloc(unsigned long num_objects)
{
    struct rq_object_pool *object_pool = (struct rq_object_pool *)
        RQ_MALLOC(sizeof(struct rq_object_pool));

    object_pool->current_offset = 0;
    object_pool->num_objects = num_objects;
    object_pool->pool = (struct rq_object *)RQ_CALLOC(
        num_objects, 
        sizeof(struct rq_object)
        );

    return object_pool;
}

RQ_EXPORT void 
rq_object_pool_free(struct rq_object_pool *op)
{
    unsigned int i;
    for (i = 0; i < op->current_offset; i++)
    {
        rq_object_t o = (struct rq_object *) (((char *)op->pool) + (i * sizeof(struct rq_object)));
        rq_object_clear(o);
    }

    RQ_FREE(op->pool);
    RQ_FREE(op);
}

RQ_EXPORT struct rq_object *
rq_object_pool_get_next_object(struct rq_object_pool *op)
{
    struct rq_object *r = NULL;

    if (op->current_offset < op->num_objects)
    {
        r = (struct rq_object *)(((char *)op->pool) + (op->current_offset * sizeof(struct rq_object)));
        r->object_type = RQ_OBJECT_TYPE_NIL;
        
        op->current_offset++;
    }

    return r;
}

/* -- object manager code ----------------------------------------- */
static void
_rq_object_mgr_free_object_pool(void *p)
{
    rq_object_pool_free((struct rq_object_pool *)p);
}

static void
_rq_object_mgr_free_symbol_table(void *p)
{
    rq_symbol_table_free((rq_symbol_table_t)p);
}

RQ_EXPORT rq_object_mgr_t
rq_object_mgr_alloc()
{
    struct rq_object_mgr *om = (struct rq_object_mgr *)RQ_MALLOC(
        sizeof(struct rq_object_mgr)
        );
    struct rq_object_pool *obj_pool = rq_object_pool_alloc(RQ_OBJECT_MGR_OBJECT_POOL_SIZE);
    rq_symbol_table_t symbol_table = rq_symbol_table_alloc(0);

    om->gc_id = 0;
    om->used_list = rq_double_linked_list_alloc(NULL);
    om->free_list = NULL;
    om->object_pools = rq_array_alloc(_rq_object_mgr_free_object_pool);
    om->symbol_tables = rq_double_linked_list_alloc(_rq_object_mgr_free_symbol_table);

    rq_array_push_back(om->object_pools, obj_pool);
    rq_double_linked_list_append(om->symbol_tables, symbol_table);

    /* set rq_object_t and rq_object_nil to be of the appropriate types */
    _rq_object_nil.object_type = RQ_OBJECT_TYPE_NIL;
    _rq_object_true.object_type = RQ_OBJECT_TYPE_TRUE;

    return om;
}

RQ_EXPORT void
rq_object_mgr_free(rq_object_mgr_t om)
{
    rq_double_linked_list_free(om->used_list);
    rq_double_linked_list_free(om->symbol_tables);
    rq_array_free(om->object_pools);

    RQ_FREE(om);
}

static struct rq_object *
_rq_object_mgr_alloc_object(struct rq_object_mgr *object_mgr, enum rq_object_type object_type)
{
    struct rq_object *o;
    if (object_mgr->free_list != NULL)
    {
        o = object_mgr->free_list;
        object_mgr->free_list = (struct rq_object *)object_mgr->free_list->value.cons.cdr;
    }
    else
    {
        /* get an object from the last object pool */
        struct rq_object_pool *obj_pool = (struct rq_object_pool *)rq_array_get_at(object_mgr->object_pools, rq_array_size(object_mgr->object_pools)-1);
        o = rq_object_pool_get_next_object(obj_pool);
        if (!o)
        {
            obj_pool = rq_object_pool_alloc(RQ_OBJECT_MGR_OBJECT_POOL_SIZE);
            rq_array_push_back(object_mgr->object_pools, obj_pool);
            o = rq_object_pool_get_next_object(obj_pool);
        }
    }

    o->object_type = object_type;
    rq_double_linked_list_append(object_mgr->used_list, o);

    return o;
}

RQ_EXPORT void
rq_object_mgr_symbol_add(rq_object_mgr_t om, const char *id, rq_object_t obj)
{
    rq_double_linked_list_iterator_t st_it = rq_double_linked_list_rbegin(om->symbol_tables);
    rq_symbol_table_t symbol_table = rq_double_linked_list_iterator_get_data(st_it);
    rq_symbol_table_add(symbol_table, id, obj);
}

RQ_EXPORT rq_object_t
rq_object_mgr_symbol_find(rq_object_mgr_t om, const char *id)
{
    rq_object_t o = rq_object_nil;
    rq_double_linked_list_iterator_t st_it = rq_double_linked_list_rbegin(om->symbol_tables);
    while (!rq_double_linked_list_iterator_at_rend(st_it))
    {
        rq_symbol_table_t symbol_table = rq_double_linked_list_iterator_get_data(st_it);
        o = rq_symbol_table_get(symbol_table, id);
        if (!rq_object_is_nil(o))
            break;
        st_it = rq_double_linked_list_iterator_decr(st_it);
    }
    /* rq_double_linked_list_iterator_free(st_it); --> no free() for ll iterators */
    return o;
}

RQ_EXPORT void
rq_object_mgr_garbage_collect(rq_object_mgr_t om)
{
    rq_double_linked_list_iterator_t st_it;
    rq_double_linked_list_iterator_t ul_it;

    om->gc_id++;

    st_it = rq_double_linked_list_begin(om->symbol_tables);
    while (!rq_double_linked_list_iterator_at_end(st_it))
    {
        rq_symbol_table_iterator_t symb_it;
        rq_symbol_table_t symbol_table = rq_double_linked_list_iterator_get_data(st_it);
        symb_it = rq_symbol_table_begin(symbol_table);
        while (!rq_symbol_table_at_end(symb_it))
        {
            rq_object_t o = rq_symbol_table_iterator_get_object(symb_it);
            rq_object_tag_hierarchy(o, om->gc_id);
            rq_symbol_table_next(symb_it);
        }
        rq_symbol_table_iterator_free(symb_it);

        st_it = rq_double_linked_list_iterator_incr(st_it);
    }

    ul_it = rq_double_linked_list_begin(om->used_list);
    while (!rq_double_linked_list_iterator_at_end(ul_it))
    {
        struct rq_object *o = (struct rq_object *)
            rq_double_linked_list_iterator_get_data(ul_it);
        if (o->gc_id != om->gc_id)
        {
            rq_object_clear(o);
            ul_it = rq_double_linked_list_erase(om->used_list, ul_it);
            o->value.cons.cdr = om->free_list;
            om->free_list = o;
        }
        else
            ul_it = rq_double_linked_list_iterator_incr(ul_it);
    }
}

RQ_EXPORT void 
rq_object_mgr_symbol_table_push(rq_object_mgr_t om)
{
    rq_symbol_table_t symbol_table = rq_symbol_table_alloc(0);
    rq_double_linked_list_append(om->symbol_tables, symbol_table);
}

RQ_EXPORT void 
rq_object_mgr_symbol_table_pop(rq_object_mgr_t om)
{
    rq_double_linked_list_iterator_t it = rq_double_linked_list_rbegin(om->symbol_tables);
    if (!rq_double_linked_list_iterator_at_rend(it))
        rq_double_linked_list_erase(om->symbol_tables, it);
}

RQ_EXPORT void
rq_object_mgr_symbol_set_local(rq_object_mgr_t om, const char *id, rq_object_t value)
{
    rq_double_linked_list_iterator_t it = rq_double_linked_list_rbegin(om->symbol_tables);
    if (!rq_double_linked_list_iterator_at_rend(it))
    {
        rq_symbol_table_t symbol_table;
        rq_object_t interned_obj;

        symbol_table = (rq_symbol_table_t)rq_double_linked_list_iterator_get_data(it);

        interned_obj = rq_symbol_table_get(symbol_table, id);
        if (rq_object_is_nil(interned_obj))
            rq_symbol_table_add(symbol_table, id, value);
        else
            rq_object_set_object(om, interned_obj, value);
    }
}
