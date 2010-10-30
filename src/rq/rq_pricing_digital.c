/*
** rq_pricing_digital.c
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
#include "rq_pricing_normdist.h"
#include "rq_pricing_digital.h"

RQ_EXPORT double
rq_pricing_digital(
    short cash, /* indicates whether payout is in domestic currency or not */
    short call, /* if call, S > X to achieve payout, otherwise X > S for pay */
	double S,
	double X,
	double r_dom,
	double r_for,
	double sigma,
    double tau_e,
    double tau_d
 ) 
{
    double m = (call ? 1.0 : -1.0);
    double f = S * exp(r_dom * tau_d) / exp(r_for * tau_d);
    double sigma_tau_sqrt = sigma * sqrt(tau_e);

    double d1 = 
        (log(f / X) + (0.5 * sigma * sigma * tau_e)) / 
        sigma_tau_sqrt;

    double d2 = (d1 - sigma_tau_sqrt) * m;
    d1 *= m;

    if (!cash)
        return S * exp(-r_for * tau_d) * rq_pricing_cumul_norm_dist(d1);
    else
        return X * exp(-r_dom * tau_d) * rq_pricing_cumul_norm_dist(d2);
}

RQ_EXPORT double
rq_pricing_digital_greeks(
    short cash, /* indicates whether payout is in domestic currency or not */
    short call, /* if call, S > X to achieve payout, otherwise X > S for pay */
	double S,
	double X,
	double r_dom,
	double r_for,
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
	double base_price = rq_pricing_digital(cash, call, S, X, r_dom, r_for, sigma, tau_e, tau_d); 
	double delta_price = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price = rq_pricing_digital(cash, call, S + delta_shift, X, r_dom, r_for, sigma, tau_e, tau_d);
		d1 = delta_price - base_price;
		
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_digital(cash, call, S + 2 * delta_shift, X, r_dom, r_for, sigma, tau_e, tau_d);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_digital(cash, call, S, X, r_dom, r_for, sigma + vega_shift, tau_e, tau_d);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_digital(cash, call, S, X, r_dom, r_for, sigma, tau_e - theta_shift, tau_d - theta_shift);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_digital(cash, call, S, X, r_dom + r_dom_shift, r_for, sigma, tau_e, tau_d);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_digital(cash, call, S, X, r_dom, r_for + r_for_shift, sigma, tau_e, tau_d);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}
