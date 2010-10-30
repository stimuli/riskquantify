/**
 * \file rq_pricing_digital_barrier.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_digital_barrier files provide digital barrier
 * option pricing routines.
 */
/*
** rq_pricing_digital_barrier.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#ifndef rq_pricing_digital_barrier_h
#define rq_pricing_digital_barrier_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely for indenting
#endif 
#endif

RQ_EXPORT double 
rq_pricing_digital_barrier(
    short cash,
    short at_hit,
	short in, 
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
	double B, /* assume this rate is passed in domestic over foreign terms */
    double r_dom, /* aka the numerator rate (continuously compounded) */
    double r_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
	double tau_e,
	double tau_d
    );

RQ_EXPORT double
rq_pricing_digital_barrier_greeks(
    short cash,
    short at_hit,
	short in, 
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
	double B, /* assume this rate is passed in domestic over foreign terms */
    double r_dom, /* aka the numerator rate (continuously compounded) */
    double r_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
	double tau_e,
	double tau_d,
	double* delta,
	double delta_shift,
	double* gamma, 
	double gamma_shift,
	double* vega,
	double vega_shift,
	double* theta,
	double theta_shift,
	double* rho_dom,
	double r_dom_shift,
	double* rho_for,
	double r_for_shift
	);

#ifdef __cplusplus
#if 0
{ // purely for indenting
#endif 
}
#endif

#endif
