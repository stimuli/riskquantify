/**
 * \file rq_equity_curve.h
 * \author Brett Hutley
 *
 * \brief Represents an equity curve with a current stock price,
 * discrete dividend payment points and an overall yield
 */
/*
** rq_equity_curve.h
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
#ifndef rq_equity_curve_h
#define rq_equity_curve_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_dividend.h"
#include "rq_array.h"
#include "rq_termstruct.h"
#include "rq_interpolate.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private equity_curve structure.
 */
typedef struct rq_equity_curve {
    struct rq_termstruct termstruct;
    rq_date curve_date;
    rq_array_t dividends;
    double dividend_yield; /**< this is the annualised dividend yield */
    double stock_price;
	unsigned int dividend_yield_function_current_size; /**< current function size */
	unsigned int dividend_yield_function_max_size; /**< max function size before this structure needs to grow */
	unsigned int upper_bound; /**< index of dy curve element containing the latest payment date */
	unsigned int lower_bound; /**< index of dy curve element containing the earliest payment date */
	struct rq_dividend_yield_elem *dividend_yield_function;
} *rq_equity_curve_t;

/* -- structures -------------------------------------------------- */
/**
 * This structure defines an individual term structure element.
 */
struct rq_dividend_yield_elem {
    rq_date date;
    double dividend_yield; /**< this is the user-defined dividend yield at time=date */
};


/** Test whether the rq_equity_curve is NULL */
RQ_EXPORT int rq_equity_curve_is_null(rq_equity_curve_t obj);

/** Allocate a new equity_curve
 */
RQ_EXPORT rq_equity_curve_t rq_equity_curve_alloc();

/** Allocate a new equity_curve and assign the curve ID and equity date.
 */
RQ_EXPORT rq_equity_curve_t rq_equity_curve_build(const char *curve_id, rq_date curve_date);

/** Free an object of type rq_equity_curve.
 */
RQ_EXPORT void rq_equity_curve_free(rq_equity_curve_t equity_curve);

/** Clone a rq_equity_curve object.
 */
RQ_EXPORT rq_equity_curve_t rq_equity_curve_clone(rq_equity_curve_t equity_curve);

/** Get the termstruct id
 */
RQ_EXPORT const char *rq_equity_curve_get_termstruct_id(rq_equity_curve_t equity_curve);

/** Get the underlying asset ID associated with this equity curve.
 */
RQ_EXPORT const char *
rq_equity_curve_get_underlying_asset_id(const rq_equity_curve_t yc);

/** Set the underlying asset ID associated with this equity curve.
 */
RQ_EXPORT void
rq_equity_curve_set_underlying_asset_id(rq_equity_curve_t yc, const char *asset_id);

/** Set the termstruct id
 */
RQ_EXPORT void rq_equity_curve_set_termstruct_id(rq_equity_curve_t equity_curve, const char *termstruct_id);

/** Set the curve date.
 */
RQ_EXPORT void rq_equity_curve_set_curve_date(rq_equity_curve_t equity_curve, rq_date curve_date);

/** Get the curve date.
 */
RQ_EXPORT rq_date rq_equity_curve_get_curve_date(rq_equity_curve_t equity_curve);

/** Set the dividend yield.
 */
RQ_EXPORT void rq_equity_curve_set_dividend_yield(rq_equity_curve_t equity_curve, double dividend_yield);

/** Get the dividend yield.
 */
RQ_EXPORT double rq_equity_curve_get_dividend_yield(rq_equity_curve_t equity_curve);

/** Get the number of dividends.
 */
RQ_EXPORT unsigned rq_equity_curve_get_dividends_count(rq_equity_curve_t equity_curve);

/** Get the dividend at a particular zero-based offset.
 */
RQ_EXPORT rq_dividend_t rq_equity_curve_get_dividends_at(rq_equity_curve_t equity_curve, unsigned int offset);

/** Add a dividend.
 */
RQ_EXPORT void rq_equity_curve_add_dividend(rq_equity_curve_t equity_curve, rq_dividend_t dividend);

/** Get the current stock price.
 */
RQ_EXPORT double
rq_equity_curve_get_stock_price(rq_equity_curve_t equity_curve);

/** Set the current stock price.
 */
RQ_EXPORT void
rq_equity_curve_set_stock_price(rq_equity_curve_t ec, double stock_price);

/** Get the forward stock price after dividend adjustments (Discounted).
*/
RQ_EXPORT double
rq_equity_curve_get_forward_stock_price(rq_equity_curve_t ec, rq_date date);

/** Get the forward stock price for discrete dividend case (for quanto and DVFEF only)
*/
RQ_EXPORT double
rq_equity_curve_get_forward_stock_price_with_discrete_dividends(rq_equity_curve_t ec, rq_date effective_date, double FX, double zcdf_1, double zcdf_2, double total_dd, double Bt, rq_date tN, double fx_eq_corr, double fx_vol, double eq_vol);

/** Get the forward stock price for dividend yield case (for quanto and DVFEF only)
*/
RQ_EXPORT double
rq_equity_curve_get_forward_stock_price_with_dividend_yield(rq_equity_curve_t ec, rq_date effective_date, double FX, double zcdf_1, double fx_eq_corr, double fx_vol, double eq_vol);

/** Get the number of discrete dividend payments
*/
RQ_EXPORT int
rq_equity_curve_get_discrete_dividend_size(rq_equity_curve_t ec);

/** Add one element to dividend yield curve
*/
RQ_EXPORT void 
rq_equity_curve_add_to_dividend_yield(rq_equity_curve_t ec, rq_date date, double dividend_yield);

/** Find the indices of the elements in the dividend yield curve with the earliest and latest payment dates
*/
RQ_EXPORT void
rq_equity_curve_dividend_yield_curve_find_date_bounds(rq_equity_curve_t ec);

/** Get the forward dividend yield value at forDate
*/
RQ_EXPORT double
rq_equity_curve_get_forward_dividend_yield(rq_equity_curve_t ec, rq_date forDate);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
