/*
** rq_array_double.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include "rq_array_double.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define RQ_DEFAULT_ARRAY_GROW_SIZE 50

RQ_EXPORT int 
rq_array_double_is_null(rq_array_double_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_array_double_t 
rq_array_double_alloc()
{
    struct rq_array_double *v = (struct rq_array_double *)RQ_MALLOC(sizeof(struct rq_array_double));
    v->array = (double *)RQ_CALLOC(RQ_DEFAULT_ARRAY_GROW_SIZE, sizeof(double));
    v->max = RQ_DEFAULT_ARRAY_GROW_SIZE;
    v->size = 0;
    v->grow_size = RQ_DEFAULT_ARRAY_GROW_SIZE;

    return v;
}

RQ_EXPORT rq_array_double_t 
rq_array_double_alloc_with_size(unsigned max_size)
{
    struct rq_array_double *v = (struct rq_array_double *)RQ_MALLOC(sizeof(struct rq_array_double));
    v->array = (double *)RQ_CALLOC(max_size, sizeof(double));
    v->max = max_size;
    v->size = 0;
    v->grow_size = RQ_DEFAULT_ARRAY_GROW_SIZE;

    return v;
}

RQ_EXPORT void
rq_array_double_free(rq_array_double_t v)
{
    unsigned i = 0;

    RQ_FREE(v->array);
    RQ_FREE(v);
}

RQ_EXPORT void
rq_array_double_clear(rq_array_double_t v)
{
    memset((char *)v->array, '\0', v->max * sizeof(double));
    v->size = 0;
}

RQ_EXPORT unsigned int 
rq_array_double_size(rq_array_double_t v)
{
    return v->size;
}

RQ_EXPORT double
rq_array_double_get_at(rq_array_double_t v, unsigned int offset)
{
    return v->array[offset];
}

static void
grow_if_necessary(struct rq_array_double *v)
{
    if (v->size == v->max - 1)
    {
        v->array = realloc(
            v->array, 
            (v->max + v->grow_size) * sizeof(double)
            );
        v->max += v->grow_size;
    }
}

RQ_EXPORT void
rq_array_double_push_back(rq_array_double_t v, double obj)
{
    grow_if_necessary(v);

    v->array[v->size] = obj;
    v->size++;
}

RQ_EXPORT void 
rq_array_double_set_at(rq_array_double_t v, unsigned long offset, double num)
{
    if (v->size <= offset)
    {
        if (offset >= v->max)
        {
            unsigned new_max = v->max + v->grow_size;
            if (new_max <= offset)
            {
                unsigned diff = offset - new_max;
                unsigned num = (int)(diff / v->grow_size);
                new_max += (v->grow_size * (num + 1));
            }

            v->array = realloc(
                v->array, 
                new_max * sizeof(double)
                );
            v->max = new_max;
        }

        v->size = offset + 1;
    }

    v->array[offset] = num;
}

RQ_EXPORT void
rq_array_double_insert_at(rq_array_double_t v, unsigned long offset, double obj)
{
    unsigned long i;

    grow_if_necessary(v);

    for (i = v->size-1; i >= offset; i--)
        v->array[i+1] = v->array[i];
    v->array[offset] = obj;
    v->size++;
}

RQ_EXPORT double
rq_array_double_remove_at(rq_array_double_t v, unsigned long offset)
{
    unsigned long i;
    double r = (offset < v->size ? v->array[offset] : 0);

    v->size--;
    for (i = offset; i < v->size; i++)
        v->array[i] = v->array[i+1];

    return r;
}
