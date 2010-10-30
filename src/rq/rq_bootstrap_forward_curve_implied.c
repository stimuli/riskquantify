/*
** rq_bootstrap_forward_curve_implied.c
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
#include "rq_bootstrap_forward_curve_implied.h"
#include <stdlib.h>
#include <string.h>
#include "rq_tree_rb.h"
#include "rq_asset_ccypair.h"

struct _dfs {
    rq_date date;
    double df1;
    double df2;
};

static void
free_func(void *p)
{
    struct _dfs *dfs = (struct _dfs *)p;
   RQ_FREE(dfs);
}

static int
cmp_func(const void *p1, const void *p2)
{
    rq_date *date1 = (rq_date *)p1;
    rq_date *date2 = (rq_date *)p2;

    return *date1 - *date2;
}

RQ_EXPORT rq_forward_curve_t
rq_bootstrap_forward_curve_implied(
    rq_date date, 
    const rq_yield_curve_mgr_t yield_curve_mgr, 
    const rq_rate_mgr_t rate_mgr,
    const rq_bootstrap_config_t config, 
    const rq_asset_mgr_t asset_mgr
    )
{
    rq_yield_curve_t yc1 = rq_yield_curve_mgr_get(yield_curve_mgr, rq_bootstrap_config_get_curve_id1(config));
    rq_yield_curve_t yc2 = rq_yield_curve_mgr_get(yield_curve_mgr, rq_bootstrap_config_get_curve_id2(config));
    rq_asset_t asset = rq_asset_mgr_get(asset_mgr, rq_bootstrap_config_get_asset_id(config));

    if (yc1 && yc2 && asset)
    {
        rq_forward_curve_t forward_curve = rq_forward_curve_build(rq_bootstrap_config_get_curve_id(config), rq_asset_get_asset_id(asset));
        rq_tree_rb_t tree = rq_tree_rb_alloc(free_func, cmp_func);
        unsigned int i;
        rq_tree_rb_iterator_t tree_it;
        int one_per_two;
        double spot_rate = 0.0;
        double forward_rate;

        if (rq_bootstrap_config_get_num_rate_class_ids(config) > 0)
        {
            rq_rate_t r = rq_rate_mgr_find(rate_mgr, rq_bootstrap_config_get_rate_class_id_at(config, 0));
            if (r)
                spot_rate = rq_rate_get_value(r);
        }
        for (i = 0; i < rq_yield_curve_size(yc1); i++)
        {
            const struct rq_yield_curve_elem *yce = rq_yield_curve_element_at(
                yc1, 
                i
                );
            double df1 = rq_yield_curve_elem_get_discount_factor(yce);
            rq_date date = rq_yield_curve_elem_get_date(yce);
            double df2 = rq_yield_curve_get_discount_factor(yc2, date);
            struct _dfs *dfs = (struct _dfs *)RQ_MALLOC(sizeof(struct _dfs));
            dfs->date = date;
            dfs->df1 = df1;
            dfs->df2 = df2;

            rq_tree_rb_add(tree, &dfs->date, dfs);
        }

        for (i = 0; i < rq_yield_curve_size(yc2); i++)
        {
            const struct rq_yield_curve_elem *yce = rq_yield_curve_element_at(
                yc2, 
                i
                );
            double df2 = rq_yield_curve_elem_get_discount_factor(yce);
            rq_date date = rq_yield_curve_elem_get_date(yce);

            struct _dfs *dfs = (struct _dfs *)rq_tree_rb_find(tree, &date);
            if (!dfs)
            {
                double df1 = rq_yield_curve_get_discount_factor(yc1, date);
                dfs = (struct _dfs *)RQ_MALLOC(sizeof(struct _dfs));
                dfs->date = date;
                dfs->df1 = df1;
                dfs->df2 = df2;

                rq_tree_rb_add(tree, &dfs->date, dfs);
            }
        }

        if (!strcmp(rq_asset_ccypair_get_ccy_code_1(asset), rq_yield_curve_get_curve_id(yc1)))
        {
            if (rq_asset_ccypair_get_quote_convention(asset) == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2)
                one_per_two = 1;
            else
                one_per_two = 0;
        }
        else
        {
            if (rq_asset_ccypair_get_quote_convention(asset) == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2)
                one_per_two = 0;
            else
                one_per_two = 1;
        }

        tree_it = rq_tree_rb_iterator_alloc();
        rq_tree_rb_begin(tree, tree_it);
        while (!rq_tree_rb_at_end(tree_it))
        {
            struct _dfs *dfs = (struct _dfs *)rq_tree_rb_iterator_deref(tree_it);
            if (one_per_two)
                forward_rate = spot_rate * dfs->df1 / dfs->df2;
            else
                forward_rate = spot_rate * dfs->df2 / dfs->df1;

            rq_forward_curve_set_rate(forward_curve, dfs->date, forward_rate, 0);

            rq_tree_rb_next(tree_it);
        }

        rq_tree_rb_iterator_free(tree_it);
        rq_tree_rb_free(tree);

        return forward_curve;
    }

    return NULL;
}

