/*
** rq_pricing_digital_barrier.c
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
#include "rq_config.h"
#include <math.h>
#include "rq_pricing_normdist.h"
#include "rq_pricing_digital_barrier.h"

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
    )
{
    short up = B > S;
    short down = !up;
    short asset = !cash;
    short out = !in;
    short at_expiry = !at_hit;
    double eta = (up ? -1 : 1);
    double phi = (in ? -eta : eta);
    double b = r_dom - r_for;
    double v_sqr = sigma * sigma;
    double v_sqrt_T = sigma * sqrt(tau_e);
    double mu = (b - v_sqr / 2.0) / v_sqr;
    double mu_plus_1 = mu + 1.0;
    double lambda = sqrt((mu * mu) + 2.0 * r_dom / v_sqr);
    double X1 = log(S / X) / v_sqrt_T + mu_plus_1 * v_sqrt_T;
    double X2 = log(S / B) / v_sqrt_T + mu_plus_1 * v_sqrt_T;
    double y1 = log((B * B) / (S * X)) / v_sqrt_T + mu_plus_1 * v_sqrt_T;
    double y2 = log(B / S) / v_sqrt_T + mu_plus_1 * v_sqrt_T;
    double Z = log(B / S) / v_sqrt_T + lambda * v_sqrt_T;
    
    double a1 = S * exp((b - r_dom) * tau_d) * rq_pricing_cumul_norm_dist(phi * X1);
    double b1 = X * exp(-r_dom * tau_d) * rq_pricing_cumul_norm_dist(phi * X1 - phi * v_sqrt_T);
    double a2 = S * exp((b - r_dom) * tau_d) * rq_pricing_cumul_norm_dist(phi * X2);
    double b2 = X * exp(-r_dom * tau_d) * rq_pricing_cumul_norm_dist(phi * X2 - phi * v_sqrt_T);
    /*
    double a3 = S * exp((b - r_dom) * T) * pow((B / S), (2 * mu_plus_1)) * rq_pricing_cumul_norm_dist(eta * y1);
    double b3 = X * exp(-r_dom * T) * pow((B / S), (2 * mu)) * rq_pricing_cumul_norm_dist(eta * y1 - eta * v_sqrt_T);
    */
    double a4 = S * exp((b - r_dom) * tau_d) * pow((B / S), (2 * mu_plus_1)) * rq_pricing_cumul_norm_dist(eta * y2);
    double b4 = X * exp(-r_dom * tau_d) * pow((B / S), (2 * mu)) * rq_pricing_cumul_norm_dist(eta * y2 - eta * v_sqrt_T);
    double a5 = X * (pow((B / S), (mu + lambda)) * rq_pricing_cumul_norm_dist(eta * Z) + pow((B / S), (mu - lambda)) * rq_pricing_cumul_norm_dist(eta * Z - 2 * eta * lambda * v_sqrt_T));

    double value = 0.0;

    if (down && in && cash && at_hit)
        value = a5;
    else if (up && in && cash && at_hit)
        value = a5;
    else if (down && in && asset && at_hit)
        value = a5;
    else if (up && in && asset && at_hit)
        value = a5;
    else if (down && in && cash && at_expiry)
        value = b2 + b4;
    else if (up && in && cash && at_expiry)
        value = b2 + b4;
    else if (down && in && asset && at_expiry)
        value = a2 + a4;
    else if (up && in && asset && at_expiry)
        value = a2 + a4;
    else if (down && out && cash)
        value = b2 - b4;
    else if (up && out && cash)
        value = b2 - b4;
    else if (down && out && asset)
        value = a2 - a4;
    else if (up && out && asset)
        value = a2 - a4;

    return value;
}

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
	)
{
	double base_price = rq_pricing_digital_barrier(cash, at_hit, in, S, X, B, r_dom, r_for, sigma, tau_e, tau_d); 
	double delta_price = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price = rq_pricing_digital_barrier(cash, at_hit, in, S + delta_shift, X, B, r_dom, r_for, sigma, tau_e, tau_d);  
		d1 = delta_price - base_price;
		
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_digital_barrier(cash, at_hit, in, S + 2 * delta_shift, X, B, r_dom, r_for, sigma, tau_e, tau_d);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_digital_barrier(cash, at_hit, in, S, X, B, r_dom, r_for, sigma + vega_shift, tau_e, tau_d);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_digital_barrier(cash, at_hit, in, S, X, B, r_dom, r_for, sigma, tau_e - theta_shift, tau_d - theta_shift);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_digital_barrier(cash, at_hit, in, S, X, B, r_dom + r_dom_shift, r_for, sigma, tau_e, tau_d);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_digital_barrier(cash, at_hit, in, S, X, B, r_dom, r_for + r_for_shift, sigma, tau_e, tau_d);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}
