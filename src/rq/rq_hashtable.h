/**
 * @file
 *
 * This file implements a general purpose hash table. The ideas here-in
 * are based on public domain code by Peter Moore @ UCB
 */
/*
** rq_hashtable.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2008-2009 Brett Hutley
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
#ifndef rq_hashtable_h
#define rq_hashtable_h

#include "rq_config.h"

/* -- defines ----------------------------------------------------- */

/** This is the max number of objects per bin we allow before increasing 
 * the number of bins.
 */
#define RQ_HASHTABLE_MAX_BIN_DENSITY 5

/* -- structs ----------------------------------------------------- */

typedef struct rq_hashtable_node {
    unsigned int hash;
    void *key;
    void *data;
    struct rq_hashtable_node *next;
} rq_hashtable_node;

struct rq_hashtable {
    int (*compfunc)(void *, void *);
    unsigned int (*hashfunc)(void *);

    int num_bins;
    int num_entries;
    struct rq_hashtable_node **bins;
};

/* -- prototypes -------------------------------------------------- */

struct rq_hashtable *rq_hashtable_init(int (*compfunc)(void *, void *), unsigned int (*hashfunc)(void *));

struct rq_hashtable *rq_hashtable_init_with_size(int (*compfunc)(void *, void *), unsigned int (*hashfunc)(void *), int);

void rq_hashtable_free(struct rq_hashtable *table, void (*freefunc)(void *));

void *rq_hashtable_find(struct rq_hashtable *, void *key);

void *rq_hashtable_insert(struct rq_hashtable *table, void *key, void *data);

void *rq_hashtable_delete(struct rq_hashtable *table, void *key);

void rq_hashtable_foreach(struct rq_hashtable *table, short (*func)(void *data, void *arg), void *arg);


unsigned int rq_hashtable_hash_string(const char *);


#endif
