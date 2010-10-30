/*
** rq_pricing_engine.h
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
#include "rq_pricing_engine.h"
#include <stdlib.h>
#include <string.h>

/* -- code -------------------------------------------------------- */
static void
rq_pricing_engine_node_add(struct rq_pricing_engine_node **n, const char *product_id, struct rq_pricing_adapter *pricing_adapter)
{
    *n = (struct rq_pricing_engine_node *)RQ_CALLOC(1, sizeof(struct rq_pricing_engine_node));
    (*n)->product_id = RQ_STRDUP(product_id);

#ifdef PA_CACHE
    if (pricing_adapter->alloc_pricing_adapter_cache)
        (*n)->pricing_adapter_cache = (*pricing_adapter->alloc_pricing_adapter_cache)();
#endif

    (*n)->pricing_adapter = pricing_adapter;
}

static void
rq_pricing_engine_node_update(struct rq_pricing_engine_node *n, struct rq_pricing_adapter *pricing_adapter)
{
    /* deallocate the old pricing adapter specific stuff */

#ifdef PA_CACHE
    if (n->pricing_adapter->free_pricing_adapter_cache)
        (*pricing_adapter->free_pricing_adapter_cache)(n->pricing_adapter_cache);
#endif

   RQ_FREE(n->pricing_adapter);

    /* assign the new pricing adapter to this node */

#ifdef PA_CACHE
    if (pricing_adapter->alloc_pricing_adapter_cache)
        n->pricing_adapter_cache = (*pricing_adapter->alloc_pricing_adapter_cache)();
#endif

    n->pricing_adapter = pricing_adapter;
}

static void
rq_pricing_engine_node_free(struct rq_pricing_engine_node *n)
{
    if (n->left)
        rq_pricing_engine_node_free(n->left);
    if (n->right)
        rq_pricing_engine_node_free(n->right);

#ifdef PA_CACHE
    if (n->pricing_adapter->free_pricing_adapter_cache)
        (*n->pricing_adapter->free_pricing_adapter_cache)(n->pricing_adapter_cache);
#endif

   RQ_FREE((char *)n->product_id);
   RQ_FREE(n->pricing_adapter);
   RQ_FREE(n);
}

RQ_EXPORT rq_pricing_engine_t 
rq_pricing_engine_alloc()
{
    struct rq_pricing_engine *e = (struct rq_pricing_engine *)RQ_CALLOC(
        1,
        sizeof(struct rq_pricing_engine)
        );
    return e;
}

RQ_EXPORT void
rq_pricing_engine_free(rq_pricing_engine_t e)
{
    if (e->head)
        rq_pricing_engine_node_free(e->head);
    RQ_FREE(e);
}

RQ_EXPORT void
rq_pricing_engine_add_pricing_adapter(rq_pricing_engine_t e, const char *product_id, struct rq_pricing_adapter *pricing_adapter)
{
    if (e->head)
    {
        struct rq_pricing_engine_node *n = e->head;
        while (n)
        {
            int cmp = strcmp(n->product_id, product_id);
            if (cmp > 0)
            {
                if (n->right)
                    n = n->right;
                else
                {
                    rq_pricing_engine_node_add(&n->right, product_id, pricing_adapter);
                    break;
                }
            }
            else if (cmp < 0)
            {
                if (n->left)
                    n = n->left;
                else
                {
                    rq_pricing_engine_node_add(&n->left, product_id, pricing_adapter);
                    break;
                }
            }
            else /* cmp == 0 */
            {
                rq_pricing_engine_node_update(n, pricing_adapter);
                break;
            }
        }
    }
    else
    {
        rq_pricing_engine_node_add(&e->head, product_id, pricing_adapter);
    }
}

RQ_EXPORT int 
rq_pricing_engine_get_pricing_results(rq_pricing_engine_t e, const char *product_id, struct rq_pricing_request *pricing_request, struct rq_pricing_result *pricing_result)
{
    struct rq_pricing_engine_node *n = e->head;
    while (n)
    {
        int cmp = strcmp(n->product_id, product_id);
        if (!cmp)
            return (*n->pricing_adapter->get_pricing_results)(pricing_request, pricing_result);
        else if (cmp > 0)
            n = n->right;
        else if (cmp < 0)
            n = n->left;
    }

    return 0;
}

RQ_EXPORT struct rq_pricing_adapter *
rq_pricing_engine_get_pricing_adapter(rq_pricing_engine_t e, const char *product_id)
{
    struct rq_pricing_engine_node *n = e->head;
    while (n)
    {
        int cmp = strcmp(n->product_id, product_id);
        if (!cmp)
            return n->pricing_adapter;
        else if (cmp > 0)
            n = n->right;
        else if (cmp < 0)
            n = n->left;
    }

    return NULL;
}

RQ_EXPORT int
rq_pricing_engine_is_null(rq_pricing_engine_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void *
rq_pricing_engine_get_pricing_adapter_cache(rq_pricing_engine_t e, const char *product_id)
{
    struct rq_pricing_engine_node *n = e->head;
    while (n)
    {
        int cmp = strcmp(n->product_id, product_id);
        if (!cmp)
            return n->pricing_adapter_cache;
        else if (cmp > 0)
            n = n->right;
        else if (cmp < 0)
            n = n->left;
    }

    return NULL;
}
