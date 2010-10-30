/*
** rq_set_rb.c
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
#include "rq_set_rb.h"
#include <stdlib.h>
#include <string.h>


/* -- functions --------------------------------------------------- */

static void
rq_set_rb_node_free(struct rq_set_rb_node *n, void (*free_func)(void *))
{
    if (free_func)
        (*free_func)((void *)n->key);
    if (n->left)
        rq_set_rb_node_free(n->left, free_func);
    if (n->right)
        rq_set_rb_node_free(n->right, free_func);
    RQ_FREE(n);
}

RQ_EXPORT rq_set_rb_t
rq_set_rb_alloc(
    void (*free_func)(void *), 
    int (*cmp_func)(const void *, const void *)
    )
{
    struct rq_set_rb *set = 
        (struct rq_set_rb *)RQ_MALLOC(sizeof(struct rq_set_rb));

    set->head = NULL;
    set->free_func = free_func;
    set->cmp_func = cmp_func;
    set->depth = 0;

    return set;
}


RQ_EXPORT void 
rq_set_rb_clear(rq_set_rb_t t)
{
    if (t->head)
    {
        rq_set_rb_node_free(t->head, t->free_func);
        t->head = NULL;
    }
}

static struct rq_set_rb_node *
rq_set_rb_node_clone(struct rq_set_rb_node *n, void *(*cpy_func)(const void *))
{
    struct rq_set_rb_node *cn = (struct rq_set_rb_node *)RQ_CALLOC(1, sizeof(struct rq_set_rb_node));
    cn->is_red = n->is_red;
    if (n->left)
        cn->left = rq_set_rb_node_clone(n->left, cpy_func);
    if (n->right)
        cn->right = rq_set_rb_node_clone(n->right, cpy_func);
    cn->key = (*cpy_func)(n->key);

    return cn;
}

RQ_EXPORT void 
rq_set_rb_copy(rq_set_rb_t t_dst, rq_set_rb_t t_src, void *(*cpy_func)(const void *))
{
    if (t_dst->head)
    {
        rq_set_rb_node_free(t_dst->head, t_dst->free_func);
        t_dst->head = NULL;
    }

    t_dst->free_func = t_src->free_func;
    t_dst->cmp_func = t_src->cmp_func;

    if (t_src->head)
        t_dst->head = rq_set_rb_node_clone(t_src->head, cpy_func);
}


RQ_EXPORT rq_set_rb_t 
rq_set_rb_clone(rq_set_rb_t t, void *(*cpy_func)(const void *))
{
    struct rq_set_rb *ct = 
        (struct rq_set_rb *)RQ_CALLOC(1, sizeof(struct rq_set_rb));
    ct->free_func = t->free_func;
    ct->cmp_func = t->cmp_func;

    if (t->head)
        ct->head = rq_set_rb_node_clone(t->head, cpy_func);

    return ct;
}

RQ_EXPORT void
rq_set_rb_free(rq_set_rb_t t)
{
    if (t->head)
        rq_set_rb_node_free(t->head, t->free_func);

    RQ_FREE(t);
}


RQ_EXPORT const void *
rq_set_rb_find(rq_set_rb_t t, const void *key)
{
    struct rq_set_rb_node *n = t->head;

    while (n)
    {
        int cmp = (*t->cmp_func)(n->key, key);
        if (!cmp)
            return n->key;
        else
            n = (cmp < 0 ? n->left : n->right);
    }
    return NULL;
}

RQ_EXPORT void
rq_set_rb_add(rq_set_rb_t t, const void *key)
{
    struct rq_set_rb_node *n = t->head;

    if (!n)
    {
        struct rq_set_rb_node *nn = (struct rq_set_rb_node *)RQ_CALLOC(1, sizeof(struct rq_set_rb_node));
        t->head = nn;
        t->depth = 1;
        nn->key = key;
		return;
    }
    else
    {
        struct rq_set_rb_node *gg = NULL;
        struct rq_set_rb_node *g = NULL;
        struct rq_set_rb_node *p = NULL;
        int gg_cmp = 0;
        int g_cmp = 0;
        int p_cmp = 0;
        int cmp = 0;
        while (n)
        {
            cmp = (*t->cmp_func)(n->key, key);
            if (cmp == 0)
            {
                /* this node already exists */

                return;
            }
            else
            {
                gg = g; /* great-grandfather = grandfather */
                g = p; /* grandfather = parent */
                p = n; /* parent = node */

				/* remember all the comparison values down the set */
				gg_cmp = g_cmp;
				g_cmp = p_cmp;
				p_cmp = cmp;
                
                n = (cmp < 0 ? n->left : n->right);

                if (n && n->left && n->right && n->left->is_red && n->right->is_red)
                {
                    n->is_red = 1;
                    n->left->is_red = n->right->is_red = 0;
                    if (p->is_red)
                    {
                        if (g)
                        {
                            g->is_red = 1;
                            if (g_cmp != p_cmp)
                            {
                                /* rotate nodes under g */
                                struct rq_set_rb_node *gc;

                                if (p_cmp < 0)
                                {
                                    gc = p->left;
                                    p->left = gc->right;
                                    gc->right = p;
                                }
                                else
                                {
                                    gc = p->right;
                                    p->right = gc->left;
                                    gc->left = p;
                                }

                                if (g_cmp < 0)
                                    g->left = gc;
                                else
                                    g->right = gc;

                                /* set parent to be gc */
                                p = gc;
                                /* p_cmp = (*t->cmp)(p->key, key); */
                            }

                            /* rotate nodes under gg */
                            if (gg)
                            {
                                struct rq_set_rb_node *gc;
                                
                                if (g_cmp < 0)
                                {
                                    gc = g->left;
                                    g->left = gc->right;
                                    gc->right = g;
                                }
                                else
                                {
                                    gc = g->right;
                                    g->right = gc->left;
                                    gc->left = g;
                                }

                                if (gg_cmp < 0)
                                    gg->left = gc;
                                else
                                    gg->right = gc;

                                n = gc;
                                n->is_red = 0;
                            } /* if gg */
                        } /* if g */
                    } /* if p->is_red */
                }
            } /* else cmp != 0 */
        } /* while (n) */

        n = (struct rq_set_rb_node *)RQ_CALLOC(1, sizeof(struct rq_set_rb_node));
        n->key = key;

        if (p_cmp < 0)
            p->left = n;
        else
            p->right = n;

        n->is_red = 1;
        if (p->is_red)
        {
            if (g)
            {
                g->is_red = 1;
                if (g_cmp != p_cmp)
                {
                    /* rotate nodes under g */
                    struct rq_set_rb_node *gc;

                    if (p_cmp < 0)
                    {
                        gc = p->left;
                        p->left = gc->right;
                        gc->right = p;
                    }
                    else
                    {
                        gc = p->right;
                        p->right = gc->left;
                        gc->left = p;
                    }

                    if (g_cmp < 0)
                        g->left = gc;
                    else
                        g->right = gc;

                    /* set parent to be gc */
                    p = gc;
                }

                /* rotate nodes under gg */
                if (gg)
                {
                    struct rq_set_rb_node *gc;

                    if (g_cmp < 0)
                    {
                        gc = g->left;
                        g->left = gc->right;
                        gc->right = g;
                    }
                    else
                    {
                        gc = g->right;
                        g->right = gc->left;
                        gc->left = g;
                    }

                    if (gg_cmp < 0)
                        gg->left = gc;
                    else
                        gg->right = gc;

                    n = gc;
                    n->is_red = 0;
                }
            }
        }

        t->depth++;
    }
}

int
rq_set_rb_is_null(rq_set_rb_t obj)
{
    return (obj == NULL);
}
