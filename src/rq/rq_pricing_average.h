/**
 * \file rq_pricing_average.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_average files contain average rate/strike
 * object pricing routines.
 */
/*
** rq_pricing_average.h
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
#ifndef rq_pricing_average_h
#define rq_pricing_average_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double 
rq_pricing_average_rate_geometric(
    short call, 
    double S, 
    double SA, /* current average */
    double X,
    double r, 
    double rf, 
    double v,
    double T, /* time from start to expiry */
    double T2 /* time from today to expiry */
    );

RQ_EXPORT double 
rq_pricing_average_rate_geometric_greeks(
    short call, 
    double S, 
    double SA, /* current average */
    double X,
    double r, 
    double rf, 
    double v,
    double T, /* time from start to expiry */
    double T2, /* time from today to expiry */
	double* delta,
	double delta_shift,
	double* gamma,
	double gamma_shift,
	double* vega,
	double vega_shift,
	double* theta,
	double theta_shift,
	double* rho,
	double rho_shift
    );

RQ_EXPORT double 
rq_pricing_average_rate_turnbullwakeman(
    short call, 
    double S, 
    double SA, 
    double X,
    double r, 
    double rf, 
    double v,
    double T,  /* time from start to expiry */
    double T2, /* time from today to expiry */
    double T_period
    );

RQ_EXPORT double 
rq_pricing_average_rate_turnbullwakeman_greeks(
    short call, 
    double S, 
    double SA, 
    double X,
    double r, 
    double rf, 
    double v,
    double T,  /* time from start to expiry */
    double T2, /* time from today to expiry */
    double T_period,
	double* delta,
	double delta_shift,
	double* gamma,
	double gamma_shift,
	double* vega,
	double vega_shift,
	double* theta,
	double theta_shift,
	double* rho,
	double rho_shift
    );

RQ_EXPORT double 
rq_pricing_average_rate_levy(
    short call, 
    double S, 
    double SA, 
    double X, 
    double r, 
    double b, 
    double v,
    double T, 
    double T2 
    );

RQ_EXPORT double 
rq_pricing_average_rate_levy_greeks(
    short call, 
    double S, 
    double SA, 
    double X, 
    double r, 
    double b, 
    double v,
    double T, 
    double T2,
	double* delta,
	double delta_shift,
	double* gamma,
	double gamma_shift,
	double* vega,
	double vega_shift,
	double* theta,
	double theta_shift,
	double* rho,
	double rho_shift
    );

RQ_EXPORT double 
rq_pricing_average_rate(
	short call,
    int num_ratesets,
	double *rateset_dates,
    double *rateset_weights,
	double S,
	double X,
	double r_dom,
	double r_for,
	double sigma,
    double tau_e,
    double tau_d
    );


RQ_EXPORT double 
rq_pricing_average_strike(
	short call,
    int num_ratesets,
	double *rateset_dates,
    double *rateset_weights,
	double S,
	double r_dom,
	double r_for,
	double sigma,
    double tau_e,
    double tau_d
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
