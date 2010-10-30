/**
 * \file rq_rate_conversions.h
 * \author Brett Hutley
 *
 * \brief The rq_rate_conversions files provide helper functions that
 * convert interest rates into various forms.
 */
/*
** rq_rate_conversions.h
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
#ifndef rq_rate_conversions_h
#define rq_rate_conversions_h

#include "rq_config.h"
#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double 
rq_rate_simple_to_continuous(
    double r, 
    double days,
    double days_in_year
    );

RQ_EXPORT double 
rq_rate_simple_to_discount(
    double r, 
    double days,
    double days_in_year
    );

RQ_EXPORT double 
rq_rate_compounding_to_continuous(
    double r,
    double days,
    double days_in_comp_period
    );

RQ_EXPORT double 
rq_rate_compounding_to_discount(
    double r,
    double days,
    double days_in_comp_period
    );

RQ_EXPORT double 
rq_rate_continuous_to_discount(
    double r,
    double days,
    double days_in_year
    );

/** Convert a discount factor to an annual compounding zero-coupon rate.
 */
RQ_EXPORT double
rq_rate_discount_to_zero(
    double df, /**< discount factor. */
	enum rq_zero_method zeroM, /**< definition of zero rate */
	unsigned int zm_compFreq, /**< compound frequency (#times in a year) */
    double year_frac /**< time in years (usually acquired via a rq_day_count_get_year_fraction() call. */
    );

/** Convert an annual compounding zero-coupon rate to a discount factor.
 */
RQ_EXPORT double
rq_rate_zero_to_discount(
    double rate, /**< Annually compounded zero-coupon rate. */
	enum rq_zero_method zeroM, /**< definition of zero rate */
	unsigned int zm_compFreq, /**< compound frequency (#times in a year) */
    double year_frac /**< time in years (usually acquired via a rq_day_count_get_year_fraction() call. */
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
