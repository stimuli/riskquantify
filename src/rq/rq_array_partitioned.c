/*
** rq_array_partitioned.c
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
#include "rq_array_partitioned.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


RQ_EXPORT int 
rq_array_partitioned_is_null(rq_array_partitioned_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_array_partitioned_t 
rq_array_partitioned_alloc(void (*free_func)(void *))
{
    struct rq_array_partitioned *a = (struct rq_array_partitioned *)RQ_MALLOC(sizeof(struct rq_array_partitioned));
    struct rq_array_partitioned_node_block *nb = (struct rq_array_partitioned_node_block *)RQ_MALLOC(sizeof(struct rq_array_partitioned_node_block));
    struct rq_array_partitioned_node *an = (struct rq_array_partitioned_node *)RQ_MALLOC(sizeof(struct rq_array_partitioned_node));

    an->array = RQ_CALLOC(RQ_DEFAULT_ARRAY_GROW_SIZE, sizeof(void *));

    nb->next = NULL;
    nb->array_nodes = (struct rq_array_partitioned_node **)RQ_CALLOC(RQ_DEFAULT_ARRAY_BLOCK_SIZE, sizeof(struct rq_array_partitioned_node *));
    nb->array_nodes[0] = an;

    a->block_count = 1;
    a->block_size = RQ_DEFAULT_ARRAY_BLOCK_SIZE;

    a->objs_per_node = RQ_DEFAULT_ARRAY_GROW_SIZE;

    a->head = a->tail = nb;

    a->size = 0;

    a->free_func = free_func;
    
    return a;
}

RQ_EXPORT void
rq_array_partitioned_free(rq_array_partitioned_t v)
{
    while (v->head)
    {
		struct rq_array_partitioned_node_block *nb = v->head;
        unsigned i;

        for (i = 0; i < v->block_size; i++)
        {
            struct rq_array_partitioned_node *an = nb->array_nodes[i];
            if (an)
            {
                unsigned j;

                if (v->free_func)
                    for (j = 0 ; j < v->objs_per_node; j++)
                        if (an->array[j])
                            (*v->free_func)(an->array[j]);

                RQ_FREE(an->array);
                RQ_FREE(an);
            }
        }

        RQ_FREE(nb->array_nodes);

        v->head = nb->next;

        RQ_FREE(nb);
    }

    RQ_FREE(v);
}

RQ_EXPORT void
rq_array_partitioned_clear(rq_array_partitioned_t v)
{
    struct rq_array_partitioned_node_block *nb = v->head;
    while (nb)
    {
        unsigned i;

        for (i = 0; i < v->block_size; i++)
        {
            struct rq_array_partitioned_node *an = nb->array_nodes[i];
            if (an)
            {
                unsigned j;

                if (v->free_func)
                    for (j = 0; j < v->objs_per_node; j++)
                        if (an->array[j])
                        {
                            (*v->free_func)(an->array[j]);
                            an->array[j] = 0;
                        }

            }
        }

        nb = nb->next;
    }

    v->size = 0;
}

RQ_EXPORT unsigned int 
rq_array_partitioned_size(rq_array_partitioned_t v)
{
    return v->size;
}

RQ_EXPORT void *
rq_array_partitioned_get_at(rq_array_partitioned_t v, unsigned int offset)
{
    if (offset < v->size)
    {
        /* special case for first node, to speed up small array access */
        if (offset < v->objs_per_node)
        {
            return v->head->array_nodes[0]->array[offset];
        }
        else
        {
            unsigned objs_per_block = v->block_size * v->objs_per_node;
            unsigned block_num = (unsigned)(offset / objs_per_block);
            unsigned i;
            struct rq_array_partitioned_node_block *nb;

            for (nb = v->head, i = 0; i < block_num && nb; i++, nb = nb->next)
                ; /* skip */

            if (nb)
            {
                unsigned block_offset = offset - (objs_per_block * block_num);

                /* which block array offset is it? */
                unsigned blk = block_offset / v->objs_per_node;

                struct rq_array_partitioned_node *an = nb->array_nodes[blk];

                unsigned node_offset = block_offset - (blk * v->objs_per_node);

                return an->array[node_offset];
            }
        }
    }

    return NULL;
}

RQ_EXPORT void
rq_array_partitioned_push_back(rq_array_partitioned_t v, void *obj)
{
    unsigned offset = v->size;
    unsigned objs_per_block = v->block_size * v->objs_per_node;
    unsigned block_num = (unsigned)(offset / objs_per_block);
    struct rq_array_partitioned_node_block *nb;
    unsigned block_offset;
    unsigned blk;
    struct rq_array_partitioned_node *an;
    unsigned node_offset;

    while (block_num >= v->block_count)
    {
        struct rq_array_partitioned_node_block *new_nb = (struct rq_array_partitioned_node_block *)RQ_CALLOC(1, sizeof(struct rq_array_partitioned_node_block));

        new_nb->array_nodes = (struct rq_array_partitioned_node **)RQ_CALLOC(RQ_DEFAULT_ARRAY_BLOCK_SIZE, sizeof(struct rq_array_partitioned_node *));

        if (v->tail)
            v->tail->next = new_nb;
        else
            v->head = new_nb;

        v->tail = new_nb;
        v->block_count++;
    }

    nb = v->tail;

    block_offset = offset - (objs_per_block * block_num);

    /* which block array offset is it? */
    blk = block_offset / v->objs_per_node;

    an = nb->array_nodes[blk];

    if (!an)
    {
        an = nb->array_nodes[blk] = (struct rq_array_partitioned_node *)RQ_CALLOC(1, sizeof(struct rq_array_partitioned_node));
        an->array = RQ_CALLOC(RQ_DEFAULT_ARRAY_GROW_SIZE, sizeof(void *));
    }

    node_offset = block_offset - (blk * v->objs_per_node);

    an->array[node_offset] = obj;

    v->size++;
}

