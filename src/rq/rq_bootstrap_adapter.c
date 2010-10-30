/*
** rq_bootstrap_adapter.c
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
#include "rq_bootstrap_adapter.h"
#include <stdlib.h>
#include <string.h>


rq_bootstrap_adapter_t
_rq_bootstrap_adapter_alloc(
    const char *adapter_id,
    enum rq_termstruct_type termstruct_type,
    void *(*bootstrap_func)(rq_bootstrap_adapter_t, const char *, const rq_system_t, rq_market_t)
    )
{
    struct rq_bootstrap_adapter *a = (struct rq_bootstrap_adapter *)
        RQ_CALLOC(1, sizeof(struct rq_bootstrap_adapter));
    a->adapter_id = RQ_STRDUP(adapter_id);
    a->termstruct_type = termstruct_type;
    a->bootstrap_func = bootstrap_func;

    return a;
}

void  
rq_bootstrap_adapter_free(rq_bootstrap_adapter_t a)
{
    if (a->adapter_id)
        RQ_FREE((char *)a->adapter_id);
    RQ_FREE(a);
}

rq_bootstrap_adapter_t 
rq_bootstrap_adapter_clone(rq_bootstrap_adapter_t a)
{
    struct rq_bootstrap_adapter *c = (struct rq_bootstrap_adapter *)
        RQ_MALLOC(sizeof(struct rq_bootstrap_adapter));
    c->adapter_id = RQ_STRDUP(a->adapter_id);
    c->termstruct_type = a->termstruct_type;
    c->bootstrap_func = a->bootstrap_func;

    return c;
}

int
rq_bootstrap_adapter_is_null(rq_bootstrap_adapter_t obj)
{
    return obj == NULL;
}

void *
rq_bootstrap_adapter_build(
    rq_bootstrap_adapter_t a,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    )
{
    void *term_struct;
    term_struct = (*a->bootstrap_func)(a, curve_id, system, market);
    if (!RQ_IS_NULL(term_struct) && rq_bootstrap_adapter_get_termstruct_type(a) == RQ_TERMSTRUCT_TYPE_EXTERNAL)
    {
        /* external adapter cannot add itself to the manager, so needs
         * to be added here */

        rq_external_termstruct_mgr_t et_mgr = rq_market_get_external_termstruct_mgr(market);
        rq_external_termstruct_mgr_add(et_mgr, term_struct);
    }
    if (!RQ_IS_NULL(term_struct) && rq_bootstrap_adapter_get_termstruct_type(a) == RQ_TERMSTRUCT_TYPE_YIELD_CURVE)
    {
        /* Now the yield curve has been built the cache can be turned on. The yield curve must not be altered after this !! */
        rq_yield_curve_cache_enable((rq_yield_curve_t) term_struct);
    }
    return term_struct;
}

void 
rq_bootstrap_adaptor_set_debug_filename(rq_bootstrap_adapter_t a, const char *filename)
{
	a->debug_filename = filename;
}

const char *
rq_bootstrap_adaptor_get_debug_filename(const rq_bootstrap_adapter_t a)
{
	return a->debug_filename;
}

const char *
rq_bootstrap_adapter_get_adapter_id(rq_bootstrap_adapter_t a)
{
    return a->adapter_id;
}

enum rq_termstruct_type
rq_bootstrap_adapter_get_termstruct_type(rq_bootstrap_adapter_t a)
{
    return a->termstruct_type;
}
