/*
** rq_pricing_fx.c
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
#include "rq_pricing_fx.h"

/* note: this function values to the spot date
 */
RQ_EXPORT double 
rq_pricing_fx_ccir(
	double S, /* quoted in domestic over foreign terms */
	double K, /* quoted in domestic over foreign terms */
    double r_dom, /* continuously compounded, the numerator currency */
    double r_for, /* continuously compounded, the denominator currency */
    double tau_d /* days from spot to delivery, over 365.0 */
 )
{
    if (tau_d > 0)
    {
		double df_dom = exp(-r_dom * tau_d);
		double df_for = exp(-r_for * tau_d);

        /* OK, at the forward date, I exchange 1 unit of the foreign currency
           for X units of the domestic currency.
           If I discount these both back to today, I get K * df_dom / df_for
           units.
        */
        double c = K * df_dom / df_for;
        return c - S;
    }
    else if (tau_d == 0)
        return K - S;
	else
		return 0.0;
}


RQ_EXPORT double 
rq_pricing_fx(
	double S, /**< The spot rate, quoted in domestic over foreign terms */
	double K, /**< The agreed exchange rate, quoted in domestic over foreign terms */
    double df_dom, /**< discount factor, the numerator currency */
    double df_for /**< discount factor, the denominator currency */
    )
{
    /* 
       OK, at the forward date, I exchange 1 unit of the foreign currency
       for X units of the domestic currency.
       If I discount these both back to today, I get K * df_dom / df_for
       units.
    */
    double c = K * df_dom / df_for;
    return c - S;
}

