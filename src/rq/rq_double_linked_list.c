/*
** rq_double_linked_list.c
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
/* -- includes ---------------------------------------------------- */
#include "rq_double_linked_list.h"
#include <stdlib.h>


/* -- functions --------------------------------------------------- */
static void
rq_double_linked_list_node_free(struct rq_double_linked_list_node *n, void (*free_func)(void *))
{
    if (n->next)
        rq_double_linked_list_node_free(n->next, free_func);
    if (n->data && free_func)
        (*free_func)(n->data);
    RQ_FREE(n);
}

RQ_EXPORT rq_double_linked_list_t 
rq_double_linked_list_alloc(void (*free_func)(void *))
{
    struct rq_double_linked_list *ll = (struct rq_double_linked_list *)RQ_CALLOC(
        1, 
        sizeof(struct rq_double_linked_list)
        );
    ll->free_func = free_func;
    return ll;
}

RQ_EXPORT void 
rq_double_linked_list_free(rq_double_linked_list_t ll)
{
    if (ll->head)
        rq_double_linked_list_node_free(ll->head, ll->free_func);

    RQ_FREE(ll);
    
}

RQ_EXPORT unsigned 
rq_double_linked_list_size(rq_double_linked_list_t ll)
{
    return ll->count;
}

/* -- generic iterator support ------------------------------------ */

struct iter_node {
    rq_double_linked_list_t ll;
    struct rq_double_linked_list_node *cur_pos;
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
    if (iter->cur_pos)
        iter->cur_pos = iter->cur_pos->next;
}

static void
iter_decr(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    if (iter->cur_pos)
        iter->cur_pos = iter->cur_pos->prev;
}

static short
iter_atend(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    return iter->cur_pos == NULL;
}

static short
iter_atbegin(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    return iter->cur_pos == NULL;
}

static short
iter_seek(void *iterdata, enum rq_iterator_seek_position seek_pos, long offset)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    if (seek_pos == RQ_ITERATOR_SEEK_START)
        iter->cur_pos = iter->ll->head;

    if (seek_pos == RQ_ITERATOR_SEEK_END)
        iter->cur_pos = iter->ll->tail;
    
    if (seek_pos == RQ_ITERATOR_SEEK_START || (seek_pos == RQ_ITERATOR_SEEK_CUR && offset >= 0))
    {
        int i;

        for (i = 0; i < offset; i++)
        {
            if (!iter->cur_pos)
                return i;
            iter->cur_pos = iter->cur_pos->next;
        }

        return 0; /* successful */
    }
    else if (seek_pos == RQ_ITERATOR_SEEK_END || (seek_pos == RQ_ITERATOR_SEEK_CUR && offset < 0))
    {
        int i;

        if (offset < 0)
            offset = -offset;

        for (i = 0; i < offset; i++)
        {
            if (!iter->cur_pos)
                return i;
            iter->cur_pos = iter->cur_pos->prev;
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
    if (iter->cur_pos)
        rq_variant_init_object(&ret, 0, iter->cur_pos->data);
    else
        rq_variant_init_nil(&ret);
    return ret;
}

RQ_EXPORT rq_iterator_t 
rq_double_linked_list_get_iterator(rq_double_linked_list_t linked_list)
{
    struct iter_node *iter = (struct iter_node *)RQ_CALLOC(1, sizeof(struct iter_node));

    iter->ll = linked_list;
    iter->cur_pos = linked_list->head;

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
/* -- end of generic iterator support ----------------------------- */




RQ_EXPORT rq_double_linked_list_iterator_t 
rq_double_linked_list_begin(rq_double_linked_list_t ll)
{
    return ll->head;
}

RQ_EXPORT short 
rq_double_linked_list_iterator_at_end(rq_double_linked_list_iterator_t iter)
{
    return iter == NULL;
}

RQ_EXPORT rq_double_linked_list_iterator_t 
rq_double_linked_list_rbegin(rq_double_linked_list_t ll)
{
    return ll->tail;
}

RQ_EXPORT short 
rq_double_linked_list_iterator_at_rend(rq_double_linked_list_iterator_t iter)
{
    return iter == NULL;
}

RQ_EXPORT rq_double_linked_list_iterator_t 
rq_double_linked_list_iterator_incr(rq_double_linked_list_iterator_t iter)
{
    if (iter)
        iter = iter->next;

    return iter;
}

RQ_EXPORT rq_double_linked_list_iterator_t 
rq_double_linked_list_iterator_decr(rq_double_linked_list_iterator_t iter)
{
    if (iter)
        iter = iter->prev;

    return iter;
}

RQ_EXPORT void *
rq_double_linked_list_iterator_get_data(rq_double_linked_list_iterator_t iter)
{
    if (iter)
        return iter->data;

    return NULL;
}

RQ_EXPORT void 
rq_double_linked_list_append(rq_double_linked_list_t ll, void *data)
{
    struct rq_double_linked_list_node *n = (struct rq_double_linked_list_node *)RQ_CALLOC(
        1,
        sizeof(struct rq_double_linked_list_node)
        );

    n->data = data;

    if (ll->tail)
    {
        ll->tail->next = n;
        n->prev = ll->tail;
        ll->tail = n;
    }
    else
        ll->tail = ll->head = n;
    ll->count++;
}

RQ_EXPORT void 
rq_double_linked_list_prepend(rq_double_linked_list_t ll, void *data)
{
    struct rq_double_linked_list_node *n = (struct rq_double_linked_list_node *)RQ_CALLOC(
        1,
        sizeof(struct rq_double_linked_list_node)
        );

    n->data = data;

    if (ll->head)
    {
        n->next = ll->head;
        ll->head->prev = n;
        ll->head = n;
    }
    else
        ll->tail = ll->head = n;
    ll->count++;
}


RQ_EXPORT int
rq_double_linked_list_is_null(rq_double_linked_list_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_double_linked_list_iterator_t
rq_double_linked_list_erase(rq_double_linked_list_t ll, rq_double_linked_list_iterator_t iter)
{
    rq_double_linked_list_iterator_t ret = NULL;

    if (iter->next)
    {
        iter->next->prev = iter->prev;

        ret = iter->next;
    }
    else
        ll->tail = iter->prev;

    if (iter->prev)
        iter->prev->next = iter->next;
    else
        ll->head = iter->next;

    if (iter->data && ll->free_func)
        (*ll->free_func)(iter->data);
    RQ_FREE(iter);

    ll->count--;

    return iter;
}
