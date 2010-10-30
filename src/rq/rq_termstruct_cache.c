/*
** rq_termstruct_cache.c
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
#include "rq_termstruct_cache.h"
#include "rq_array.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>

static void
spec_free(void *p)
{
    struct rq_termstruct_specification *spec = (struct rq_termstruct_specification *)p;
    if (spec->termstruct_group_id)
        RQ_FREE((char *)spec->termstruct_group_id);
    if (spec->asset_id)
        RQ_FREE((char *)spec->asset_id);
    if (spec->termstruct_id)
        RQ_FREE((char *)spec->termstruct_id);
    if (spec->key)
        RQ_FREE((char *)spec->key);
    RQ_FREE(spec);
}

static int
spec_cmp(const void *lhs, const void *rhs)
{
    struct rq_termstruct_specification_key *lkey = (struct rq_termstruct_specification_key *)lhs;
    struct rq_termstruct_specification_key *rkey = (struct rq_termstruct_specification_key *)rhs;
	int group_cmp;

	if (lkey->termstruct_type < rkey->termstruct_type) return -1;
	if (lkey->termstruct_type > rkey->termstruct_type) return 1;

	group_cmp = strcmp(lkey->termstruct_group_id, rkey->termstruct_group_id);
	if (group_cmp != 0) return group_cmp;

	return strcmp(lkey->asset_id, rkey->asset_id);
}

RQ_EXPORT rq_termstruct_cache_t
rq_termstruct_cache_alloc()
{
    struct rq_termstruct_cache *tc = (struct rq_termstruct_cache *)RQ_CALLOC(1, sizeof(struct rq_termstruct_cache));

	tc->termstruct_cache = rq_tree_rb_alloc(spec_free, spec_cmp);

    return tc;
}

RQ_EXPORT void  
rq_termstruct_cache_free(rq_termstruct_cache_t tc)
{
	rq_tree_rb_free(tc->termstruct_cache);
    RQ_FREE(tc);
}

RQ_EXPORT int
rq_termstruct_cache_is_null(rq_termstruct_cache_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT struct rq_termstruct_specification *
rq_termstruct_cache_add_requested_term_structure(
    rq_termstruct_cache_t tc,
    enum rq_termstruct_type termstruct_type,
    const char *termstruct_group_id,
    const char *asset_id,
    rq_date maturity_date
    )
{

    struct rq_termstruct_specification *spec = (struct rq_termstruct_specification *)
        RQ_MALLOC(sizeof(struct rq_termstruct_specification));

    struct rq_termstruct_specification_key *key = (struct rq_termstruct_specification_key *)
        RQ_MALLOC(sizeof(struct rq_termstruct_specification_key));

	spec->key = key;
	spec->termstruct_type = termstruct_type;
    spec->termstruct_group_id = RQ_STRDUP(termstruct_group_id);
    spec->asset_id = RQ_STRDUP(asset_id);
    spec->termstruct_id = spec->termstruct = NULL;
    spec->maturity_date = maturity_date;

	key->termstruct_type = spec->termstruct_type;
	key->termstruct_group_id = spec->termstruct_group_id;
	key->asset_id = spec->asset_id;

	rq_tree_rb_add(tc->termstruct_cache, key, spec);

	return spec;
}

RQ_EXPORT void *
rq_termstruct_cache_find_term_structure(
    rq_termstruct_cache_t tc,
    enum rq_termstruct_type termstruct_type,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
	struct rq_termstruct_specification *spec;
    struct rq_termstruct_specification_key key;

	key.termstruct_type = termstruct_type;
	key.termstruct_group_id = termstruct_group_id;
	key.asset_id = asset_id;

	spec = rq_tree_rb_find(tc->termstruct_cache, &key);
	if (spec)
		return spec->termstruct;

    return NULL;
}

RQ_EXPORT void 
rq_termstruct_cache_clear(rq_termstruct_cache_t tc)
{
	rq_tree_rb_clear(tc->termstruct_cache);
}

RQ_EXPORT void 
rq_termstruct_cache_init(
    rq_termstruct_cache_t tc,
    rq_market_requirements_t market_requirements
    )
{
    unsigned int i;

	rq_termstruct_cache_clear(tc);

    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
    {
        unsigned j;

        for (j = 0; j < rq_market_requirements_termstruct_size(market_requirements, (enum rq_termstruct_type) i); j++)
        {
            struct rq_termstruct_req *req = rq_market_requirements_termstruct_get_at(market_requirements, (enum rq_termstruct_type) i, j);

            rq_termstruct_cache_add_requested_term_structure(
                tc,
                i,
                req->termstruct_group_id,
                req->asset_id,
                req->maturity_date
                );
        }
    }
}

RQ_EXPORT rq_yield_curve_t
rq_termstruct_cache_find_yield_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_yield_curve_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_YIELD_CURVE,
            termstruct_group_id,
            asset_id
            );
}


RQ_EXPORT rq_forward_curve_t
rq_termstruct_cache_find_forward_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_forward_curve_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_FORWARD_CURVE,
            termstruct_group_id,
            asset_id
            );
}

RQ_EXPORT rq_future_curve_t
rq_termstruct_cache_find_future_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_future_curve_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_FUTURE_CURVE,
            termstruct_group_id,
            asset_id
            );
}

RQ_EXPORT rq_vol_surface_t
rq_termstruct_cache_find_vol_surface(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_vol_surface_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_VOL_SURFACE,
            termstruct_group_id,
            asset_id
            );
}

/** Get an ir vol surface from the cache by asset ID
 */
RQ_EXPORT rq_ir_vol_surface_t
rq_termstruct_cache_find_ir_vol_surface(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_ir_vol_surface_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_IR_VOL_SURFACE,
            termstruct_group_id,
            asset_id
            );
}


RQ_EXPORT rq_spread_curve_t
rq_termstruct_cache_find_spread_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_spread_curve_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_SPREAD_CURVE,
            termstruct_group_id,
            asset_id
            );
}

RQ_EXPORT rq_cds_curve_t
rq_termstruct_cache_find_cds_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_cds_curve_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_CDS_SPREAD_CURVE,
            termstruct_group_id,
            asset_id
            );
}

RQ_EXPORT rq_equity_curve_t
rq_termstruct_cache_find_equity_curve(
    rq_termstruct_cache_t termstruct_cache,
    const char *termstruct_group_id,
    const char *asset_id
    )
{
    return (rq_equity_curve_t)
        rq_termstruct_cache_find_term_structure(
            termstruct_cache,
            RQ_TERMSTRUCT_TYPE_EQUITY_CURVE,
            termstruct_group_id,
            asset_id
            );
}

RQ_EXPORT rq_termstruct_cache_iterator_t
rq_termstruct_cache_begin(rq_termstruct_cache_t tc)
{
    rq_termstruct_cache_iterator_t it = (rq_termstruct_cache_iterator_t)
        RQ_MALLOC(sizeof(struct rq_termstruct_cache_iterator));
    it->tree_rb_iterator = rq_tree_rb_iterator_alloc();

	rq_tree_rb_begin(tc->termstruct_cache, it->tree_rb_iterator);

    return it;
}

RQ_EXPORT int 
rq_termstruct_cache_at_end(rq_termstruct_cache_iterator_t it)
{
	return rq_tree_rb_at_end(it->tree_rb_iterator);
}

RQ_EXPORT void
rq_termstruct_cache_next(rq_termstruct_cache_iterator_t it)
{
	rq_tree_rb_next(it->tree_rb_iterator);
}

RQ_EXPORT rq_termstruct_specification_t
rq_termstruct_cache_iterator_deref(rq_termstruct_cache_iterator_t it)
{
	return (rq_termstruct_specification_t)rq_tree_rb_iterator_deref(it->tree_rb_iterator);
}

RQ_EXPORT void
rq_termstruct_cache_iterator_free(rq_termstruct_cache_iterator_t it)
{
	rq_tree_rb_iterator_free(it->tree_rb_iterator);
    RQ_FREE(it);
}
