/*
** rq_rate_conversions.c
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
#include <math.h>
#include "rq_config.h"
#include "rq_rate_conversions.h"

RQ_EXPORT double 
rq_rate_simple_to_continuous(
    double r, 
    double days,
    double days_in_year
    )
{
    double af = 1.0 + r * days / days_in_year;
    return log(af) / (days / 365.0);
}

RQ_EXPORT double 
rq_rate_simple_to_discount(
    double r, 
    double days,
    double days_in_year
    )
{
    return 1.0 / (1.0 + r * days / days_in_year);
}

RQ_EXPORT double 
rq_rate_compounding_to_continuous(
    double r,
    double days,
    double days_in_comp_period
    )
{
    double af = pow(1.0 + r, days / days_in_comp_period);
    return log(af) / (days / 365.0);
}

RQ_EXPORT double 
rq_rate_compounding_to_discount(
    double r,
    double days,
    double days_in_comp_period
    )
{
    return 1.0 / pow(1.0 + r, days / days_in_comp_period);
}

RQ_EXPORT double 
rq_rate_continuous_to_discount(
    double r,
    double days,
    double days_in_year
    )
{
    return exp(-r * (days / days_in_year));
}

RQ_EXPORT double
rq_rate_discount_to_zero(
    double df,
	enum rq_zero_method zeroM,
	unsigned int zm_compFreq,
    double year_frac
    )
{
    double ret;
	switch (zeroM)
    {
        case RQ_ZERO_SIMPLE:
			ret = ((1.0 / df) - 1.0) * (1.0 / year_frac);
			break;
		case RQ_ZERO_COMPOUNDING:
			ret = (pow(1.0 / df, 1.0 / (zm_compFreq * year_frac)) - 1.0) * zm_compFreq;
			break;
		case RQ_ZERO_CONTINUOUS_COMPOUNDING:
			ret = -log(df) / year_frac;
			break;
		case RQ_ZERO_CASH_DEPOSIT:
		default:
		    if (year_frac > 1.0)
		    {
		        /* return an annual compounding zero rate. */
		        ret = pow(1.0 / df, 1.0 / year_frac) - 1.0;			
			}
		    else
		    {
				/* return a simple zero rate. */
		        ret = ((1.0 / df) - 1.0) * (1.0 / year_frac);
		    }
			break;

	}
    return ret;
}

RQ_EXPORT double
rq_rate_zero_to_discount(
    double rate,
	enum rq_zero_method zeroM,
	unsigned int zm_compFreq,
    double year_frac
    )
{
    double ret;
	switch (zeroM)
    {
        case RQ_ZERO_SIMPLE:
			ret = 1.0 / (1.0 + (rate * year_frac));
			break;
		case RQ_ZERO_COMPOUNDING:
			ret = 1.0 / pow(1.0 + rate / zm_compFreq, zm_compFreq * year_frac);
			break;
		case RQ_ZERO_CONTINUOUS_COMPOUNDING:
			ret = exp(-rate * year_frac);
			break;
		case RQ_ZERO_CASH_DEPOSIT:
		default:
		    if (year_frac > 1.0)
		    {
				/* return discount factor from an annual compounding zero rate. */
		        ret = 1.0 / pow(1.0 + rate, year_frac);
			}
		    else
		    {
				/* return a discount factor from a simple zero rate. */
		        ret = 1.0 / (1.0 + (rate * year_frac));
		    }
			break;

	}
    return ret;
}

