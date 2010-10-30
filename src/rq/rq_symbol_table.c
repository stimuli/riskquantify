/*
** rq_symbol_table.c
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
#include "rq_symbol_table.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>


static void
rq_symbol_table_node_free_obj(void *p)
{
    struct rq_symbol_table_node *n = (struct rq_symbol_table_node *)p;
   RQ_FREE((char *)n->id);
   RQ_FREE(n);
}

static void
rq_symbol_table_node_free(void *p)
{
    struct rq_symbol_table_node *n = (struct rq_symbol_table_node *)p;
	RQ_FREE((char *)n->id);
	RQ_FREE(n);
}

RQ_EXPORT int 
rq_symbol_table_is_null(rq_symbol_table_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_symbol_table_t 
rq_symbol_table_alloc(unsigned short free_objects)
{
    struct rq_symbol_table *st = (struct rq_symbol_table *)
        RQ_MALLOC(sizeof(struct rq_symbol_table));
    if (free_objects)
        st->tree = rq_tree_rb_alloc(
            rq_symbol_table_node_free_obj,
            (int (*)(const void *, const void *))strcmp
            );
    else
        st->tree = rq_tree_rb_alloc(
            rq_symbol_table_node_free,
            (int (*)(const void *, const void *))strcmp
            );

    return st;
}

RQ_EXPORT void 
rq_symbol_table_free(rq_symbol_table_t st)
{
    rq_tree_rb_free(st->tree);
    RQ_FREE(st);
}

RQ_EXPORT void 
rq_symbol_table_clear(rq_symbol_table_t st)
{
    rq_tree_rb_clear(st->tree);
}

RQ_EXPORT void 
rq_symbol_table_add(rq_symbol_table_t st, const char *id, rq_object_t object)
{
    struct rq_symbol_table_node *n = (struct rq_symbol_table_node *)
        RQ_MALLOC(sizeof(struct rq_symbol_table_node));
    n->id = (const char *)RQ_STRDUP(id);
    n->object = object;
    rq_tree_rb_add(
        st->tree,
        (void *)n->id,
        n
        );
}

RQ_EXPORT rq_object_t 
rq_symbol_table_get(rq_symbol_table_t st, const char *id)
{
    struct rq_symbol_table_node *n = (struct rq_symbol_table_node *)
        rq_tree_rb_find(st->tree, (void *)id);
    if (n)
        return n->object;
    return rq_object_nil;
}

RQ_EXPORT rq_symbol_table_iterator_t 
rq_symbol_table_begin(rq_symbol_table_t st)
{
    rq_symbol_table_iterator_t si = (rq_symbol_table_iterator_t)RQ_MALLOC(
        sizeof(struct rq_symbol_table_iterator)
        );
    si->symbol_it = rq_tree_rb_iterator_alloc();
    rq_tree_rb_begin(st->tree, si->symbol_it);
    return si;
}

RQ_EXPORT int 
rq_symbol_table_at_end(rq_symbol_table_iterator_t it)
{
    return rq_tree_rb_at_end(it->symbol_it);
}

RQ_EXPORT void 
rq_symbol_table_next(rq_symbol_table_iterator_t it)
{
    rq_tree_rb_next(it->symbol_it);
}

RQ_EXPORT void 
rq_symbol_table_iterator_free(rq_symbol_table_iterator_t it)
{
    rq_tree_rb_iterator_free(it->symbol_it);
    RQ_FREE(it);
}

RQ_EXPORT const char *
rq_symbol_table_iterator_get_identifier(rq_symbol_table_iterator_t it)
{
    struct rq_symbol_table_node *n = 
        (struct rq_symbol_table_node *)rq_tree_rb_iterator_deref(it->symbol_it);
    return n->id;
}

RQ_EXPORT rq_object_t 
rq_symbol_table_iterator_get_object(rq_symbol_table_iterator_t it)
{
    struct rq_symbol_table_node *n = 
        (struct rq_symbol_table_node *)rq_tree_rb_iterator_deref(it->symbol_it);
    return n->object;
}
