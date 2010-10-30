/*
** rq_array.c
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
#include "rq_array.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RQ_DEFAULT_ARRAY_GROW_SIZE 1024
#define RQ_DEFAULT_ARRAY_START_SIZE 2
#define RQ_DEFAULT_ARRAY_BINARY_GROWTH_UNTIL_SIZE 1024

RQ_EXPORT int 
rq_array_is_null(rq_array_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_array_t 
rq_array_alloc(void (*free_func)(void *))
{
    struct rq_array *v = (struct rq_array *)RQ_MALLOC(sizeof(struct rq_array));
    v->array = RQ_MALLOC(RQ_DEFAULT_ARRAY_START_SIZE * sizeof(void *));
    v->max = RQ_DEFAULT_ARRAY_START_SIZE;
    v->size = 0;
    v->grow_size = RQ_DEFAULT_ARRAY_GROW_SIZE;

    v->free_func = free_func;

    return v;
}

RQ_EXPORT void
rq_array_free(rq_array_t v)
{
    unsigned i = 0;

    if (v->free_func)
        for ( ; i < v->size; i++)
            (*v->free_func)(v->array[i]);

   RQ_FREE(v->array);
   RQ_FREE(v);
}

RQ_EXPORT void
rq_array_clear(rq_array_t v)
{
    unsigned i = 0;
    for ( ; i < v->size; i++)
        (*v->free_func)(v->array[i]);
    v->size = 0;
}

RQ_EXPORT unsigned int 
rq_array_size(rq_array_t v)
{
    return v->size;
}

RQ_EXPORT void *
rq_array_get_at(rq_array_t v, unsigned int offset)
{
    return v->array[offset];
}

static void
grow_if_necessary(struct rq_array *v)
{
    if (v->size == v->max - 1)
    {
		/* Binary growth while small */
		if ( v->size <= RQ_DEFAULT_ARRAY_BINARY_GROWTH_UNTIL_SIZE )
		{
			v->array = RQ_REALLOC( v->array, (v->max + v->max ) * sizeof(void *) );
			v->max += v->max;
		}
		else
		{
			v->array = RQ_REALLOC( v->array, (v->max + v->grow_size) * sizeof(void *) );
			v->max += v->grow_size;
		}
    }
}

RQ_EXPORT void
rq_array_push_back(rq_array_t v, void *obj)
{
    grow_if_necessary(v);

    v->array[v->size] = obj;
    v->size++;
}

RQ_EXPORT void
rq_array_insert_at(rq_array_t v, unsigned long offset, void *obj)
{
    unsigned long i;

    grow_if_necessary(v);

    for (i = v->size-1; i >= offset; i--)
        v->array[i+1] = v->array[i];
    v->array[offset] = obj;
    v->size++;
}

RQ_EXPORT void *
rq_array_remove_at(rq_array_t v, unsigned long offset)
{
    unsigned long i;
    void *r = (offset < v->size ? v->array[offset] : 0);

    v->size--;
    for (i = offset; i < v->size; i++)
        v->array[i] = v->array[i+1];

    return r;
}

/* -- generic iterator support ------------------------------------ */

struct iter_node {
    rq_array_t array;
    long cur_pos;
};

static void
iter_free(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    RQ_FREE(iter);
}

static void
iter_incr(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    if (iter->cur_pos < iter->array->size)
        iter->cur_pos++;
}

static void
iter_decr(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    if (iter->cur_pos >= 0)
        iter->cur_pos--;
}

static short
iter_atend(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    return (iter->cur_pos >= iter->array->size);
}

static short
iter_atbegin(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    return iter->cur_pos < 0;
}

static short
iter_seek(void *iterdata, enum rq_iterator_seek_position seek_pos, long offset)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    if (seek_pos == RQ_ITERATOR_SEEK_START)
        iter->cur_pos = 0;

    if (seek_pos == RQ_ITERATOR_SEEK_END)
        if (iter->array->size > 0)
            iter->cur_pos = iter->array->size - 1;
    
    if (seek_pos == RQ_ITERATOR_SEEK_START || (seek_pos == RQ_ITERATOR_SEEK_CUR && offset >= 0))
    {
        iter->cur_pos += offset;
        if (iter->cur_pos >= iter->array->size)
        {
            if (iter->array->size > 0)
                iter->cur_pos = iter->array->size-1;
            else
                iter->cur_pos = 0;

            return 1;
        }
        return 0; /* successful */
    }
    else if (seek_pos == RQ_ITERATOR_SEEK_END || (seek_pos == RQ_ITERATOR_SEEK_CUR && offset < 0))
    {
        if (offset < 0)
            offset = -offset;

        if (iter->cur_pos >= offset)
            iter->cur_pos -= offset;
        else
        {
            iter->cur_pos = 0;
            return 1;
        }

        return 0; /* successful */
    }

    return -1; /* not supported */
}

static struct rq_variant
iter_getvalue(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    struct rq_variant ret;
    if (iter->cur_pos < iter->array->size)
        rq_variant_init_object(&ret, 0, iter->array->array[iter->cur_pos]);
    else
        rq_variant_init_nil(&ret);
    return ret;
}

RQ_EXPORT rq_iterator_t 
rq_array_get_iterator(rq_array_t array)
{
    struct iter_node *iter = (struct iter_node *)RQ_CALLOC(1, sizeof(struct iter_node));

    iter->array = array;
    iter->cur_pos = 0;

    return _rq_iterator_alloc(
        iter_free,
        iter_incr,
        iter_decr,
        iter_atend,
        iter_atbegin,
        iter_seek,
        iter_getvalue,
        iter
        );
}

