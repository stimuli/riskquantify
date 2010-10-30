/*
** rq_bootstrap_adapter_mgr.c
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
#include "rq_bootstrap_adapter_mgr.h"
#include "rq_bootstrap_adapter_equity_curve_simple.h"
#include "rq_bootstrap_adapter_forward_curve_crossccy.h"
#include "rq_bootstrap_adapter_forward_curve_simple.h"
#include "rq_bootstrap_adapter_future_curve_simple.h"
#include "rq_bootstrap_adapter_spread_curve_simple.h"
#include "rq_bootstrap_adapter_vol_surface_simple.h"
#include "rq_bootstrap_adapter_ir_vol_surface.h"
#include "rq_bootstrap_adapter_yield_curve.h"
#include "rq_bootstrap_adapter_yield_curve_simple.h"
#include "rq_bootstrap_adapter_yield_curve_spread.h"
#include "rq_bootstrap_adapter_yield_curve_composite.h"
#include "rq_bootstrap_adapter_yield_curve_cubic_spline.h"
#include "rq_bootstrap_adapter_yield_curve_day_count.h"
#include "rq_bootstrap_adapter_cds_curve.h"
#include "rq_bootstrap_adapter_forward_curve_bbi.h"
#include "rq_enum.h"
#include "rq_tree_rb.h"
#include "rq_error.h"
#include <stdlib.h>
#include <string.h>

static void
bootstrap_adapter_free(void *p)
{
    rq_bootstrap_adapter_free((rq_bootstrap_adapter_t)p);
}

static const void *
bootstrap_adapter_get_id(const void *p)
{
    return rq_bootstrap_adapter_get_adapter_id((const rq_bootstrap_adapter_t)p);
}

static void *
bootstrap_adapter_clone(const void *p)
{
    return rq_bootstrap_adapter_clone((const rq_bootstrap_adapter_t)p);
}

RQ_EXPORT rq_bootstrap_adapter_mgr_t
rq_bootstrap_adapter_mgr_alloc()
{
    struct rq_bootstrap_adapter_mgr *m = (struct rq_bootstrap_adapter_mgr *)
        RQ_MALLOC(sizeof(struct rq_bootstrap_adapter_mgr));

    int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        m->adapters[i] = rq_tree_rb_alloc(bootstrap_adapter_free, (int (*)(const void *, const void *))strcmp);

    return m;
}

RQ_EXPORT void  
rq_bootstrap_adapter_mgr_free(rq_bootstrap_adapter_mgr_t m)
{
    int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        rq_tree_rb_free(m->adapters[i]);
    RQ_FREE(m);
}

RQ_EXPORT rq_bootstrap_adapter_mgr_t 
rq_bootstrap_adapter_mgr_clone(rq_bootstrap_adapter_mgr_t o)
{
    struct rq_bootstrap_adapter_mgr *c = (struct rq_bootstrap_adapter_mgr *)
        RQ_MALLOC(sizeof(struct rq_bootstrap_adapter_mgr));

    int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        c->adapters[i] = rq_tree_rb_clone(
            o->adapters[i],
            (const void *(*)(const void *))bootstrap_adapter_get_id,
            (void *(*)(const void *))bootstrap_adapter_clone
            );

    return c;
}

RQ_EXPORT int
rq_bootstrap_adapter_mgr_is_null(rq_bootstrap_adapter_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void
rq_bootstrap_adapter_mgr_add(
    rq_bootstrap_adapter_mgr_t m,
    rq_bootstrap_adapter_t bootstrap_adapter
    )
{
    rq_tree_rb_add(m->adapters[(int)rq_bootstrap_adapter_get_termstruct_type(bootstrap_adapter)], rq_bootstrap_adapter_get_adapter_id(bootstrap_adapter), bootstrap_adapter);
}

RQ_EXPORT rq_bootstrap_adapter_t
rq_bootstrap_adapter_mgr_find(
    rq_bootstrap_adapter_mgr_t m,
    enum rq_termstruct_type termstruct_type,
    const char *adapter_id
    )
{
    void *adapter = rq_tree_rb_find(m->adapters[termstruct_type], adapter_id);

    return adapter;
}

RQ_EXPORT void *
rq_bootstrap_adapter_mgr_build(
    rq_bootstrap_adapter_mgr_t m,
    enum rq_termstruct_type termstruct_type,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    )
{
    void *curve = rq_market_get_termstruct(
        market,
        termstruct_type,
        curve_id
        );

    if (!curve)
    {
        rq_bootstrap_adapter_t adapter = (rq_bootstrap_adapter_t)rq_tree_rb_find(m->adapters[termstruct_type], adapter_id);

        if (adapter)
        {
            short deps_built = 1;
            if (adapter->get_bootstrap_dependency_list)
            {
                struct rq_bootstrap_dependency_list deplist;
                rq_error_code status;

                rq_bootstrap_dependency_list_init(&deplist);
                status = (*adapter->get_bootstrap_dependency_list)(
                    adapter,
                    curve_id,
                    system,
                    market,
                    &deplist
                    );
                if (status == RQ_OK)
                {
                    unsigned short num_deps = rq_bootstrap_dependency_list_size(&deplist);
                    unsigned short dep_iter;
                    rq_bootstrap_config_mgr_t bootstrap_config_mgr = rq_system_get_bootstrap_config_mgr(system);

                    deps_built = 1;

                    for (dep_iter = 0; dep_iter < num_deps; dep_iter++)
                    {
                        enum rq_termstruct_type dep_termstruct_type;
                        const char *dep_curve_id;

                        deps_built = 0;

                        if (rq_bootstrap_dependency_list_get_at(
                                &deplist,
                                dep_iter,
                                &dep_termstruct_type,
                                &dep_curve_id
                                ) == RQ_OK)
                        {
                            rq_bootstrap_config_t dep_config = rq_bootstrap_config_mgr_find(bootstrap_config_mgr, dep_curve_id, dep_termstruct_type);
                            if (dep_config)
                            {
                                void *dep_curve = rq_bootstrap_adapter_mgr_build(
                                    m,
                                    dep_termstruct_type,
                                    dep_config->bootstrap_method_id,
                                    dep_curve_id,
                                    system,
                                    market
                                    );
                                if (dep_curve)
                                    deps_built = 1;
                            }
                        }

                        if (deps_built == 0)
                            break;
                    }
                }
                else
                    deps_built = 0;
            }

            if (deps_built)
            {
                curve = rq_bootstrap_adapter_build(
                    (rq_bootstrap_adapter_t)adapter,
                    curve_id,
                    system,
                    market
                    );
            }
        }
    }

    return curve;
}

RQ_EXPORT rq_yield_curve_t
rq_bootstrap_adapter_mgr_build_yield_curve(
    rq_bootstrap_adapter_mgr_t m,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    )
{
    return (rq_yield_curve_t) rq_bootstrap_adapter_mgr_build(
        m,
        RQ_TERMSTRUCT_TYPE_YIELD_CURVE,
        adapter_id,
        curve_id,
        system,
        market
        );
}

RQ_EXPORT rq_forward_curve_t
rq_bootstrap_adapter_mgr_build_forward_curve(
    rq_bootstrap_adapter_mgr_t m,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    )
{
    return (rq_forward_curve_t) rq_bootstrap_adapter_mgr_build(
        m,
        RQ_TERMSTRUCT_TYPE_FORWARD_CURVE,
        adapter_id,
        curve_id,
        system,
        market
        );
}

RQ_EXPORT rq_vol_surface_t
rq_bootstrap_adapter_mgr_build_vol_surface(
    rq_bootstrap_adapter_mgr_t m,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    )
{
    return (rq_vol_surface_t) rq_bootstrap_adapter_mgr_build(
        m,
        RQ_TERMSTRUCT_TYPE_VOL_SURFACE,
        adapter_id,
        curve_id,
        system,
        market
        );
}

RQ_EXPORT rq_external_termstruct_t
rq_bootstrap_adapter_mgr_build_external(
    rq_bootstrap_adapter_mgr_t m,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    )
{
    rq_external_termstruct_t et = (rq_external_termstruct_t)rq_bootstrap_adapter_mgr_build(
        m,
        RQ_TERMSTRUCT_TYPE_EXTERNAL,
        adapter_id,
        curve_id,
        system,
        market
        );

    return (rq_external_termstruct_t)m;
}

RQ_EXPORT rq_future_curve_t
rq_bootstrap_adapter_mgr_build_future_curve(
    rq_bootstrap_adapter_mgr_t m,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    )
{
    return (rq_future_curve_t) rq_bootstrap_adapter_mgr_build(
        m,
        RQ_TERMSTRUCT_TYPE_FUTURE_CURVE,
        adapter_id,
        curve_id,
        system,
        market
        );
}

void
rq_bootstrap_adapter_mgr_add_standard_adapters(rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr)
{
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_yield_curve_simple_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_forward_curve_simple_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_forward_curve_crossccy_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_vol_surface_simple_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_ir_vol_surface_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_equity_curve_simple_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_spread_curve_simple_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_yield_curve_spread_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_yield_curve_composite_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_cds_curve_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_yield_curve_cubic_spline_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_forward_curve_bbi_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_yield_curve_day_count_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_future_curve_simple_alloc()
        );
    rq_bootstrap_adapter_mgr_add(
        bootstrap_adapter_mgr, 
        rq_bootstrap_adapter_yield_curve_alloc()
        );
}
