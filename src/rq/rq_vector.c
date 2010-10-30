/*
** rq_vector.c
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
#include "rq_vector.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



RQ_EXPORT int 
rq_vector_is_null(rq_vector_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_vector_t 
rq_vector_alloc()
{
    struct rq_vector *v = (struct rq_vector *)RQ_MALLOC(sizeof(struct rq_vector));
    struct rq_vector_block *vb = (struct rq_vector_block *)RQ_MALLOC(sizeof(struct rq_vector_block));
    struct rq_vector_data *vd = (struct rq_vector_data *)RQ_MALLOC(sizeof(struct rq_vector_data));

    vd->vector = RQ_CALLOC(RQ_VECTOR_DATA_SIZE, sizeof(double));

    vb->next = NULL;
    vb->vector_data = (struct rq_vector_data **)RQ_CALLOC(RQ_VECTOR_BLOCK_SIZE, sizeof(struct rq_vector_data *));
    vb->vector_data[0] = vd;

    v->block_count = 1;
    v->block_size = RQ_VECTOR_BLOCK_SIZE;

    v->vector_data_size = RQ_VECTOR_DATA_SIZE;

    v->head = v->tail = vb;

    v->size = 0;
    
    return v;
}

RQ_EXPORT void
rq_vector_free(rq_vector_t v)
{
    while (v->head)
    {
		struct rq_vector_block *vb = v->head;
        unsigned i;

        for (i = 0; i < v->block_size; i++)
        {
            struct rq_vector_data *vd = vb->vector_data[i];
            if (vd)
            {
                unsigned j;

                RQ_FREE(vd->vector);
                RQ_FREE(vd);
            }
        }

        RQ_FREE(vb->vector_data);

        v->head = vb->next;

        RQ_FREE(vb);
    }

    RQ_FREE(v);
}

RQ_EXPORT void
rq_vector_clear(rq_vector_t v)
{
    struct rq_vector_block *vb = v->head;
    while (vb)
    {
        unsigned i;

        for (i = 0; i < v->block_size; i++)
        {
            struct rq_vector_data *vd = vb->vector_data[i];
            if (vd)
            {
                unsigned j;

                for (j = 0; j < v->vector_data_size; j++)
                    vd->vector[j] = 0;
            }
        }

        vb = vb->next;
    }

    v->size = 0;
}

RQ_EXPORT unsigned long
rq_vector_size(rq_vector_t v)
{
    return v->size;
}

RQ_EXPORT double
rq_vector_get(rq_vector_t v, unsigned long offset)
{
    if (offset < v->size)
    {
        /* special case for first node, to speed up small vector access */
        if (offset < v->vector_data_size && v->head && v->head->vector_data[0])
        {
            return v->head->vector_data[0]->vector[offset];
        }
        else
        {
            unsigned vector_data_size = v->block_size * v->vector_data_size;
            unsigned block_num = (unsigned)(offset / vector_data_size);
            unsigned i;
            struct rq_vector_block *vb;

            for (vb = v->head, i = 0; i < block_num && vb; i++, vb = vb->next)
                ; /* skip */

            if (vb)
            {
                unsigned block_offset = offset - (vector_data_size * block_num);

                /* which block array offset is it? */
                unsigned blk = block_offset / v->vector_data_size;

                struct rq_vector_data *vd = vb->vector_data[blk];

                unsigned data_offset = block_offset - (blk * v->vector_data_size);

                return vd->vector[data_offset];
            }
        }
    }

    return 0.0;
}

RQ_EXPORT void
rq_vector_set(rq_vector_t v, unsigned long offset, double val)
{
    unsigned objs_per_block = v->block_size * v->vector_data_size;
    unsigned block_num = (unsigned)(offset / objs_per_block);
    struct rq_vector_block *vb;
    unsigned block_offset;
    unsigned blk;
    struct rq_vector_data *vd;
    unsigned node_offset;

    while (block_num >= v->block_count)
    {
        struct rq_vector_block *new_nb = (struct rq_vector_block *)RQ_CALLOC(1, sizeof(struct rq_vector_block));

        new_nb->vector_data = (struct rq_vector_data **)RQ_CALLOC(RQ_VECTOR_BLOCK_SIZE, sizeof(struct rq_vector_data *));

        if (v->tail)
            v->tail->next = new_nb;
        else
            v->head = new_nb;

        v->tail = new_nb;
        v->block_count++;
    }

    vb = v->tail;

    block_offset = offset - (objs_per_block * block_num);

    /* which block array offset is it? */
    blk = block_offset / v->vector_data_size;

    vd = vb->vector_data[blk];

    if (!vd)
    {
        vd = vb->vector_data[blk] = (struct rq_vector_data *)RQ_CALLOC(1, sizeof(struct rq_vector_data));
        vd->vector = RQ_CALLOC(RQ_VECTOR_DATA_SIZE, sizeof(double));
    }

    node_offset = block_offset - (blk * v->vector_data_size);

    vd->vector[node_offset] = val;

    if (v->size <= offset)
        v->size = offset+1;
}

/* -- generic iterator support ------------------------------------ */

struct iter_node {
    rq_vector_t vector;
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
    if (iter->cur_pos < rq_vector_size(iter->vector))
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
    return (iter->cur_pos >= rq_vector_size(iter->vector));
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
    unsigned long vector_size = rq_vector_size(iter->vector);

    if (seek_pos == RQ_ITERATOR_SEEK_START)
        iter->cur_pos = 0;

    if (seek_pos == RQ_ITERATOR_SEEK_END)
        if (vector_size > 0)
            iter->cur_pos = vector_size - 1;
    
    if (seek_pos == RQ_ITERATOR_SEEK_START || (seek_pos == RQ_ITERATOR_SEEK_CUR && offset >= 0))
    {
        iter->cur_pos += offset;
        if (iter->cur_pos >= vector_size)
        {
            if (vector_size > 0)
                iter->cur_pos = vector_size-1;
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
    if (iter->cur_pos >= 0)
    {
        double d = rq_vector_get(iter->vector, (unsigned long)iter->cur_pos);
        rq_variant_init_double(&ret, d);
    }
    else
        rq_variant_init_nil(&ret);
    return ret;
}

RQ_EXPORT rq_iterator_t 
rq_vector_get_iterator(rq_vector_t vector)
{
    struct iter_node *iter = (struct iter_node *)RQ_CALLOC(1, sizeof(struct iter_node));

    iter->vector = vector;
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

