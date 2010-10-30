/*
** rq_floating_flow.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include <stdlib.h>
#include <string.h>
#include <float.h>
#include "rq_floating_flow.h"
#include "rq_interpolate.h"
#include "rq_day_count.h"
#include "rq_ir_vol_surface.h"
#include "rq_vol_surface.h"
#include "rq_pricing_blackscholes.h"

RQ_EXPORT rq_floating_flow_rate_list_t 
rq_floating_flow_rate_list_alloc()
{
    rq_floating_flow_rate_list_t ffr_list = (rq_floating_flow_rate_list_t)RQ_CALLOC(1, sizeof(struct rq_floating_flow_rate_list));
    ffr_list->floating_flow_rate_list = rq_array_alloc((void (*)(void *))rq_floating_flow_rate_free);
    return ffr_list;
}

RQ_EXPORT void 
rq_floating_flow_rate_list_free(rq_floating_flow_rate_list_t ffr_list)
{
    rq_array_free(ffr_list->floating_flow_rate_list);
    RQ_FREE(ffr_list);
}

RQ_EXPORT void 
rq_floating_flow_rate_free(rq_floating_flow_rate_t ffr)
{
    if (ffr->termstruct_id)
        RQ_FREE((char *)ffr->termstruct_id);

    if (ffr->currency)
        RQ_FREE((char *)ffr->currency);

    if (ffr->cms_data)
    {
        RQ_FREE(ffr->cms_data->cm_freq);
        RQ_FREE(ffr->cms_data->cm_term);
        RQ_FREE(ffr->cms_data);
    }

    RQ_FREE(ffr);
}

RQ_EXPORT void 
rq_floating_flow_rate_list_add(rq_floating_flow_rate_list_t ffr_list, rq_floating_flow_rate_t ffr)
{
    rq_array_push_back(ffr_list->floating_flow_rate_list, ffr);
}

RQ_EXPORT unsigned long
rq_floating_flow_rate_list_size(rq_floating_flow_rate_list_t ffr_list)
{
    return rq_array_size(ffr_list->floating_flow_rate_list);
}

RQ_EXPORT const char *
rq_floating_flow_type_to_string(enum rq_floating_flow_type flowtype)
{
    switch (flowtype)
    {
		case RQ_FLOATING_FLOW_TYPE_FLOATING_RATE_PAYMENT:
			return "FLOATING_RATE";
			break;
		case RQ_FLOATING_FLOW_TYPE_FIXED_RATE_PAYMENT:
			return "FIXED_RATE";
			break;
		case RQ_FLOATING_FLOW_TYPE_STATIC_PAYMENT:
			return "STATIC";
			break;
		case RQ_FLOATING_FLOW_TYPE_INTEREST_REINVESTMENT:
			return "NOTIONAL";
			break;
		case RQ_FLOATING_FLOW_TYPE_RATE_SET:
			return "RATE_SET";
			break;
		case RQ_FLOATING_FLOW_TYPE_AVERAGE_RATE_PAYMENT:
			return "AVERAGE_RATE";
			break;
		default:
			return "UNKNOWN";
			break;
    }
}

RQ_EXPORT int 
rq_floating_flow_is_null(rq_floating_flow_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_floating_flow_t 
rq_floating_flow_alloc()
{
    rq_floating_flow_t ff = (rq_floating_flow_t)RQ_CALLOC(
        1, 
        sizeof(struct rq_floating_flow)
        );

    ff->flow_type = RQ_FLOATING_FLOW_TYPE_UNKNOWN;
    ff->pay_receive = RQ_PAY_RECEIVE_INVALID;
    ff->payment_asset_id = NULL;
	ff->floating_rate = 0;
    ff->frBasket = 0;

    ff->override_get_payment_func = NULL;
    ff->override_set_rate_func = NULL;
    return ff;
}

RQ_EXPORT void 
rq_floating_flow_free(rq_floating_flow_t ff)
{
    rq_floating_flow_rate_list_t ffr = ff->frBasket;

    if (ff->floating_rate && ff->floating_rate->termstruct_id)
	{
		if (ff->floating_rate && ff->floating_rate->termstruct_id)
			RQ_FREE((char *)ff->floating_rate->termstruct_id);
        RQ_FREE(ff->floating_rate);
	}

    if (ff->payment_asset_id)
        RQ_FREE((char *)ff->payment_asset_id);

    if (ffr != 0)
    {
        rq_floating_flow_rate_list_free(ffr);
    }

    RQ_FREE(ff);
}

RQ_EXPORT rq_floating_flow_cms_t 
rq_floating_flow_rate_cms_clone(rq_floating_flow_cms_t ffrCms)
{
    rq_floating_flow_cms_t ffrCmsClone = (rq_floating_flow_cms_t)RQ_MALLOC(sizeof(struct rq_floating_flow_cms));
    memcpy(ffrCmsClone, ffrCms, sizeof(struct rq_floating_flow_cms));

    if (ffrCmsClone->cm_freq)
    {
        rq_term_t cm_freq_clone = (rq_term_t) RQ_MALLOC(sizeof(struct rq_term));
        memcpy(cm_freq_clone, ffrCmsClone->cm_freq, sizeof(struct rq_term));
    }
    if (ffrCmsClone->cm_term)
    {
        rq_term_t cm_term_clone = (rq_term_t) RQ_MALLOC(sizeof(struct rq_term));
        memcpy(cm_term_clone, ffrCmsClone->cm_term, sizeof(struct rq_term));
    }

    return ffrCmsClone;
}


RQ_EXPORT rq_floating_flow_rate_t 
rq_floating_flow_rate_clone(rq_floating_flow_rate_t ffr)
{
    rq_floating_flow_rate_t ffrClone = (rq_floating_flow_rate_t)RQ_MALLOC(sizeof(struct rq_floating_flow_rate));
    memcpy(ffrClone, ffr, sizeof(struct rq_floating_flow_rate));

    if (ffr->termstruct_id)
        ffrClone->termstruct_id = RQ_STRDUP(ffr->termstruct_id);
    if (ffr->currency)
        ffrClone->currency = RQ_STRDUP(ffr->currency);

    if (ffr->cms_data)
        ffrClone->cms_data = rq_floating_flow_rate_cms_clone(ffr->cms_data);

    return ffrClone;
}

RQ_EXPORT rq_floating_flow_t
rq_floating_flow_clone(rq_floating_flow_t ff)
{
    rq_floating_flow_rate_list_iterator it, itEnd;

    rq_floating_flow_t ffc = (rq_floating_flow_t)RQ_MALLOC(
        sizeof(struct rq_floating_flow)
        );
    memcpy(ffc, ff, sizeof(struct rq_floating_flow));

	if (ff->floating_rate)
	{
        ffc->floating_rate = rq_floating_flow_rate_clone(ff->floating_rate);
	}

    if (ff->frBasket)
    {
        ffc->frBasket = rq_floating_flow_rate_list_alloc();

        it = rq_floating_flow_rate_list_begin(ff->frBasket);
        itEnd = rq_floating_flow_rate_list_end(ff->frBasket);
        for (; it != itEnd; it++)
        {
            rq_floating_flow_rate_t ffrClone = rq_floating_flow_rate_clone(*it);
            rq_floating_flow_rate_list_add(ffc->frBasket, ffrClone);
        }
    }

	if (ff->payment_asset_id)
		ffc->payment_asset_id = RQ_STRDUP(ff->payment_asset_id);

    return ffc;
}

RQ_EXPORT void 
rq_floating_flow_add_floating_flow_rate_element(
    rq_floating_flow_rate_list_t ffrList,
    const char *termstruct_id,
    const char* currency,
    rq_date fixing_date,
    rq_date rate_start_date,
    rq_date rate_end_date,
    double spread,
    double fixing_spread,
    double cap_rate,
    double floor_rate,
    unsigned rate_group_id,
    rq_date date_observed,
    double rate_multiplier,
    rq_term_t cm_term,
    rq_term_t cm_freq
    )
{
    rq_floating_flow_rate_t ffr = RQ_CALLOC(1, sizeof(struct rq_floating_flow_rate));

    ffr->termstruct_id = RQ_STRDUP(termstruct_id);
    ffr->currency = RQ_STRDUP(currency);

    ffr->fixing_date = fixing_date;
    ffr->rate_start_date = rate_start_date;
    ffr->rate_end_date = rate_end_date;
    ffr->spread = spread;
    ffr->fixing_spread = fixing_spread;
    ffr->cap_rate = cap_rate;
    ffr->floor_rate = floor_rate;
    ffr->rate_group_id = rate_group_id;
    ffr->date_observed = date_observed;
    ffr->rate_multiplier = rate_multiplier;
    if (cm_term && cm_freq)
    {
        ffr->cms_data = RQ_CALLOC(1, sizeof(struct rq_floating_flow_cms));
        ffr->cms_data->cm_term = cm_term;
        ffr->cms_data->cm_freq = cm_freq;
    }
    else
        ffr->cms_data = 0;

    rq_floating_flow_rate_list_add(ffrList, ffr);
}

RQ_EXPORT void 
rq_floating_flow_set_type_floating_rate_basket_payment(
    rq_floating_flow_t ff,
    enum rq_pay_receive pay_receive,
    const char *payment_asset_id,
    rq_date payment_date,
    rq_date accrual_start_date,
    rq_date accrual_end_date,
    enum rq_day_count_convention day_count_convention,
    double notional_amount,
    unsigned notional_exchange_id
    )
{
    if (ff->payment_asset_id)
        RQ_FREE((char *)ff->payment_asset_id);
    ff->payment_asset_id = RQ_STRDUP(payment_asset_id);

    ff->flow_type = RQ_FLOATING_FLOW_TYPE_FLOATING_RATE_PAYMENT;
    ff->flow_status = 0;
    ff->pay_receive = pay_receive;

    ff->payment_date = payment_date;
    ff->accrual_start_date = accrual_start_date;
    ff->accrual_end_date = accrual_end_date;
	ff->notional_amount = notional_amount;
    ff->notional_exchange_id = notional_exchange_id;

    ff->rate = 0.0;
    ff->payment_amount = 0.0;

    ff->day_count_convention = day_count_convention;

	ff->year_fraction_calc = rq_day_count_get_year_fraction(day_count_convention, accrual_start_date, accrual_end_date);
	ff->notional_fraction_calc = ff->notional_amount * ff->year_fraction_calc;
}

RQ_EXPORT void 
rq_floating_flow_set_type_floating_rate_payment(
    rq_floating_flow_t ff,
    enum rq_pay_receive pay_receive,
    const char *termstruct_id,
    const char *payment_asset_id,
    rq_date payment_date,
    rq_date fixing_date,
    rq_date rate_start_date,
    rq_date rate_end_date,
    rq_date accrual_start_date,
    rq_date accrual_end_date,
    enum rq_day_count_convention day_count_convention,
    double notional_amount,
    unsigned notional_exchange_id,
    double rate_multiplier
    )
{
    if (ff->floating_rate && ff->floating_rate->termstruct_id)
        RQ_FREE((char *)ff->floating_rate->termstruct_id);

    if (ff->payment_asset_id)
        RQ_FREE((char *)ff->payment_asset_id);

	if (!ff->floating_rate)
    {
		ff->floating_rate = (rq_floating_flow_rate_t)RQ_CALLOC(
			1, 
			sizeof(struct rq_floating_flow_rate)
			);
    }

    ff->floating_rate->termstruct_id = RQ_STRDUP(termstruct_id);
    ff->floating_rate->currency = NULL;

    ff->payment_asset_id = RQ_STRDUP(payment_asset_id);

    ff->flow_type = RQ_FLOATING_FLOW_TYPE_FLOATING_RATE_PAYMENT;
    ff->flow_status = 0;

    ff->pay_receive = pay_receive;

    ff->payment_date = payment_date;
    ff->floating_rate->fixing_date = fixing_date;
    ff->floating_rate->rate_start_date = rate_start_date;
    ff->floating_rate->rate_end_date = rate_end_date;
    ff->accrual_start_date = accrual_start_date;
    ff->accrual_end_date = accrual_end_date;
	ff->notional_amount = notional_amount;
    ff->floating_rate->spread = 0.0;
    ff->floating_rate->fixing_spread = 0.0;
    ff->floating_rate->cap_rate = 0.0;
    ff->floating_rate->floor_rate = 0.0;
    ff->notional_exchange_id = notional_exchange_id;

    ff->rate = 0.0;
    ff->floating_rate->rate_group_id = 0;

    ff->floating_rate->date_observed = 0;
    ff->payment_amount = 0.0;

    ff->day_count_convention = day_count_convention;
    ff->floating_rate->rate_multiplier = rate_multiplier;

	ff->year_fraction_calc = rq_day_count_get_year_fraction(day_count_convention, accrual_start_date, accrual_end_date);
	ff->notional_fraction_calc = ff->notional_amount * ff->year_fraction_calc;
}

RQ_EXPORT void 
rq_floating_flow_set_type_fixed_rate_payment(
    rq_floating_flow_t ff,
    enum rq_pay_receive pay_receive,
    const char *payment_asset_id,
    rq_date payment_date,
    rq_date accrual_start_date,
    rq_date accrual_end_date,
    enum rq_day_count_convention day_count_convention,
    double notional_amount,
    double fixed_rate
    )
{
	if (ff->floating_rate)
	{
		if (ff->floating_rate->termstruct_id)
			RQ_FREE((char *)ff->floating_rate->termstruct_id);
		RQ_FREE(ff->floating_rate);
		ff->floating_rate = 0;
	}

    if (ff->payment_asset_id)
        RQ_FREE((char *)ff->payment_asset_id);

    memset(ff, '\0', sizeof(struct rq_floating_flow));

    ff->flow_type = RQ_FLOATING_FLOW_TYPE_FIXED_RATE_PAYMENT;
    ff->flow_status = 0;
    ff->pay_receive = pay_receive;
    ff->payment_asset_id = RQ_STRDUP(payment_asset_id);
    ff->payment_date = payment_date;
    ff->accrual_start_date = accrual_start_date;
    ff->accrual_end_date = accrual_end_date;
    ff->notional_amount = notional_amount;
    ff->rate = fixed_rate;
    ff->day_count_convention = day_count_convention;
	ff->year_fraction_calc = rq_day_count_get_year_fraction(day_count_convention, accrual_start_date, accrual_end_date);
	ff->notional_fraction_calc = ff->notional_amount * ff->year_fraction_calc;
	ff->payment_amount = ff->notional_fraction_calc * ff->rate;
}

RQ_EXPORT void 
rq_floating_flow_set_type_static_payment(
    rq_floating_flow_t ff,
    enum rq_pay_receive pay_receive,
    const char *payment_asset_id,
    rq_date payment_date,
    double payment_amount
    )
{
	if (ff->floating_rate)
	{
		if (ff->floating_rate->termstruct_id)
			RQ_FREE((char *)ff->floating_rate->termstruct_id);
		RQ_FREE(ff->floating_rate);
		ff->floating_rate = 0;
	}
    if (ff->payment_asset_id)
        RQ_FREE((char *)ff->payment_asset_id);

    memset(ff, '\0', sizeof(struct rq_floating_flow));

    ff->flow_type = RQ_FLOATING_FLOW_TYPE_STATIC_PAYMENT;
    ff->flow_status = 0;

    ff->pay_receive = pay_receive;
    ff->payment_asset_id = RQ_STRDUP(payment_asset_id);
    ff->payment_date = payment_date;
    ff->payment_amount = payment_amount;
}

RQ_EXPORT void 
rq_floating_flow_set_type_rate_set(
    rq_floating_flow_t ff,
    const char *termstruct_id,
    rq_date fixing_date,
    rq_date rate_start_date,
    rq_date rate_end_date,
    unsigned rate_group_id
    )
{
    if (ff->floating_rate && ff->floating_rate->termstruct_id)
        RQ_FREE((char *)ff->floating_rate->termstruct_id);

    if (ff->payment_asset_id)
        RQ_FREE((char *)ff->payment_asset_id);

    memset(ff, '\0', sizeof(struct rq_floating_flow));

	if (!ff->floating_rate)
	{
		ff->floating_rate = (rq_floating_flow_rate_t)RQ_CALLOC(
			1, 
			sizeof(struct rq_floating_flow_rate)
			);
        ff->override_get_payment_func = NULL;
        ff->override_set_rate_func = NULL;
	}

    memset(ff->floating_rate, '\0', sizeof(struct rq_floating_flow_rate));

    ff->flow_type = RQ_FLOATING_FLOW_TYPE_RATE_SET;
    ff->flow_status = 0;

    ff->floating_rate->termstruct_id = RQ_STRDUP(termstruct_id);
    ff->floating_rate->fixing_date = fixing_date;
    ff->floating_rate->rate_start_date = rate_start_date;
    ff->floating_rate->rate_end_date = rate_end_date;
    ff->floating_rate->rate_group_id = rate_group_id;
    ff->floating_rate->rate_multiplier = 1.0;
}

RQ_EXPORT void 
rq_floating_flow_set_type_average_rate_payment(
    rq_floating_flow_t ff,
    enum rq_pay_receive pay_receive,
    const char *payment_asset_id,
    rq_date payment_date,
    rq_date accrual_start_date,
    rq_date accrual_end_date,
    double notional_amount,
    unsigned long rate_group_id
    )
{
    if (ff->floating_rate && ff->floating_rate->termstruct_id)
        RQ_FREE((char *)ff->floating_rate->termstruct_id);

    if (ff->payment_asset_id)
        RQ_FREE((char *)ff->payment_asset_id);

    memset(ff, '\0', sizeof(struct rq_floating_flow));

	if (!ff->floating_rate)
	{
		ff->floating_rate = (rq_floating_flow_rate_t)RQ_CALLOC(
			1, 
			sizeof(struct rq_floating_flow_rate)
			);
	}

    memset(ff->floating_rate, '\0', sizeof(struct rq_floating_flow_rate));

    ff->flow_type = RQ_FLOATING_FLOW_TYPE_AVERAGE_RATE_PAYMENT;
    ff->flow_status = 0;
    ff->pay_receive = pay_receive;
    ff->payment_asset_id = RQ_STRDUP(payment_asset_id);
    ff->payment_date = payment_date;
    ff->accrual_start_date = accrual_start_date;
    ff->accrual_end_date = accrual_end_date;
    ff->notional_amount = notional_amount;
    ff->floating_rate->rate_group_id = rate_group_id;
    ff->floating_rate->rate_multiplier = 1.0;
}

/* Wrapper function - if only C had default args */
RQ_EXPORT short
rq_floating_flow_set_floating_rate_from_curve(
    rq_floating_flow_t ff,
    rq_date current_date,
    rq_yield_curve_t yc1
	)
{
    struct rq_rateset yc;
    rq_rateset_init(&yc);
    yc.yc1 = yc1;
    yc.current_date = current_date;
	return rq_floating_flow_set_floating_rate_from_curve_and_historic(ff, &yc, 0.0, 0);
}

RQ_EXPORT int rq_rateset_is_valid(rq_rateset_t r)
{
    return r && r->current_date;
}

RQ_EXPORT void rq_rateset_init(rq_rateset_t r)
{
    r->yc1 = r->yc2 = r->vs = r->reval_yc = NULL;
    r->yc_basket = NULL;
    r->spot = 1.0;
    r->current_date = 0;
    r->vol_override = 0.0;
}

RQ_EXPORT void rq_rateset_clear(rq_rateset_t r)
{
    if (r->yc_basket)
        rq_yield_curve_basket_list_free(r->yc_basket);

    r->yc_basket = NULL;
}

RQ_EXPORT rq_date rq_rateset_get_from_date(rq_rateset_t r)
{
    rq_date from_date;
    rq_yield_curve_basket_list_iterator itYC;
    if (r->yc_basket)
    {
        itYC = rq_yield_curve_basket_list_begin(r->yc_basket);
        from_date = (*itYC)->from_date;
    }
    else if(!r->yc1)
    {
        /* Fixed rate */
        from_date = r->current_date;
    }
    else if(r->yc2)
    {
        /* FX Fwd */
        from_date = r->yc2->from_date;
    }
    else
    {
        /* Floating rate */
        from_date = r->yc1->from_date;
    }
    return from_date;
}

RQ_EXPORT double
rq_rateset_get_forward_rate(rq_rateset_t yc, rq_date rate_start_date, rq_date rate_end_date, enum rq_day_count_convention day_count_convention)
{
    double rate;
    if(!yc->yc1)
    {
        /* Fixed rate */
        rate = yc->spot;
    }
    else if(yc->yc2)
    {
        /* FX fwd = spot * for / dom */
        rate = rq_yield_curve_get_forward_discount_factor(yc->yc1, rate_start_date, rate_end_date) /
               rq_yield_curve_get_forward_discount_factor(yc->yc2, rate_start_date, rate_end_date) *
               yc->spot;
    }
    else
    {
        /* Floating rate */
        rate = rq_yield_curve_get_forward_simple_rate(
            yc->yc1,
            rate_start_date,
            rate_end_date,
            day_count_convention
            );
    }
    return rate;
}

RQ_EXPORT double
rq_rateset_get_discount_factor(const rq_rateset_t yc, rq_date for_date)
{
    double rate;
    if(!yc->yc1)
    {
        // Unused for now. TODO : make ZC. */
        rate = yc->spot * (for_date - yc->current_date);
    }
    else if(yc->yc2)
    {
        rate = rq_yield_curve_get_discount_factor(yc->yc1, for_date) /
               rq_yield_curve_get_discount_factor(yc->yc2, for_date);
    }
    else
    {
        /* IR */
        rate = rq_yield_curve_get_discount_factor(yc->yc1, for_date);
    }
    return rate;
}

RQ_EXPORT double
rq_rateset_get_forward_discount_factor(const rq_rateset_t rs, rq_date start_date, rq_date end_date)
{
    /*
      <-- df1 -->
      <-- df -------------->
                 <-- df2 -->

         df = df1 * df2

      => df2 = df / df1
    */
    double df = rq_rateset_get_discount_factor(rs, end_date);
    double df1 = rq_rateset_get_discount_factor(rs, start_date);

    return (df1 != 0 ? df / df1 : df);
}

RQ_EXPORT double
rq_rateset_get_forward_par_rate_day_count(
    const rq_rateset_t rs,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    )
{
    double r = 0.0;

    if(!rs->yc1)
        return rs->spot;
    if (num_dates > 0)
    {
        double sum_df = 0.0;
        double year_count_frac = 1.0;
        double last_df = rq_rateset_get_forward_discount_factor(
                rs,
                from_date,
                dates[num_dates-1]
                );
        unsigned int i;
        for (i = 1; i < num_dates - 1; i++)
        {
            if (dates[i] > from_date)
            {
                double df = rq_rateset_get_forward_discount_factor(
                    rs,
                    from_date,
                    dates[i]
                    );
                year_count_frac = rq_day_count_get_year_fraction(day_count, dates[i-1], dates[i]);
                sum_df += df * year_count_frac;
            }
        }

        if (num_dates > 1)
            year_count_frac = rq_day_count_get_year_fraction(day_count, dates[num_dates-2], dates[num_dates-1]);

        sum_df += last_df * year_count_frac;

        r = (1.0 - last_df) / sum_df;
    }

    return r;
}

RQ_EXPORT double
rq_rateset_get_accrual_day_count(
    const rq_rateset_t rs,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    )
{
    double r = 0.0;
    double year_count_frac = 1.0;

    if (num_dates > 0)
    {
        double sum_df = 0.0;
        unsigned int i;
        for (i = 1; i < num_dates; i++)
        {
            double df = rq_rateset_get_forward_discount_factor(
                rs,
                from_date,
                dates[i]
                );
            year_count_frac = rq_day_count_get_year_fraction(day_count, dates[i-1], dates[i]);
            sum_df += df * year_count_frac;
        }

        r = sum_df;
    }

    return r;
}

RQ_EXPORT int
rq_floating_flow_has_status( rq_floating_flow_t ff, unsigned short flag )
{
    return ff->flow_status & flag;
}

/* historic_rate is the grossed up rate ( >= 1.0) from the start_date to the current date, value 0 if not available. */
RQ_EXPORT short
rq_floating_flow_set_floating_rate_from_curve_and_historic(
    rq_floating_flow_t ff,
    rq_rateset_t yc,
	double historic_rate,
    int resetFreq
    )
{
    short set = 0;
    rq_date ts_from_date = rq_rateset_get_from_date(yc);

	/* check if flow requires a rate setting and has not already been done */
    if ((ff->flow_type & RQ_FLOATING_FLOW_TYPE_IS_RATE_SET_MASK) != 0 &&
        (ff->flow_status & RQ_FLOATING_FLOW_STATUS_FLOATING_RATE_SET) == 0)
    {
        if (ff->floating_rate->fixing_date >= ts_from_date)
        {
            /* fixing in the future. */
            /* adjust our rate start date based on the difference
               between the fixing date and the rate_start_date on
               the flow.
            */
            rq_date rate_end_date = ff->floating_rate->rate_end_date;
            rq_date rate_start_date = ff->floating_rate->rate_start_date;
            if (resetFreq == 1 && rate_start_date != rate_end_date)
            {
                // OIS case
                double prodSum = (historic_rate != 0) ? historic_rate : 1.0;
                double spread = ff->floating_rate->fixing_spread;
                rq_date start_date = ff->accrual_start_date;
                rq_date end_date   = ff->accrual_end_date;
                double overallDcf = rq_day_count_get_year_fraction(ff->day_count_convention, start_date, end_date);
                double dcf;
                double rate;

                int tenor = rate_end_date - rate_start_date;

                if (start_date < ts_from_date)
                    start_date = ts_from_date;

                while (start_date < end_date)
                {
                    rq_date endDate = start_date + resetFreq;
                    if (endDate > end_date)
                    {
                        endDate = end_date;
                        resetFreq = endDate - start_date;
                    }

                    dcf = rq_day_count_get_year_fraction(ff->day_count_convention, 0, resetFreq);
                    rate = (1/rq_yield_curve_get_forward_discount_factor(yc->yc1, start_date, (start_date+tenor)) - 1) / dcf;
                    prodSum *= (1 + (rate + spread) * dcf);
                    start_date += resetFreq;
                }

                ff->rate = (prodSum - 1.0)/overallDcf;
            }
            else
            {
                // If rate is partially in the past and no historic_rate is available 
                // then we need to adjust dates otherwise the fwd rate will be way out.
                if(historic_rate == 0.0 && rate_start_date < ts_from_date)
                {
                    rate_end_date += (ts_from_date - rate_start_date);
                    rate_start_date = ts_from_date;
                }

                ff->rate = rq_rateset_get_forward_rate(
                    yc,
                    rate_start_date,
                    rate_end_date,
                    ff->day_count_convention
                    );

			    /* Incorporate the historic segment of the period. Note that rq_yield_curve_get_forward_simple_rate() returns a partial rate when there is a stub. */
			    if(historic_rate != 0.0)
			    {
				    /* historic_rate needs to be converted to an annual simple rate, apportioned to it's fraction of the flow term. */
				    double day_count_frac = rq_day_count_get_year_fraction(ff->day_count_convention, rate_start_date, rate_end_date);
				    if(day_count_frac > 0.0)
					    ff->rate = ff->rate + (historic_rate - 1.0) / day_count_frac;
			    }
            }

            ff->floating_rate->date_observed = ts_from_date;
            set = 1;

			if (ff->floating_rate->fixing_date == ts_from_date)
			{
	            /* boundary case indicate our rate is set */
    	        ff->flow_status |= RQ_FLOATING_FLOW_STATUS_FLOATING_RATE_SET;
			}
		}
        else if (ff->floating_rate->date_observed < ff->floating_rate->fixing_date)
        {
            /* OK, The rate was actually fixed between my last observed rate,
               and my current date. Interpolate the possible rate that this
               flow was fixed at.
            */
            /* adjust our rate start date based on the difference
               between the fixing date and the rate_start_date on
               the flow.
            */
            rq_date rate_start_date = ts_from_date + (ff->floating_rate->rate_start_date - ff->floating_rate->fixing_date);
            rq_date rate_end_date = ts_from_date + (ff->floating_rate->rate_end_date - ff->floating_rate->fixing_date);
            double rate = rq_rateset_get_forward_rate(
                yc,
                rate_start_date,
                rate_end_date,
                ff->day_count_convention
                );

            if (ff->floating_rate->date_observed != 0)
            {
                /* make sure we actually did a rate set last time! */
                ff->rate = rq_interpolate_linear(
                    (double)ff->floating_rate->fixing_date,
                    (double)ff->floating_rate->date_observed,
                    ff->rate, /* the last observed rate. */
                    (double)ts_from_date,
                    rate
                    );
                ff->floating_rate->date_observed = ff->floating_rate->fixing_date;
            }
            else // this is case where fixing should have occurred but has not been observerd at all so use historic if provided
            {
				if (historic_rate == 0.0)
					ff->rate = rate;
				else
					ff->rate = historic_rate;
                ff->floating_rate->date_observed = ff->floating_rate->fixing_date;
            }

            set = 1;

            /* indicate our rate is set */
            ff->flow_status |= RQ_FLOATING_FLOW_STATUS_FLOATING_RATE_SET;
        }
		/* if not set then retain the previous rate setting */
    }

    return set;
}

RQ_EXPORT void
rq_floating_flow_set_floating_rate(
    rq_floating_flow_t ff,
    double rate
    )
{
    ff->flow_status |= RQ_FLOATING_FLOW_STATUS_FLOATING_RATE_SET;
    ff->rate = rate;
}

RQ_EXPORT void
rq_floating_flow_set_cap_rate(
    rq_floating_flow_t ff,
    double rate
    )
{
    ff->flow_status |= RQ_FLOATING_FLOW_STATUS_CAP_RATE_SET;
    ff->floating_rate->cap_rate = rate;
}

RQ_EXPORT void
rq_floating_flow_set_floor_rate(
    rq_floating_flow_t ff,
    double rate
    )
{
    ff->flow_status |= RQ_FLOATING_FLOW_STATUS_FLOOR_RATE_SET;
    ff->floating_rate->floor_rate = rate;
}

RQ_EXPORT void
rq_floating_flow_set_spread(
    rq_floating_flow_t ff,
    double spread
    )
{
    ff->flow_status |= RQ_FLOATING_FLOW_STATUS_SPREAD_SET;
    ff->floating_rate->spread = spread;
}

RQ_EXPORT void
rq_floating_flow_set_fixing_spread(
    rq_floating_flow_t ff,
    double fixing_spread
    )
{
	ff->flow_status |= RQ_FLOATING_FLOW_STATUS_FIXING_SPREAD_SET;
    ff->floating_rate->fixing_spread = fixing_spread;
}

RQ_EXPORT double
rq_rateset_vol(rq_rateset_t yc, rq_date exercise_date, double strike, double tenor)
{
    double vol = 0.0;
    /* If not IR vol. */
	if(yc->vs)
    {
		if(yc->yc2)
		{
			rq_vol_surface_t vol_surface = (rq_vol_surface_t) yc->vs;
			if(!rq_vol_surface_is_strike_vol(vol_surface) || isnan(strike))
				strike = 0.50, /* 50 delta */
			rq_vol_surface_get_volatility(
				vol_surface,
				strike,
				exercise_date,
				&vol
				);
		}
		else
		{
			rq_ir_vol_surface_t vol_surface = (rq_ir_vol_surface_t) yc->vs;
			rq_ir_vol_surface_get_volatility(
				vol_surface,
				tenor,
				exercise_date,
				&vol
				);
		}
    }
    else
    {
        vol = yc->vol_override;
    }
    return vol;
}

/* Reference: CalyonNewRazorPricingModels.doc section 4.1 */
/* Method is to price a call option expiring on fixing_date. */
/* floor value = capped_rate + call_option. */
/* Note we are not calculating delivery. */
/* Return price is the forward price of the call (forward because its used for ratesetting). */
static double
rq_floating_flow_price_floor(rq_floating_flow_t ff, rq_rateset_t yc, double capped_rate)
{
    /* exercise date has no option value past the rate start. */
    rq_date exercise_date = min(ff->floating_rate->fixing_date, ff->floating_rate->rate_start_date);
    double strike = capped_rate - ff->floating_rate->spread;
    int reverse_floating = ff->floating_rate->rate_multiplier < 0;
    double reverse_floating_mult = reverse_floating ? -1.0 : 1.0;
    double df_dom, df_for;

    ff->floating_rate->vol_observed = rq_rateset_vol(yc, exercise_date, strike, rq_day_count_get_year_fraction(ff->day_count_convention, ff->floating_rate->rate_start_date, ff->floating_rate->rate_end_date));

    if(yc->yc2)
    {
        /* These are already the risk free rates. */
        df_dom = rq_yield_curve_get_forward_discount_factor(yc->yc2, yc->current_date, exercise_date);
        df_for = rq_yield_curve_get_forward_discount_factor(yc->yc1, yc->current_date, exercise_date);
	}
	else
    {
        /* Must use risk free rate, i.e. Reval curve */
        df_dom = rq_yield_curve_get_forward_discount_factor(yc->reval_yc, yc->current_date, exercise_date);
        df_for = 1.0;
    }

    /* Note garmankhol is grossed up to exercise date by (1 / df_dom). */
    return capped_rate + (1.0 / df_dom) * rq_pricing_garmankhol(
            !reverse_floating, /* if reverse_floating then Put else Call */
            reverse_floating_mult * ff->rate * df_dom / df_for * ff->floating_rate->rate_multiplier,  /* garman expects spot price. */
            reverse_floating_mult * strike,
            df_dom,
            df_for,
            ff->floating_rate->vol_observed,
            (exercise_date - yc->current_date) / 365.0,
            0.0 /* unused dummy arg*/ );
}

static double
rq_floating_flow_basket_price_floor(rq_floating_flow_t ff, rq_rateset_t yc, double capped_rate)
{
    rq_floating_flow_rate_list_iterator it = rq_floating_flow_rate_list_begin(ff->frBasket);
    rq_floating_flow_rate_t ffr = *it;
    rq_date exercise_date = min(ffr->fixing_date, ffr->rate_start_date);
    double vol = rq_rateset_vol(yc, exercise_date, capped_rate, rq_day_count_get_year_fraction(ff->day_count_convention, ffr->rate_start_date, ffr->rate_end_date));

    if (ffr->fixing_date <= yc->current_date || ff->rate <= 0)
    {
        return max(ff->rate, capped_rate);
    }

    if (capped_rate <= 0)
        return ff->rate;

    return ff->rate + rq_pricing_blackscholes_gen(
        0, 
        ff->rate, 
        capped_rate, 
        (ffr->fixing_date - yc->current_date)/365.0, 
        0, 
        0, 
        vol
        );
}

/* Apply multiplier, spread. */
RQ_EXPORT double
rq_floating_flow_get_allin_rate(rq_floating_flow_t ff)
{
    double rate = ff->rate;
    if (ff->floating_rate)
    {
        rate = (rate * ff->floating_rate->rate_multiplier) + ff->floating_rate->spread;
    }
    return rate;
}

/* Apply multiplier, spread, floor_rate, and cap_rate. */
RQ_EXPORT double
rq_floating_flow_get_effective_rate(rq_floating_flow_t ff, rq_rateset_t yc)
{
    double rate = ff->rate;
    if (ff->frBasket)
    {
        rq_floating_flow_rate_list_iterator it = rq_floating_flow_rate_list_begin(ff->frBasket);
        rq_floating_flow_rate_t ffr = *it;
        if ((ff->flow_status & RQ_FLOATING_FLOW_STATUS_FLOOR_RATE_SET) && yc)
        {
            rate = rq_floating_flow_basket_price_floor(ff, yc, ffr->floor_rate);
        }
    }
    else if (ff->floating_rate)
    {
        rate = (rate * ff->floating_rate->rate_multiplier) + ff->floating_rate->spread;
        if ((ff->flow_status & RQ_FLOATING_FLOW_STATUS_FLOOR_RATE_SET) && yc)
        {
            /* Floor = floor_rate + call at floor rate. */
            rate = rq_floating_flow_price_floor(ff, yc, ff->floating_rate->floor_rate);
        }

        if ((ff->flow_status & RQ_FLOATING_FLOW_STATUS_CAP_RATE_SET) && yc)
        {
            /* Cap = (plain rateset) - (call at cap rate). */
            /* Collar = (floor value) - (call at cap rate). */
            /* This is calculated using put-call parity C+K=P+S */
            /* Cap = K - P = S - C = S - Floor + K */
            rate -= (rq_floating_flow_price_floor(ff, yc, ff->floating_rate->cap_rate) - ff->floating_rate->cap_rate);
        }

    }
    return rate;
}

RQ_EXPORT void
rq_floating_flow_set_payment_amount(rq_floating_flow_t ff)
{
    rq_floating_flow_set_payment(ff, NULL);
}

RQ_EXPORT void
rq_floating_flow_set_payment(rq_floating_flow_t ff, rq_rateset_t yc)
{
    if (rq_floating_flow_has_interest_payment_characteristic(ff))
    {
        /* If it's an average rate payment, assume that the average rate
           has been calculated and put in the rate field.
        */
        if (ff->override_get_payment_func)
        {
            ff->payment_amount = (ff->override_get_payment_func)(ff, yc);
        }
        else
        {
            double rate;
            rate = rq_floating_flow_get_effective_rate(ff, yc);
            ff->payment_amount = rq_floating_flow_get_notional_fraction_calc(ff) * rate;
        }
	}
}

RQ_EXPORT short
rq_floating_flow_has_payment_characteristic(
    rq_floating_flow_t ff
    )
{
    return (ff->flow_type & RQ_FLOATING_FLOW_TYPE_IS_PAYMENT_MASK) != 0;
}

RQ_EXPORT short
rq_floating_flow_has_rate_set_characteristic(
    rq_floating_flow_t ff
    )
{
    return (ff->flow_type & RQ_FLOATING_FLOW_TYPE_IS_RATE_SET_MASK) != 0;
}

RQ_EXPORT rq_floating_flow_rate_list_iterator
rq_floating_flow_rate_list_begin(rq_floating_flow_rate_list_t ffl)
{
    return (rq_floating_flow_rate_list_iterator)(ffl->floating_flow_rate_list->array);
}

RQ_EXPORT rq_floating_flow_rate_list_iterator
rq_floating_flow_rate_list_end(rq_floating_flow_rate_list_t ffl)
{
    return (rq_floating_flow_rate_list_iterator)(ffl->floating_flow_rate_list->array + ffl->floating_flow_rate_list->size);
}

RQ_EXPORT rq_yield_curve_list_t rq_yield_curve_basket_list_alloc()
{
    rq_yield_curve_list_t yc_list = (rq_yield_curve_list_t)RQ_CALLOC(1, sizeof(struct rq_yield_curve_list));
    yc_list->yield_curve_list = rq_array_alloc(NULL);
    return yc_list;
}

RQ_EXPORT void rq_yield_curve_basket_list_free(rq_yield_curve_list_t yc_list)
{
    rq_array_free(yc_list->yield_curve_list);
    RQ_FREE(yc_list);
}

RQ_EXPORT void rq_yield_curve_basket_list_add(rq_yield_curve_list_t yc_list, rq_yield_curve_t yc)
{
    rq_array_push_back(yc_list->yield_curve_list, yc);
}

RQ_EXPORT rq_yield_curve_basket_list_iterator
rq_yield_curve_basket_list_begin(rq_yield_curve_list_t yc_list)
{
    return (rq_yield_curve_basket_list_iterator)(yc_list->yield_curve_list->array);
}

RQ_EXPORT rq_yield_curve_basket_list_iterator
rq_yield_curve_basket_list_end(rq_yield_curve_list_t yc_list)
{
    return (rq_yield_curve_basket_list_iterator)(yc_list->yield_curve_list->array + yc_list->yield_curve_list->size);
}

RQ_EXPORT int
rq_floating_flow_has_interest_payment_characteristic(
    rq_floating_flow_t ff
    )
{
	return (ff->flow_type == RQ_FLOATING_FLOW_TYPE_FLOATING_RATE_PAYMENT ||
			ff->flow_type == RQ_FLOATING_FLOW_TYPE_FIXED_RATE_PAYMENT ||
			ff->flow_type == RQ_FLOATING_FLOW_TYPE_AVERAGE_RATE_PAYMENT);
}

