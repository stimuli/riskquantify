/*
** rq_tree_rb.c
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
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* -- functions --------------------------------------------------- */

static void
rq_tree_rb_node_free(struct rq_tree_rb_node *n, void (*free_func)(void *))
{
    if (free_func)
        (*free_func)(n->data);
    if (n->left)
        rq_tree_rb_node_free(n->left, free_func);
    if (n->right)
        rq_tree_rb_node_free(n->right, free_func);
    RQ_FREE(n);
}

RQ_EXPORT rq_tree_rb_t
rq_tree_rb_alloc(
    void (*free_func)(void *), 
    int (*cmp_func)(const void *, const void *)
    )
{
    struct rq_tree_rb *tree = 
        (struct rq_tree_rb *)RQ_MALLOC(sizeof(struct rq_tree_rb));

    tree->head = NULL;
    tree->free_func = free_func;
    tree->cmp_func = cmp_func;
    tree->size = 0;

    return tree;
}

RQ_EXPORT void 
rq_tree_rb_clear(rq_tree_rb_t t)
{
    if (t->head)
    {
        rq_tree_rb_node_free(t->head, t->free_func);
        t->head = NULL;
    }
    t->size = 0;
}

static struct rq_tree_rb_node *
rq_tree_rb_node_clone(struct rq_tree_rb_node *n, const void *(*key_func)(const void *), void *(*cpy_func)(const void *))
{
    struct rq_tree_rb_node *cn = (struct rq_tree_rb_node *)RQ_CALLOC(1, sizeof(struct rq_tree_rb_node));
    cn->is_red = n->is_red;
    if (n->left)
    {
        cn->left = rq_tree_rb_node_clone(n->left, key_func, cpy_func);
        cn->left->parent = cn;
    }
    if (n->right)
    {
        cn->right = rq_tree_rb_node_clone(n->right, key_func, cpy_func);
        cn->right->parent = cn;
    }
    cn->data = (*cpy_func)(n->data);

    cn->key = (*key_func)(cn->data);

    return cn;
}

RQ_EXPORT void 
rq_tree_rb_copy(rq_tree_rb_t t_dst, rq_tree_rb_t t_src, const void *(*key_func)(const void *), void *(*cpy_func)(const void *))
{
    if (t_dst->head)
    {
        rq_tree_rb_node_free(t_dst->head, t_dst->free_func);
        t_dst->head = NULL;
    }

    t_dst->free_func = t_src->free_func;
    t_dst->cmp_func = t_src->cmp_func;
    t_dst->size = t_src->size;

    if (t_src->head)
        t_dst->head = rq_tree_rb_node_clone(t_src->head, key_func, cpy_func);
}


RQ_EXPORT rq_tree_rb_t 
rq_tree_rb_clone(rq_tree_rb_t t, const void *(*key_func)(const void *), void *(*cpy_func)(const void *))
{
    struct rq_tree_rb *ct = 
        (struct rq_tree_rb *)RQ_CALLOC(1, sizeof(struct rq_tree_rb));
    ct->free_func = t->free_func;
    ct->cmp_func = t->cmp_func;
    ct->size = t->size;

    if (t->head)
        ct->head = rq_tree_rb_node_clone(t->head, key_func, cpy_func);

    return ct;
}

RQ_EXPORT void
rq_tree_rb_free(rq_tree_rb_t t)
{
    if (t->head)
        rq_tree_rb_node_free(t->head, t->free_func);

    RQ_FREE(t);
}

RQ_EXPORT unsigned long 
rq_tree_rb_size(rq_tree_rb_t t)
{
    return t->size;
}

static void
rq_tree_rb_node_dump(FILE *fh, struct rq_tree_rb_node *n, int depth, const char *print_func(const void *))
{
    if (n)
    {
        fprintf(
            fh, 
            "%*.*s%s (%s) ", 
            depth*4, depth*4, "", 
            print_func(n->key), 
            (n->is_red ? "r" : "b")
            );
        fprintf(
            fh,
            "[%s] ",
            (n->parent ? print_func(n->parent->key) : "")
            );
        fprintf(
            fh,
            "<l:%s,",
            (n->left ? print_func(n->left->key) : "")
            );
        fprintf(
            fh,
            "r:%s>\n",
            (n->right ? print_func(n->right->key) : "")
            );

        rq_tree_rb_node_dump(fh, n->left, depth+1, print_func);
        rq_tree_rb_node_dump(fh, n->right, depth+1, print_func);
    }
}

RQ_EXPORT void
rq_tree_rb_dump(rq_tree_rb_t t, FILE *fh, const char *print_func(const void *))
{
    rq_tree_rb_node_dump(fh, t->head, 0, print_func);
}

RQ_EXPORT void *
rq_tree_rb_find(rq_tree_rb_t t, const void *key)
{
    struct rq_tree_rb_node *n = t->head;

    while (n)
    {
        int cmp = (*t->cmp_func)(n->key, key);
        if (!cmp)
            return n->data;
        else
            n = (cmp < 0 ? n->left : n->right);
    }
    return NULL;
}

RQ_EXPORT void *
rq_tree_rb_locate(rq_tree_rb_t t, int (*cmp_func)(const void *, const void *), const void *key)
{
    struct rq_tree_rb_node *n = t->head;

    while (n)
    {
        int cmp = (*cmp_func)(n->data, key);
        if (!cmp)
            return n->data;
        else
            n = (cmp < 0 ? n->left : n->right);
    }

    return NULL;
}

static void
_rq_tree_rb_traverse_inorder(struct rq_tree_rb_node *n, void *user_data, void (*func)(void *user_data, void *node_data))
{
    if (n->right)
        _rq_tree_rb_traverse_inorder(n->right, user_data, func);
    (*func)(user_data, n->data);
    if (n->left)
        _rq_tree_rb_traverse_inorder(n->left, user_data, func);
}

RQ_EXPORT void 
rq_tree_rb_traverse_inorder(rq_tree_rb_t t, void *user_data, void (*func)(void *user_data, void *node_data))
{
    struct rq_tree_rb_node *n = t->head;

    if (n)
    {
        _rq_tree_rb_traverse_inorder(n, user_data, func);
    }
}

static void
right_rotate(struct rq_tree_rb *t, struct rq_tree_rb_node *n) 
{
    struct rq_tree_rb_node *o = n->left;
    /* Turn o's left sub-tree into n's right sub-tree */
    n->left = o->right;
    if (o->right != NULL)
        o->right->parent = n;
    /* o's new parent was n's parent */
    o->parent = n->parent;
    /* If we're not at the root, set the parent to point to o instead of n 
     */
    if (n->parent == NULL) 
        t->head = o;
    else if (n == n->parent->right)
        n->parent->right = o;
    else
        n->parent->left = o;
    /* Finally, put n on o's right */
    o->right = n;
    n->parent = o;
}

static void
left_rotate(struct rq_tree_rb *t, struct rq_tree_rb_node *n) 
{
    struct rq_tree_rb_node *o = n->right;
    /* Turn o's left sub-tree into n's right sub-tree */
    n->right = o->left;
    if (o->left != NULL)
        o->left->parent = n;
    /* o's new parent was n's parent */
    o->parent = n->parent;

    if (n->parent == NULL) 
        t->head = o;
    else if (n == n->parent->left)
        n->parent->left = o;
    else
        n->parent->right = o;
    /* Finally, put n on o's left */
    o->left = n;
    n->parent = o;
}

RQ_EXPORT void
rq_tree_rb_add(rq_tree_rb_t t, const void *key, void *data)
{
    struct rq_tree_rb_node *n = t->head;

    if (!n)
    {
        struct rq_tree_rb_node *nn = (struct rq_tree_rb_node *)RQ_CALLOC(1, sizeof(struct rq_tree_rb_node));
        t->head = nn;
        nn->key = key;
        nn->data = data;
        t->size++;
		return;
    }

    while (1)
    {
        int cmp = (*t->cmp_func)(n->key, key);
        if (cmp == 0)
        {
            /* this node already exists */

            if (n->data != data)
            {
                if (t->free_func)
                    (*t->free_func)(n->data);
                n->data = data;
                n->key = key;
            }

            return;
        }

        if (cmp < 0)
        {
            if (!n->left)
            {
                struct rq_tree_rb_node *nn = (struct rq_tree_rb_node *)RQ_CALLOC(1, sizeof(struct rq_tree_rb_node));
                n->left = nn;
                nn->parent = n;
                n = nn;
                break;
            }
            else
                n = n->left;
        }
        else
        {
            if (!n->right)
            {
                struct rq_tree_rb_node *nn = (struct rq_tree_rb_node *)RQ_CALLOC(1, sizeof(struct rq_tree_rb_node));
                n->right = nn;
                nn->parent = n;
                n = nn;
                break;
            }
            else
                n = n->right;
        }
    }

    t->size++;
    
    n->key = key;
    n->data = data;
    n->is_red = 1;
    if (n->parent && n->parent->parent)
    {
        while (n != t->head && n->parent && n->parent->parent && n->parent->is_red) 
        {
            if (n->parent == n->parent->parent->left) 
            {
                /* If n's parent is a left, o is n's right 'uncle' */
                struct rq_tree_rb_node *o = n->parent->parent->right;
                if (o)
                {
                    if (o->is_red) 
                    {
                        /* case 1 - flip the is_red flag */
                        n->parent->is_red = 0;
                        o->is_red = 0;
                        n->parent->parent->is_red = 1;
                        /* Move n up the tree */
                        n = n->parent->parent;
                    }
                    else 
                    {
                        /* o is a black node */
                        if (n == n->parent->right ) 
                        {
                            /* and n is to the right */ 
                            /* case 2 - move n up and rotate */
                            n = n->parent;
                            left_rotate(t, n);
                        }
                        /* case 3 */
                        n->parent->is_red = 0;
                        n->parent->parent->is_red = 1;
                        right_rotate(t, n->parent->parent);
                    }
                }
                else
                {
                    /* case 3 */
                    n->parent->is_red = 0;
                    n->parent->parent->is_red = 1;
                    right_rotate(t, n->parent->parent);
                    n = n->parent;
                }
            }
            else 
            {
                struct rq_tree_rb_node *o = n->parent->parent->left;
                if (o)
                {
                    if (o->is_red) 
                    {
                        /* case 1 - flip the is_red flag */
                        n->parent->is_red = 0;
                        o->is_red = 0;
                        n->parent->parent->is_red = 1;
                        /* Move n up the tree */
                        n = n->parent->parent;
                    }
                    else 
                    {
                        /* o is a black node */
                        if (n == n->parent->left) 
                        {
                            /* and n is to the left */ 
                            /* case 2 - move n up and rotate */
                            n = n->parent;
                            right_rotate(t, n);
                        }
                        /* case 3 */
                        n->parent->is_red = 0;
                        n->parent->parent->is_red = 1;
                        left_rotate(t, n->parent->parent);
                    }
                }
                else
                {
                    /* case 3 */
                    n->parent->is_red = 0;
                    n->parent->parent->is_red = 1;
                    left_rotate(t, n->parent->parent);
                    n = n->parent;
                }
            }
        }
    }

    /* Make sure the root is black */
    t->head->is_red = 0;
}

struct min_max_depths {
    unsigned long min_depth;
    unsigned long max_depth;
};

static void
recurse_find_depths(struct rq_tree_rb_node *n, struct min_max_depths *depths, unsigned long cur_depth)
{
    if (n)
    {
        if (!n->left && !n->right)
        {
            if (cur_depth > depths->max_depth)
                depths->max_depth = cur_depth;

            if (!depths->min_depth || cur_depth < depths->min_depth)
                depths->min_depth = cur_depth;
        }

        if (n->left)
            recurse_find_depths(n->left, depths, cur_depth+1);

        if (n->right)
            recurse_find_depths(n->right, depths, cur_depth+1);
    }
}

unsigned long
recurse_count_nodes(struct rq_tree_rb_node *n)
{
    unsigned long count = 0;
    if (n)
    {
        count++;
        if (n->left)
            count += recurse_count_nodes(n->left);
        if (n->right)
            count += recurse_count_nodes(n->right);
    }
    return count;
}

RQ_EXPORT short
rq_tree_rb_is_consistant(rq_tree_rb_t t)
{
    struct min_max_depths depths;
    unsigned long count;
    depths.min_depth = 0;
    depths.max_depth = 0;
    recurse_find_depths(t->head, &depths, 1);

    /* printf("min_depth = %d, max_depth = %d\n", depths.min_depth, depths.max_depth); */

    if (depths.max_depth - depths.min_depth > 3)
        return 0;

    count = recurse_count_nodes(t->head);
    /* printf("Count = %d, size = %d\n", count, t->size); */

    if (count != t->size)
        return 0;

    return 1;
}

RQ_EXPORT void
rq_tree_rb_remove(rq_tree_rb_t t, const void *key)
{
    struct rq_tree_rb_node *n = t->head;
    struct rq_tree_rb_node *x;
    struct rq_tree_rb_node *y;

    while (n)
    {
        int cmp = (*t->cmp_func)(n->key, key);
        if (!cmp)
            break;
        else
            n = (cmp < 0 ? n->left : n->right);
    }

    if (!n)
        return; /* Couldn't find it. */

    t->size--;
    if (t->free_func)
        (*t->free_func)(n->data);

    if (!n->left || !n->right) 
        y = n;
    else 
    {
        /* find tree decendant with zero or 1 child */
        y = n->right;
        while (y->left) 
            y = y->left;
    }

    /* x is y's only child */
    if (y->left)
        x = y->left;
    else
        x = y->right;

    /* remove y from the parent chain */
    if (x)
        x->parent = y->parent;
    if (y->parent)
    {
        if (y == y->parent->left)
            y->parent->left = x;
        else
            y->parent->right = x;
    }
    else
        t->head = x;

    if (y != n) 
    {
        n->key = y->key;
        n->data = y->data;
    }


    if (!y->is_red)
    {
        while (x && x != t->head && !x->is_red)
        {
            if (x == x->parent->left) 
            {
                struct rq_tree_rb_node *w = x->parent->right;
                if (w->is_red) 
                {
                    w->is_red = 0;
                    x->parent->is_red = 1;
                    left_rotate(t, x->parent);
                    w = x->parent->right;
                }
                if (!w->left->is_red && !w->right->is_red) 
                {
                    w->is_red = 1;
                    x = x->parent;
                } 
                else 
                {
                    if (!w->right->is_red) 
                    {
                        w->left->is_red = 0;
                        w->is_red = 1;
                        right_rotate(t, w);
                        w = x->parent->right;
                    }

                    w->is_red = x->parent->is_red;
                    x->parent->is_red = 0;
                    w->right->is_red = 0;
                    left_rotate(t, x->parent);
                    x = t->head;
                }
            } 
            else 
            {
                struct rq_tree_rb_node *w = x->parent->left;
                if (w->is_red) 
                {
                    w->is_red = 0;
                    x->parent->is_red = 1;
                    right_rotate(t, x->parent);
                    w = x->parent->left;
                }
                if (!w->right->is_red && !w->left->is_red) 
                {
                    w->is_red = 1;
                    x = x->parent;
                } 
                else 
                {
                    if (!w->left->is_red) 
                    {
                        w->right->is_red = 0;
                        w->is_red = 1;
                        left_rotate(t, w);
                        w = x->parent->left;
                    }

                    w->is_red = x->parent->is_red;
                    x->parent->is_red = 0;
                    w->left->is_red = 0;
                    right_rotate(t, x->parent);
                    x = t->head;
                }
            }
        }

        if (x)
            x->is_red = 0;
    }

    RQ_FREE(y);
}

RQ_EXPORT int 
rq_tree_rb_is_null(rq_tree_rb_t tree)
{
    return (tree == NULL);
}

static struct rq_tree_rb_node *
recurse_find_first(struct rq_tree_rb_node *n)
{
    if (!n)
        return NULL;

    if (n->right)
        return recurse_find_first(n->right);
    return n;
}

static struct rq_tree_rb_node *
recurse_find_last(struct rq_tree_rb_node *n)
{
    if (!n)
        return NULL;

    if (n->left)
        return recurse_find_last(n->left);
    return n;
}

/* -- start of generic iterator support --------------------------- */

struct iter_node {
    rq_tree_rb_t tree;
    struct rq_tree_rb_node *cur_node;
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
    struct iter_node *it = (struct iter_node *)iterdata;
    struct rq_tree_rb_node *n = it->cur_node;
    if (n)
    {
        if (n->left)
            it->cur_node = recurse_find_first(n->left);
        else if (n->parent)
        {
            struct rq_tree_rb_node *r;

            it->cur_node = NULL;

            while ((r = n->parent) != NULL)
            {
                if (r->right == n)
                {
                    it->cur_node = r;
                    break;
                }
                n = r;
            }
        }
        else
            it->cur_node = NULL;
    }
}

static void
iter_decr(void *iterdata)
{
    struct iter_node *it = (struct iter_node *)iterdata;
    struct rq_tree_rb_node *n = it->cur_node;
    if (n)
    {
        if (n->right)
            it->cur_node = recurse_find_last(n->right);
        else if (n->parent)
        {
            struct rq_tree_rb_node *l;

            it->cur_node = NULL;

            while ((l = n->parent) != NULL)
            {
                if (l->left == n)
                {
                    it->cur_node = l;
                    break;
                }
                n = l;
            }
        }
        else
            it->cur_node = NULL;
    }
}

static short
iter_atend(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    return iter->cur_node == NULL;
}

static short
iter_atbegin(void *iterdata)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    return iter->cur_node == NULL;
}

static short
iter_seek(void *iterdata, enum rq_iterator_seek_position seek_pos, long offset)
{
    struct iter_node *iter = (struct iter_node *)iterdata;
    if (seek_pos == RQ_ITERATOR_SEEK_START)
        iter->cur_node = recurse_find_first(iter->tree->head);

    if (seek_pos == RQ_ITERATOR_SEEK_END)
        iter->cur_node = recurse_find_last(iter->tree->head);
    
    if (seek_pos == RQ_ITERATOR_SEEK_START || (seek_pos == RQ_ITERATOR_SEEK_CUR && offset >= 0))
    {
        int i;

        for (i = 0; i < offset; i++)
        {
            if (!iter->cur_node)
                return i;
            iter_incr(iter);
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
            if (!iter->cur_node)
                return i;
            iter_decr(iter);
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
    if (iter->cur_node)
        rq_variant_init_object(&ret, 0, iter->cur_node->data);
    else
        rq_variant_init_nil(&ret);
    return ret;
}


RQ_EXPORT rq_iterator_t 
rq_tree_rb_get_iterator(rq_tree_rb_t tree)
{
    struct iter_node *iter = (struct iter_node *)RQ_CALLOC(1, sizeof(struct iter_node));

    iter->tree = tree;

	if (tree->head)
        iter->cur_node = recurse_find_first(tree->head);
    else
        iter->cur_node = NULL;

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

RQ_EXPORT rq_tree_rb_iterator_t 
rq_tree_rb_iterator_alloc()
{
    struct rq_tree_rb_iterator *it = (struct rq_tree_rb_iterator *)RQ_MALLOC(sizeof(struct rq_tree_rb_iterator));

    it->cur_node = NULL;

    return it;
}

RQ_EXPORT void 
rq_tree_rb_iterator_free(rq_tree_rb_iterator_t i)
{
    RQ_FREE(i);
}

RQ_EXPORT void 
rq_tree_rb_begin(rq_tree_rb_t t, rq_tree_rb_iterator_t it)
{
    struct rq_tree_rb_node *n = t->head;

	if (t->head)
        it->cur_node = recurse_find_first(t->head);
    else
        it->cur_node = NULL;
}

RQ_EXPORT int 
rq_tree_rb_at_end(rq_tree_rb_iterator_t it)
{
    return it->cur_node == NULL;
}

RQ_EXPORT void
rq_tree_rb_next(rq_tree_rb_iterator_t it)
{
    struct rq_tree_rb_node *n = it->cur_node;
    if (n)
    {
        if (n->left)
            it->cur_node = recurse_find_first(n->left);
        else if (n->parent)
        {
            struct rq_tree_rb_node *r;

            it->cur_node = NULL;

            while ((r = n->parent) != NULL)
            {
                if (r->right == n)
                {
                    it->cur_node = r;
                    break;
                }
                n = r;
            }
        }
        else
            it->cur_node = NULL;
    }
}

RQ_EXPORT void *
rq_tree_rb_iterator_deref(rq_tree_rb_iterator_t it)
{
    if (it->cur_node)
        return it->cur_node->data;
    return NULL;
}
