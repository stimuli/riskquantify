/*
** rq_bootstrap_forward_curve_bbi.c
**
** Written by Hendra
**
** Copyright (C) 2008 Brett Hutley
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
#include "rq_bootstrap_forward_curve_bbi.h"
#include "rq_bootstrap_config_mgr.h"
#include "rq_forward_curve.h"
#include "rq_asset_irdiscount.h"
#include "rq_system.h"
#include "rq_defs.h"
#include "rq_term.h"
#include "rq_date_roll.h"
#include <stdlib.h>
#include <stdio.h>

RQ_EXPORT rq_forward_curve_t
rq_bootstrap_forward_curve_bbi(
    rq_date date, 
    const rq_rate_mgr_t rate_mgr, 
    const rq_bootstrap_config_t config, 
    const rq_system_t system,
    double* rate_adjust_factors,
    rq_date last_date_to_bootstrap
    )
{
#define MAX_CASH_RATE 3
#define MAX_BILL_FUTURE 13
#define DAYS_PER_YEAR 365

#ifdef DEBUG_BBI
    FILE* fh = NULL;
    static int firstTime = 1;
#endif
    unsigned int i = 0;
    int j = 0;
    int k = 0;
    int ndays = 0;
    int nrow = 0;
    double cashrate = 0.0;
    double totalValue = 0.0;
    double value = 0.0;
    rq_forward_curve_t fc = NULL;
    rq_bootstrap_config_t yc_config = NULL;
    const char* yc_curve_id = rq_bootstrap_config_get_curve_id1(config);
    const char* curve_id = rq_bootstrap_config_get_asset_id(config);
    rq_calendar_t cal = rq_calendar_mgr_get(rq_system_get_calendar_mgr(system), curve_id);

    rq_asset_mgr_t asset_mgr = rq_system_get_asset_mgr(system);
    rq_asset_t asset;
    long maturityDate;
    char buf[16];
    double df;
    double pv;

    rq_rate_t rate = NULL;

    /* Only 3 cash rate is given by TCV */
    double BBSW_rate[MAX_CASH_RATE] = {0.0, 0.0, 0.0};

    /* This ratio is given by TCV.  13 weeks using 3 cash rates. */
    double BBI_rate[MAX_BILL_FUTURE * MAX_CASH_RATE] = {
        1.0,     0.0,     0.0, 
        2.0/3.0, 1.0/3.0, 0.0, 
        1.0/3.0, 2.0/3.0, 0.0, 
        0.0,     1.0,     0.0, 
        0.0,     8.0/9.0, 1.0/9.0,
        0.0,     7.0/9.0, 2.0/9.0,
        0.0,     6.0/9.0, 3.0/9.0,
        0.0,     5.0/9.0, 4.0/9.0,
        0.0,     4.0/9.0, 5.0/9.0,
        0.0,     3.0/9.0, 6.0/9.0,
        0.0,     2.0/9.0, 7.0/9.0,
        0.0,     1.0/9.0, 8.0/9.0,
        0.0,     0.0,     1.0
        };

    if (yc_curve_id != NULL)
    {
        yc_config = rq_bootstrap_config_mgr_find(
            rq_system_get_bootstrap_config_mgr(system), 
            yc_curve_id, 
            RQ_TERMSTRUCT_TYPE_FORWARD_CURVE
            );

        if (yc_config == NULL)
            return NULL;
    }
    else
        return NULL;

    /* Get the cash rates */
    for (i = 0; i < rq_bootstrap_config_get_num_rate_class_ids(yc_config) && i<MAX_CASH_RATE; i++)
    {
        rate = rq_rate_mgr_find(rate_mgr, 
            rq_bootstrap_config_get_rate_class_id_at(yc_config, i)
            );

        if (rate)
        {
            BBSW_rate[i] = rq_rate_get_value(rate);
        }
        else
            return NULL;
    }

#ifdef DEBUG_BBI
    if (firstTime != 0)
    {
        fh = fopen("..\\log\\Bootstrap_BBI.log", "w");
        if (fh != NULL)
        {
            rq_date_to_string(buf, "yyyy-mm-dd", date);
            fprintf(fh, "The base date used is %s. Day Count Convention used is ACT/%d.\nThe %d cash rates:\n", buf, DAYS_PER_YEAR, MAX_CASH_RATE);
            firstTime = 0;
        }
    }

    for (i=0; i<MAX_CASH_RATE && fh!= NULL; i++)
    {
        fprintf(fh, "%.10f\t", BBSW_rate[i]);
    }

    if (fh != NULL)
    {
        fprintf(fh, "\n\nMaturity Date\tNDays\tRate\tFaceValue\tdf\tPV\n");
    }
#endif

    /* construct an empty forward curve */
    fc = rq_forward_curve_build(
        rq_bootstrap_config_get_curve_id(config),
        curve_id);

    for (k=0; k<2; k++)
    {
        totalValue = 0.0;

        /* iterate through the bootstrap_config getting asset ids */
        for (i=0; i < rq_bootstrap_config_get_num_rate_class_ids(config) && i<MAX_BILL_FUTURE ; i++)
        {
            rate = rq_rate_mgr_find(rate_mgr, 
                rq_bootstrap_config_get_rate_class_id_at(config, i)
                );

            if (rate)
            {
                asset = rq_asset_mgr_get(asset_mgr, rq_rate_get_asset_id(rate));
                maturityDate = rq_asset_irdiscount_get_maturity_date(asset);
                value = rq_rate_get_value(rate);
                ndays = maturityDate - date;
                if (ndays < 0)
                    continue;

                nrow = (int)((ndays-1) / 7);
                if (nrow < 0)
                    nrow = 0;

                cashrate = 0.0;
                for (j=0; j<MAX_CASH_RATE; j++)
                {
                    cashrate += BBI_rate[nrow*MAX_CASH_RATE+j] * BBSW_rate[j];
                }

                df = 1 / ((double)ndays/DAYS_PER_YEAR * cashrate + 1);
                pv = value * df;
                totalValue += pv;

#ifdef DEBUG_BBI
                if (fh != NULL)
                {
                    rq_date_to_string(buf, "yyyy-mm-dd", maturityDate);
                    fprintf(fh, "%s\t%d\t%.10f\t%.0f\t%.10f\t%.2f\n", buf, ndays, cashrate, value, df, pv);
                }
#endif

                if (last_date_to_bootstrap > 0 && maturityDate > last_date_to_bootstrap)
                    break; /* break out of for loop, we've bootstrapped enough */
            }
        }

        rq_forward_curve_set_rate(fc, date, totalValue, 0);
#ifdef DEBUG_BBI
        if (fh != NULL)
        {
            rq_date_to_string(buf, "yyyy-mm-dd", date);
            fprintf(fh, "Date and Total Face Value set in Forward Curve are: %s, %.2f.\n", buf, totalValue);
        }
#endif

        /* TCV 191 - GBID/PBID Change Request to support various scenarios
		  // date++;
        */
		date--;

        if (cal != NULL)
        {
			/* TCV 191 - GBID/PBID Change Request to support various scenarios
			//date = rq_date_roll_adjust_date(date, RQ_DATE_ROLL_FOLLOWING, &cal, 1);
            */
            date = rq_date_roll_adjust_date(date, RQ_DATE_ROLL_PRECEDING, &cal, 1);
        }
#ifdef DEBUG_BBI
        else if (fh != NULL)
        {
            fprintf(fh, "Calendar is null for asset id: %s.\n", curve_id);
        }
#endif
    }

#ifdef DEBUG_BBI
    if (fh != NULL)
        fclose(fh);
#endif

    if (rq_forward_curve_size(fc))
        return fc;

    rq_forward_curve_free(fc);
    return NULL;
}
