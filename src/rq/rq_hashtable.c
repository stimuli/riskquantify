/*
** rq_hashtable.c
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "rq_hashtable.h"


static int
rq_hashtable_calc_size(int size)
{
    /* Table of prime numbers 2^n+a, 2<=n<=30. */
    static long primes[] = {
        8 + 3,
        16 + 3,
        32 + 5,
        64 + 3,
        128 + 3,
        256 + 27,
        512 + 9,
        1024 + 9,
        2048 + 5,
        4096 + 3,
        8192 + 27,
        16384 + 43,
        32768 + 3,
        65536 + 45,
        131072 + 29,
        262144 + 3,
        524288 + 21,
        1048576 + 7,
        2097152 + 17,
        4194304 + 15,
        8388608 + 9,
        16777216 + 43,
        33554432 + 35,
        67108864 + 15,
        134217728 + 29,
        268435456 + 3,
        536870912 + 11,
        1073741824 + 85,
        0
    };

    int i;
    int newsize;

    for (i = 0, newsize = 8;
         i < sizeof(primes)/sizeof(primes[0]);
         i++, newsize <<= 1)
    {
        if (newsize > size) 
            return primes[i];
    }

    assert(0);

    return 0;
}

static void
rq_hashtable_rehash(struct rq_hashtable *table)
{
    int i;
    int old_num_bins = table->num_bins;
    int new_num_bins = rq_hashtable_calc_size(old_num_bins+1);
    struct rq_hashtable_node **new_bins = (struct rq_hashtable_node**)RQ_CALLOC(new_num_bins, sizeof(struct rq_hashtable_node *));

    for (i = 0; i < old_num_bins; i++) 
    {
        struct rq_hashtable_node *ptr;
        struct rq_hashtable_node *next_ptr;
        for (ptr = table->bins[i]; ptr != 0; ptr = next_ptr) 
        {
            unsigned int hash_off = ptr->hash % new_num_bins;

            next_ptr = ptr->next;
            ptr->next = NULL;

            if (!new_bins[hash_off] || new_bins[hash_off]->hash > ptr->hash)
            {
                ptr->next = new_bins[hash_off];
                new_bins[hash_off] = ptr;
            }
            else
            {
                struct rq_hashtable_node *link_ptr;

                /* We can guarantee that the first node hash is not greater 
                   than node we want to insert's hash. Therefore we can 
                   work with the next_ptr here.
                */
                for (link_ptr = new_bins[hash_off]; link_ptr->next != NULL; link_ptr = link_ptr->next)
                {
                    if (link_ptr->next->hash > ptr->hash)
                    {
                        ptr->next = link_ptr->next;
                        link_ptr->next = ptr;
                        break;
                    }
                }
                if (!link_ptr->next)
                    link_ptr->next = ptr;
            }
        }
    }

    RQ_FREE(table->bins);
    table->num_bins = new_num_bins;
    table->bins = new_bins;
}

struct rq_hashtable *
rq_hashtable_init_with_size(int (*compfunc)(void *, void *), unsigned int (*hashfunc)(void *), int size)
{
    struct rq_hashtable *tbl = (struct rq_hashtable *)RQ_MALLOC(sizeof(struct rq_hashtable));

    int new_size = rq_hashtable_calc_size(size);	/* round up to prime number */

    tbl->compfunc = compfunc;
    tbl->hashfunc = hashfunc;
    tbl->num_entries = 0;
    tbl->num_bins = new_size;
    tbl->bins = (struct rq_hashtable_node **)RQ_CALLOC(new_size, sizeof(struct rq_hashtable_node *));

    return tbl;
}

struct rq_hashtable *
rq_hashtable_init(int (*compfunc)(void *, void *), unsigned int (*hashfunc)(void *))
{
    return rq_hashtable_init_with_size(compfunc, hashfunc, 0);
}

void
rq_hashtable_free(struct rq_hashtable *table, void (*freefunc)(void *))
{
    int i;

    for (i = 0; i < table->num_bins; i++) 
    {
        struct rq_hashtable_node *ptr;
        for (ptr = table->bins[i]; ptr != 0; ) 
        {
            struct rq_hashtable_node *next = ptr->next;
            if (freefunc)
                (*freefunc)(ptr->data);
            RQ_FREE(ptr);
            ptr = next;
        }
    }

    RQ_FREE(table->bins);
    RQ_FREE(table);
}

void *
rq_hashtable_find(struct rq_hashtable *table, void *key)
{
    unsigned int hash_val = (*(table)->hashfunc)(key);
    unsigned int bin_pos = hash_val % table->num_bins;
    struct rq_hashtable_node *ptr = table->bins[bin_pos];

    for (ptr = table->bins[bin_pos]; ptr; ptr = ptr->next)
        if (ptr->hash == hash_val && (key == ptr->key || (*table->compfunc)(key, ptr->key) == 0))
            return ptr->data;

    return 0;
}

void *
rq_hashtable_insert(struct rq_hashtable *table, void *key, void *data)
{
    unsigned int hash_val = (*table->hashfunc)(key);
    unsigned int bin_pos = hash_val % table->num_bins;
    struct rq_hashtable_node *ptr;
    struct rq_hashtable_node *last_ptr = NULL;

    if (table->num_entries / table->num_bins > RQ_HASHTABLE_MAX_BIN_DENSITY) 
    {
        rq_hashtable_rehash(table);
        bin_pos = hash_val % table->num_bins;
    }

    for (ptr = table->bins[bin_pos]; ptr; ptr = ptr->next)
    {
        if (ptr->hash == hash_val)
        {
            if (ptr->key == key || (*table->compfunc)(key, ptr->key) == 0)
            {
                if (ptr->data != data)
                {
                    /* same key, but different data */
                    /* can either free our existing record and replace,
                       or add a new record after.
                    */
                    void *old_data = ptr->data;
                    ptr->data = data;

                    return old_data;
                }
                else
                {
                    /* else same record */
                    return NULL;
                }
            }
            else
            {
                /* same hash vals but different keys */
                /* just keep going. Either we find a record that matches
                   or we insert before the node.
                */
            }
        }
        else if (ptr->hash > hash_val)
        {
            /* insert before ptr */
            if (last_ptr == NULL)
            {
                table->bins[bin_pos] = (struct rq_hashtable_node *)RQ_MALLOC(sizeof(struct rq_hashtable_node));
                table->bins[bin_pos]->hash = hash_val;
                table->bins[bin_pos]->key = key;
                table->bins[bin_pos]->data = data;
                table->bins[bin_pos]->next = ptr;
                table->num_entries++;

                return NULL;
            }
            else
            {
                last_ptr->next = (struct rq_hashtable_node *)RQ_MALLOC(sizeof(struct rq_hashtable_node));
                last_ptr->next->hash = hash_val;
                last_ptr->next->key = key;
                last_ptr->next->data = data;
                last_ptr->next->next = ptr;
                table->num_entries++;

                return NULL;
            }
        }

        if (!ptr->next)
        {
            ptr->next = (struct rq_hashtable_node *)RQ_MALLOC(sizeof(struct rq_hashtable_node));
            ptr->next->hash = hash_val;
            ptr->next->key = key;
            ptr->next->data = data;
            ptr->next->next = NULL;
            table->num_entries++;
            
            return NULL;
        }

        last_ptr = ptr;
    }

    table->bins[bin_pos] = (struct rq_hashtable_node *)RQ_MALLOC(sizeof(struct rq_hashtable_node));
    table->bins[bin_pos]->hash = hash_val;
    table->bins[bin_pos]->key = key;
    table->bins[bin_pos]->data = data;
    table->bins[bin_pos]->next = NULL;
    table->num_entries++;

    return NULL;
}

void *
rq_hashtable_delete(struct rq_hashtable *table, void *key)
{
    unsigned int hash_val = (unsigned int)(*table->hashfunc)(key);
    struct rq_hashtable_node *ptr = table->bins[hash_val];

    if (ptr)
    {
        struct rq_hashtable_node *prev = NULL;
        for ( ; ptr != 0; prev = ptr, ptr = ptr->next) 
        {
            if (ptr->key == key || (*table->compfunc)(ptr->key, key) == 0)
            {
                void *data = ptr->data;
                if (ptr == table->bins[hash_val])
                {
                    /* This is the first node in the list */
                    table->bins[hash_val] = ptr->next;
                }
                else
                    prev->next = ptr->next;
                table->num_entries--;
                /* (*table->freefunc)(ptr->data); */
                RQ_FREE(ptr);
                return data;
            }
        }
    }

    return NULL;
}

void
rq_hashtable_foreach(struct rq_hashtable *table, short (*func)(void *arg, void *data), void *arg)
{
    int i;

    for (i = 0; i < table->num_bins; i++) 
    {
        struct rq_hashtable_node *ptr;

        for (ptr = table->bins[i]; ptr != 0; ptr = ptr->next) 
        {
            if ((*func)(arg, ptr->data) == 0)
                return;
        }
    }
}

/**< This string has function has been taken from the RUBY source code. */
unsigned int
rq_hashtable_hash_string(const char *string)
{
    int c;

#ifdef HASH_ELFHASH
    unsigned int h = 0, g;

    while ((c = *string++) != '\0') 
    {
        h = ( h << 4 ) + c;
        if ( g = h & 0xF0000000 )
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
#elif defined(HASH_PERL)
    register int val = 0;

    while ((c = *string++) != '\0') 
    {
        val += c;
        val += (val << 10);
        val ^= (val >> 6);
    }
    val += (val << 3);
    val ^= (val >> 11);

    return val + (val << 15);
#else
    int val = 0;

    while ((c = *string++) != '\0') 
        val = val*997 + c;

    return val + (val>>5);
#endif
}

#if 0
short 
foreach_func(void *arg, void *data)
{
    printf("%lu\n", (unsigned long)data);
    return 1;
}

int
main(int argc, char **argv)
{
    struct rq_hashtable *ht = rq_hashtable_init(
        (int (*)(void *, void *))numcmp, 
        (unsigned int (*)(void *))numhash
        );

    int i;

    for (i = 0; i < 1000; i++)
    {
        unsigned long num = rand() % 200;
        void *r = rq_hashtable_insert(ht, (void *)num, (void *)num);
        if (r)
        {
            /* printf("Inserted %lu and found previous %lu\n", num, num); */
            if (num != (unsigned long)r)
                printf("Inserted %lu BUT found previous %lu\n", num, (unsigned long)r);
        }
        else
        {
            /* printf("Inserted %lu\n", num, num); */
        }

        r = rq_hashtable_find(ht, (void *)num);

        if (num != (unsigned long)r)
        {
            printf("Looking for %lu and found %lu", num, (unsigned long)r);
            printf(" INVALID!!\n");
        }
    }

    rq_hashtable_foreach(
        ht, 
        foreach_func, 
        NULL
        );

    rq_hashtable_free(ht, NULL);

    return 0;
}
#endif
