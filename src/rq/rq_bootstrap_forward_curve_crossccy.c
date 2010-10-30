/*
** rq_bootstrap_forward_curve_crossccy.c
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
#include "rq_bootstrap_forward_curve_crossccy.h"
#include <stdlib.h>
#include <string.h>
#include "rq_tree_rb.h"
#include "rq_asset_ccypair.h"


RQ_EXPORT rq_forward_curve_t
rq_bootstrap_forward_curve_crossccy(
    rq_date date, 
    const rq_forward_curve_mgr_t forward_curve_mgr, 
    const rq_rate_mgr_t rate_mgr,
    const rq_bootstrap_config_t config, 
    const rq_asset_mgr_t asset_mgr
    )
{
    rq_forward_curve_t fc = NULL;
    const rq_asset_t asset = rq_asset_mgr_get(asset_mgr, rq_bootstrap_config_get_asset_id(config));
    const char *fromCcy;
    const char *toCcy;

    rq_forward_curve_t start_curve = NULL;
    rq_forward_curve_t end_curve = NULL;
    short start_invert = 0;
    short end_invert = 0;

    const rq_forward_curve_t fc1 = rq_forward_curve_mgr_get(forward_curve_mgr, rq_bootstrap_config_get_curve_id1(config));
    const rq_forward_curve_t fc2 = rq_forward_curve_mgr_get(forward_curve_mgr, rq_bootstrap_config_get_curve_id2(config));

    rq_asset_t asset1;
    rq_asset_t asset2;
    const char *asset1_ccy1;
    const char *asset1_ccy2;
    const char *asset2_ccy1;
    const char *asset2_ccy2;

    unsigned i;

    if (fc1 && fc2 && asset)
    {
        /* 
           OK we need to ensure that the forward curves passed in have
           this criteria: CCY1 THRUCCY CCY2
        */
        if(rq_asset_ccypair_get_quote_convention(asset) == RQ_CCYPAIR_QUOTE_CONVENTION_2PER1)
        {
            fromCcy = rq_asset_ccypair_get_ccy_code_1(asset);
            toCcy = rq_asset_ccypair_get_ccy_code_2(asset);
        }
        else
        {
            fromCcy = rq_asset_ccypair_get_ccy_code_2(asset);
            toCcy = rq_asset_ccypair_get_ccy_code_1(asset);
        }

        asset1 = rq_asset_mgr_get(asset_mgr, rq_forward_curve_get_asset_id(fc1));
        asset1_ccy1 = rq_asset_ccypair_get_ccy_code_1(asset1);
        asset1_ccy2 = rq_asset_ccypair_get_ccy_code_2(asset1);

        asset2 = rq_asset_mgr_get(asset_mgr, rq_forward_curve_get_asset_id(fc2));
        asset2_ccy1 = rq_asset_ccypair_get_ccy_code_1(asset2);
        asset2_ccy2 = rq_asset_ccypair_get_ccy_code_2(asset2);

        /* at the moment I am calculating as a direct rate: ie how to get
           from currency fromCcy to currency toCcy directly - ie: two per one.
        */

        /* OK we either need to use asset 1 or asset two to start off with */
        if ((strcmp(fromCcy, asset1_ccy1) == 0 || strcmp(fromCcy, asset1_ccy2) == 0) &&
            (strcmp(toCcy,   asset2_ccy1) == 0 || strcmp(toCcy,   asset2_ccy2) == 0))
        {
            start_curve = fc1;
            end_curve = fc2;
            start_invert = rq_asset_ccypair_is_inverted_term(asset1, fromCcy);
            end_invert = rq_asset_ccypair_is_inverted_base(asset2, toCcy);
        }
        else if ((strcmp(fromCcy, asset2_ccy1) == 0 || strcmp(fromCcy, asset2_ccy2) == 0) &&
                 (strcmp(toCcy,   asset1_ccy1) == 0 || strcmp(toCcy,   asset1_ccy2) == 0))

        {
            start_curve = fc2;
            end_curve = fc1;
            start_invert = rq_asset_ccypair_is_inverted_term(asset2, fromCcy);
            end_invert = rq_asset_ccypair_is_inverted_base(asset1, toCcy);
        }
            
        if (start_curve && end_curve)
        {
            fc = rq_forward_curve_build(rq_bootstrap_config_get_curve_id(config), rq_asset_get_asset_id(asset));
            for (i = 0; i < rq_forward_curve_size(start_curve); i++)
            {
                double r2;
                const struct rq_forward_rate *fr = rq_forward_curve_element_at(
                    start_curve,
                    i
                    );
                double r1 = fr->rate;
                if (start_invert)
                    r1 = 1.0 / r1;
                rq_forward_curve_get_rate(
                    end_curve,
                    fr->date,
                    &r2
                    );
                if (end_invert)
                    r2 = 1.0 / r2;
                rq_forward_curve_set_rate(
                    fc,
                    fr->date,
                    r1 * r2,
				    0
                    );
            }
        }        
    }

    return fc;
}

