/*
** rq_asset_ccypair.c
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
#include "rq_asset_ccypair.h"
#include "rq_asset_mgr.h"
#include "rq_asset_ccy.h"
#include <stdlib.h>
#include <string.h>

const char * const rq_asset_type_id_ccypair = "CcyPair";


static struct rq_asset_ccypair *
rq_asset_ccypair_alloc()
{
    struct rq_asset_ccypair *cp = 
        (struct rq_asset_ccypair *)RQ_MALLOC(sizeof(struct rq_asset_ccypair));

    cp->ccys = NULL;
    cp->ccy_code_1 = NULL;
    cp->ccy_code_2 = NULL;
    cp->quote_convention = RQ_CCYPAIR_QUOTE_CONVENTION_1PER2;
    cp->days_per_year = 365;
    cp->spot_days = 2;

    return cp;
}

static void
rq_asset_ccypair_free(struct rq_asset_ccypair *asset_ccypair)
{
    if (asset_ccypair->ccys)
        RQ_FREE((char *)asset_ccypair->ccys);
    if (asset_ccypair->ccy_code_1)
        RQ_FREE((char *)asset_ccypair->ccy_code_1);
    if (asset_ccypair->ccy_code_2)
        RQ_FREE((char *)asset_ccypair->ccy_code_2);
    RQ_FREE(asset_ccypair);
}

void
rq_asset_ccypair_freefunc(void *asset_data)
{
	rq_asset_ccypair_free((struct rq_asset_ccypair *)asset_data);
}

RQ_EXPORT 
rq_asset_t
rq_asset_ccypair_build(
    const char *asset_id,
    const char *ccy_code_1,
    const char *ccy_code_2,
    enum rq_ccypair_quote_convention quote_convention,
    unsigned int days_per_year,
    unsigned int spot_days
    )
{
    struct rq_asset_ccypair *asset_ccypair = rq_asset_ccypair_alloc();
    rq_asset_t asset = _rq_asset_alloc(
        rq_asset_type_id_ccypair, 
        asset_id,
        asset_ccypair,
        rq_asset_ccypair_freefunc
        );

    asset_ccypair->ccys = (const char *)RQ_MALLOC(strlen(ccy_code_1) + strlen(ccy_code_2) + 1);
    strcpy((char *)asset_ccypair->ccys, ccy_code_1);
    strcat((char *)asset_ccypair->ccys, ccy_code_2);
    asset_ccypair->ccy_code_1 = (const char *)RQ_STRDUP(ccy_code_1);
    asset_ccypair->ccy_code_2 = (const char *)RQ_STRDUP(ccy_code_2);
    asset_ccypair->quote_convention = quote_convention;
    asset_ccypair->days_per_year = days_per_year;
    asset_ccypair->spot_days = spot_days;

    return asset;
}

int
rq_asset_ccypair_cmp(void *node_data, void *ccys_data)
{
    struct rq_asset_ccypair *ccypair = _rq_asset_get_asset_type_data((rq_asset_t)node_data);

	return strcmp(ccypair->ccys, (const char *)ccys_data);
}

RQ_EXPORT 
rq_asset_t
rq_asset_ccypair_find(const rq_asset_mgr_t asset_mgr, const char *ccy_code_1, const char *ccy_code_2)
{
    char ccys[8];
	rq_asset_t asset;

	memcpy(ccys, ccy_code_1, 3);
    ccys[3] = '/';
	strcpy(ccys+4, ccy_code_2);

    asset = rq_asset_mgr_get(
        asset_mgr, 
        ccys
        );
	if (!asset)
	{
        memcpy(ccys, ccy_code_2, 3);
        ccys[3] = '/';
        strcpy(ccys+4, ccy_code_1);

		asset = rq_asset_mgr_get(
			asset_mgr, 
			ccys
			);
	}

	return asset;
}

RQ_EXPORT 
const char *
rq_asset_ccypair_get_ccy_code_1(const rq_asset_t asset)
{
    struct rq_asset_ccypair *asset_ccypair = 
        (struct rq_asset_ccypair *)_rq_asset_get_asset_type_data(asset);
    return asset_ccypair->ccy_code_1;
}

RQ_EXPORT 
const char *
rq_asset_ccypair_get_ccy_code_2(const rq_asset_t asset)
{
    struct rq_asset_ccypair *asset_ccypair = 
        (struct rq_asset_ccypair *)_rq_asset_get_asset_type_data(asset);
    return asset_ccypair->ccy_code_2;
}

RQ_EXPORT 
enum rq_ccypair_quote_convention 
rq_asset_ccypair_get_quote_convention(const rq_asset_t asset)
{
    struct rq_asset_ccypair *asset_ccypair = 
        (struct rq_asset_ccypair *)_rq_asset_get_asset_type_data(asset);
    return asset_ccypair->quote_convention;
}

RQ_EXPORT 
unsigned short 
rq_asset_ccypair_get_days_per_year(const rq_asset_t asset)
{
    struct rq_asset_ccypair *asset_ccypair = 
        (struct rq_asset_ccypair *)_rq_asset_get_asset_type_data(asset);
    return asset_ccypair->days_per_year;
}

RQ_EXPORT short
rq_asset_is_ccypair(rq_asset_t asset)
{
    return !strcmp(rq_asset_get_asset_type_id(asset), rq_asset_type_id_ccypair);
}

RQ_EXPORT
unsigned short 
rq_asset_ccypair_get_spot_days(const rq_asset_t asset)
{
    const struct rq_asset_ccypair *asset_ccypair = _rq_asset_get_asset_type_data(asset);
    return asset_ccypair->spot_days;
}

RQ_EXPORT
short
rq_asset_ccypair_is_inverted_base(const rq_asset_t asset, const char *base_ccy)
{
    short is_inverted;
    const char *ccy1 = rq_asset_ccypair_get_ccy_code_1(asset);
    const char *ccy2 = rq_asset_ccypair_get_ccy_code_2(asset);
    enum rq_ccypair_quote_convention quote = rq_asset_ccypair_get_quote_convention(asset);
    if (quote == RQ_CCYPAIR_QUOTE_CONVENTION_2PER1)
        is_inverted = (strcmp(ccy2, base_ccy) == 0 ? 0 : 1);
    else
        is_inverted = (strcmp(ccy1, base_ccy) == 0 ? 0 : 1);

    return is_inverted;
}

RQ_EXPORT
short
rq_asset_ccypair_is_inverted_term(const rq_asset_t asset, const char *term_ccy)
{
    return !rq_asset_ccypair_is_inverted_base(asset, term_ccy);
}

RQ_EXPORT
const rq_asset_t
rq_ccy_pair_get_ccy_pair(
    const rq_asset_mgr_t asset_mgr,
    const char *ccy1,
    const char *ccy2,
    const char **base_foreign_ccy,
    const char **term_domestic_ccy)
{
    rq_asset_t asset = NULL;
    const char *baseCcy = NULL;

    if (asset_mgr)
        asset = rq_asset_ccypair_find(asset_mgr, ccy1, ccy2);

    /* first try using base ccy ranking list */
    if (asset_mgr->asset_ranking)
    {
        unsigned long size = rq_string_list_size(rq_asset_mgr_get_ranking_list(asset_mgr));
        unsigned long i;
        for (i = 0; i < size && !baseCcy; ++i)
        {
            const char *ccy = rq_string_list_get_at(rq_asset_mgr_get_ranking_list(asset_mgr), i);
            if (strcmp(ccy1, ccy)==0)
            {
                *base_foreign_ccy = ccy2;
                *term_domestic_ccy = ccy1;
                baseCcy = ccy;
                return asset;
            }
            else if (strcmp(ccy2, ccy)==0)
            {
                *base_foreign_ccy = ccy1;
                *term_domestic_ccy = ccy2;
                baseCcy = ccy;
                return asset;
            }
        }
    }

    /* imply using quote convention (term/domestic per base/foreign) */
    if (asset)
	{
	    if ( rq_asset_ccypair_get_quote_convention( asset ) == RQ_CCYPAIR_QUOTE_CONVENTION_1PER2 )
	    {
		    *base_foreign_ccy = rq_asset_ccypair_get_ccy_code_2(asset);
		    *term_domestic_ccy = rq_asset_ccypair_get_ccy_code_1(asset);
	    }
	    else
	    {
		    *base_foreign_ccy = rq_asset_ccypair_get_ccy_code_1(asset);
		    *term_domestic_ccy = rq_asset_ccypair_get_ccy_code_2(asset);
	    }

        return asset;
	}

    /* use the way it's written */
    *base_foreign_ccy = ccy1;
    *term_domestic_ccy = ccy2;
    return asset;
}

RQ_EXPORT
rq_date
rq_asset_ccypair_get_spot_date(
    const rq_calendar_mgr_t cal_mgr,
    const rq_asset_mgr_t asset_mgr,
    const char *ccy1,
    const char *ccy2,
    rq_date from_date
    )
{
    rq_date spotDate = from_date;
    unsigned short spotDays = 0;
    unsigned short spotDays1 = 0;
    unsigned short spotDays2 = 0;

    rq_calendar_t cal1 = NULL;
    rq_calendar_t cal2 = NULL;
    rq_calendar_t cal3 = NULL;

    const rq_asset_t asset = rq_asset_ccypair_find(asset_mgr, ccy1, ccy2);
    if (asset)
    {
        cal1 = rq_calendar_mgr_get(cal_mgr, rq_asset_get_asset_id(asset));
        spotDays = rq_asset_ccypair_get_spot_days(asset);
    }
    else /* now let's try each ccy with a common base ccy */
    {
        if (asset_mgr && asset_mgr->asset_ranking)
        {
            unsigned long size = rq_string_list_size(rq_asset_mgr_get_ranking_list(asset_mgr));
            unsigned long i;
            for (i = 0; i < size; i++)
            {
                const char *baseCcy = rq_string_list_get_at(rq_asset_mgr_get_ranking_list(asset_mgr), i);
                const rq_asset_t asset1 = rq_asset_ccypair_find(asset_mgr, ccy1, baseCcy); 
                const rq_asset_t asset2 = rq_asset_ccypair_find(asset_mgr, ccy2, baseCcy);

                /* has to be same base ccy */
                if (asset1 && asset2)
                {
                    cal1 = rq_calendar_mgr_get(cal_mgr, rq_asset_get_asset_id(asset1));
                    cal2 = rq_calendar_mgr_get(cal_mgr, rq_asset_get_asset_id(asset2));
                    spotDays1 = rq_asset_ccypair_get_spot_days(asset1);
                    spotDays2 = rq_asset_ccypair_get_spot_days(asset2);
                    spotDays = spotDays1 > spotDays2 ? spotDays1 : spotDays2;

                    if (!cal1 || !cal2)
                        /* don't have explicit base ccy pair calendar,
                        so must need calendar for base ccy &
                        individual ccy later
                        */
                        cal3 = rq_calendar_mgr_get(cal_mgr, baseCcy);

                    break;
                }
            }
        }
    }

    /* can't find same base ccy or no calendar for the base ccy pair,
       so try each ccy on it's own
    */
    if (!cal1)
    {
        const rq_asset_t asset1 = rq_asset_ccy_find(asset_mgr, ccy1);
        if (asset1)
        {
            if (rq_asset_ccy_get_spot_days(asset1) > spotDays)
                spotDays = rq_asset_ccy_get_spot_days(asset1);
            cal1 = rq_calendar_mgr_get(cal_mgr, rq_asset_get_asset_id(asset1));
        }
    }

    if (!cal2)
    {
        const rq_asset_t asset2 = rq_asset_ccy_find(asset_mgr, ccy2);
        if (asset2)
        {
            if (rq_asset_ccy_get_spot_days(asset2) > spotDays)
                spotDays = rq_asset_ccy_get_spot_days(asset2);
            cal2 = rq_calendar_mgr_get(cal_mgr, rq_asset_get_asset_id(asset2));
        }
    }

    while(spotDays > 0 && rq_date_diff(spotDate, from_date) < 365) /* sanity check so no infinite loop */
    {
        spotDate += 1;
        /* TODO: may need to change to first get good date from premium ccy,
           then roll forward (roll convention) if holiday for dealt ccy */
        if (
            rq_calendar_is_good_date(cal1, spotDate) &&
            (cal2 == NULL || rq_calendar_is_good_date(cal2, spotDate)) &&
            (cal3 == NULL || rq_calendar_is_good_date(cal3, spotDate))
           )
            spotDays--;
    }

    return spotDate;
}
