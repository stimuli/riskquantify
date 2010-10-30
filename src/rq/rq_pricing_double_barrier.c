/*
** rq_pricing_double_barrier.c
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
#include "rq_config.h"
#include <math.h>
#include "rq_pricing_normdist.h"
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_double_barrier.h"

RQ_EXPORT double 
rq_pricing_double_barrier(
    short in,
    short call,
    double S,
    double X,
    double L,
    double H,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d
    )
{
    double delta1 = 0.0;
    double delta2 = 0.0;
    double b = r_dom - r_for;
    double E = L * exp(delta1 * tau_e);
    double F = H * exp(delta1 * tau_e);
    double sum1 = 0.0;
    double sum2 = 0.0;
    double value = 0.0;
	double prevValue = 0.0;
    int n = 0;

    double ret = 0.0;
	int max = 6;
	double d1;
	double d2;
	double d3;
	double d4;
	double mu1;
	double mu2;
	double mu3;
	double diff = 0.0;

	double lvalue = 0.0;
	double a1 = 0.0;
	double a2 = 0.0;

	n = 0;
    if (call)
    {
		do
		{
			d1 = (log(S * pow(H, (2.0 * n)) / (X * pow(L, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));
			d2 = (log(S * pow(H, (2.0 * n)) / (F * pow(L, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));
			d3 = (log(pow(L, (2.0 * n + 2.0)) / (X * S * pow(H, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));
			d4 = (log(pow(L, (2.0 * n + 2.0)) / (F * S * pow(H, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));

			mu1 = 2.0 * (b - delta2 - n * (delta1 - delta2)) / (sigma * sigma) + 1;
			mu2 = 2.0 * n * (delta1 - delta2) / (sigma * sigma);
			mu3 = 2.0 * (b - delta2 + n * (delta1 - delta2)) / (sigma * sigma) + 1;

			sum1 = sum1 + pow((pow(H, n) / pow(L, n)), mu1) * pow((L / S), mu2) * (rq_pricing_cumul_norm_dist(d1) - rq_pricing_cumul_norm_dist(d2)) - pow((pow(L, (n + 1)) / (pow(H, n) * S)), mu3) * (rq_pricing_cumul_norm_dist(d3) - rq_pricing_cumul_norm_dist(d4));
			sum2 = sum2 + pow((pow(H, n) / pow(L, n)), (mu1 - 2.0)) * pow((L / S), mu2) * (rq_pricing_cumul_norm_dist(d1 - sigma * sqrt(tau_e)) - rq_pricing_cumul_norm_dist(d2 - sigma * sqrt(tau_e))) - pow((pow(L, (n + 1)) / (pow(H, n) * S)), (mu3 - 2.0)) * (rq_pricing_cumul_norm_dist(d3 - sigma * sqrt(tau_e)) - rq_pricing_cumul_norm_dist(d4 - sigma * sqrt(tau_e)));

			if (n >= 0) 
			{
				n++;				
				prevValue = value;
				value = S * exp(-r_for * tau_e) * sum1 - X * exp(-r_dom * tau_e) * sum2;
				
				diff = fabs(value - prevValue);
			}
			n *= - 1;

		} while (fabs(n) < max && diff > 0.0000005);
    }
    else
    {
        do
		{
            double d1 = (log(S * pow(H, (2.0 * n)) / (E * pow(L, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));
            double d2 = (log(S * pow(H, (2.0 * n)) / (X * pow(L, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));
            double d3 = (log(pow(L, (2.0 * n + 2.0)) / (E * S * pow(H, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));
            double d4 = (log(pow(L, (2.0 * n + 2.0)) / (X * S * pow(H, (2.0 * n)))) + (b + (sigma * sigma) / 2.0) * tau_e) / (sigma * sqrt(tau_e));
            double mu1 = 2.0 * (b - delta2 - n * (delta1 - delta2)) / (sigma * sigma) + 1;
            double mu2 = 2.0 * n * (delta1 - delta2) / (sigma * sigma);
            double mu3 = 2.0 * (b - delta2 + n * (delta1 - delta2)) / (sigma * sigma) + 1;
            sum1 = sum1 + pow((pow(H, n) / pow(L, n)), mu1) * pow((L / S), mu2) * (rq_pricing_cumul_norm_dist(d1) - rq_pricing_cumul_norm_dist(d2)) - pow((pow(L, (n + 1)) / (pow(H, n) * S)), mu3) * (rq_pricing_cumul_norm_dist(d3) - rq_pricing_cumul_norm_dist(d4));
            sum2 = sum2 + pow((pow(H, n) / pow(L, n)), (mu1 - 2.0)) * pow((L / S), mu2) * (rq_pricing_cumul_norm_dist(d1 - sigma * sqrt(tau_e)) - rq_pricing_cumul_norm_dist(d2 - sigma * sqrt(tau_e))) - pow((pow(L, (n + 1)) / (pow(H, n) * S)), (mu3 - 2.0)) * (rq_pricing_cumul_norm_dist(d3 - sigma * sqrt(tau_e)) - rq_pricing_cumul_norm_dist(d4 - sigma * sqrt(tau_e)));
        	
			if (n >= 0) 
			{
				n++;				
				prevValue = value;
				value = X * exp(-r_dom * tau_e) * sum2 - S * exp(-r_for * tau_e) * sum1;
				diff = fabs(value - prevValue);
			}
			n *= - 1;

		} while (fabs(n) < max && diff > 0.0000005);
	}

    if (!in)
        ret = value;
    else
        ret = rq_pricing_blackscholes(call, S, X, r_dom, r_for, sigma, tau_e, tau_d) - value;

    return ret;
}

RQ_EXPORT double 
rq_pricing_double_barrier_greeks(
    short in,
    short call,
    double S,
    double X,
    double L,
    double H,
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
	double base_price = rq_pricing_double_barrier(in, call, S, X, L, H, r_dom, r_for, sigma, tau_e, tau_d); 
	double delta_price = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price = rq_pricing_double_barrier(in, call, S + delta_shift, X, L, H, r_dom, r_for, sigma, tau_e, tau_d);
		d1 = delta_price - base_price;
		
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_double_barrier(in, call, S + 2 * delta_shift, X, L, H, r_dom, r_for, sigma, tau_e, tau_d);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_double_barrier(in, call, S, X, L, H, r_dom, r_for, sigma + vega_shift, tau_e, tau_d);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_double_barrier(in, call, S, X, L, H, r_dom, r_for, sigma, tau_e - theta_shift, tau_d - theta_shift);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_double_barrier(in, call, S, X, L, H, r_dom + r_dom_shift, r_for, sigma, tau_e, tau_d);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_double_barrier(in, call, S, X, L, H, r_dom, r_for + r_for_shift, sigma, tau_e, tau_d);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}
