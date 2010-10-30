/*
** rq_named_variant_mgr.c
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
#include "rq_named_variant_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>

static void
named_variant_free(void *p)
{
    struct rq_named_variant *nv = (struct rq_named_variant *)p;
    RQ_FREE((char *)nv->name);
    rq_variant_free(nv->value);
    RQ_FREE(nv);
}

RQ_EXPORT rq_named_variant_mgr_t 
rq_named_variant_mgr_alloc()
{
    rq_named_variant_mgr_t nvm = (rq_named_variant_mgr_t)RQ_MALLOC(sizeof(struct rq_named_variant_mgr));
    nvm->named_variants = rq_tree_rb_alloc(named_variant_free, (int (*)(const void *, const void *))strcmp);

    return nvm;
}

RQ_EXPORT void 
rq_named_variant_mgr_free(rq_named_variant_mgr_t named_variant_mgr)
{
    rq_tree_rb_free(named_variant_mgr->named_variants);
    RQ_FREE(named_variant_mgr);
}

RQ_EXPORT void 
rq_named_variant_mgr_add_string(rq_named_variant_mgr_t named_variant_mgr, const char *name, const char *value)
{
    struct rq_named_variant *nv = (struct rq_named_variant *)RQ_MALLOC(sizeof(struct rq_named_variant));
    nv->name = RQ_STRDUP(name);
    nv->value = rq_variant_alloc_string(value);
    rq_tree_rb_add(named_variant_mgr->named_variants, nv->name, nv);
}

RQ_EXPORT void 
rq_named_variant_mgr_add_double(rq_named_variant_mgr_t named_variant_mgr, const char *name, double d)
{
    struct rq_named_variant *nv = (struct rq_named_variant *)RQ_MALLOC(sizeof(struct rq_named_variant));
    nv->name = RQ_STRDUP(name);
    nv->value = rq_variant_alloc_double(d);
    rq_tree_rb_add(named_variant_mgr->named_variants, nv->name, nv);
}

RQ_EXPORT void 
rq_named_variant_mgr_add_integer(rq_named_variant_mgr_t named_variant_mgr, const char *name, int i)
{
    struct rq_named_variant *nv = (struct rq_named_variant *)RQ_MALLOC(sizeof(struct rq_named_variant));
    nv->name = RQ_STRDUP(name);
    nv->value = rq_variant_alloc_integer(i);
    rq_tree_rb_add(named_variant_mgr->named_variants, nv->name, nv);
}

RQ_EXPORT void 
rq_named_variant_mgr_add_date(rq_named_variant_mgr_t named_variant_mgr, const char *name, rq_date dt)
{
    struct rq_named_variant *nv = (struct rq_named_variant *)RQ_MALLOC(sizeof(struct rq_named_variant));
    nv->name = RQ_STRDUP(name);
    nv->value = rq_variant_alloc_date(dt);
    rq_tree_rb_add(named_variant_mgr->named_variants, nv->name, nv);
}


RQ_EXPORT rq_variant_t 
rq_named_variant_mgr_get(rq_named_variant_mgr_t named_variant_mgr, const char *name)
{
	if (named_variant_mgr)
    {
		struct rq_named_variant *nv = 
		    (struct rq_named_variant *)rq_tree_rb_find(named_variant_mgr->named_variants, name);
		if (nv)
			return nv->value;
    }
	return NULL;
}

RQ_EXPORT int
rq_named_variant_mgr_is_null(rq_named_variant_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_named_variant_mgr_clear(rq_named_variant_mgr_t named_variant_mgr)
{
    rq_tree_rb_clear(named_variant_mgr->named_variants);
}

RQ_EXPORT rq_named_variant_iterator_t 
rq_named_variant_iterator_alloc()
{
    rq_named_variant_iterator_t nvi = (rq_named_variant_iterator_t)RQ_MALLOC(
        sizeof(struct rq_named_variant_iterator)
        );
    nvi->named_variant_it = rq_tree_rb_iterator_alloc();

    return nvi;
}

RQ_EXPORT void 
rq_named_variant_iterator_free(rq_named_variant_iterator_t it)
{
    rq_tree_rb_iterator_free(it->named_variant_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_named_variant_begin(rq_named_variant_mgr_t named_variant_mgr, rq_named_variant_iterator_t it)
{
    rq_tree_rb_begin(named_variant_mgr->named_variants, it->named_variant_it);
}

RQ_EXPORT int 
rq_named_variant_at_end(rq_named_variant_iterator_t it)
{
    return rq_tree_rb_at_end(it->named_variant_it);
}

RQ_EXPORT void 
rq_named_variant_next(rq_named_variant_iterator_t it)
{
    rq_tree_rb_next(it->named_variant_it);
}

RQ_EXPORT const char *
rq_named_variant_iterator_get_name(rq_named_variant_iterator_t it)
{
    struct rq_named_variant *nv = (struct rq_named_variant *)
        rq_tree_rb_iterator_deref(it->named_variant_it);
    if (nv)
        return nv->name;
    return NULL;
}

RQ_EXPORT rq_variant_t
rq_named_variant_iterator_get_value(rq_named_variant_iterator_t it)
{
    struct rq_named_variant *nv = (struct rq_named_variant *)
        rq_tree_rb_iterator_deref(it->named_variant_it);
    if (nv)
        return nv->value;
    return NULL;
}
