/**
 * @file
 *
 * Pricing models for partial barrier options
 */
/*
** rq_pricing_partial_barrier.h
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
**
** Heynan and Kat (1994) - Haug 'The Complete Guide to Option Pricing Formula' 
**					       2nd Edition - p160-162.
**
*/
#ifndef rq_pricing_partial_barrier_h
#define rq_pricing_partial_barrier_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double 
rq_pricing_partial_barrier_early_end(
    short in,
    short call,
    double S, 
    double X, 
    double H,
    double r, 
    double rf, 
    double v,
    double t1, 
    double T2 
    );

RQ_EXPORT double 
rq_pricing_partial_barrier_delayed_start_up_or_down(
    short in,
    short call,
    double S, 
    double X, 
    double H,
    double r, 
    double rf, 
    double v,
    double t1, 
    double t2 
    );

RQ_EXPORT double 
rq_pricing_partial_barrier_delayed_start(
    short in,
    short call,
    double S, 
    double X, 
    double H,
    double r, 
    double b, 
    double v,
    double t1, 
    double t2
    );

RQ_EXPORT double 
rq_pricing_partial_barrier_early_end_greeks(
    short in,
    short call,
    double S, 
    double X, 
    double H,
    double r, 
    double rf, 
    double v,
    double t1, 
    double t2,
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

RQ_EXPORT double 
rq_pricing_partial_barrier_delayed_start_greeks(
    short in,
    short call,
    double S, 
    double X, 
    double H,
    double r, 
    double rf, 
    double v,
    double t1, 
    double t2,
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
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
