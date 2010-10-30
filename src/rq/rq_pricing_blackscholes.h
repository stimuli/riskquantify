/**
 * \file rq_pricing_blackscholes.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_blackscholes files provide Black-Scholes
 * pricing routines.
 */
/*
** rq_pricing_blackscholes.h
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
#ifndef rq_pricing_blackscholes_h
#define rq_pricing_blackscholes_h


#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double 
rq_pricing_blackscholes(
    int call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
    double r_dom, /* aka the numerator rate (continuously compounded) */
    double r_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
    double tau_e,
    double tau_d
    );

/**
 * Generalized Black/Scholes
 */
/* Black (1976)  S(spot)=F(fwd) and b = 0
   Merton (1973) b=r-q where q is continous dividend yield
*/
RQ_EXPORT double 
rq_pricing_blackscholes_gen(
    int call, 
    double S, 
    double X, 
    double T, 
    double r, 
    double b, 
    double v
    );

/* Calculates greeks. It's the same as garman-kohlagen. PDE of Black 76. 
   Black (1976)  S(spot)=F(fwd) and b = 0
   Merton (1973) b=r-q where q is continous dividend yield
*/
RQ_EXPORT double 
rq_pricing_blackscholes_gen_greeks(
    int call, 
    double S, 
    double X, 
    double tau, 
    double r, 
    double b, 
    double sigma,
	double *delta,
	double *gamma,
	double *vega,
	double *rho,
	double *theta
    );

RQ_EXPORT double 
rq_pricing_garmankhol(
    int call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
    double df_dom, /* aka the numerator discount factor */
    double df_for, /* aka the denominator discount factor */
    double sigma,
    double tau_e,
    double tau_d  /* NOT USED, misleading !! */
    );

RQ_EXPORT double 
rq_greeks_garmankhol(
    int call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
    double df_dom, /* aka the numerator rate (continuously compounded) */
    double df_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
    double tau_e,
    double tau_d, /* NOT USED, misleading !! */
	double *delta,
	double *gamma,
	double *vega,
	double *rho_dom,
	double *rho_for,
	double *theta
    );

RQ_EXPORT double 
rq_pricing_blackscholes_delta(
    int call,
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
    );

RQ_EXPORT double 
rq_pricing_blackscholes_gamma(
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
    );

RQ_EXPORT double 
rq_pricing_blackscholes_theta(
    int call,
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
    );

RQ_EXPORT double 
rq_pricing_blackscholes_vega(
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
    );

RQ_EXPORT double
rq_pricing_blackscholes_rho(
    int call,
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
 );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
