/**
 * \file rq_floating_flow.h
 * \author Brett Hutley
 *
 * \brief The rq_floating_flow class represents the payment of am asset
 * based on a floating rate of interest.
 */
/*
** rq_floating_flow.h
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
#ifndef rq_floating_flow_h
#define rq_floating_flow_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_yield_curve.h"
#include "rq_array.h"
#include "rq_pricing_request.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
/* Define a workflow for this floating flow.
   We can do a rate set using a termstruct_id, a observation date, etc.
   We can then make an interest payment using this rate.
   We can apply a spread on the interest rate.
   We can cap or floor the interest rate.

   The workflows can operate on the rates, can operate on the notional
   amount, or can operate on the actual payment.

enum rq_floating_flow_operation {
    RQ_FLOATING_FLOW_OPERATION_FLOATING_RATE_SET,
    RQ_FLOATING_FLOW_OPERATION_SET_RATE,
    RQ_FLOATING_FLOW_OPERATION_CALC_INTEREST,
    RQ_FLOATING_FLOW_OPERATION_APPLY_FORMULA,
    RQ_FLOATING_FLOW_OPERATION_AVERAGE_RATE,
    RQ_FLOATING_FLOW_MAKE_PAYMENT
};

 */

enum rq_floating_flow_type {
    RQ_FLOATING_FLOW_TYPE_UNKNOWN = 0, /**< Unknown floating flow type. */
    RQ_FLOATING_FLOW_TYPE_FLOATING_RATE_PAYMENT = 0x0111, /**< Make a floating rate payment. */
    RQ_FLOATING_FLOW_TYPE_FIXED_RATE_PAYMENT = 0x0012, /**< Make a fixed rate payment. */
    RQ_FLOATING_FLOW_TYPE_STATIC_PAYMENT = 0x0014, /**< Make a payment based on the asset_id and payment amount. */
    RQ_FLOATING_FLOW_TYPE_INTEREST_REINVESTMENT = 0x0108, /**< Make a notional payment. Assume the payment is reinvested. */
    RQ_FLOATING_FLOW_TYPE_RATE_SET = 0x0100, /**< Set the rate only. */
    RQ_FLOATING_FLOW_TYPE_AVERAGE_RATE_PAYMENT = 0x0018 /**< Compute and pay an average payment, based on the rate_group_id. */
};

RQ_EXPORT const char *rq_floating_flow_type_to_string(enum rq_floating_flow_type flowtype);

#define RQ_FLOATING_FLOW_TYPE_IS_RATE_SET_MASK 0x0100
#define RQ_FLOATING_FLOW_TYPE_IS_PAYMENT_MASK 0x0010

/* Various status flags */
#define RQ_FLOATING_FLOW_STATUS_FLOATING_RATE_SET 0x0001
#define RQ_FLOATING_FLOW_STATUS_SPREAD_SET 0x0002
#define RQ_FLOATING_FLOW_STATUS_CAP_RATE_SET 0x0004
#define RQ_FLOATING_FLOW_STATUS_FLOOR_RATE_SET 0x0008
#define RQ_FLOATING_FLOW_STATUS_FIXING_SPREAD_SET 0x0010

typedef struct rq_floating_flow_cms {
    rq_term_t cm_term;
    rq_term_t cm_freq;
} *rq_floating_flow_cms_t;

typedef struct rq_yield_curve_list {
    rq_array_t yield_curve_list;
} *rq_yield_curve_list_t;

typedef struct rq_yield_curve** rq_yield_curve_basket_list_iterator;

/* Would nicer as a Class but rq is a problem. */
/* Curves required for ratesetting. They could be IR or FX, and may include volatility. */
typedef struct rq_rateset {
    rq_yield_curve_t yc1;       /* Primary curve, reference rate if IR, foreign rate if FX */
    rq_yield_curve_t yc2;       /* Second curve, NULL if IR, domestic rate if FX */
    rq_yield_curve_list_t yc_basket; /* Primary curve for basket, if this is set then NULL for yc1 & yc2 */
    double spot;                /* spot rate, 1.0 if IR */
    void *vs;                   /* rq_ir_vol_surface_t or rq_vol_surface_t */
    double vol_override;        /* trade extension override of the vol surface. */
    rq_yield_curve_t reval_yc;  /* Riskfree rate, only used for IR option valuation. */
    rq_date current_date;       /* Valuation date */
} *rq_rateset_t;

typedef struct rq_floating_flow_rate {
    const char *termstruct_id;
    const char* currency;   /* ccy of the termstruct_id. set for basket, and 0 otherwise. to be extended to vanilla swap */
    rq_date fixing_date;
    rq_date rate_start_date;
    rq_date rate_end_date;
    double rate;
    double spread;
    double fixing_spread;
    double cap_rate;
    double floor_rate;
    unsigned rate_group_id; /**< This holds the group ID used for average rate sets, etc */
    rq_date date_observed; /**< This holds the date the rate was observed. */
    double rate_multiplier;
    double vol_observed; /* Holds the vol used to price the cap/floor. */
    rq_floating_flow_cms_t cms_data; /* Constant Maturity information */
} *rq_floating_flow_rate_t;

typedef struct rq_floating_flow_rate_list {
    rq_array_t floating_flow_rate_list;
} *rq_floating_flow_rate_list_t;

typedef struct rq_floating_flow_rate** rq_floating_flow_rate_list_iterator;

typedef struct rq_floating_flow *rq_floating_flow_t;
struct rq_floating_flow {
    enum rq_floating_flow_type flow_type;
    unsigned short flow_status;
    enum rq_pay_receive pay_receive;
    rq_date payment_date;
    double notional_amount;
    const char *payment_asset_id;
    double rate; /**< This holds the last observed rate, or the fixed rate. */
    unsigned notional_exchange_id; /**< If non-zero, there is a notional exchange with other flows with the same notional exchange id. */
    enum rq_day_count_convention day_count_convention;
    rq_date accrual_start_date;
    rq_date accrual_end_date;
    double payment_amount;
	double year_fraction_calc;
	double notional_fraction_calc;

	rq_floating_flow_rate_t floating_rate;
    rq_floating_flow_rate_list_t frBasket;  /* IR basket */
    double (*override_get_payment_func)(rq_floating_flow_t, rq_rateset_t);
    short (*override_set_rate_func)(rq_floating_flow_t, rq_date marketDate, rq_rateset_t, const char* historicalRateClass, const struct rq_pricing_request *);
};

/* -- macros -------------------------------------------------- */
#define rq_floating_flow_get_year_fraction_calc(ff) ff->year_fraction_calc
#define rq_floating_flow_get_notional_fraction_calc(ff) ff->notional_fraction_calc

/* -- prototypes -------------------------------------------------- */
/** Test whether the rateset has been setup */
RQ_EXPORT int rq_rateset_is_valid(rq_rateset_t r);

RQ_EXPORT void rq_rateset_init(rq_rateset_t r);
RQ_EXPORT void rq_rateset_clear(rq_rateset_t r);

RQ_EXPORT double rq_rateset_get_forward_par_rate_day_count(
    const rq_rateset_t rs,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    );

/* returns actual start/from date from the term structure instead of current_date */
RQ_EXPORT rq_date rq_rateset_get_from_date(rq_rateset_t r);

RQ_EXPORT double
rq_rateset_get_accrual_day_count(
    const rq_rateset_t rs,
    rq_date from_date,
    unsigned int num_dates,
    rq_date *dates,
    enum rq_day_count_convention day_count
    );

/** Test whether the rq_floating_flow is NULL */
RQ_EXPORT int rq_floating_flow_is_null(rq_floating_flow_t obj);

/** Allocate a new floating flow.
 */
RQ_EXPORT rq_floating_flow_t rq_floating_flow_alloc();

/** Free a previously allocated floating flow.
 */
RQ_EXPORT void rq_floating_flow_free(rq_floating_flow_t aflow);

/** Clone a floating flow.
 */
RQ_EXPORT rq_floating_flow_cms_t    rq_floating_flow_rate_cms_clone(rq_floating_flow_cms_t ffrCms);
RQ_EXPORT rq_floating_flow_rate_t   rq_floating_flow_rate_clone(rq_floating_flow_rate_t ffr);
RQ_EXPORT rq_floating_flow_t        rq_floating_flow_clone(rq_floating_flow_t ff);


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
    );

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
    );

RQ_EXPORT void rq_floating_flow_rate_free(rq_floating_flow_rate_t ff);
RQ_EXPORT rq_floating_flow_rate_list_t rq_floating_flow_rate_list_alloc();
RQ_EXPORT void rq_floating_flow_rate_list_free(rq_floating_flow_rate_list_t ffl);
RQ_EXPORT void rq_floating_flow_rate_list_add(rq_floating_flow_rate_list_t ffl, rq_floating_flow_rate_t f);
RQ_EXPORT unsigned long rq_floating_flow_rate_list_size(rq_floating_flow_rate_list_t ffl);
RQ_EXPORT rq_floating_flow_rate_list_iterator rq_floating_flow_rate_list_begin(rq_floating_flow_rate_list_t ffl);
RQ_EXPORT rq_floating_flow_rate_list_iterator rq_floating_flow_rate_list_end(rq_floating_flow_rate_list_t ffl);

/* rq_yield_curve_t */
RQ_EXPORT rq_yield_curve_list_t rq_yield_curve_basket_list_alloc();
RQ_EXPORT void rq_yield_curve_basket_list_free(rq_yield_curve_list_t yc_list);
RQ_EXPORT void rq_yield_curve_basket_list_add(rq_yield_curve_list_t yc_list, rq_yield_curve_t yc);
RQ_EXPORT rq_yield_curve_basket_list_iterator rq_yield_curve_basket_list_begin(rq_yield_curve_list_t yc_list);
RQ_EXPORT rq_yield_curve_basket_list_iterator rq_yield_curve_basket_list_end(rq_yield_curve_list_t yc_list);

/** Set up this floating flow as a floating rate payment.
 */
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
    double rate_multipler
    );

/** Set up this floating flow as a fixed rate payment.
 */
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
    );

/** Set up this floating flow as an average rate payment.
 */
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
    );

/** Set up this floating flow as a static payment.
 */
RQ_EXPORT void 
rq_floating_flow_set_type_static_payment(
    rq_floating_flow_t ff,
    enum rq_pay_receive pay_receive,
    const char *payment_asset_id,
    rq_date payment_date,
    double payment_amount
    );

/** Set up this floating flow as a floating rate set.
 */
RQ_EXPORT void 
rq_floating_flow_set_type_rate_set(
    rq_floating_flow_t ff,
    const char *termstruct_id,
    rq_date fixing_date,
    rq_date rate_start_date,
    rq_date rate_end_date,
    unsigned rate_group_id
    );

/* -- helper functions -- */
/** Update the floating_flow->rate member and floating_flow->date_observed
 * with rates taken from the supplied yield curve.
 */
RQ_EXPORT short
rq_floating_flow_set_floating_rate_from_curve(
    rq_floating_flow_t ff,
    rq_date current_date,
    rq_yield_curve_t yc
    );

RQ_EXPORT short
rq_floating_flow_set_floating_rate_from_curve_and_historic(
    rq_floating_flow_t ff,
    rq_rateset_t yc,
    double historic_rate,
    int resetFreq
    );

/** Set the floating_flow->rate member directly and set the
 * flow_status to indicate that the floating rate has been set.
 */
RQ_EXPORT void
rq_floating_flow_set_floating_rate(
    rq_floating_flow_t ff,
    double rate
    );

/** Set the floating_flow->cap_rate and set the flow_status to
 * indicate that the cap rate has been set.
 */
RQ_EXPORT void
rq_floating_flow_set_cap_rate(
    rq_floating_flow_t ff,
    double rate
    );

/** Set the floating_flow->floor_rate and set the flow_status to
 * indicate that the floor rate has been set.
 */
RQ_EXPORT void
rq_floating_flow_set_floor_rate(
    rq_floating_flow_t ff,
    double rate
    );

/** Set the floating_flow->floor_rate and set the flow_status to
 * indicate that the floor rate has been set.
 */
RQ_EXPORT void
rq_floating_flow_set_spread(
    rq_floating_flow_t ff,
    double rate
    );

RQ_EXPORT void
rq_floating_flow_set_fixing_spread(
    rq_floating_flow_t ff,
    double rate
    );

RQ_EXPORT int
rq_floating_flow_has_status( rq_floating_flow_t ff, unsigned short flag );

/* Return rate with all affects applied: multiplier, spread, floor_rate, and cap_rate. */
RQ_EXPORT double
rq_floating_flow_get_allin_rate( rq_floating_flow_t ff );

/* Apply multiplier, spread, floor_rate, and cap_rate. */
RQ_EXPORT double
rq_floating_flow_get_effective_rate(rq_floating_flow_t ff, rq_rateset_t yc);

/** Calculate the payment amount from the rate field if this flow
 * has payment characteristics and isn't a static payment type.
 */
RQ_EXPORT void
rq_floating_flow_set_payment_amount(rq_floating_flow_t ff);

RQ_EXPORT void
rq_floating_flow_set_payment(rq_floating_flow_t ff, rq_rateset_t yc);

/** Test whether the payment details are available.
 * @return non-zero if has payment characteristics.
 */
RQ_EXPORT short
rq_floating_flow_has_payment_characteristic(
    rq_floating_flow_t ff
    );

/** Test whether this floating flow wants to rate set.
 */
RQ_EXPORT short
rq_floating_flow_has_rate_set_characteristic(
    rq_floating_flow_t ff
    );


RQ_EXPORT double
rq_rateset_get_forward_rate(rq_rateset_t yc, rq_date rate_start_date, rq_date rate_end_date, enum rq_day_count_convention day_count_convention);

RQ_EXPORT double
rq_rateset_vol(rq_rateset_t yc, rq_date exercise_date, double strike, double tenor);

/** Test whether this floating flow is an interest payment.
 */
RQ_EXPORT int
rq_floating_flow_has_interest_payment_characteristic(
    rq_floating_flow_t ff
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
