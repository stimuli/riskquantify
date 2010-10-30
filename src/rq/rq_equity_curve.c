/*
** rq_equity_curve.c
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
#include "rq_equity_curve.h"
#include "rq_array.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>


static void
free_func(void *p)
{
    rq_dividend_free((rq_dividend_t)p);
}

RQ_EXPORT rq_equity_curve_t
rq_equity_curve_alloc()
{
    struct rq_equity_curve *ec = (struct rq_equity_curve *)RQ_CALLOC(1, sizeof(struct rq_equity_curve));
    ec->termstruct.termstruct_type = RQ_TERMSTRUCT_TYPE_EQUITY_CURVE;
    ec->termstruct.termstruct_id = NULL;
    ec->curve_date = 0L;
    ec->dividends = rq_array_alloc(free_func);
    ec->dividend_yield = 0.0;
    ec->stock_price = 0.0;
	ec->dividend_yield_function_current_size = 0;
	ec->dividend_yield_function_max_size = RQ_DIVIDEND_YIELD_FUNCTION_MAX_SIZE;
	ec->dividend_yield_function = (struct rq_dividend_yield_elem *)RQ_CALLOC((ec->dividend_yield_function_max_size), sizeof(struct rq_dividend_yield_elem));
	return ec;
}

RQ_EXPORT void  
rq_equity_curve_free(rq_equity_curve_t ec)
{
    rq_termstruct_clear(&ec->termstruct);
    rq_array_free(ec->dividends);
	RQ_FREE(ec->dividend_yield_function);
    RQ_FREE(ec);
}

RQ_EXPORT rq_equity_curve_t 
rq_equity_curve_clone(rq_equity_curve_t ec)
{
    struct rq_equity_curve *ec_clone = (struct rq_equity_curve *)RQ_CALLOC(1, sizeof(struct rq_equity_curve));
    unsigned int i;

    if (ec->termstruct.termstruct_id)
        ec_clone->termstruct.termstruct_id = 
            (const char *)RQ_STRDUP(ec->termstruct.termstruct_id);
    else
        ec_clone->termstruct.termstruct_id = NULL;

    if (ec->termstruct.underlying_asset_id)
        ec_clone->termstruct.underlying_asset_id = RQ_STRDUP(ec->termstruct.underlying_asset_id);

    ec_clone->curve_date = ec->curve_date;

    ec_clone->dividends = rq_array_alloc(free_func);
    ec_clone->dividend_yield = ec->dividend_yield;
    ec_clone->stock_price = ec->stock_price;

    for (i = 0; i < rq_array_size(ec->dividends); i++)
    {
        rq_dividend_t div; 
        rq_dividend_t div_clone;

        div = (rq_dividend_t)rq_array_get_at(ec->dividends, i);
        div_clone = rq_dividend_clone(div);

        rq_array_push_back(ec_clone->dividends, div_clone);
    }

	ec_clone->dividend_yield_function_current_size = ec->dividend_yield_function_current_size;
	ec_clone->dividend_yield_function_max_size = ec->dividend_yield_function_max_size;

	ec_clone->dividend_yield_function = (struct rq_dividend_yield_elem *)RQ_CALLOC(ec_clone->dividend_yield_function_max_size, sizeof(struct rq_dividend_yield_elem));
	if (ec_clone->dividend_yield_function != NULL)
	{
	    for (i = 0; i < ec->dividend_yield_function_current_size; i++)
		{
			ec_clone->dividend_yield_function[i].date = ec->dividend_yield_function[i].date;
	        ec_clone->dividend_yield_function[i].dividend_yield = ec->dividend_yield_function[i].dividend_yield;
		}
	}

	return ec_clone;
}

RQ_EXPORT rq_equity_curve_t
rq_equity_curve_build(const char *termstruct_id, rq_date curve_date)
{
	struct rq_equity_curve *ec = rq_equity_curve_alloc();

    ec->termstruct.termstruct_id = RQ_STRDUP(termstruct_id);
    ec->curve_date = curve_date;

    return ec;
}

RQ_EXPORT const char *
rq_equity_curve_get_underlying_asset_id(const rq_equity_curve_t yc)
{
    return yc->termstruct.underlying_asset_id;
}

RQ_EXPORT void
rq_equity_curve_set_underlying_asset_id(rq_equity_curve_t yc, const char *asset_id)
{
    rq_termstruct_set_underlying_asset_id(&yc->termstruct, asset_id);
}

RQ_EXPORT int
rq_equity_curve_is_null(rq_equity_curve_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT const char *
rq_equity_curve_get_termstruct_id(rq_equity_curve_t ec)
{
    return ec->termstruct.termstruct_id;
}

RQ_EXPORT void 
rq_equity_curve_set_termstruct_id(rq_equity_curve_t ec, const char *termstruct_id)
{
    if (ec->termstruct.termstruct_id)
       RQ_FREE((char *)ec->termstruct.termstruct_id);
    ec->termstruct.termstruct_id = (const char *)RQ_STRDUP(termstruct_id);
}

RQ_EXPORT void 
rq_equity_curve_set_curve_date(rq_equity_curve_t ec, rq_date curve_date)
{
    ec->curve_date = curve_date;
}

RQ_EXPORT rq_date 
rq_equity_curve_get_curve_date(rq_equity_curve_t ec)
{
    return ec->curve_date;
}

RQ_EXPORT void 
rq_equity_curve_set_dividend_yield(rq_equity_curve_t ec, double dividend_yield)
{
    ec->dividend_yield = dividend_yield;
}

RQ_EXPORT double 
rq_equity_curve_get_dividend_yield(rq_equity_curve_t ec)
{
    return ec->dividend_yield;
}

RQ_EXPORT unsigned 
rq_equity_curve_get_dividends_count(rq_equity_curve_t ec)
{
    return rq_array_size(ec->dividends);
}

RQ_EXPORT rq_dividend_t 
rq_equity_curve_get_dividends_at(rq_equity_curve_t ec, unsigned int offset)
{
    return (rq_dividend_t) rq_array_get_at(ec->dividends, offset);
}

RQ_EXPORT void 
rq_equity_curve_add_dividend(rq_equity_curve_t ec, rq_dividend_t dividend)
{
    rq_array_push_back(ec->dividends, dividend);
}

RQ_EXPORT double
rq_equity_curve_get_stock_price(rq_equity_curve_t ec)
{
    return ec->stock_price;
}

RQ_EXPORT void
rq_equity_curve_set_stock_price(rq_equity_curve_t ec, double stock_price)
{
    ec->stock_price = stock_price;
}

RQ_EXPORT double
rq_equity_curve_get_forward_stock_price(rq_equity_curve_t ec, rq_date date)
{
    double price = ec->stock_price;
    if (rq_array_size(ec->dividends) > 0)
    {
        unsigned int i;
        for (i = 0; i < rq_array_size(ec->dividends); i++)
        {
            rq_dividend_t div = rq_array_get_at(ec->dividends, i);
            if (rq_dividend_get_date(div) > ec->curve_date)
            {
                if (rq_dividend_get_date(div) > date)
                    break;

                price -= rq_dividend_get_value(div);
            }
        }
    }
    else if (ec->dividend_yield > 0.0)
    {
        /* adjust price by the dividend yield */
        /* (I'm assuming that dividend yield is continuously compounded) */
        double df = exp(-ec->dividend_yield * (double)(date - ec->curve_date) / 365.0);
        price *= df;
    }
    return price;
}

RQ_EXPORT double
rq_equity_curve_get_forward_stock_price_with_discrete_dividends(rq_equity_curve_t ec, rq_date effective_date, double FX, double zcdf_1, double zcdf_2, double total_dd, double df_bt, rq_date tN, double fx_eq_corr, double fx_vol, double eq_vol)
{
	double S = ec->stock_price;	
	double delta_t = (double) (effective_date - ec->curve_date) / 365.0;
	double delta_tN = (double) (tN - ec->curve_date) / 365.0;
	double Q1 = rq_equity_curve_get_forward_dividend_yield(ec, effective_date);
	double Q2 = rq_equity_curve_get_forward_dividend_yield(ec, tN);
	double q = (Q1 * delta_t - Q2 * delta_tN);
	double e_pow = (- fx_eq_corr * fx_vol * eq_vol)* (double) (effective_date - ec->curve_date) - q;
	double price = FX * (S * zcdf_2 / zcdf_1 - total_dd) * exp(e_pow) / (df_bt * zcdf_2);
	return price;
}

RQ_EXPORT double
rq_equity_curve_get_forward_stock_price_with_dividend_yield(rq_equity_curve_t ec, rq_date effective_date, double FX, double zcdf_1, double fx_eq_corr, double fx_vol, double eq_vol)
{
	double S = ec->stock_price;
	double e_pow = (-ec->dividend_yield - fx_eq_corr * fx_vol * eq_vol)  * (double) (effective_date - ec->curve_date) / 365.0;
	double price = FX * S * exp(e_pow) / zcdf_1;
	return price;
}

RQ_EXPORT int
rq_equity_curve_get_discrete_dividend_size(rq_equity_curve_t ec)
{
	return rq_array_size(ec->dividends);
}

RQ_EXPORT void 
rq_equity_curve_add_to_dividend_yield(rq_equity_curve_t ec, rq_date date, double dividend_yield)
{
	if (ec->dividend_yield_function_current_size == ec->dividend_yield_function_max_size)
	{
		/* need to grow size */
		unsigned int new_max_size = ec->dividend_yield_function_max_size * 2;
        ec->dividend_yield_function = 
            (struct rq_dividend_yield_elem *) realloc(
            ec->dividend_yield_function, 
            new_max_size * sizeof(struct rq_dividend_yield_elem)
            );
		ec->dividend_yield_function_max_size = new_max_size;
	}

	/* Funcition (insertion) is not sorted by date
       ToDo: Sort this function while inserting for faster retrival later
    */
	if (ec->dividend_yield_function != NULL)
	{		
		ec->dividend_yield_function[ec->dividend_yield_function_current_size].date = date;
		ec->dividend_yield_function[ec->dividend_yield_function_current_size].dividend_yield = dividend_yield;
		ec->dividend_yield_function_current_size ++;
	}
}

RQ_EXPORT void
rq_equity_curve_dividend_yield_curve_find_date_bounds(rq_equity_curve_t ec)
{	
	rq_date earlist_date = ec->dividend_yield_function[0].date;
	rq_date latest_date = ec->dividend_yield_function[0].date;
	int lower_bound = 0;
	int upper_bound = 0;
	int i;
	for (i = 0; i < ec->dividend_yield_function_current_size; i ++)
	{
		if (ec->dividend_yield_function[i].date < earlist_date)
		{
			earlist_date = ec->dividend_yield_function[i].date;
			lower_bound = i;
		}
		else if (ec->dividend_yield_function[i].date > latest_date)
		{
			latest_date = ec->dividend_yield_function[i].date;
			upper_bound = i;
		}
	}
	ec->lower_bound = lower_bound;
	ec->upper_bound = upper_bound;
}

RQ_EXPORT double
rq_equity_curve_get_forward_dividend_yield(rq_equity_curve_t ec, rq_date forDate)
{
	int just_before = ec->upper_bound; /* set to the latest element */
	int just_after = ec->lower_bound; /* set to the earliest element */
	int i;
	double forwardDY;
	for (i = 0; i < ec->dividend_yield_function_current_size; i++)
	{
		if (ec->dividend_yield_function[i].date == forDate)
		{
			/* find the exact date */
			return ec->dividend_yield_function[i].dividend_yield;
		}
		else
		{
			if ((ec->dividend_yield_function[i].date < forDate) && 
				(((ec->dividend_yield_function[i].date > ec->dividend_yield_function[just_before].date) && (just_before != ec->upper_bound)) ||
				(just_before == ec->upper_bound)))				
			{
				/* found a closer just_before */
				just_before = i;
			}
			else if ((ec->dividend_yield_function[i].date > forDate) &&
					(((ec->dividend_yield_function[i].date < ec->dividend_yield_function[just_after].date) && (just_after != ec->lower_bound)) ||
					(just_after == ec->lower_bound)))
			{
				/* found a close just_after */
				just_after = i;
			}
		}
	}
	
	if (just_before == ec->upper_bound)
	{
		/* forDate is later than the latest date on curve : need to extrapolate
           ToDo: implement different extrapolation methods. For now just return the last dividend yield 
        */
		forwardDY = ec->dividend_yield_function[ec->upper_bound].dividend_yield;
	}
	else if (just_after == ec->lower_bound)
	{
		/* forDate is earlier than the earliest date on curve : need to extrapolate
           ToDo: implement different extrapolation methods. For now just return the first dividend yield
        */
		forwardDY = ec->dividend_yield_function[ec->lower_bound].dividend_yield;
	}
	else
	{
		/* interpolate between just_before and just_after of forDate */
		forwardDY = rq_interpolate_linear(
					(double)forDate,
					(double)ec->dividend_yield_function[just_before].date,
					ec->dividend_yield_function[just_before].dividend_yield,
					(double)ec->dividend_yield_function[just_after].date,
					ec->dividend_yield_function[just_after].dividend_yield
					);
	}
	return forwardDY;
}
