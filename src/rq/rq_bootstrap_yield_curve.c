/*
** rq_bootstrap_yield_curve.c
**
** Written by Joey Nicotra
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
#include "rq_bootstrap_yield_curve.h"
#include "rq_bootstrap_config.h"
#include "rq_asset_mgr.h"
#include "rq_asset_ccy.h"
#include "rq_asset_irovernight.h"
#include "rq_asset_irdiscount.h"
#include "rq_asset_irbond.h"
#include "rq_asset_irfuture.h"
#include "rq_asset_irswap.h"
#include "rq_asset_credit_spread.h"
#include "rq_date_roll.h"
#include "rq_defs.h"
#include "rq_interpolate.h"
#include "rq_day_count.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <math.h>

/*#define DEBUG_BOOTSTRAP */ /* defining this gives a helpful printout of the assets, points and attributes that make up the yc */
#ifdef DEBUG_BOOTSTRAP
#define DEBUG_BOOTSTRAP_FH
#define debugFile "debugRqYC.log"
#endif


#ifdef DEBUG_BOOTSTRAP
void
dumpYieldCurve(rq_system_t system, rq_yield_curve_t yc)
{
#ifdef DEBUG_BOOTSTRAP_FH
    FILE *fh = fopen(debugFile, "a");
#else
	FILE *fh = stdout;
#endif
	unsigned i;

	fprintf(fh, "%s", "dumpYieldCurve:\n");
	fprintf(fh, "\tfromDate:\t%d\n", yc->from_date);
	fprintf(fh, "\tyield curve type:\t%d\n", yc->yield_curve_type);
	fprintf(fh, "\tinterpolation method:\t%d\n", (unsigned)yc->interpolation_method);
	fprintf(fh, "\textrapolation curve start method:\t%d\n", (unsigned)yc->curve_start_extrapolation_method);
	fprintf(fh, "\textrapolation curve end method:\t%d\n", (unsigned)yc->curve_end_extrapolation_method);
	fprintf(fh, "\ttermstruct:\t%s\n", yc->termstruct.termstruct_id);
	fprintf(fh, "\ttermstruct type asset:\t%d\n", (unsigned)yc->termstruct.termstruct_type);
	fprintf(fh, "\tunderlying asset:\t%s\n", yc->termstruct.underlying_asset_id);
	fprintf(fh, "\tday count convention:\t%d\n", (unsigned)yc->default_day_count_convention);

	for (i = 0; i < rq_yield_curve_size(yc); i++)
    {
        struct rq_yield_curve_elem *yce = rq_yield_curve_element_at(yc, i);
        /* if (rq_linked_list_size(output_fields) == 0) */
        {
			char buf[20];
            rq_date dates[1024];
            unsigned num_dates;
            struct rq_term term;
            double zero_rate;
            double par_rate;

            rq_asset_t asset_ccy =
                rq_asset_mgr_get(rq_system_get_asset_mgr(system), rq_yield_curve_get_underlying_asset_id(yc));

            enum rq_day_count_convention day_count = RQ_DAY_COUNT_ACTUAL_365;

            if (asset_ccy)
			{
				if (rq_asset_ccy_get_days_per_year(asset_ccy) == 360)
				{
	                day_count = RQ_DAY_COUNT_ACTUAL_360;
				}
			}
            rq_term_fill(&term, 0, 0, 3, 0);

			rq_date_to_string(buf, "yyyy-mm-dd", yce->date);
			fprintf(fh, "\t%s", buf);
            fprintf(fh, "\t%.12f\t", yce->discount_factor);

            zero_rate = rq_yield_curve_get_simple_rate(yc, yce->date, day_count);

            fprintf(fh, "%.12f\t", zero_rate);

            num_dates = rq_date_roll_generate_dates(
                dates,
                120,
                yc->from_date,
                yce->date,
                &term,
                RQ_ROLL_CONVENTION_NONE,
                RQ_DATE_ROLL_MOD_FOLLOWING,
                NULL,
                0,
                RQ_DATE_ROLL_STUB_POSITION_NONE,
                15, /* allow 15 days error on date creation */
                NULL);

            if (num_dates > 1)
            {
                /* skip the first date */
                par_rate = rq_yield_curve_get_par_rate(yc, num_dates-1, dates+1, 4);
            }
            else
			{
				par_rate = zero_rate;
			}

            fprintf(fh, "%.12f\n", par_rate);
        }
	}
    fprintf(fh, "\n");
#ifdef DEBUG_BOOTSTRAP_FH
    fclose(fh);
#endif
}
#endif

void
setDiscountFactor(rq_yield_curve_t ts, rq_date for_date, double df)
{
#ifdef DEBUG_BOOTSTRAP_FH
    FILE *fh = fopen(debugFile, "a");
	char buf[20];
	rq_date_to_string(buf, "yyyy-mm-dd", for_date);
	fprintf(fh, "\t\tsetDF: \t%s\t%.12f\n",buf,df);
#else
	FILE *fh = stdout;
#endif

	rq_yield_curve_set_discount_factor(ts, for_date, df);

#ifdef DEBUG_BOOTSTRAP
#ifdef DEBUG_BOOTSTRAP_FH
    fclose(fh);
#endif
#endif
}

double
bootstrapAssetCcy(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped)
{
	double ret;
	double days_per_year = rq_asset_ccy_get_days_per_year(asset);
	double r = rq_rate_get_value(rate);

	*last_date_bootstrapped = rq_rate_get_value_date(rate);

    ret = rq_simple_rate_to_discount_factor(
        r,
        rq_rate_get_observation_date(rate),
        *last_date_bootstrapped,
        days_per_year);

	return ret;
}

double
bootstrapAssetIROvernight(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped)
{
	double ret;
	double days_per_year = 365.0; 
	double r = rq_rate_get_value(rate);

	/** 
        \todo Base days_per_year on rq_asset_irovernight_get_day_count_convention(asset); 
     */
    *last_date_bootstrapped = rq_rate_get_value_date(rate);

    ret = rq_simple_rate_to_discount_factor(
        r,
        rq_rate_get_observation_date(rate),
        *last_date_bootstrapped,
        days_per_year
        );

	return ret;
}

double
bootstrapAssetIRDiscount(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped,
						const rq_calendar_t* cals, unsigned short numCals, rq_yield_curve_t ts)
{
	double ret;
    rq_date startDate;
    rq_date maturityDate;
    rq_date maturityDateUnadjusted;
    double r;
    double dayCountFrac;
	struct rq_term year_term;
	enum rq_zero_method zeroMethod = RQ_ZERO_INVALID;
	long zeroMethodCompFreq = 1;

    startDate = rq_yield_curve_get_curve_date(ts);
    //will override what's done in rate factory for start & end date
    startDate = rq_date_roll_offset_term(startDate,
                                         rq_asset_irdiscount_get_start_tenor_type(asset),
                                         rq_asset_irdiscount_get_start_tenor(asset),
                                         cals,
                                         numCals,
                                         rq_asset_irdiscount_get_date_roll_convention(asset));

    maturityDate = rq_date_roll_get_date(startDate, 
                                         rq_asset_irdiscount_get_tenor(asset),
                                         RQ_ROLL_CONVENTION_NONE,
                                         rq_asset_irdiscount_get_date_roll_convention(asset),
                                         cals,
		                                 numCals);

	// this gets the unadjusted maturity date to determine if it's the 1yr point
	// and hence use simple interest instead of compounding 
    maturityDateUnadjusted = rq_date_roll_get_date(startDate,
                                                   rq_asset_irdiscount_get_tenor(asset),
                                                   RQ_ROLL_CONVENTION_NONE,
                                                   RQ_DATE_ROLL_NO_ADJUSTMENT,
                                                   cals,
	                                               numCals);

    dayCountFrac = rq_day_count_get_year_fraction(rq_asset_irdiscount_get_day_count_convention(asset),
                                                  startDate,
                                                  maturityDate);

	r = rq_rate_get_value(rate);
	
	zeroMethod = rq_asset_irdiscount_get_zero_method(asset);

	switch (zeroMethod)
	{
		case RQ_ZERO_SIMPLE:
			ret = 1.0 / (1.0 + r * dayCountFrac); // simple rate
			break;
		case RQ_ZERO_COMPOUNDING:
			zeroMethodCompFreq = rq_asset_irdiscount_get_zero_method_compounding_frequency(asset);
			ret = 1.0 / (pow(1.0 + (r / zeroMethodCompFreq), zeroMethodCompFreq * dayCountFrac));
			break;
		case RQ_ZERO_CONTINUOUS_COMPOUNDING:
			ret = exp(-r * dayCountFrac);
			break;
		case RQ_ZERO_CASH_DEPOSIT:
		default:
			rq_term_fill(&year_term, 0, 0, 0, 1);
			if(rq_date_add_term(startDate, &year_term) < maturityDateUnadjusted)
			{
				ret = 1.0 / pow(1.0 + r, dayCountFrac);
			}
			else // less than 1 year
			{
				ret = 1.0 / (1.0 + r * dayCountFrac); // simple rate
			}
			break;
	}

    if (startDate > ts->from_date)
    {
        //so this is a forward discount factor
        if (rq_yield_curve_get_last_date(ts) < startDate)
            //will extrapolate for now
            ret *= rq_yield_curve_get_discount_factor(ts, startDate);
        else
            ret *= rq_yield_curve_get_discount_factor(ts, startDate);
    }

    *last_date_bootstrapped = maturityDate;
	return ret;
}

// Ref: LCH SwapClear Curve Construction.doc
// This is the Classic Futures Strip bootstrapping method.
// Each point is assumed to terminate at the following futures point. The last point is a dummy.
// TS points are order by settlement + tenor, it assumed this will place the futures point after the relevant cash point, as it is not possible when sorting to know the termination point of the underlying.
// The stub point is inserted into the curve.
double
bootstrapAssetIRFutureStrip(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped, rq_yield_curve_t ts,
                       const rq_calendar_t* cals, unsigned short num_cals, rq_rate_t lastRate, rq_asset_t lastAsset, double lastFutureDF)
{
    // JN Method/Assumptions
    // 1. First future settlement date will stub out the preceding rate using interpolation
    //      or we extraolate to the first future's settlement date.
    // 2. Assumption (TODO) Assume that the underlying bill matures on the delivery date of the next
    //      futures contract. SFE quarterlies actually expire 1 week prior to underlying fut contracts.
    // 3. To compute df(xk+1):
    //      first discount over interval [xk,xk+1] using the forward rate corresponding to the futures price Fk,
    //      then discount over the remaining interval [0,xk] using df(xk)

    double df = 0;
        //  long lastFutureMaturityDate = rq_rate_get_value_date(rate);
    long futureMaturityDate = rq_asset_irfuture_get_settlement_date(asset);

    if (!lastFutureDF)
    {
        // there's no previous future
        rq_date last_date_on_curve = rq_yield_curve_get_last_date(ts);

        if (futureMaturityDate < last_date_on_curve)
        {
            // ok grab the df off the curve
            df = rq_yield_curve_get_discount_factor(ts, futureMaturityDate);
        }
        else // extrapolate to get a rate (same as above for now)
        {
            df = rq_yield_curve_get_discount_factor(ts, futureMaturityDate);
        }
    }
    else // the previous asset was a similar bill future. So, starting at the maturity of the previous
    {
        double lastFuturePrice = 0.0;
        long lastFutureMaturityDate = futureMaturityDate;
        double tau = 0.0;

        enum rq_ir_future_quote_convention lastFutureQuoteConv = rq_asset_irfuture_get_quote_convention(lastAsset);
        lastFutureMaturityDate = rq_asset_irfuture_get_settlement_date(lastAsset);

        tau = rq_day_count_get_year_fraction(
            rq_asset_irfuture_get_day_count_convention(lastAsset),
            lastFutureMaturityDate,
            futureMaturityDate);

        if (lastFutureQuoteConv == RQ_IR_FUTURE_QC_100_MINUS_YIELD)
        {
            lastFuturePrice = rq_rate_get_value(lastRate);
        }
        else if (lastFutureQuoteConv == RQ_IR_FUTURE_QC_YIELD)
        {
            lastFuturePrice = 100*(1 - rq_rate_get_value(lastRate));
        }
        else if (lastFutureQuoteConv == RQ_IR_FUTURE_QC_100_MINUS_DISCOUNT)
        {
            // TODO  when I feel like it
        }

        //      =dfa/(1+0.01*(100-Fa)*(xb-xa))
        df = lastFutureDF / (1 + 0.01*(100-lastFuturePrice)*(tau));
    }
    *last_date_bootstrapped = futureMaturityDate;

    return df;
}

/* JG New implementation to support gaps and overlaps with cash/swap rates. */
/* Each futures point is independent, dummy point at the end is not needed. */
/* The termination point is settlement date + tenor. */
/* Stub point is not inserted into the curve. */
double
bootstrapAssetIRFutureTenor(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped,
                       rq_yield_curve_t ts, const rq_calendar_t* cals, unsigned short num_cals)
{
    double df, futuresPrice, thisRate, extrapolated_start_rate, tau;
    rq_date startDate;   /* start date of underlying bank bill */
    rq_date maturityDate;  /* end date of underlying bank bill */

    startDate = rq_asset_irfuture_get_settlement_date(asset);
    // Assume tenor is already adjusted. eg. AUD BB tenor should be 90 days. No calendar or roll convention should be applied because the settled rate is actually an average of 85-95 day bills.
    maturityDate = rq_date_add_term(startDate, rq_asset_irfuture_get_tenor(asset));

    thisRate = rq_rate_get_value(rate);
    switch(rq_asset_irfuture_get_quote_convention(asset))
    {
        case RQ_IR_FUTURE_QC_100_MINUS_YIELD :
            futuresPrice = thisRate;
            break;
        case RQ_IR_FUTURE_QC_YIELD :
            futuresPrice = 100*(1 - thisRate);
            break;
        case RQ_IR_FUTURE_QC_PRICE_PER_100 :
			// TODO
        case RQ_IR_FUTURE_QC_100_MINUS_DISCOUNT :
			// TODO, not available from front end.
            break;
		}
    // 		=dfa/(1+0.01*(100-Fa)*(xb-xa))
		tau = rq_day_count_get_year_fraction(
			rq_asset_irfuture_get_day_count_convention(asset),
        startDate,
        maturityDate);
    extrapolated_start_rate = rq_yield_curve_get_discount_factor(ts, startDate);
    df = extrapolated_start_rate / (1.0 + 0.01 * (100.0 - futuresPrice) * tau);
    *last_date_bootstrapped = maturityDate;

	return df;
}

// Merged into processSwapsDCF().
void
do_not_use_bootstrapAssetIRSwap(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped, rq_yield_curve_t ts, const rq_calendar_t* base_cals,
					 unsigned short base_num_cals, rq_calendar_mgr_t calendar_mgr,
                     rq_date* last_swap_date, double* last_swap_rate, double * last_swap_period)
{
    /*
      Work out dates that the payments fall on for the swap.
      Calculate discount factors for all the dates except 
      the last date.
    */
    double discount_factors[RQ_YIELD_CURVE_MAX_FACTORS]; /* 4 times a year * 30 years */
    rq_date dates[RQ_YIELD_CURVE_MAX_FACTORS];
    double r = rq_rate_get_value(rate);
    int num_cals;
    rq_calendar_t cals[3];

    rq_date start_date = ts->from_date; /* rq_rate_get_observation_date(rate); */
    double df;
    double dfStart = 1.0;
    int num_dates;
    unsigned short num_discount_factors;
	unsigned short j;
    rq_date last_date_on_curve = rq_yield_curve_get_last_date(ts);
    double swap_period = rq_term_get_annualized_period(rq_asset_irswap_get_frequency(asset));
    double sum_dfs = 0.0;
    rq_date lastDateStrapped;

    num_cals = base_num_cals;
    for(j=0; j < num_cals; j++)
        cals[j] = base_cals[j];
    if(rq_asset_irswap_get_additional_calendar(asset))
    {
        cals[num_cals] = rq_calendar_mgr_get(calendar_mgr, rq_asset_irswap_get_additional_calendar(asset));
        if(cals[num_cals])
            num_cals++;
    }
    start_date = rq_date_roll_offset_term(start_date,
                                          rq_asset_irswap_get_start_tenor_type(asset),
                                          rq_asset_irswap_get_start_tenor(asset),
                                          cals,
                                          num_cals,
                                          rq_asset_irswap_get_date_roll_convention(asset));

    if (start_date > ts->from_date)
        dfStart = rq_yield_curve_get_discount_factor(ts, start_date);

    lastDateStrapped = rq_date_roll_get_date(
        start_date,
        rq_asset_irswap_get_tenor(asset),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(asset),
        cals,
        num_cals
        );

    num_dates = rq_date_roll_generate_dates(
        dates,
        RQ_YIELD_CURVE_MAX_FACTORS,
        start_date,
        lastDateStrapped,
        rq_asset_irswap_get_frequency(asset),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irswap_get_date_roll_convention(asset),
        cals,
        num_cals,
        RQ_DATE_ROLL_STUB_POSITION_NONE,
        15, /* allow 15 days error on date creation */
		NULL
        );

    /* ignore the first date */
    num_discount_factors = (unsigned short)(num_dates - 2);
    for (j = 0; j < num_discount_factors; sum_dfs += discount_factors[j], j++)
    {
        if (dates[j] < last_date_on_curve)
        {
            discount_factors[j] =
                rq_yield_curve_get_discount_factor(
                    ts,
                    dates[j+1]
                    );
        }
        else
        {
            /* need to imply a swap rate for this point */
            if (j > 0)
            {
                double par_rate_prev;
                double par_rate;
                rq_date date_prev;

                if (*last_swap_rate == 0.0 || *last_swap_period != swap_period)
                {
                    /* get the swap rate for this date */
                    if (start_date > ts->from_date)
                    {
                        // need to get forward par rate instead
                        par_rate_prev = 
                            rq_yield_curve_get_forward_par_rate_period(ts, start_date, j, dates+1, swap_period);
                    }
                    else
                    {
                        par_rate_prev =
                            rq_yield_curve_get_par_rate_period(ts, j, dates+1, swap_period);
                    }
                    date_prev = dates[j];
                }
                else
                {
                    par_rate_prev = *last_swap_rate;
                    date_prev = *last_swap_date;
                }


                /* now linearly interpolate a new swap rate for
                   the date we actually want */
                par_rate = rq_interpolate_linear(
                    (double)dates[j+1],
                    (double)date_prev,
                    par_rate_prev,
                    (double)lastDateStrapped,
                    r
                    );

                /* now add the discount factor implied by this
                   par rate to the yield curve */
                df = (dfStart - (par_rate * swap_period * sum_dfs)) / (1.0 + par_rate * swap_period);
                /* This is too slow and is O(n^2), replaced by above formula.
                df = rq_discount_factor_from_par_rate(
                    (par_rate * swap_period),
                    j,
                    discount_factors
                    );
                */

                setDiscountFactor(ts, dates[j+1], df);
                
                discount_factors[j] = df;
            }
        }
    }

    df = rq_discount_factor_from_par_rate(
        r * swap_period,
        num_discount_factors,
        discount_factors
        );

    if (start_date > ts->from_date)
    {
        // df was based on forward par rate, need to bring it back to today
        df = df - (1.0 - dfStart)/(1.0 + r * swap_period);
    }

    setDiscountFactor(ts, lastDateStrapped, df);


    *last_swap_date = lastDateStrapped;
    *last_swap_rate = r;
    *last_swap_period = swap_period;
	*last_date_bootstrapped = lastDateStrapped;
}

void
bootstrapAssetIRBond(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped, rq_yield_curve_t ts, const rq_calendar_t* cals, unsigned short num_cals)
{
    /*
      Work out dates that the payments fall on for the bond.
      Calculate discount factors for all the dates except 
      the last date.
    */
    double discount_factors[RQ_YIELD_CURVE_MAX_FACTORS]; /* 4 times a year * 30 years */
    rq_date dates[RQ_YIELD_CURVE_MAX_FACTORS];
    double r = rq_rate_get_value(rate);

    rq_date start_date = rq_rate_get_observation_date(rate); // <-- shouldn't this be ts->from_date?
    rq_date end_date = rq_asset_irbond_get_maturity_date(asset);

    double df;
	int j;

    int num_dates = rq_date_roll_generate_dates(
        dates,
        80,
        start_date,
        end_date,
        rq_asset_irbond_get_frequency(asset),
        RQ_ROLL_CONVENTION_NONE,
        rq_asset_irbond_get_date_roll_convention(asset),
        cals,
        num_cals,
        RQ_DATE_ROLL_STUB_POSITION_NONE,
        15, /* allow 15 days error on date creation */
        NULL
		);

    unsigned short num_discount_factors = (unsigned short)(num_dates - 1);

    /* get discount factors for the number of dates */
    for (j = 0; j < num_discount_factors; j++)
    {
        discount_factors[j] = 
            rq_yield_curve_get_discount_factor(
                ts,
                dates[j]
                );
    }

    df = rq_discount_factor_from_par_rate(
        r * rq_term_get_annualized_period(rq_asset_irswap_get_frequency(asset)),
        num_discount_factors,
        discount_factors
        );

    rq_yield_curve_set_discount_factor(ts, end_date, df);
    *last_date_bootstrapped = end_date;
}

double
bootstrapAssetCreditSpread(const rq_asset_t asset, const rq_rate_t rate, rq_date* last_date_bootstrapped)
{
	/** 
     * \todo Instead of storing the credit spreads in a yield
     * curve, I need to have a credit spread curve object.
     * That way I won't need to convert to discount factors
     * and back again....
     */
	double ret;
    double r = rq_rate_get_value(rate);

	*last_date_bootstrapped = rq_rate_get_value_date(rate);

    ret = rq_simple_rate_to_discount_factor(
        r,
        rq_rate_get_observation_date(rate),
        *last_date_bootstrapped,
        365
        );

	return ret;
}

