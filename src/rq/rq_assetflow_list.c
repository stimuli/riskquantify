/*
** rq_assetflow_list.c
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
#include "rq_assetflow_list.h"
#include "rq_assetflow.h"
#include "rq_linked_list.h"
#include <stdlib.h>


/* -- code -------------------------------------------------------- */
static void
free_node(void *data)
{
    rq_assetflow_free((rq_assetflow_t)data);
}

RQ_EXPORT rq_assetflow_list_t 
rq_assetflow_list_alloc()
{
    struct rq_assetflow_list *assetflow_list = (struct rq_assetflow_list *)RQ_CALLOC(
        1,
        sizeof(struct rq_assetflow_list)
        );
    assetflow_list->list = rq_linked_list_alloc(free_node);
    return assetflow_list;
}

RQ_EXPORT void 
rq_assetflow_list_free(rq_assetflow_list_t assetflow_list)
{
    rq_linked_list_free(assetflow_list->list);
    RQ_FREE(assetflow_list);
}

RQ_EXPORT rq_assetflow_list_iterator_t 
rq_assetflow_list_begin(rq_assetflow_list_t l)
{
    rq_assetflow_list_iterator_t r = (rq_assetflow_list_iterator_t)RQ_MALLOC(
        sizeof(struct rq_assetflow_list_iterator)
        );
	r->list_it = rq_linked_list_begin(l->list);
    return r;
}

RQ_EXPORT short 
rq_assetflow_list_iterator_at_end(rq_assetflow_list_iterator_t iter)
{
    return rq_linked_list_iterator_at_end(iter->list_it);
}

RQ_EXPORT rq_assetflow_list_iterator_t 
rq_assetflow_list_iterator_incr(rq_assetflow_list_iterator_t iter)
{
    iter->list_it = rq_linked_list_iterator_incr(iter->list_it);
    return iter;
}

RQ_EXPORT rq_assetflow_t 
rq_assetflow_list_iterator_get(rq_assetflow_list_iterator_t iter)
{
    return (rq_assetflow_t)rq_linked_list_iterator_get_data(iter->list_it);
}

RQ_EXPORT void
rq_assetflow_list_append(rq_assetflow_list_t l, rq_assetflow_t aflow)
{
    rq_linked_list_append(l->list, aflow);
}

RQ_EXPORT int
rq_assetflow_list_is_null(rq_assetflow_list_t obj)
{
    return obj == NULL;
}

RQ_EXPORT unsigned int
rq_assetflow_list_size(rq_assetflow_list_t l)
{
    return rq_linked_list_size(l->list);
}
