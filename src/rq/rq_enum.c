/*
** rq_enum.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
#include "rq_config.h"
#include "rq_enum.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT enum rq_fra_discounting_method 
rq_fra_discounting_method_from_string(const char* fdm)
{
	if(strcmp(fdm,"ISDA")==0)
        return RQ_FRA_DISC_ISDA;
	 if(strcmp(fdm,"AFMA")==0)
		return RQ_FRA_DISC_AFMA;
	if(strcmp(fdm,"RATIO")==0)
		return RQ_FRA_DISC_RATIO;
	if(strcmp(fdm,"FXD")==0)
		return RQ_FRA_DISC_FXD;
	else
		return 	RQ_FRA_DISC_NONE;
}

RQ_EXPORT const char *
rq_fra_discounting_method_to_string(enum rq_fra_discounting_method fdm)
{
    switch (fdm)
    {
        case RQ_FRA_DISC_ISDA:
            return "ISDA";

        case RQ_FRA_DISC_AFMA:
            return "AFMA";

        case RQ_FRA_DISC_RATIO:
            return "RATIO";

        case RQ_FRA_DISC_FXD:
            return "FXD";

        case RQ_FRA_DISC_NONE:
        default:
            return "NONE";
    }
}

RQ_EXPORT enum rq_payer_receiver 
rq_payer_receiver_from_string(const char* pr)
{
	if(strcmp(pr,"PAYER")==0)
        return RQ_PAYER_RECEIVER_PAYER;
	 if(strcmp(pr,"RECEIVER")==0)
		return RQ_PAYER_RECEIVER_RECEIVER;
	else
		return RQ_PAYER_RECEIVER_INVALID;
}

RQ_EXPORT const char *
rq_payer_receiver_to_string(enum rq_payer_receiver pr)
{
    switch (pr)
    {
        case RQ_PAYER_RECEIVER_PAYER:
            return "PAYER";

        case RQ_PAYER_RECEIVER_RECEIVER:
            return "RECEIVER";

        default:
            return "INVALID";
    }
}

RQ_EXPORT const char *
rq_business_day_convention_to_string(enum rq_business_day_convention bdc)
{
    switch (bdc)
    {
        case RQ_BUSINESS_DAY_CONVENTION_FOLLOWING:
            return "FOLLOWING";

        case RQ_BUSINESS_DAY_CONVENTION_FRN:
            return "FRN";

        case RQ_BUSINESS_DAY_CONVENTION_MODFOLLOWING:
            return "MODFOLLOWING";

        case RQ_BUSINESS_DAY_CONVENTION_MODPRECEDING:
            return "MODPRECEDING";

        case RQ_BUSINESS_DAY_CONVENTION_NONE:
            return "NONE";

        case RQ_BUSINESS_DAY_CONVENTION_PRECEDING:
            return "PRECEDING";

        default:
            return "UNKNOWN";
    }
}

/* DS(IT&E): Added to ensure fpml setting could be converted to rq enums. */

RQ_EXPORT enum rq_date_roll_convention 
rq_date_roll_convention_from_string(const char* bdc)
{
    if(strcmp(bdc,"FOLLOWING")==0)
        return RQ_DATE_ROLL_FOLLOWING;

    else if(strcmp(bdc,"MODFOLLOWING")==0)
        return RQ_DATE_ROLL_MOD_FOLLOWING;

    else if(strcmp(bdc,"MODPRECEDING")==0)
        return RQ_DATE_ROLL_MOD_PRECEDING;

    else if(strcmp(bdc,"NONE")==0)
        return RQ_DATE_ROLL_NO_ADJUSTMENT;

    else if(strcmp(bdc,"PRECEDING")==0)
        return RQ_DATE_ROLL_PRECEDING;

    else
        return RQ_DATE_ROLL_NO_ADJUSTMENT;
}

/* GB(IT&E): Added to ensure rq could be converted to strings. */

RQ_EXPORT const char*  
rq_date_roll_convention_to_string(enum rq_date_roll_convention date_roll_convention)
{
	switch (date_roll_convention)
	{
		case RQ_DATE_ROLL_FOLLOWING:
			return "FOLLOWING";

		case RQ_DATE_ROLL_MOD_FOLLOWING:
			return "MODFOLLOWING";

		case RQ_DATE_ROLL_MOD_PRECEDING:
			return "MODPRECEDING";

		case RQ_DATE_ROLL_NO_ADJUSTMENT:
			return "NONE";

		case RQ_DATE_ROLL_PRECEDING:
			return "PRECEDING";
	};

	return "NONE";
}

RQ_EXPORT enum rq_roll_convention 
rq_roll_convention_from_string(const char *str)
{
    enum rq_roll_convention roll_conv = RQ_ROLL_CONVENTION_INVALID;

    if (!strcmp(str, "1"))
        roll_conv = RQ_ROLL_CONVENTION_1;
    else if (!strcmp(str, "10"))
        roll_conv = RQ_ROLL_CONVENTION_10;
    else if (!strcmp(str, "11"))
        roll_conv = RQ_ROLL_CONVENTION_11;
    else if (!strcmp(str, "12"))
        roll_conv = RQ_ROLL_CONVENTION_12;
    else if (!strcmp(str, "13"))
        roll_conv = RQ_ROLL_CONVENTION_13;
    else if (!strcmp(str, "14"))
        roll_conv = RQ_ROLL_CONVENTION_14;
    else if (!strcmp(str, "15"))
        roll_conv = RQ_ROLL_CONVENTION_15;
    else if (!strcmp(str, "16"))
        roll_conv = RQ_ROLL_CONVENTION_16;
    else if (!strcmp(str, "17"))
        roll_conv = RQ_ROLL_CONVENTION_17;
    else if (!strcmp(str, "18"))
        roll_conv = RQ_ROLL_CONVENTION_18;
    else if (!strcmp(str, "19"))
        roll_conv = RQ_ROLL_CONVENTION_19;
    else if (!strcmp(str, "2"))
        roll_conv = RQ_ROLL_CONVENTION_2;
    else if (!strcmp(str, "20"))
        roll_conv = RQ_ROLL_CONVENTION_20;
    else if (!strcmp(str, "21"))
        roll_conv = RQ_ROLL_CONVENTION_21;
    else if (!strcmp(str, "22"))
        roll_conv = RQ_ROLL_CONVENTION_22;
    else if (!strcmp(str, "23"))
        roll_conv = RQ_ROLL_CONVENTION_23;
    else if (!strcmp(str, "24"))
        roll_conv = RQ_ROLL_CONVENTION_24;
    else if (!strcmp(str, "25"))
        roll_conv = RQ_ROLL_CONVENTION_25;
    else if (!strcmp(str, "26"))
        roll_conv = RQ_ROLL_CONVENTION_26;
    else if (!strcmp(str, "27"))
        roll_conv = RQ_ROLL_CONVENTION_27;
    else if (!strcmp(str, "28"))
        roll_conv = RQ_ROLL_CONVENTION_28;
    else if (!strcmp(str, "29"))
        roll_conv = RQ_ROLL_CONVENTION_29;
    else if (!strcmp(str, "3"))
        roll_conv = RQ_ROLL_CONVENTION_3;
    else if (!strcmp(str, "30"))
        roll_conv = RQ_ROLL_CONVENTION_30;
    else if (!strcmp(str, "4"))
        roll_conv = RQ_ROLL_CONVENTION_4;
    else if (!strcmp(str, "5"))
        roll_conv = RQ_ROLL_CONVENTION_5;
    else if (!strcmp(str, "6"))
        roll_conv = RQ_ROLL_CONVENTION_6;
    else if (!strcmp(str, "7"))
        roll_conv = RQ_ROLL_CONVENTION_7;
    else if (!strcmp(str, "8"))
        roll_conv = RQ_ROLL_CONVENTION_8;
    else if (!strcmp(str, "9"))
        roll_conv = RQ_ROLL_CONVENTION_9;
    else if (!strcmp(str, "CADIMM"))
        roll_conv = RQ_ROLL_CONVENTION_CADIMM;
    else if (!strcmp(str, "EOM"))
        roll_conv = RQ_ROLL_CONVENTION_EOM;
    else if (!strcmp(str, "FRI"))
        roll_conv = RQ_ROLL_CONVENTION_FRI;
    else if (!strcmp(str, "FRN"))
        roll_conv = RQ_ROLL_CONVENTION_FRN;
    else if (!strcmp(str, "IMM"))
        roll_conv = RQ_ROLL_CONVENTION_IMM;
    else if (!strcmp(str, "MON"))
        roll_conv = RQ_ROLL_CONVENTION_MON;
    else if (!strcmp(str, "NONE"))
        roll_conv = RQ_ROLL_CONVENTION_NONE;
    else if (!strcmp(str, "SAT"))
        roll_conv = RQ_ROLL_CONVENTION_SAT;
    else if (!strcmp(str, "SFE"))
        roll_conv = RQ_ROLL_CONVENTION_SFE;
    else if (!strcmp(str, "SUN"))
        roll_conv = RQ_ROLL_CONVENTION_SUN;
    else if (!strcmp(str, "TBILL"))
        roll_conv = RQ_ROLL_CONVENTION_TBILL;
    else if (!strcmp(str, "THU"))
        roll_conv = RQ_ROLL_CONVENTION_THU;
    else if (!strcmp(str, "TUE"))
        roll_conv = RQ_ROLL_CONVENTION_TUE;
    else if (!strcmp(str, "WED"))
        roll_conv = RQ_ROLL_CONVENTION_WED;

    return roll_conv;
}

RQ_EXPORT const char *
rq_roll_convention_to_string(enum rq_roll_convention roll_convention)
{
    switch (roll_convention)
    {
        case RQ_ROLL_CONVENTION_1:
            return "1";
        
        case RQ_ROLL_CONVENTION_10:
            return "10";

        case RQ_ROLL_CONVENTION_11:
            return "11";

        case RQ_ROLL_CONVENTION_12:
            return "12";

        case RQ_ROLL_CONVENTION_13:
            return "13";

        case RQ_ROLL_CONVENTION_14:
            return "14";

        case RQ_ROLL_CONVENTION_15:
            return "15";

        case RQ_ROLL_CONVENTION_16:
            return "16";

        case RQ_ROLL_CONVENTION_17:
            return "17";

        case RQ_ROLL_CONVENTION_18:
            return "18";

        case RQ_ROLL_CONVENTION_19:
            return "19";

        case RQ_ROLL_CONVENTION_2:
            return "2";

        case RQ_ROLL_CONVENTION_20:
            return "20";

        case RQ_ROLL_CONVENTION_21:
            return "21";

        case RQ_ROLL_CONVENTION_22:
            return "22";

        case RQ_ROLL_CONVENTION_23:
            return "23";

        case RQ_ROLL_CONVENTION_24:
            return "24";

        case RQ_ROLL_CONVENTION_25:
            return "25";

        case RQ_ROLL_CONVENTION_26:
            return "26";

        case RQ_ROLL_CONVENTION_27:
            return "27";

        case RQ_ROLL_CONVENTION_28:
            return "28";

        case RQ_ROLL_CONVENTION_29:
            return "29";

        case RQ_ROLL_CONVENTION_3:
            return "3";

        case RQ_ROLL_CONVENTION_30:
            return "30";

        case RQ_ROLL_CONVENTION_4:
            return "4";

        case RQ_ROLL_CONVENTION_5:
            return "5";

        case RQ_ROLL_CONVENTION_6:
            return "6";

        case RQ_ROLL_CONVENTION_7:
            return "7";

        case RQ_ROLL_CONVENTION_8:
            return "8";

        case RQ_ROLL_CONVENTION_9:
            return "9";

        case RQ_ROLL_CONVENTION_CADIMM:
            return "CADIMM";

        case RQ_ROLL_CONVENTION_EOM:
            return "EOM";

        case RQ_ROLL_CONVENTION_FRI:
            return "FRI";

        case RQ_ROLL_CONVENTION_FRN:
            return "FRN";

        case RQ_ROLL_CONVENTION_IMM:
            return "IMM";

        case RQ_ROLL_CONVENTION_MON:
            return "MON";

        case RQ_ROLL_CONVENTION_NONE:
            return "NONE";

        case RQ_ROLL_CONVENTION_SAT:
            return "SAT";

        case RQ_ROLL_CONVENTION_SFE:
            return "SFE";

        case RQ_ROLL_CONVENTION_SUN:
            return "SUN";

        case RQ_ROLL_CONVENTION_TBILL:
            return "TBILL";

        case RQ_ROLL_CONVENTION_THU:
            return "THU";

        case RQ_ROLL_CONVENTION_TUE:
            return "TUE";

        case RQ_ROLL_CONVENTION_WED:
            return "WED";

        default:
            return "UNKNOWN";
    }
}

static const char *termstruct_type_strings[] = {
    "YIELD_CURVE",
    "FORWARD_CURVE",
    "VOL_SURFACE",
    "SPREAD_CURVE",
    "EQUITY_CURVE",
    "EXTERNAL",
    "IR_VOL_SURFACE",
	"CDS_CURVE",
	"FUTURE_CURVE",
    "ELECTRICITY_CURVE"
};

RQ_EXPORT enum rq_termstruct_type
rq_termstruct_type_from_string(const char *str)
{
    unsigned i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        if (!strcmp(str, termstruct_type_strings[i]))
            return (enum rq_termstruct_type)i;
    return RQ_TERMSTRUCT_TYPE_INVALID;
}

RQ_EXPORT const char *
rq_termstruct_type_to_string(enum rq_termstruct_type type)
{
    return termstruct_type_strings[type];
}

static const char *cds_quote_convention_strings[] = {
    "SURVIVAL_RATE",
    "DEFAULT_RATE",
    "CDS_SPREAD",
	NULL
};

RQ_EXPORT enum rq_cds_quote_convention 
rq_cds_quote_convention_from_string(const char *str)
{
    unsigned i;
    for (i = 0; cds_quote_convention_strings[i]; ++i)
        if (!strcmp(str, cds_quote_convention_strings[i]))
            return (enum rq_cds_quote_convention)i;
    return RQ_CDS_QUOTE_CONVENTION_INVALID;
}

RQ_EXPORT const char *
rq_cds_quote_convention_to_string(enum rq_cds_quote_convention convention)
{
	if (convention == RQ_CDS_QUOTE_CONVENTION_INVALID)
		return NULL;
	else
	    return cds_quote_convention_strings[convention];
}

static const char *rate_type_strings[] = {
    "SIMPLE",
    "COMPOUNDING",
    "PAR",
    "EXCHANGE",
    "VOLATILITY",
    "PRICE",
    "DIVIDEND",
    "DIVIDEND_YIELD",
    "YIELD",
	"DIVIDEND_YIELD_CURVE"
};

enum rq_rate_type
rq_rate_type_from_string(const char *str)
{
    unsigned i;
    for (i = 0; i < RQ_RATE_TYPE_MAX_ENUM; i++)
        if (!strcmp(str, rate_type_strings[i]))
            return (enum rq_rate_type)i;
    return RQ_RATE_TYPE_INVALID;
}

RQ_EXPORT const char *
rq_rate_type_to_string(enum rq_rate_type type)
{
    return rate_type_strings[type];
}

static struct day_count_conv {
    enum rq_day_count_convention conv;
    const char *str;
} day_count_convs[] = {
    { RQ_DAY_COUNT_INVALID, "Invalid" },
    { RQ_DAY_COUNT_ACTUAL_360, "ACT/360" },
    { RQ_DAY_COUNT_ACTUAL_365, "ACT/365" },
    { RQ_DAY_COUNT_30_360, "30/360" },
    { RQ_DAY_COUNT_E30_360, "30E/360" },
    { RQ_DAY_COUNT_ACTUAL_ACTUAL, "ACT/ACT" },
    { RQ_DAY_COUNT_ACTUAL_365_25, "ACT/365.25" },
    { RQ_DAY_COUNT_ACTUAL_365_FIXED, "ACT/365.FIXED" },
    { RQ_DAY_COUNT_ACTUAL_ACTUAL_ISDA, "ACT/ACT.ISDA" },
    { RQ_DAY_COUNT_ACTUAL_ACTUAL_ISMA, "ACT/ACT.ISMA" },
    { RQ_DAY_COUNT_ACTUAL_ACTUAL_AFB, "ACT/ACT.AFB" },
    { RQ_DAY_COUNT_ACTUAL_365_END, "ACT/365.END" },
    { RQ_DAY_COUNT_ACTUAL_1_1, "1/1" },
    { RQ_DAY_COUNT_ACTUAL_365_ACTUAL, "ACT/365.ACT" },
    { RQ_DAY_COUNT_BUS_252, "BUS/252" }
};

RQ_EXPORT const char *
rq_day_count_convention_to_string(enum rq_day_count_convention convention)
{
    unsigned i;
    for (i = 0; i < sizeof(day_count_convs)/sizeof(struct day_count_conv); i++)
    {
        if (convention == day_count_convs[i].conv)
            return day_count_convs[i].str;
    }
    return day_count_convs[0].str; /* INVALID */
}

RQ_EXPORT enum rq_day_count_convention 
rq_day_count_convention_from_string(const char *str)
{
    unsigned i;
    for (i = 0; i < sizeof(day_count_convs)/sizeof(struct day_count_conv); i++)
    {
        if (!stricmp(str, day_count_convs[i].str))
            return day_count_convs[i].conv;
    }
    
    return RQ_DAY_COUNT_INVALID;
}

RQ_EXPORT const char *
rq_ir_future_quote_convention_to_string(enum rq_ir_future_quote_convention quoteConvention)
{
    switch (quoteConvention)
    {
        case RQ_IR_FUTURE_QC_100_MINUS_YIELD:
            return "HUNDRED_MINUS_YIELD";

        case RQ_IR_FUTURE_QC_100_MINUS_DISCOUNT:
            return "HUNDRED_MINUS_DISCOUNT";

        case RQ_IR_FUTURE_QC_YIELD:
            return "YIELD";

		case RQ_IR_FUTURE_QC_PRICE_PER_100:
            return "PRICE_PER_HUNDRED";

		default:
            return "UNKNOWN";
    }
}

RQ_EXPORT enum rq_ir_future_quote_convention 
rq_ir_future_quote_convention_from_string(const char* quoteConvention)
{
	if(strcmp(quoteConvention,"HUNDRED_MINUS_YIELD")==0)
        return RQ_IR_FUTURE_QC_100_MINUS_YIELD;
	
	if(strcmp(quoteConvention,"HUNDRED_MINUS_DISCOUNT")==0)
        return RQ_IR_FUTURE_QC_100_MINUS_DISCOUNT;
	
	if(strcmp(quoteConvention,"YIELD")==0)
		return RQ_IR_FUTURE_QC_YIELD;
	
	if(strcmp(quoteConvention,"PRICE_PER_HUNDRED")==0)
		return RQ_IR_FUTURE_QC_PRICE_PER_100;
	
	else
		return 	RQ_IR_FUTURE_QC_INVALID;
}

RQ_EXPORT enum rq_interpolation_method 
rq_interpolation_method_from_string(const char* interpolationMethod)
{
	if(strcmp(interpolationMethod,"LINEAR_DISCOUNT_FACTOR")==0)
        return RQ_INTERPOLATION_LINEAR_DISCOUNT_FACTOR;
	else if(strcmp(interpolationMethod,"LINEAR_ZERO")==0)
		return RQ_INTERPOLATION_LINEAR_ZERO;
	else if(strcmp(interpolationMethod,"LOG_LINEAR_DISCOUNT_FACTOR")==0)
		return RQ_INTERPOLATION_LOG_LINEAR_DISCOUNT_FACTOR;
	else if(strcmp(interpolationMethod,"LOG_LINEAR_ZERO")==0)
		return RQ_INTERPOLATION_LOG_LINEAR_ZERO;
	else if(strcmp(interpolationMethod,"SPLINE_DISCOUNT_FACTOR")==0)
		return RQ_INTERPOLATION_SPLINE_DISCOUNT_FACTOR;
	else if(strcmp(interpolationMethod,"NONE")==0)
		return RQ_INTERPOLATION_NONE;
	else
		return 	RQ_INTERPOLATION_INVALID;
}

RQ_EXPORT const char *
rq_interpolation_method_to_string(enum rq_interpolation_method interpolationMethod)
{
    switch (interpolationMethod)
    {
        case RQ_INTERPOLATION_LINEAR_DISCOUNT_FACTOR:
            return "LINEAR_DISCOUNT_FACTOR";

        case RQ_INTERPOLATION_LINEAR_ZERO:
            return "LINEAR_ZERO";

        case RQ_INTERPOLATION_LOG_LINEAR_DISCOUNT_FACTOR:
            return "LOG_LINEAR_DISCOUNT_FACTOR";

        case RQ_INTERPOLATION_LOG_LINEAR_ZERO:
            return "LOG_LINEAR_ZERO";

        case RQ_INTERPOLATION_SPLINE_DISCOUNT_FACTOR:
            return "SPLINE_DISCOUNT_FACTOR";

        case RQ_INTERPOLATION_NONE:
            return "NONE";

        default:
            return "INVALID";
    }
}

RQ_EXPORT enum rq_extrapolation_method 
rq_extrapolation_method_from_string(const char* extrapolationMethod)
{
	if(strcmp(extrapolationMethod,"LAST_ZERO")==0)
        return RQ_EXTRAPOLATION_LAST_ZERO;
	else if(strcmp(extrapolationMethod,"LINEAR_DISCOUNT_FACTOR")==0)
		return RQ_EXTRAPOLATION_LINEAR_DISCOUNT_FACTOR;
	else if(strcmp(extrapolationMethod,"LINEAR_ZERO")==0)
		return RQ_EXTRAPOLATION_LINEAR_ZERO;
	else if(strcmp(extrapolationMethod,"LINEAR_ZERO_ZERO")==0)
		return RQ_EXTRAPOLATION_LINEAR_ZERO_ZERO;
	else
		return 	RQ_EXTRAPOLATION_INVALID;
}

RQ_EXPORT const char *
rq_extrapolation_method_to_string(enum rq_extrapolation_method extrapolationMethod)
{
    switch (extrapolationMethod)
    {
        case RQ_EXTRAPOLATION_LAST_ZERO:
            return "LAST_ZERO";

        case RQ_EXTRAPOLATION_LINEAR_DISCOUNT_FACTOR:
            return "LINEAR_DISCOUNT_FACTOR";

        case RQ_EXTRAPOLATION_LINEAR_ZERO:
            return "LINEAR_ZERO";

        case RQ_EXTRAPOLATION_LINEAR_ZERO_ZERO:
            return "LINEAR_ZERO_ZERO";

        default:
            return "INVALID";
    }
}

RQ_EXPORT enum rq_zero_method 
rq_zero_method_from_string(const char* zeroMethod)
{
	if(strcmp(zeroMethod,"SIMPLE")==0)
        return RQ_ZERO_SIMPLE;
	else if(strcmp(zeroMethod,"COMPOUNDING")==0)
        return RQ_ZERO_COMPOUNDING;
	else if(strcmp(zeroMethod,"CONTINUOUS_COMPOUNDING")==0)
        return RQ_ZERO_CONTINUOUS_COMPOUNDING;
	else if(strcmp(zeroMethod,"CASH_DEPOSIT")==0)
        return RQ_ZERO_CASH_DEPOSIT;
	else if(strcmp(zeroMethod,"")==0)
		return RQ_ZERO_CASH_DEPOSIT; /* if empty use cash deposit rate (default) */
	else
		return 	RQ_ZERO_INVALID;
}

RQ_EXPORT const char *
rq_zero_method_to_string(enum rq_zero_method zeroMethod)
{
    switch (zeroMethod)
    {
        case RQ_ZERO_SIMPLE:
            return "SIMPLE";

        case RQ_ZERO_COMPOUNDING:
            return "COMPOUNDING";

        case RQ_ZERO_CONTINUOUS_COMPOUNDING:
            return "CONTINUOUS_COMPOUNDING";

        case RQ_ZERO_CASH_DEPOSIT:
            return "CASH_DEPOSIT";

        default:
            return "INVALID";
    }
}

RQ_EXPORT enum rq_billFuture_method 
rq_billFuture_method_from_string(const char* billFutureMethod)
{
	if(strcmp(billFutureMethod,"STRIP")==0)
        return RQ_BILL_FUTURE_STRIP;
	else if(strcmp(billFutureMethod,"TENOR")==0)
        return RQ_BILL_FUTURE_TENOR;
	else if(strcmp(billFutureMethod,"")==0)
		return RQ_BILL_FUTURE_STRIP; /* default */
	else
		return RQ_BILL_FUTURE_INVALID;
}

RQ_EXPORT const char *
rq_billFuture_method_to_string(enum rq_billFuture_method billFutureMethod)
{
    switch (billFutureMethod)
    {
        case RQ_BILL_FUTURE_STRIP:
            return "STRIP";

        case RQ_BILL_FUTURE_TENOR:
            return "TENOR";

        default:
            return "INVALID";
    }
}

RQ_EXPORT enum rq_swapParInterpolation_method
rq_swapParInterpolation_method_from_string(const char* s)
{
	if(strcmp(s,"LINEAR")==0)
        return RQ_SWAP_PAR_INTERPOLATION_LINEAR;
	else if(strcmp(s,"CUBIC_SPLINE")==0)
        return RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE;
	else if(strcmp(s,"CUBIC_SPLINE_SWAPS_ONLY")==0)
        return RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE_SWAPS_ONLY;
	else if(strcmp(s,"")==0)
		return RQ_SWAP_PAR_INTERPOLATION_LINEAR; /* default */
	else
		return RQ_SWAP_PAR_INTERPOLATION_INVALID;
}

RQ_EXPORT const char *
rq_swapParInterpolation_method_to_string(enum rq_swapParInterpolation_method method)
{
    switch (method)
    {
        case RQ_SWAP_PAR_INTERPOLATION_LINEAR:
            return "LINEAR";

        case RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE:
            return "CUBIC_SPLINE";

        case RQ_SWAP_PAR_INTERPOLATION_CUBIC_SPLINE_SWAPS_ONLY:
            return "CUBIC_SPLINE_SWAPS_ONLY";

        default:
            return "INVALID";
    }
}

RQ_EXPORT enum rq_swapDayCount_method
rq_swapDayCount_method_from_string(const char*s)
{
	if(strcmp(s,"SIMPLE")==0)
        return RQ_SWAP_DAY_COUNT_SIMPLE;
	else if(strcmp(s,"DAYCOUNT")==0)
        return RQ_SWAP_DAY_COUNT_DAYCOUNT;
	else if(strcmp(s,"")==0)
		return RQ_SWAP_DAY_COUNT_SIMPLE; /* default */
	else
		return RQ_SWAP_DAY_COUNT_INVALID;
}

RQ_EXPORT const char *
rq_swapDayCount_method_to_string(enum rq_swapDayCount_method method)
{
    switch (method)
    {
        case RQ_SWAP_DAY_COUNT_SIMPLE:
            return "SIMPLE";

        case RQ_SWAP_DAY_COUNT_DAYCOUNT:
            return "DAYCOUNT";

        default:
            return "INVALID";
    }
}

RQ_EXPORT enum rq_day_type
rq_day_type_from_string(const char* dayType)
{
    if (stricmp(dayType, "BUSINESS")==0)
        return RQ_DAY_TYPE_BUSINESS;
    else if (stricmp(dayType, "WEEK")==0)
        return RQ_DAY_TYPE_WEEK;
    //default to calendar
    else //if (strcmp(dayType, "CALENDAR")==0)
        return RQ_DAY_TYPE_CALENDAR;
}

RQ_EXPORT const char *
rq_day_type_to_string(enum rq_day_type dayType)
{
    switch (dayType)
    {
        case RQ_DAY_TYPE_BUSINESS:
            return "BUSINESS";
        case RQ_DAY_TYPE_WEEK:
            return "WEEK";
        case RQ_DAY_TYPE_CALENDAR:
            return "CALENDAR";
        default:
            return "CALENDAR";
    }
}
