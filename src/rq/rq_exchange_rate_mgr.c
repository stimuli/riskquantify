/*
** rq_exchange_rate_mgr.c
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
#include "rq_exchange_rate_mgr.h"
#include "rq_asset_ccypair.h"
#include "rq_tree_rb.h"
#include "rq_tokenizer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>


/* -- code -------------------------------------------------------- */

static struct rq_exchange_rate_cross_thru_node *
rq_exchange_rate_mgr_cross_thru_node_alloc(const char *ccy_code)
{
    struct rq_exchange_rate_cross_thru_node *n = 
        (struct rq_exchange_rate_cross_thru_node *)
        RQ_CALLOC(1, sizeof(struct rq_exchange_rate_cross_thru_node));
    strcpy(n->ccy_code, ccy_code);
	return n;
}

static struct rq_exchange_rate_cross_thru_node *
rq_exchange_rate_cross_thru_node_clone(struct rq_exchange_rate_cross_thru_node *n)
{
    struct rq_exchange_rate_cross_thru_node *cn = (struct rq_exchange_rate_cross_thru_node *)RQ_MALLOC(sizeof(struct rq_exchange_rate_cross_thru_node));

    if (n->next)
        cn->next = rq_exchange_rate_cross_thru_node_clone(n->next);
	else
		cn->next = NULL;

    strcpy(cn->ccy_code, n->ccy_code);

    return cn;
}

static void
rq_exchange_rate_mgr_cross_thru_node_free(struct rq_exchange_rate_cross_thru_node *n)
{
    if (n->next)
        rq_exchange_rate_mgr_cross_thru_node_free(n->next);
   RQ_FREE(n);
}

RQ_EXPORT rq_exchange_rate_mgr_t 
rq_exchange_rate_mgr_clone(rq_exchange_rate_mgr_t m)
{
    struct rq_exchange_rate_mgr *cm = (struct rq_exchange_rate_mgr *)RQ_MALLOC(sizeof(struct rq_exchange_rate_mgr));

    cm->tree = rq_tree_rb_clone(m->tree, (const void *(*)(const void *))rq_exchange_rate_get_key, (void *(*)(const void *))rq_exchange_rate_clone);

	if (m->cross_thru_node)
		cm->cross_thru_node = rq_exchange_rate_cross_thru_node_clone(m->cross_thru_node);
	else
		cm->cross_thru_node = NULL;

    return cm;
}

RQ_EXPORT rq_exchange_rate_mgr_t 
rq_exchange_rate_mgr_alloc()
{
    struct rq_exchange_rate_mgr *mgr = (struct rq_exchange_rate_mgr *)RQ_CALLOC(1, sizeof(struct rq_exchange_rate_mgr));
    mgr->tree = rq_tree_rb_alloc(
        (void (*)(void *))rq_exchange_rate_free, 
        (int (*)(const void *, const void *))rq_exchange_rate_cmp
        );
    mgr->cross_thru_node = NULL;
    return mgr;
}

RQ_EXPORT void 
rq_exchange_rate_mgr_free(rq_exchange_rate_mgr_t m)
{
    rq_tree_rb_free(m->tree);
    if (m->cross_thru_node)
        rq_exchange_rate_mgr_cross_thru_node_free(m->cross_thru_node);
    RQ_FREE(m);
}

RQ_EXPORT void 
rq_exchange_rate_mgr_clear(rq_exchange_rate_mgr_t m)
{
    rq_tree_rb_clear(m->tree);
    if (m->cross_thru_node)
        rq_exchange_rate_mgr_cross_thru_node_free(m->cross_thru_node);
    m->cross_thru_node = NULL;
}

static int
lookup_exchange_rate(
    struct rq_exchange_rate_cross_thru_node *cross_thru_node,
    rq_forward_curve_mgr_t forward_curve_mgr, 
    rq_asset_mgr_t asset_mgr, 
    const char *ccy_code_from,
    const char *ccy_code_to,
    rq_date date, 
    double *exchange_rate
    )
{
    if (CCY_CODE_EQUAL(ccy_code_from, ccy_code_to))
    {
        *exchange_rate = 1.0;
        return 0;
    }
    else
    {
        rq_asset_t asset = rq_asset_ccypair_find(asset_mgr, ccy_code_from, ccy_code_to);
        if (asset)
        {
            const char *c1 = rq_asset_ccypair_get_ccy_code_1(asset);
            const char *c2 = rq_asset_ccypair_get_ccy_code_2(asset);
            enum rq_ccypair_quote_convention qc = rq_asset_ccypair_get_quote_convention(asset);
            rq_forward_curve_t fc = rq_forward_curve_mgr_get(forward_curve_mgr, rq_asset_get_asset_id(asset));

            if (fc)
            {

                if (!rq_forward_curve_get_rate(fc, date, exchange_rate))
                {
                    int is_direct = 0;

                    if (CCY_CODE_EQUAL(ccy_code_from, c1) && qc == RQ_CCYPAIR_QUOTE_CONVENTION_2PER1 ||
                        CCY_CODE_EQUAL(ccy_code_from, c2) && qc == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2)
                    {
                        is_direct = 1;
                    }
                    else
                        is_direct = 0;

                    if (!is_direct)
                        *exchange_rate = 1.0 / *exchange_rate;

                    return 0;
                }
            }
            else
            {
                /* 
                   couldn't find it. We'll try to imply the cross ccy by 
                   going through our list of crosses.
                */

                /* make sure our cross thru currency is neither ccy1 or ccy2 */
                while (cross_thru_node && 
                       (CCY_CODE_EQUAL(cross_thru_node->ccy_code, c1) ||
                        CCY_CODE_EQUAL(cross_thru_node->ccy_code, c2)))
                    cross_thru_node = cross_thru_node->next;

                if (cross_thru_node)
                {
                    /* try and go from ccy_code_from -> cross_thru_ccy
                       and then from cross_thru_ccy -> ccy_code_to
                    */
                    double rate1;
                    double rate2;
                    if (!lookup_exchange_rate(cross_thru_node->next,
                                              forward_curve_mgr, 
                                              asset_mgr, 
                                              ccy_code_from,
                                              cross_thru_node->ccy_code,
                                              date, 
                                              &rate1) &&
                        !lookup_exchange_rate(cross_thru_node->next,
                                              forward_curve_mgr, 
                                              asset_mgr, 
                                              cross_thru_node->ccy_code,
                                              ccy_code_to,
                                              date, 
                                              &rate2))
                    {
                        /* success! */
                        *exchange_rate = rate1 * rate2;
                        return 0;
                    }
                }
            }
        }
        else
        {
            /* 
               couldn't find it. We'll try to imply the cross ccy by 
               going through our list of crosses.
            */

            /* make sure our cross thru currency is neither ccy1 or ccy2 */
            while (cross_thru_node && 
                   (CCY_CODE_EQUAL(cross_thru_node->ccy_code, ccy_code_from) ||
                    CCY_CODE_EQUAL(cross_thru_node->ccy_code, ccy_code_to)))
                cross_thru_node = cross_thru_node->next;

            if (cross_thru_node)
            {
                /* try and go from ccy_code_from -> cross_thru_ccy
                   and then from cross_thru_ccy -> ccy_code_to
                */
                double rate1;
                double rate2;
                if (!lookup_exchange_rate(NULL,
                                          forward_curve_mgr, 
                                          asset_mgr, 
                                          ccy_code_from,
                                          cross_thru_node->ccy_code,
                                          date, 
                                          &rate1) &&
                    !lookup_exchange_rate(NULL,
                                          forward_curve_mgr, 
                                          asset_mgr, 
                                          cross_thru_node->ccy_code,
                                          ccy_code_to,
                                          date, 
                                          &rate2))
                {
                    /* success! */
                    *exchange_rate = rate1 * rate2;
                    return 0;
                }
            }
        }

        return 1;
    }
}

RQ_EXPORT int 
rq_exchange_rate_mgr_get_or_imply(
    rq_exchange_rate_mgr_t m, 
    rq_forward_curve_mgr_t forward_curve_mgr,
    rq_asset_mgr_t asset_mgr,
    const char *ccy_code_from,
    const char *ccy_code_to,
    rq_date date,
    double *exchange_rate,
    short cache_result
    )
{
    rq_exchange_rate_t er;
	struct rq_exchange_rate_key key;

	key.ccy_code_from = ccy_code_from;
	key.ccy_code_to = ccy_code_to;

    /* find the cached node if it exists */
    er = rq_tree_rb_find(m->tree, &key);
    if (er == NULL)
    {
        if (!lookup_exchange_rate(m->cross_thru_node, forward_curve_mgr, asset_mgr, ccy_code_from, ccy_code_to, date, exchange_rate))
        {
            if (cache_result)
            {
				double other_rate = 0.0;

                er = rq_exchange_rate_build(ccy_code_from, ccy_code_to, *exchange_rate);
                rq_tree_rb_add(m->tree, (void *)rq_exchange_rate_get_key(er), er);

				if (*exchange_rate)
					other_rate = 1.0 / *exchange_rate;

                er = rq_exchange_rate_build(ccy_code_to, ccy_code_from, other_rate);
                rq_tree_rb_add(m->tree, (void *)rq_exchange_rate_get_key(er), er);
            }
        }
        else 
        {
            *exchange_rate = 0.0;
            return 1;
        }
    }
    else
    {
		*exchange_rate = er->exchange_rate;
    }

    return 0;
}

RQ_EXPORT void
rq_exchange_rate_mgr_add(
    rq_exchange_rate_mgr_t m, 
    const char *ccy_code_from,
    const char *ccy_code_to,
    double exchange_rate
    )
{
    rq_exchange_rate_t er = rq_exchange_rate_build(ccy_code_from, ccy_code_to, exchange_rate);
    rq_tree_rb_add(m->tree, (void *)rq_exchange_rate_get_key(er), er);
}


RQ_EXPORT double
rq_exchange_rate_mgr_get(
    rq_exchange_rate_mgr_t m, 
    const char *ccy_code_from,
    const char *ccy_code_to
    )
{
    rq_exchange_rate_t er;
	struct rq_exchange_rate_key key;

    if (CCY_CODE_EQUAL(ccy_code_from, ccy_code_to))
        return 1.0;

	key.ccy_code_from = ccy_code_from;
	key.ccy_code_to = ccy_code_to;

    /* find the cached node if it exists */
    er = rq_tree_rb_find(m->tree, &key);
    if (er)
	{
		return er->exchange_rate;
	}
	else
	{
	    struct rq_exchange_rate_cross_thru_node *cross_thru_node = m->cross_thru_node;
		while (cross_thru_node)
		{
			if (!CCY_CODE_EQUAL(ccy_code_from, cross_thru_node->ccy_code) &&
				!CCY_CODE_EQUAL(ccy_code_to, cross_thru_node->ccy_code))
			{
				double r1 = rq_exchange_rate_mgr_get(
					m, 
					ccy_code_from,
					cross_thru_node->ccy_code
					);
				double r2 = rq_exchange_rate_mgr_get(
					m, 
					cross_thru_node->ccy_code,
					ccy_code_to
					);
				return r1 * r2;
			}
			cross_thru_node = cross_thru_node->next;
		}
	}
    
    return 0.0;
}

RQ_EXPORT void
rq_exchange_rate_mgr_add_cross_thru_ccy_code(
    rq_exchange_rate_mgr_t m, 
    const char *ccy_code
    )
{
    if (m->cross_thru_node)
    {
        struct rq_exchange_rate_cross_thru_node *n = m->cross_thru_node;
        while (n->next)
            n = n->next;
        n->next = rq_exchange_rate_mgr_cross_thru_node_alloc(ccy_code);
    }
    else
    {
        m->cross_thru_node = rq_exchange_rate_mgr_cross_thru_node_alloc(ccy_code);
    }
}

RQ_EXPORT void
rq_exchange_rate_mgr_add_cross_thru_ccy_codes(
    rq_exchange_rate_mgr_t m, 
    const char *ccy_codes
    )
{
    if (ccy_codes && ccy_codes[0])
    {
        char ccy[256];
        while (!RQ_IS_NULL(rq_tokenizer_get_token(&ccy_codes, ccy, 256, ",", " ", "\"")))
        {
            if (strlen(ccy))
            {
		        rq_exchange_rate_mgr_add_cross_thru_ccy_code(m, ccy);
            }
        }
    }
    else
    {
        rq_exchange_rate_mgr_add_cross_thru_ccy_code(m, "USD");
    }
}

RQ_EXPORT int
rq_exchange_rate_mgr_is_null(rq_exchange_rate_mgr_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_exchange_rate_mgr_iterator_t 
rq_exchange_rate_mgr_iterator_alloc()
{
    rq_exchange_rate_mgr_iterator_t ycmi = (rq_exchange_rate_mgr_iterator_t)
        RQ_MALLOC(sizeof(struct rq_exchange_rate_mgr_iterator));
    ycmi->exchange_rate_it = rq_tree_rb_iterator_alloc();
    return ycmi;
}

RQ_EXPORT void 
rq_exchange_rate_mgr_iterator_free(rq_exchange_rate_mgr_iterator_t it)
{
    rq_tree_rb_iterator_free(it->exchange_rate_it);
    RQ_FREE(it);
}

RQ_EXPORT void 
rq_exchange_rate_mgr_begin(rq_exchange_rate_mgr_t m, rq_exchange_rate_mgr_iterator_t it)
{
    rq_tree_rb_begin(m->tree, it->exchange_rate_it);
}

RQ_EXPORT int 
rq_exchange_rate_mgr_at_end(rq_exchange_rate_mgr_iterator_t it)
{
    return rq_tree_rb_at_end(it->exchange_rate_it);
}

RQ_EXPORT void 
rq_exchange_rate_mgr_next(rq_exchange_rate_mgr_iterator_t it)
{
    rq_tree_rb_next(it->exchange_rate_it);
}

RQ_EXPORT rq_exchange_rate_t 
rq_exchange_rate_mgr_iterator_deref(rq_exchange_rate_mgr_iterator_t i)
{
    return (rq_exchange_rate_t) rq_tree_rb_iterator_deref(i->exchange_rate_it);
}
