/*
** rq_pricing_fra.c
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
#include "rq_pricing_fra.h"

RQ_EXPORT double 
rq_pricing_fra_arrears(double r_x, double r_s, double tau_s, double r_d, double tau_d)
{
    /* OK, a fra is based on the implied forward interest rate */
    /* af_2 = af_1 * af_1_2 => af_1_2 = af_2 / af_1 */
    double fwd_af = exp(r_d * tau_d) / exp(r_s * tau_s);
    double strike_af = exp(r_x * (tau_d - tau_s));
    double value = fwd_af - strike_af;
    /* discount value back to today */
    return value * exp(-r_d * tau_d);
}

RQ_EXPORT double 
rq_pricing_fra_advance(double r_x, double r_s, double tau_s, double r_d, double tau_d)
{
    /* OK, a fra is based on the implied forward interest rate */
    /* af_2 = af_1 * af_1_2 => af_1_2 = af_2 / af_1 */
    /* => df_1_2 = 1 / af_1_2 */
    /* => df_1_2 = af_1 / af_2 */
    double fwd_df = exp(r_s * tau_s) / exp(r_d * tau_d);
    double strike_df = exp(-r_x * (tau_d - tau_s));
    double value = fwd_df - strike_df;
    /* discount value back to today */
    return value * exp(-r_s * tau_s);
}
