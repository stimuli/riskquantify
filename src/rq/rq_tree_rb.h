/**
 * \file rq_tree_rb.h
 * \author Brett Hutley
 *
 * \brief The rq_tree_rb files contain functions for implementing
 * red-black trees.
 * These trees have the following properties:
 *
 * Every node has a value.
 * The value of any node is greater than the value of its left child and less than the value of its right child.
 * Every node is colored either red or black.
 * Every red node that is not a leaf has only black children.
 * Every path from the root to a leaf contains the same number of black nodes.
 * The root node is black.
 */
/*
** rq_tree_rb.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA 
*/
#ifndef rq_tree_rb_h
#define rq_tree_rb_h

/* -- includes ---------------------------------------------------- */
#include <stdio.h>
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_iterator.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
struct rq_tree_rb_node {
    struct rq_tree_rb_node *left;
    struct rq_tree_rb_node *right;
    struct rq_tree_rb_node *parent;
    short is_red;
    void *data;
    const void *key;
};

typedef struct rq_tree_rb {
    struct rq_tree_rb_node *head;
    void (*free_func)(void *);
    int (*cmp_func)(const void *, const void *);
    unsigned long size;
} *rq_tree_rb_t;

typedef struct rq_tree_rb_iterator {
    struct rq_tree_rb_node *cur_node;
} *rq_tree_rb_iterator_t;

/* -- prototypes -------------------------------------------------- */

/** Allocate a new red-black tree.
 *
 * Allocate a new red-black tree. Pass in the free callback function
 * and the key comparison callback function as parameters.
 */
RQ_EXPORT rq_tree_rb_t rq_tree_rb_alloc(void (*free_func)(void *), int (*cmp_func)(const void *, const void *));

/** Clone a red-black tree.
 *
 * Clone the red-black tree. The data is copied using the copy function
 * callback parameter. The key function parameter is used to get the
 * key out of the cloned data.
 */
RQ_EXPORT rq_tree_rb_t rq_tree_rb_clone(rq_tree_rb_t tree, const void *(*key_func)(const void *), void *(*cpy_func)(const void *));

/** Copy the red-black tree into another tree.
 *
 * Copy the red-black tree into another tree. The data is copied using
 * the copy function callback parameter. The key function parameter is
 * used to get the key out of the cloned data.  
*/
RQ_EXPORT void rq_tree_rb_copy(rq_tree_rb_t tree_dst, rq_tree_rb_t tree_src, const void *(*key_func)(const void *), void *(*cpy_func)(const void *));

/** Free the red-black tree
 */
RQ_EXPORT void rq_tree_rb_free(rq_tree_rb_t tree);

/** Clear the tree, freeing all the nodes.
 */
RQ_EXPORT void rq_tree_rb_clear(rq_tree_rb_t tree);

/** Test whether the rq_tree_rb is NULL */
RQ_EXPORT int rq_tree_rb_is_null(rq_tree_rb_t obj);

/** Find the data in the tree associated with a certain key.
 */
RQ_EXPORT void *rq_tree_rb_find(rq_tree_rb_t tree, const void *key);


/** Locate the data in the tree associated with a certain key, using
 * the passed in comparison callback.
 */
RQ_EXPORT void *rq_tree_rb_locate(rq_tree_rb_t tree, int (*cmp)(const void *, const void *), const void *key);

/** Add the data to the tree, using the key parameter as the key.
 */
RQ_EXPORT void rq_tree_rb_add(rq_tree_rb_t tree, const void *key, void *data);

/** Remove the data specified by the following key from the tree.
 */
RQ_EXPORT void rq_tree_rb_remove(rq_tree_rb_t t, const void *key);

/** Test if the red-black tree is consistant.
 *
 * At the moment this test only checks the minimum and maximum depths of
 * the leaf nodes to make sure the tree is balanced.
 */
RQ_EXPORT short rq_tree_rb_is_consistant(rq_tree_rb_t t);

/** Get the size of the red-black tree (ie the number of nodes).
 */
RQ_EXPORT unsigned long rq_tree_rb_size(rq_tree_rb_t t);

/** Write all the nodes in the tree to the specified file pointer.
 *
 * print_func should return a NULL-terminated string representing the
 * value of the key passed as an argument.
 */
RQ_EXPORT void rq_tree_rb_dump(rq_tree_rb_t tree, FILE *f, const char *print_func(const void *));

/** Do an inorder traversal of the tree
 */
RQ_EXPORT void rq_tree_rb_traverse_inorder(rq_tree_rb_t tree, void *user_data, void (*func)(void *user_data, void *node_data));

/*
RQ_EXPORT rq_iterator_t rq_tree_rb_begin(rq_tree_rb_t tree);
RQ_EXPORT rq_iterator_t rq_tree_rb_rbegin(rq_tree_rb_t tree);
*/

/** Get a generic iterator for the tree.
 */
RQ_EXPORT rq_iterator_t rq_tree_rb_get_iterator(rq_tree_rb_t tree);

/** Allocate an iterator to walk the tree.
 */
RQ_EXPORT rq_tree_rb_iterator_t rq_tree_rb_iterator_alloc();

/**
 * Free an allocated tree iterator.
 */
RQ_EXPORT void rq_tree_rb_iterator_free(rq_tree_rb_iterator_t it);

/**
 * Start an in-order walk of the tree.
 */
RQ_EXPORT void rq_tree_rb_begin(rq_tree_rb_t tree, rq_tree_rb_iterator_t it);

/**
 * Test to see if we have passed the last node in the tree.
 */
RQ_EXPORT int rq_tree_rb_at_end(rq_tree_rb_iterator_t it);

/**
 * Move to the next node in the tree.
 */
RQ_EXPORT void rq_tree_rb_next(rq_tree_rb_iterator_t i);

/**
 * Get the data associated with the current node in the
 * tree.
 */
RQ_EXPORT void *rq_tree_rb_iterator_deref(rq_tree_rb_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
