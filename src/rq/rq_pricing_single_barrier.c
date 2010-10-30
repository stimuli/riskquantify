/*
** rq_pricing_single_barrier.c
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
#include <stdio.h>
#include "rq_config.h"
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_normdist.h"
#include "rq_pricing_single_barrier.h"
#include "rq_defs.h"

RQ_EXPORT double 
rq_pricing_single_barrier
 (
	short in, 
    short call,
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
	int eta = (S > B ? 1 : -1);
    int fi = (call ? 1 : -1);

    int S_gt_B = (S > B);
    int X_gt_B = (X > B);

    int b_over = ((S_gt_B == X_gt_B) && (X_gt_B == 0));
    
	int a2 = (call ? !X_gt_B : X_gt_B);
    int a3 = (S_gt_B == X_gt_B);
    int a4 = a2;

    double f = S * exp(r_dom * tau_d) / exp(r_for * tau_d);
    double b = log(f / S) / tau_d;
	double z = 1 - X / B;

    double df_dom = exp(-r_dom * tau_d);
    double df_for = exp(-r_for * tau_d);

    double v1;
    double v2;
    double v3;
    double v4;

    double v_sq = sigma * sigma;
    double one_p_b_m_half_vsq_o_vsq = 1.0 + (b - 0.5 * v_sq) / v_sq;
    double v_sqrt_t = sigma * sqrt(tau_e);
    double d1_S_B = ((log(S / B) + (b * tau_d) + (v_sq / 2.0) * tau_e) / v_sqrt_t);
    double d1_B_S = ((log(B / S) + (b * tau_d) + (v_sq / 2.0) * tau_e) / v_sqrt_t);
	double d1_S_X = ((log(S / X) + (b * tau_d) + (v_sq / 2.0) * tau_e) / v_sqrt_t);

	int a1 = 
        (in  && ((S_gt_B && !X_gt_B) || (!S_gt_B && X_gt_B))) || /* xor */
        (!in && (S_gt_B == X_gt_B)); /* and (both zero or both one is true */

    if (a2)
        if ((in && call && S_gt_B) ||
            (in && !call && !S_gt_B) ||
            (!in && call && !S_gt_B) ||
            (!in && !call && S_gt_B))
            a2 = -1;

    if (a3)
        if ((in && call && b_over) || 
            (in && !call && !b_over) ||
            (!in && call && !b_over) ||
            (!in && !call && b_over))
            a3 = -1;
            
    if (!in && a4)
        a4 = -1;

    v1 = (a1 * rq_pricing_blackscholes(call, S, X, r_dom, r_for, sigma, tau_e, tau_d));

    v2 = ((double)a2) * ((double)fi) * 
        ((df_for * S * rq_pricing_cumul_norm_dist(d1_S_B * fi)) - 
         (df_dom * X * rq_pricing_cumul_norm_dist((d1_S_B - v_sqrt_t) * fi)));

    v3 = ((double)a3) * ((double)fi) * 
        (df_for * S * pow(B / S, 2.0 * one_p_b_m_half_vsq_o_vsq) * rq_pricing_cumul_norm_dist((log((B * B) / (S * X)) / v_sqrt_t + one_p_b_m_half_vsq_o_vsq * v_sqrt_t) * eta) -
        (df_dom * X  * pow(B / S, 2.0 * one_p_b_m_half_vsq_o_vsq - 2.0) * rq_pricing_cumul_norm_dist(((log((B * B) / (S * X)) / v_sqrt_t + one_p_b_m_half_vsq_o_vsq * v_sqrt_t) - v_sqrt_t) * eta)));

    v4 = ((double)a4) * ((double)fi) * 
        ((df_for * S * pow(B / S, 2.0 * one_p_b_m_half_vsq_o_vsq) * rq_pricing_cumul_norm_dist(d1_B_S * eta) -
          (df_dom * X * pow(B / S, 2.0 * one_p_b_m_half_vsq_o_vsq - 2.0) * rq_pricing_cumul_norm_dist((d1_B_S - v_sqrt_t) * eta))));

    return v1 + v2 + v3 + v4;
}

RQ_EXPORT double
rq_pricing_single_barrier_greeks_approx
 (
	short in, 
    short call,
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

	/* Greeks currently calculated from first principles... */
	double base_price = rq_pricing_single_barrier(in, call, S, X, B, r_dom, r_for, sigma, tau_e, tau_d); 
	double delta_price = 0, delta_price_up = 0, delta_price_down = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price_up = rq_pricing_single_barrier(in, call, S + delta_shift, X, B, r_dom, r_for, sigma, tau_e, tau_d);
		delta_price_down = rq_pricing_single_barrier(in, call, S - delta_shift, X, B, r_dom, r_for, sigma, tau_e, tau_d);

		d1 = ((delta_price_up - base_price) - (delta_price_down - base_price)) / 2;
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_single_barrier(in, call, S + 2 * delta_shift, X, B, r_dom, r_for, sigma, tau_e, tau_d);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_single_barrier(in, call, S, X, B, r_dom, r_for, sigma + vega_shift, tau_e, tau_d);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_single_barrier(in, call, S, X, B, r_dom, r_for, sigma, tau_e - theta_shift, tau_d - theta_shift);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_single_barrier(in, call, S, X, B, r_dom + r_dom_shift, r_for, sigma, tau_e, tau_d);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_single_barrier(in, call, S, X, B, r_dom, r_for + r_for_shift, sigma, tau_e, tau_d);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}

RQ_EXPORT double
rq_pricing_single_barrier_greeks_continuous
 (
	short in, 
    short call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
	double B, /* assume this rate is passed in domestic over foreign terms */
    double r_dom, /* aka the numerator rate (continuously compounded) */
    double r_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
	double tau_e,
	double tau_d,
	double* delta,
	double* gamma, 
	double* vega,
	double* theta,
	double* rho_dom,
	double* rho_for
 ) 
{	
/*	return rq_pricing_single_barrier_value(in, call, S, X, B, r_dom, r_for, sigma, tau_e, tau_d, delta, gamma, vega, theta, rho_dom, rho_for); */
	return 0;
}
