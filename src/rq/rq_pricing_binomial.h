/**
 * \file rq_pricing_binomial.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_binomial files contain binomial pricing
 * routines.
 */
/*
** rq_pricing_binomial.h
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
#ifndef rq_pricing_binomial_h
#define rq_pricing_binomial_h

#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double
rq_pricing_binomial(
    short call, /**< non-zero value for a call option on foreign asset */
    double S, /**< The spot rate, passed in domestic over foreign terms */
    double X, /**< The strike rate, passed in domestic over foreign terms */
    double r_dom, /**< The continuously compounded domestic interest rate, aka the numerator rate */
    double r_for, /**< The continuously compounded foreign interest rate, aka the denominator rate */
    double sigma, /**< The annualized volatility */
    double tau_e, /**< Time from today to expiry in years */
    double tau_d, /**< Time from spot to delivery in years */
    int num_iters, /**< Number of iterations in binomial tree */
    double *vals /**< An array of doubles for performing calculations. Must be at least num_iters+1 in size. */
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
