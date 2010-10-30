/**
 * \file rq_enum.h
 * \author Brett Hutley
 *
 * \brief rq_enum.h provides helpful enumerations.
 */
/*
** rq_enum.h
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
#ifndef rq_enum_h
#define rq_enum_h

#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/**
 * Term Structure type enumeration: Note that if you add another element to
 * this enumeration, move down RQ_TERMSTRUCT_TYPE_MAX_ENUM (used in 
 * rq_bootstrap_config_mgr). 
 * @note These enums are used as array offsets!
 */
enum rq_termstruct_type {
    RQ_TERMSTRUCT_TYPE_INVALID = -1,
    RQ_TERMSTRUCT_TYPE_YIELD_CURVE = 0,
    RQ_TERMSTRUCT_TYPE_FORWARD_CURVE = 1,
    RQ_TERMSTRUCT_TYPE_VOL_SURFACE = 2,
    RQ_TERMSTRUCT_TYPE_SPREAD_CURVE = 3,
    RQ_TERMSTRUCT_TYPE_EQUITY_CURVE = 4,
    RQ_TERMSTRUCT_TYPE_EXTERNAL = 5,
    RQ_TERMSTRUCT_TYPE_IR_VOL_SURFACE = 6,
	RQ_TERMSTRUCT_TYPE_CDS_SPREAD_CURVE = 7,
    RQ_TERMSTRUCT_TYPE_FUTURE_CURVE = 8,
    RQ_TERMSTRUCT_TYPE_ELECTRICITY_CURVE = 9,
    /* IF YOU ADD NEW TERMSTRUCT_TYPES, REMEMBER TO ADD THE STRING TO
     rq_enum.c */
 
    RQ_TERMSTRUCT_TYPE_MAX_ENUM = 10 /**< The number of elements in the enum */
};

RQ_EXPORT enum rq_termstruct_type rq_termstruct_type_from_string(const char *str);

/** Convert a termstructure type enum to a string. */
RQ_EXPORT const char *rq_termstruct_type_to_string(enum rq_termstruct_type type);

enum rq_termstruct_structure {
    RQ_TERMSTRUCT_STRUCTURE_CURVE,
    RQ_TERMSTRUCT_STRUCTURE_SURFACE
};

/*! Interpolation method to use between curve points. */
enum rq_interpolation_method {
    RQ_INTERPOLATION_LINEAR_DISCOUNT_FACTOR = 0,
    RQ_INTERPOLATION_LINEAR_ZERO = 1,
    RQ_INTERPOLATION_LOG_LINEAR_DISCOUNT_FACTOR = 2,
    RQ_INTERPOLATION_LOG_LINEAR_ZERO = 3,
    RQ_INTERPOLATION_SPLINE_DISCOUNT_FACTOR = 4,
	RQ_INTERPOLATION_NONE = 100,
    RQ_INTERPOLATION_INVALID = -1
};

/*! extrapolation method to use at either start or end of a curve */
enum rq_extrapolation_method {
    RQ_EXTRAPOLATION_LAST_ZERO = 0, /**< Assume a flat zero rate */
    RQ_EXTRAPOLATION_LINEAR_ZERO = 1, /**< Imply a zero rate in a linear fashion */
    RQ_EXTRAPOLATION_LINEAR_DISCOUNT_FACTOR = 2, /**< Imply a discount factor in a linear fashion */
    RQ_EXTRAPOLATION_LINEAR_ZERO_ZERO = 3, /**< Imply a zero rate in a linear fashion, the zero rate at curve start is assumed to be zero (primarily for spread curves). The only time there is a difference between this and LINEAR_ZERO is on curve start. */
    RQ_EXTRAPOLATION_INVALID = -1
};

/*! Definition of zero rate */
enum rq_zero_method {
    RQ_ZERO_SIMPLE = 1, 
    RQ_ZERO_COMPOUNDING = 2,
    RQ_ZERO_CONTINUOUS_COMPOUNDING = 3,
    RQ_ZERO_CASH_DEPOSIT = 4, // default
    RQ_ZERO_INVALID = -1
};

// Definition of Bill Future bootstrapping
enum rq_billFuture_method {
    RQ_BILL_FUTURE_STRIP = 0, // Classical method (default)
    RQ_BILL_FUTURE_TENOR = 1, // Use tenor instead of searching for following future.
    RQ_BILL_FUTURE_INVALID = -1
};

// Definition of Swap par rate interpolation bootstrapping
// Note this is distinct from cubic spline DF interpolation.
enum rq_swapParInterpolation_method {
    RQ_SWAP_PAR_INTERPOLATION_LINEAR = 0, // Classical method (default)
    RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE = 1,
    RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE_SWAPS_ONLY = 2,
    RQ_SWAP_PAR_INTERPOLATION_INVALID = -1
};

// Definition of Swap day count method bootstrapping
// Note this is distinct from cubic spline DF interpolation.
enum rq_swapDayCount_method {
    RQ_SWAP_DAY_COUNT_SIMPLE = 0, // Classical method (default) uses fixed swap period
    RQ_SWAP_DAY_COUNT_DAYCOUNT = 1, // daycount is computed from swap asset daycount convention.
    RQ_SWAP_DAY_COUNT_INVALID = -1
};

enum rq_ccypair_quote_convention {
    RQ_CCYPAIR_QUOTE_CONVENTION_1PER2 = 1,
    RQ_CCYPAIR_QUOTE_CONVENTION_2PER1 = 2
};

enum rq_cds_quote_convention {
	RQ_CDS_QUOTE_CONVENTION_INVALID = -1,
    RQ_CDS_QUOTE_CONVENTION_SURVIVAL = 0,
	RQ_CDS_QUOTE_CONVENTION_DEFAULT = 1,
	RQ_CDS_QUOTE_CONVENTION_CDS_SPREAD = 2
};

RQ_EXPORT enum rq_cds_quote_convention rq_cds_quote_convention_from_string(const char *str);
RQ_EXPORT const char *rq_cds_quote_convention_to_string(enum rq_cds_quote_convention convention);

enum rq_date_format {
    RQ_DATE_FORMAT_DMY = 1,
    RQ_DATE_FORMAT_MDY = 2,
    RQ_DATE_FORMAT_YMD = 3
};

enum rq_date_roll_convention {
	RQ_DATE_ROLL_NO_ADJUSTMENT = 1,	/**< No Adjustment */
	RQ_DATE_ROLL_MOD_FOLLOWING = 2,	/**< Modified Following */
	RQ_DATE_ROLL_MOD_PRECEDING = 3,	/**< Modified Preceding */
	RQ_DATE_ROLL_FOLLOWING = 4,	/**< Following */
	RQ_DATE_ROLL_PRECEDING = 5	/**< Preceding */
};

RQ_EXPORT enum rq_date_roll_convention rq_date_roll_convention_from_string(const char *str);
RQ_EXPORT const char* rq_date_roll_convention_to_string(enum rq_date_roll_convention date_roll_convention);

/*! Day Count Conventions */
enum rq_day_count_convention {
    RQ_DAY_COUNT_INVALID = -1, /**< An invalid day count. */
    RQ_DAY_COUNT_ACTUAL_360 = 1, /**< Actual / 360 */
    RQ_DAY_COUNT_ACTUAL_365 = 2, /**< Actual / 365 */
    RQ_DAY_COUNT_30_360 = 3, /**< 30/360 Assumes ISDA 2006 Definition */
    RQ_DAY_COUNT_E30_360 = 4, /**< 30E/360 */
    RQ_DAY_COUNT_ACTUAL_ACTUAL = 5, /**< Actual / Actual */
    RQ_DAY_COUNT_ACTUAL_365_25 = 6, /**< Actual / 365.25 */
    RQ_DAY_COUNT_ACTUAL_365_FIXED = 7, /**< Actual / 365 Fixed */
    RQ_DAY_COUNT_ACTUAL_ACTUAL_ISDA = 8, /**< Actual / 365 ISDA */
    RQ_DAY_COUNT_ACTUAL_ACTUAL_ISMA = 9, /**< Actual / 365 ISMA */
    RQ_DAY_COUNT_ACTUAL_ACTUAL_AFB = 10, /**< Actual / 365 AFB */
    RQ_DAY_COUNT_ACTUAL_365_END = 11, /**< Actual / 365 Fixed */
    RQ_DAY_COUNT_ACTUAL_1_1 = 12, /**< 1 / 1 */
    RQ_DAY_COUNT_ACTUAL_365_ACTUAL = 13, /**< Actual / 365(Actual) */
    RQ_DAY_COUNT_BUS_252 = 14 /**< bus days / 252 for Brazilian Real */
};

RQ_EXPORT enum rq_day_count_convention rq_day_count_convention_from_string(const char *str);

RQ_EXPORT const char *rq_day_count_convention_to_string(enum rq_day_count_convention convention);

enum rq_date_roll_stub_position {
    RQ_DATE_ROLL_STUB_POSITION_NONE = 0, /**< No stub on date roll generation */
    RQ_DATE_ROLL_STUB_POSITION_END = 1, /**< stub at end on date roll generation */
    RQ_DATE_ROLL_STUB_POSITION_START = 2 /**< stub at start on date roll generation */
};

enum rq_day_of_week {
    RQ_DAY_OF_WEEK_SUNDAY = 0,
    RQ_DAY_OF_WEEK_MONDAY = 1,
    RQ_DAY_OF_WEEK_TUESDAY = 2,
    RQ_DAY_OF_WEEK_WEDNESDAY = 3,
    RQ_DAY_OF_WEEK_THURSDAY = 4,
    RQ_DAY_OF_WEEK_FRIDAY = 5,
    RQ_DAY_OF_WEEK_SATURDAY = 6
};

enum rq_rate_type {
    RQ_RATE_TYPE_INVALID = -1,
    RQ_RATE_TYPE_SIMPLE = 0,
    RQ_RATE_TYPE_COMPOUNDING = 1,
    RQ_RATE_TYPE_PAR = 2,
    RQ_RATE_TYPE_EXCHANGE = 3,
    RQ_RATE_TYPE_VOLATILITY = 4,
    RQ_RATE_TYPE_PRICE = 5,
    RQ_RATE_TYPE_DIVIDEND = 6,
    RQ_RATE_TYPE_DIVIDEND_YIELD = 7,
    RQ_RATE_TYPE_YIELD = 8,
	RQ_RATE_TYPE_DIVIDEND_YIELD_CURVE = 9,
    /* IF YOU ADD NEW RATE_TYPES, REMEMBER TO ADD THE STRING TO
     rq_enum.c */
 
    RQ_RATE_TYPE_MAX_ENUM = 10 /**< The number of elements in the enum */
};

enum rq_rate_type rq_rate_type_from_string(const char *str);

/** Convert a rate type enum to a string. */
RQ_EXPORT const char* rq_rate_type_to_string(enum rq_rate_type type);

enum rq_error_severity {
    RQ_ERROR_SEVERITY_NO_ERROR = 0,
    RQ_ERROR_SEVERITY_INFORMATION = 1,
    RQ_ERROR_SEVERITY_WARNING = 2,
    RQ_ERROR_SEVERITY_CRITICAL = 3
};

/* -- FpML Schemes ------------------------------------------------ */

enum rq_fra_discounting_method {
	RQ_FRA_DISC_NONE,
	RQ_FRA_DISC_ISDA,
	RQ_FRA_DISC_AFMA,
	RQ_FRA_DISC_RATIO,
	RQ_FRA_DISC_FXD
};

RQ_EXPORT enum rq_fra_discounting_method rq_fra_discounting_method_from_string(const char *str);
RQ_EXPORT const char *rq_fra_discounting_method_to_string(enum rq_fra_discounting_method fdm);

enum rq_averaging_method {
    RQ_AVERAGING_METHOD_UNWEIGHTED,
    RQ_AVERAGING_METHOD_WEIGHTED
};

enum rq_business_day_convention {
    RQ_BUSINESS_DAY_CONVENTION_FOLLOWING,
    RQ_BUSINESS_DAY_CONVENTION_FRN,
    RQ_BUSINESS_DAY_CONVENTION_MODFOLLOWING,
    RQ_BUSINESS_DAY_CONVENTION_MODPRECEDING,
    RQ_BUSINESS_DAY_CONVENTION_NONE,
    RQ_BUSINESS_DAY_CONVENTION_PRECEDING
};

RQ_EXPORT const char *rq_business_day_convention_to_string(enum rq_business_day_convention bdc);

/**
 * How a calculation agent will be determined.
 */
enum rq_calculation_party {
    RQ_CALCULATION_PARTY_EXERCISINGPARTY,
    RQ_CALCULATION_PARTY_NONEXERCISINGPARTY
};

enum rq_clearance_system {
    RQ_CLEARANCE_SYSTEM_CLEARSTREAM,
    RQ_CLEARANCE_SYSTEM_CREST,
    RQ_CLEARANCE_SYSTEM_DTCC,
    RQ_CLEARANCE_SYSTEM_EUROCLEAR,
    RQ_CLEARANCE_SYSTEM_MONTETITOLI
};

/**
 * Compound calculation method
 */
enum rq_compound_method {
    RQ_COMPOUND_METHOD_FLAT,
    RQ_COMPOUND_METHOD_NONE,
    RQ_COMPOUND_METHOD_STRAIGHT
};

enum rq_contractual_definitions {
    RQ_CONTRACTUAL_DEFINITIONS_ISDA1991,
    RQ_CONTRACTUAL_DEFINITIONS_ISDA1996EQUITY,
    RQ_CONTRACTUAL_DEFINITIONS_ISDA1997GOVBOND,
    RQ_CONTRACTUAL_DEFINITIONS_ISDA1998FX,
    RQ_CONTRACTUAL_DEFINITIONS_ISDA2000,
    RQ_CONTRACTUAL_DEFINITIONS_ISDA2002EQUITY
};

enum rq_cut_name {
    RQ_CUT_NAME_COMEX,
    RQ_CUT_NAME_ECB,
    RQ_CUT_NAME_LONDONEVENINGGOLD,
    RQ_CUT_NAME_LONDONEVENINGPGM,
    RQ_CUT_NAME_LONDONMORNINGGOLD,
    RQ_CUT_NAME_LONDONMORNINGPGM,
    RQ_CUT_NAME_MEXICO,
    RQ_CUT_NAME_NEWYORK,
    RQ_CUT_NAME_NEWYORKPGM,
    RQ_CUT_NAME_SILVERLONDON
};

enum rq_date_relative_to {
    RQ_DATE_RELATIVE_TO_CALCPERIODENDDATE,
    RQ_DATE_RELATIVE_TO_CALCPERIODSTARTDATE,
    RQ_DATE_RELATIVE_TO_CASHSETTLEPAYDATE,
    RQ_DATE_RELATIVE_TO_EXERCISEDATE,
    RQ_DATE_RELATIVE_TO_MANDATORYEARLYTERMDATE,
    RQ_DATE_RELATIVE_TO_PAYMENTDATE,
    RQ_DATE_RELATIVE_TO_RESETDATE,
    RQ_DATE_RELATIVE_TO_SETTLEMENTDATE,
    RQ_DATE_RELATIVE_TO_VALIDATIONDATE
};

enum rq_day_count_fraction {
    RQ_DAY_COUNT_FRACTION_1_1,
    RQ_DAY_COUNT_FRACTION_30_360,
    RQ_DAY_COUNT_FRACTION_30E_360,
    RQ_DAY_COUNT_FRACTION_ACT_360,
    RQ_DAY_COUNT_FRACTION_ACT_365_FIXED,
    RQ_DAY_COUNT_FRACTION_ACT_ACT_AFB,
    RQ_DAY_COUNT_FRACTION_ACT_ACT_ISDA,
    RQ_DAY_COUNT_FRACTION_ACT_ACT_ISMA
};

enum rq_day_type {
    RQ_DAY_TYPE_BUSINESS,
    RQ_DAY_TYPE_WEEK,
    RQ_DAY_TYPE_CALENDAR
};

enum rq_discounting_type {
    RQ_DISCOUNTING_TYPE_FRA,
    RQ_DISCOUNTING_TYPE_STANDARD
};

enum rq_exercise_style {
    RQ_EXERCISE_STYLE_AMERICAN,
    RQ_EXERCISE_STYLE_EUROPEAN
};

enum rq_fx_barrier_type {
    RQ_FX_BARRIER_TYPE_KNOCKIN,
    RQ_FX_BARRIER_TYPE_KNOCKOUT,
    RQ_FX_BARRIER_TYPE_REVERSEKNOCKIN,
    RQ_FX_BARRIER_TYPE_REVERSEKNOCKOUT
};

enum rq_information_provider {
    RQ_INFORMATION_PROVIDER_BANKOFCANADA,
    RQ_INFORMATION_PROVIDER_BLOOMBERG,
    RQ_INFORMATION_PROVIDER_FEDERALRESERVE,
    RQ_INFORMATION_PROVIDER_FHLBSF,
    RQ_INFORMATION_PROVIDER_ISDA,
    RQ_INFORMATION_PROVIDER_REUTERS,
    RQ_INFORMATION_PROVIDER_SAFEX,
    RQ_INFORMATION_PROVIDER_TELERATE
};

enum rq_master_agreement_type {
    RQ_MASTER_AGREEMENT_TYPE_AFB,
    RQ_MASTER_AGREEMENT_TYPE_GERMAN,
    RQ_MASTER_AGREEMENT_TYPE_ISDA1987,
    RQ_MASTER_AGREEMENT_TYPE_ISDA1992,
    RQ_MASTER_AGREEMENT_TYPE_ISDA2002,
    RQ_MASTER_AGREEMENT_TYPE_SWISS
};

enum rq_method_of_adjustment {
    RQ_METHOD_OF_ADJUSTMENT_CALCULATIONAGENT,
    RQ_METHOD_OF_ADJUSTMENT_OPTIONSEXCHANGE
};

enum rq_option_type {
    RQ_OPTION_TYPE_CALL,
    RQ_OPTION_TYPE_PUT
};

enum rq_payer_receiver {
	RQ_PAYER_RECEIVER_INVALID,
    RQ_PAYER_RECEIVER_PAYER,
    RQ_PAYER_RECEIVER_RECEIVER
};

RQ_EXPORT enum rq_payer_receiver rq_payer_receiver_from_string(const char* pr);
RQ_EXPORT const char *rq_payer_receiver_to_string(enum rq_payer_receiver pr);

enum rq_pay_receive {
    RQ_PAY_RECEIVE_INVALID,
    RQ_PAY_RECEIVE_PAY,
    RQ_PAY_RECEIVE_RECEIVE
};

enum rq_payout {
    RQ_PAYOUT_DEFERRED,
    RQ_PAYOUT_IMMEDIATE
};

enum rq_pay_relative_to {
    RQ_PAY_RELATIVE_TO_CALCPERIODENDDATE,
    RQ_PAY_RELATIVE_TO_CALCPERIODSTARTDATE,
    RQ_PAY_RELATIVE_TO_RESETDATE
};

enum rq_pricing_results {
    RQ_PRICING_RESULTS_VALIDATE               = 0x00000001,
    RQ_PRICING_RESULTS_FACE_VALUE             = 0x00000002,
    RQ_PRICING_RESULTS_VALUE                  = 0x00000004,
    RQ_PRICING_RESULTS_MERGE_TRADE_PRODUCT    = 0x00000100,
	RQ_PRICING_RESULTS_CALIBRATE			  = 0x00000200,

    RQ_PRICING_RESULTS_EXPOSURE_PROFILE       = 0x00010000,
    RQ_PRICING_RESULTS_ASSETFLOW_LIST         = 0x00020000,

    RQ_PRICING_RESULTS_NAMED_VALUES			  = 0x00040000,
    RQ_PRICING_RESULTS_NAMED_VALUES_ALL       = 0x00080000,
    RQ_PRICING_RESULTS_PRICING_VALUES         = 0x00100000,
    RQ_PRICING_RESULTS_PRICING_VALUES_ALL     = 0x00200000
};

enum rq_trade_results {
    RQ_TRADE_RESULTS_LOAD_FROM_FILE = 0x00000001,
    RQ_TRADE_RESULTS_SAVE_TO_FILE = 0x00000002
};

enum rq_premium_quote_basis {
    RQ_PREMIUM_QUOTE_BASIS_CALLCCYPERPUTCCY,
    RQ_PREMIUM_QUOTE_BASIS_EXPLICIT,
    RQ_PREMIUM_QUOTE_BASIS_PERCENTOFCALLCCYAMOUNT,
    RQ_PREMIUM_QUOTE_BASIS_PERCENTOFPUTCCYAMOUNT,
    RQ_PREMIUM_QUOTE_BASIS_PUTCCYPERCALLCCY
};

enum rq_quotation_rate_type {
    RQ_QUOTATION_RATE_TYPE_ASK,
    RQ_QUOTATION_RATE_TYPE_BID,
    RQ_QUOTATION_RATE_TYPE_EXERCISINGPARTYPAYS,
    RQ_QUOTATION_RATE_TYPE_MID
};

enum rq_quote_basis {
    RQ_QUOTE_BASIS_CCYONEPERCCYTWO,
    RQ_QUOTE_BASIS_CCYTWOPERCCYONE
};

enum rq_rate_treatment {
    RQ_RATE_TREATMENT_BONDEQUIVALENTYIELD,
    RQ_RATE_TREATMENT_MONEYMARKETYIELD
};

enum rq_reset_relative_to {
    RQ_RESET_RELATIVE_TO_CALCULATIONPERIODENDDATE,
    RQ_RESET_RELATIVE_TO_CALCULATIONPERIODSTARTDATE
};

enum rq_roll_convention {
    RQ_ROLL_CONVENTION_INVALID = 0,
    RQ_ROLL_CONVENTION_1 = 1, /* Rolls on the 1st day of the month. */
    RQ_ROLL_CONVENTION_2 = 2, /* Rolls on the 2nd day of the month. */
    RQ_ROLL_CONVENTION_3 = 3, /* Rolls on the 3rd day of the month. */
    RQ_ROLL_CONVENTION_4 = 4, /* Rolls on the 4th day of the month. */
    RQ_ROLL_CONVENTION_5 = 5, /* Rolls on the 4th day of the month. */
    RQ_ROLL_CONVENTION_6 = 6, /* Rolls on the 6th day of the month. */
    RQ_ROLL_CONVENTION_7 = 7, /* Rolls on the 7th day of the month. */
    RQ_ROLL_CONVENTION_8 = 8, /* Rolls on the 8th day of the month. */
    RQ_ROLL_CONVENTION_9 = 9, /* Rolls on the 9th day of the month. */
    RQ_ROLL_CONVENTION_10 = 10, /* Rolls on the 10th day of the month. */
    RQ_ROLL_CONVENTION_11 = 11, /* Rolls on the 11th day of the month. */
    RQ_ROLL_CONVENTION_12 = 12, /* Rolls on the 12th day of the month. */
    RQ_ROLL_CONVENTION_13 = 13, /* Rolls on the 13th day of the month. */
    RQ_ROLL_CONVENTION_14 = 14, /* Rolls on the 14th day of the month. */
    RQ_ROLL_CONVENTION_15 = 15, /* Rolls on the 15th day of the month. */
    RQ_ROLL_CONVENTION_16 = 16, /* Rolls on the 16th day of the month. */
    RQ_ROLL_CONVENTION_17 = 17, /* Rolls on the 17th day of the month. */
    RQ_ROLL_CONVENTION_18 = 18, /* Rolls on the 18th day of the month. */
    RQ_ROLL_CONVENTION_19 = 19, /* Rolls on the 19th day of the month. */
    RQ_ROLL_CONVENTION_20 = 20, /* Rolls on the 20th day of the month. */
    RQ_ROLL_CONVENTION_21 = 21, /* Rolls on the 21st day of the month. */
    RQ_ROLL_CONVENTION_22 = 22, /* Rolls on the 22nd day of the month. */
    RQ_ROLL_CONVENTION_23 = 23, /* Rolls on the 23rd day of the month. */
    RQ_ROLL_CONVENTION_24 = 24, /* Rolls on the 24th day of the month. */
    RQ_ROLL_CONVENTION_25 = 25, /* Rolls on the 25th day of the month. */
    RQ_ROLL_CONVENTION_26 = 26, /* Rolls on the 26th day of the month. */
    RQ_ROLL_CONVENTION_27 = 27, /* Rolls on the 27th day of the month. */
    RQ_ROLL_CONVENTION_28 = 28, /* Rolls on the 28th day of the month. */
    RQ_ROLL_CONVENTION_29 = 29, /* Rolls on the 29th day of the month. */
    RQ_ROLL_CONVENTION_30 = 30, /* Rolls on the 30th day of the month. */

    RQ_ROLL_CONVENTION_MON = 40, /* Rolling weekly on a Monday */
    RQ_ROLL_CONVENTION_TUE = 41, /* Rolling weekly on a Tuesday. */
    RQ_ROLL_CONVENTION_WED = 42, /* Rolling weekly on a Wednesday. */
    RQ_ROLL_CONVENTION_THU = 43, /* Rolling weekly on a Thursday. */
    RQ_ROLL_CONVENTION_FRI = 44, /* Rolling weekly on a Friday. */
    RQ_ROLL_CONVENTION_SAT = 45, /* Rolling weekly on a Saturday. */
    RQ_ROLL_CONVENTION_SUN = 46, /* Rolling weekly on a Sunday. */

    RQ_ROLL_CONVENTION_EOM, /* Rolls on month end dates irrespective of the length of the month and the previous roll day. */

    RQ_ROLL_CONVENTION_CADIMM, /* The last trading day/expiration day of the Canadian */
    /* Derivatives Exchange (Bourse de Montr# Inc) */
    /* Three-month Canadian Bankers' Acceptance Futures */
    /* (Ticker Symbol BAX). The second London banking day */
    /* prior to the third Wednesday of the contract month. If the */
    /* determined day is a Bourse or bank holiday in Montreal or */
    /* Toronto, the last trading day shall be the previous bank */
    /* business day. Per Canadian Derivatives Exchange BAX */
    /* contract specification. */
    RQ_ROLL_CONVENTION_FRN, /* Roll days are determined according to the FRN */
    /* Convention or Eurodollar Convention as described in */
    /* ISDA 1991 definitions. */
    RQ_ROLL_CONVENTION_IMM, /* IMM Settlement Dates. The third Wednesday of the */
    /* (delivery) month. */
    RQ_ROLL_CONVENTION_SFE, /* Sydney Futures Exchange 90-Day Bank Accepted Bill */
    /* Futures Settlement Dates. The second Friday of the */
    /* (delivery) month. */
    RQ_ROLL_CONVENTION_TBILL, /* Each Monday except for U.S. (New York) holidays when it will occur on a Tuesday. */

    RQ_ROLL_CONVENTION_NONE /* The roll convention is not required. For example, in the */
    /* case of a daily calculation frequency. */
};

RQ_EXPORT const char *rq_roll_convention_to_string(enum rq_roll_convention roll_convention);

RQ_EXPORT enum rq_roll_convention rq_roll_convention_from_string(const char *str);

enum rq_rounding_direction {
    RQ_ROUNDING_DIRECTION_DOWN, /* A fractional number will be rounded down to the specified */
    /* number of decimal places (the precision). */
    RQ_ROUNDING_DIRECTION_NEAREST, /* A fractional number will be rounded either up or down to */
    /* the specified number of decimal palces depending on it's value. */
    RQ_ROUNDING_DIRECTION_UP /* A fractional number will be rounded up to the specified */
    /* number of decimal places (the precision). */
};


enum rq_routing_id_code {
    RQ_ROUTING_ID_CODE_ABA, /* ABA number. */
    RQ_ROUTING_ID_CODE_BIC, /* SWIFT-assigned BIC code. */
    RQ_ROUTING_ID_CODE_ChapsNumber, /* Chaps account number. */
    RQ_ROUTING_ID_CODE_ChipsUID, /* Chips UID code. */
    RQ_ROUTING_ID_CODE_IBAN, /* European Banking Federation number. */
    RQ_ROUTING_ID_CODE_NatBankId /* National Bank id code. */
};

enum rq_settlement_method {
    RQ_SETTLEMENT_METHOD_Chaps, /* To be settled via Chaps network. */
    RQ_SETTLEMENT_METHOD_ChipsABA, /* To be settled via Chips ABA. */
    RQ_SETTLEMENT_METHOD_ChipsUID, /* To be settled via Chips UID. */
    RQ_SETTLEMENT_METHOD_CLS, /* To be settled via CLS Bank. */
    RQ_SETTLEMENT_METHOD_DDA, /* To be settled over DDA account. */
    RQ_SETTLEMENT_METHOD_Fedwire, /* To be settled via U.S. Fedwire. */
    RQ_SETTLEMENT_METHOD_SWIFT /* To be settled via SWIFT network. */
};

enum rq_settlement_price_source {
    RQ_SETTLEMENT_PRICE_SOURCE_Bid, /* The bid price per share on the exchange at the valuation */
    /* time on the valuation date */
    RQ_SETTLEMENT_PRICE_SOURCE_Mid, /* The mid-market price per share on the exchange at the */
    /* valuation time on the valuation date */
    RQ_SETTLEMENT_PRICE_SOURCE_NASDAQ, /* An amount equal to the arithmetic average of the two */
    /* prices constituting the Bid/Offer Spread. "Bid/Offer */
    /* Spread" means the highest bid price per share and the */
    /* corresponding lowest offer price per share last published */
    /* prior to or at the expiration time on the expiration date. */
    RQ_SETTLEMENT_PRICE_SOURCE_Offer, /* The offer price per share on the exchange at the valuation */
    /* time on the valuation date */
    RQ_SETTLEMENT_PRICE_SOURCE_OfficialClose, /* (i) The published official closing price of the shares on the */
    /* exchange on the valuation date, or (ii) the official closing */
    /* level of the index, as published by the index sponsor, on */
    /* the valuation date */
    RQ_SETTLEMENT_PRICE_SOURCE_OfficialSettlement, /* The official settlement price (however described under the */
    /* rules of the relevant exchange or its clearing house) on */
    /* maturity of any of the relevant exchange-traded contracts */
    /* published by the exchange or its clearing house. For this */
    /* purpose, exchange-traded contract shall mean a future or */
    /* listed option contract on the Index whose delivery date is */
    /* expected to be on the valuation date */
    RQ_SETTLEMENT_PRICE_SOURCE_PrezzoDiRiferimento /* The official reference price per share quoted by the */
    /* exchange on the exchange business day immediately */
    /* prior to the expiration date equal to the weighted average */
    /* of the last 10% traded volume on the share */
};

enum rq_settlement_type {
    RQ_SETTLEMENT_TYPE_CASH, /* The intrinsic value of the option will be delivered by way of */
    /* a cash settlement amount determined, (i) by reference to */
    /* the differential between the strike price and the settlement */
    /* price; or (ii) in accordance with a bilateral agreement */
    /* between the parties */
    RQ_SETTLEMENT_TYPE_PHYSICAL, /* The securities underlying the transaction will be delivered */
    /* by (i) in the case of a call, the seller to the buyer, or (ii) in */
    /* the case of a put, the buyer to the seller versus a */
    /* settlement amount equivalent to the strike price per share */
};

enum rq_share_extraordinary_event {
    RQ_SHARE_EXTRAORDINARY_EVENT_AlternativeObligation, /* The trade continues on the consideration paid out to */
    /* holders of the original shares. */
    RQ_SHARE_EXTRAORDINARY_EVENT_CancellationAndPayment, /* The trade is terminated. */
    RQ_SHARE_EXTRAORDINARY_EVENT_OptionsExchange, /* The trade will be adjusted in accordance with any */
    /* adjustment made by the exchange on which options on */
    /* the underlying are listed. */
};

enum rq_side_rate_basis {
    RQ_SIDE_RATE_BASIS_BaseCurrencyPerCurrency1, /* The amount of the baseCurrency for one unit of */
    /* exchangedCurrency1. */
    RQ_SIDE_RATE_BASIS_BaseCurrencyPerCurrency2, /* The amount of the baseCurrency for one unit of */
    /* exchangedCurrency2. */
    RQ_SIDE_RATE_BASIS_Currency1PerBaseCurrency, /* The amount of the exchangedCurrency1 for one unit of */
    /* baseCurrency. */
    RQ_SIDE_RATE_BASIS_Currency2PerBaseCurrency /* The amount of the exchangedCurrency2 for one unit of */
    /* baseCurrency. */
};

/**
 * The code specification of whether a trade is settling using
 * standard settlement instructions as well as whether it is a
 * candidate for settlement netting.  
 */
enum rq_standard_settlement_style {
    RQ_STANDARD_SETTLEMENT_STYLE_Net, /* This trade is a candidate for settlement netting. */
    RQ_STANDARD_SETTLEMENT_STYLE_Standard, /* This trade will settle using standard pre-determined funds */
    /* settlement instructions. */
    RQ_STANDARD_SETTLEMENT_STYLE_StandardAndNet /* This trade will settle using standard pre-determined funds */
    /* settlement instructions and is a candidate for settlement */
    /* netting. */
};

/**
 * The specification of whether a percentage rate change, used to
 * calculate a change in notional outstanding, is expressed as a
 * percentage of the initial notional amount or the previously
 * outstanding notional amount.  
 */
enum rq_step_relative_to {
    RQ_STEP_RELATIVE_TO_INITIAL, /*Change in notional to be applied is calculated by */
    /* multiplying the percentage rate by the initial notional */
    /* amount. */
    RQ_STEP_RELATIVE_TO_PREVIOUS /*Change in notional to be applied is calculated by */
    /* multiplying the percentage rate by the previously */
    /* outstanding notional amount. */
};

/**
 * The specification of how an FX OTC option strike price is quoted.
 */
enum rq_strike_quote_basis {
    RQ_STRIKE_QUOTE_BASIS_CallCurrencyPerPutCurrency, /* The strike price is an amount of callCurrency per one unit */
    /* of putCurrency. */
    RQ_STRIKE_QUOTE_BASIS_PutCurrencyPerCallCurrency /* The strike price is an amount of putCurrency per one unit */
    /* of callCurrency. */
};

/**
 * Defines points in the day when equity option exercise and valuation
 * can occur.  
 */
enum rq_time_type {
    RQ_TIME_TYPE_Close, /* The official closing time of the exchange on the valuation */
    /* date. */
    RQ_TIME_TYPE_Open, /* The official opening time of the exchange on the valuation */
    /* date. */
    RQ_TIME_TYPE_OSP, /* The time at which the official settlement price is */
    /* determined. */
    RQ_TIME_TYPE_SpecificTime, /* The time specified in the element equityExpirationTime or */
    /* valuationTime (as appropriate) */
    RQ_TIME_TYPE_XETRA /* The time at which the official settlement price (following */
    /* the auction by the exchange) is determined by the */
    /* exchange. */
};

/**
 * The specification of, for American-style digitals, whether the
 * trigger level must be touched or not touched.  
 */
enum rq_touch_condition {
    RQ_TOUCH_CONDITION_NOTOUCH, /* The spot rate has not touched the predetermined trigger */
    /* rate at any time over the life of the option for the payout to */
    /* occur. */
    RQ_TOUCH_CONDITION_TOUCH /* The spot rate must have touched the predetermined */
    /* trigger rate at any time over the life of the option for the */
    /* payout to occur. */
};

/**
 * The specification of whether a payout will occur on an option
 * depending upon whether the spot rate is above or below the trigger
 * rate.  
 */
enum rq_trigger_condition {
    RQ_TRIGGER_CONDITION_ABOVE, /* The spot rate must be greater than or equal to the trigger */
    /* rate. */
    RQ_TRIGGER_CONDITION_BELOW /* The spot rate must be less than or equal to the trigger */
    /* rate. */
};

/** The specification of a weekly roll day.
 */
enum rq_weekly_roll_convention {
    RQ_WEEKLY_ROLL_CONVENTION_MON, /* Monday */
    RQ_WEEKLY_ROLL_CONVENTION_TUE, /* Tuesday */
    RQ_WEEKLY_ROLL_CONVENTION_WED, /* Wednesday */
    RQ_WEEKLY_ROLL_CONVENTION_THU, /* Thursday */
    RQ_WEEKLY_ROLL_CONVENTION_FRI, /* Friday */
    RQ_WEEKLY_ROLL_CONVENTION_SAT, /* Saturday */
    RQ_WEEKLY_ROLL_CONVENTION_SUN  /* Sunday */
};

/** Periods per year.
 */
enum rq_periods_per_year {
    RQ_PERIODS_PER_YEAR_INVALID,
    RQ_PERIODS_PER_YEAR_WEEKLY = 52,
    RQ_PERIODS_PER_YEAR_FORTNIGHTLY = 26,
    RQ_PERIODS_PER_YEAR_MONTHLY = 12,
    RQ_PERIODS_PER_YEAR_QUARTERLY = 4,
    RQ_PERIODS_PER_YEAR_SEMI = 2,
    RQ_PERIODS_PER_YEAR_YEARLY = 1
};

/** Periods per year.
 */
enum rq_ir_future_quote_convention {
    RQ_IR_FUTURE_QC_INVALID = -1,
    RQ_IR_FUTURE_QC_YIELD = 1,
    RQ_IR_FUTURE_QC_100_MINUS_DISCOUNT = 98,
    RQ_IR_FUTURE_QC_100_MINUS_YIELD = 99,
    RQ_IR_FUTURE_QC_PRICE_PER_100 = 100,
};

RQ_EXPORT enum rq_ir_future_quote_convention rq_ir_future_quote_convention_from_string(const char* quoteConvention);
RQ_EXPORT const char* rq_ir_future_quote_convention_to_string(enum rq_ir_future_quote_convention quoteConvention);
RQ_EXPORT enum rq_interpolation_method rq_interpolation_method_from_string(const char* interpolationMethod);
RQ_EXPORT const char* rq_interpolation_method_to_string(enum rq_interpolation_method interpolationMethod);
RQ_EXPORT enum rq_extrapolation_method rq_extrapolation_method_from_string(const char* extrapolationMethod);
RQ_EXPORT const char* rq_extrapolation_method_to_string(enum rq_extrapolation_method extrapolationMethod);
RQ_EXPORT enum rq_zero_method rq_zero_method_from_string(const char* zeroMethod);
RQ_EXPORT const char *rq_zero_method_to_string(enum rq_zero_method zeroMethod);
RQ_EXPORT enum rq_billFuture_method rq_billFuture_method_from_string(const char* billFutureMethod);
RQ_EXPORT const char *rq_billFuture_method_to_string(enum rq_billFuture_method billFutureMethod);
RQ_EXPORT enum rq_swapParInterpolation_method rq_swapParInterpolation_method_from_string(const char*);
RQ_EXPORT const char *rq_swapParInterpolation_method_to_string(enum rq_swapParInterpolation_method);
RQ_EXPORT enum rq_swapDayCount_method rq_swapDayCount_method_from_string(const char*);
RQ_EXPORT const char *rq_swapDayCount_method_to_string(enum rq_swapDayCount_method);
RQ_EXPORT enum rq_day_type rq_day_type_from_string(const char* dayType);
RQ_EXPORT const char *rq_day_type_to_string(enum rq_day_type dayType);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
