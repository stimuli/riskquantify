/*
** rq_bootstrap_config_mgr.c
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
#include "rq_bootstrap_config_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


static void
rq_bootstrap_config_mgr_free_func(void *n)
{
    rq_bootstrap_config_free((rq_bootstrap_config_t)n);
}

RQ_EXPORT rq_bootstrap_config_mgr_t
rq_bootstrap_config_mgr_alloc()
{
    /* use RQ_CALLOC to zero the memory */
    struct rq_bootstrap_config_mgr *m = 
        (struct rq_bootstrap_config_mgr *)
        RQ_MALLOC(sizeof(struct rq_bootstrap_config_mgr));
    int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        m->termstruct_configs[i] = rq_tree_rb_alloc(rq_bootstrap_config_mgr_free_func, (int (*)(const void *, const void *))strcmp);

    return m;
}

RQ_EXPORT void 
rq_bootstrap_config_mgr_free(rq_bootstrap_config_mgr_t m)
{
    int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        rq_tree_rb_free(m->termstruct_configs[i]);

   RQ_FREE(m);
}

RQ_EXPORT void 
rq_bootstrap_config_mgr_clear(rq_bootstrap_config_mgr_t m)
{
    int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        rq_tree_rb_clear(m->termstruct_configs[i]);
}

RQ_EXPORT void 
rq_bootstrap_config_mgr_add(rq_bootstrap_config_mgr_t m, rq_bootstrap_config_t c)
{
    rq_tree_rb_add(m->termstruct_configs[rq_bootstrap_config_get_termstruct_type(c)], rq_bootstrap_config_get_curve_id(c), c);
}

RQ_EXPORT rq_bootstrap_config_t
rq_bootstrap_config_mgr_find(rq_bootstrap_config_mgr_t m, const char *curve_id, enum rq_termstruct_type termstruct_type)
{
    return (rq_bootstrap_config_t) rq_tree_rb_find(m->termstruct_configs[termstruct_type], curve_id);
}

RQ_EXPORT int
rq_bootstrap_config_mgr_is_null(rq_bootstrap_config_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_string_list_t 
rq_bootstrap_config_mgr_get_curve_id_list(rq_bootstrap_config_mgr_t m, enum rq_termstruct_type termstruct_type)
{
    rq_string_list_t string_list = rq_string_list_alloc();
    rq_tree_rb_iterator_t tree_it = rq_tree_rb_iterator_alloc();

    rq_tree_rb_begin(m->termstruct_configs[termstruct_type], tree_it);
    while (!rq_tree_rb_at_end(tree_it))
    {
        rq_bootstrap_config_t c = (rq_bootstrap_config_t)rq_tree_rb_iterator_deref(tree_it);

        rq_string_list_add(string_list, rq_bootstrap_config_get_curve_id(c));

        rq_tree_rb_next(tree_it);
    }
    rq_tree_rb_iterator_free(tree_it);

    return string_list;
}

RQ_EXPORT rq_bootstrap_config_mgr_iterator_t
rq_bootstrap_config_mgr_begin(rq_bootstrap_config_mgr_t m, enum rq_termstruct_type termstruct_type)
{
    rq_bootstrap_config_mgr_iterator_t it = (rq_bootstrap_config_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_bootstrap_config_mgr_iterator));
    it->bootstrap_config_it = rq_tree_rb_iterator_alloc();

    rq_tree_rb_begin(m->termstruct_configs[termstruct_type], it->bootstrap_config_it);

    return it;
}

RQ_EXPORT int 
rq_bootstrap_config_mgr_at_end(rq_bootstrap_config_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->bootstrap_config_it);
}

RQ_EXPORT void
rq_bootstrap_config_mgr_next(rq_bootstrap_config_mgr_iterator_t it)
{
    rq_tree_rb_next(it->bootstrap_config_it);
}

RQ_EXPORT rq_bootstrap_config_t
rq_bootstrap_config_mgr_iterator_deref(rq_bootstrap_config_mgr_iterator_t it)
{
    return (rq_bootstrap_config_t)rq_tree_rb_iterator_deref(it->bootstrap_config_it);
}

RQ_EXPORT void
rq_bootstrap_config_mgr_iterator_free(rq_bootstrap_config_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->bootstrap_config_it);
    RQ_FREE(it);
}
