/*
** rq_pricing_partial_double_barrier.c
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
#include "rq_pricing_partial_double_barrier.h"

RQ_EXPORT double 
rq_pricing_partial_double_barrier_delayed_start(
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
    double tau_bar
    ) 
{
    double fi = (call ? 1.0 : -1.0);
    double b = r_dom - r_for;

    double v_sq = sigma * sigma;
    double b_t = b * tau_d;
    double exp_b_t = exp(b_t);
    double half_v_sq_t = 0.5 * v_sq * tau_e;
    double v_sqrt_t = sigma * sqrt(tau_e);
    double log_S_X = log(S / X);
    double log_S_H = log(S / H);
    double log_L_sq_o_S_X = log((L * L) / (S * X));
    double log_L_sq_o_S_H = log((L * L) / (S * H));
    double log_S_L = log(S / L);
    double log_L_sq_o_S_L = log((L * L) / (S * L));
    double d1_fact = (call ? log_S_X : log_S_L);
    double d2_fact = (call ? log_S_H : log_S_X);
    double d3_fact = (call ? log_L_sq_o_S_X : log_L_sq_o_S_L);
    double d4_fact = (call ? log_L_sq_o_S_H : log_L_sq_o_S_X);

    double asset_prob_d1 = (d1_fact + b_t + half_v_sq_t) / v_sqrt_t;
    double cash_prob_d2 = (d2_fact + b_t + half_v_sq_t) / v_sqrt_t;
    double asset_bar_prob_d3 = (d3_fact + b_t + half_v_sq_t) / v_sqrt_t;
    double cash_bar_prob_d4 = (d4_fact + b_t + half_v_sq_t) / v_sqrt_t;

    double two_b_vsq_1 = 2.0 * b / v_sq + 1.0;
    double expected_value = fi * S * exp_b_t * 
        (rq_pricing_cumul_norm_dist(asset_prob_d1) - rq_pricing_cumul_norm_dist(cash_prob_d2));
    double expected_bar_cost = fi * S * exp_b_t * pow(L / S, two_b_vsq_1) * 
        (rq_pricing_cumul_norm_dist(asset_bar_prob_d3) - rq_pricing_cumul_norm_dist(cash_bar_prob_d4));
    double agreed_value = fi * X * 
        (rq_pricing_cumul_norm_dist(asset_prob_d1 - v_sqrt_t) - 
         rq_pricing_cumul_norm_dist(cash_prob_d2 - v_sqrt_t));
    double agreed_bar_cost = fi * X * pow(L / S, two_b_vsq_1 - 2.0) *
        (rq_pricing_cumul_norm_dist(asset_bar_prob_d3 - v_sqrt_t) - 
         rq_pricing_cumul_norm_dist(cash_bar_prob_d4 - v_sqrt_t));

    double ko = exp(-r_dom * tau_d) * (expected_value - expected_bar_cost  - agreed_value + agreed_bar_cost);
    if (in)
        return ko;
    else
        return rq_pricing_blackscholes(
            call,
            S,
            X,
            r_dom,
            r_for,
            sigma,
            tau_e,
            tau_d) - ko;
}

RQ_EXPORT double 
rq_pricing_partial_double_barrier_early_end(
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
    double tau_bar
    ) 
{
    double fi = (call ? 1.0 : -1.0);
    double b = r_dom - r_for;

    double v_sq = sigma * sigma;
    double b_t = b * tau_d;
    double exp_b_t = exp(b_t);
    double half_v_sq_t = 0.5 * v_sq * tau_e;
    double v_sqrt_t = sigma * sqrt(tau_e);
    double log_S_X = log(S / X);
    double log_S_H = log(S / H);
    double log_L_sq_o_S_X = log((L * L) / (S * X));
    double log_L_sq_o_S_H = log((L * L) / (S * H));
    double log_S_L = log(S / L);
    double log_L_sq_o_S_L = log((L * L) / (S * L));
    double d1_fact = (call ? log_S_X : log_S_L);
    double d2_fact = (call ? log_S_H : log_S_X);
    double d3_fact = (call ? log_L_sq_o_S_X : log_L_sq_o_S_L);
    double d4_fact = (call ? log_L_sq_o_S_H : log_L_sq_o_S_X);

    double asset_prob_d1 = (d1_fact + b_t + half_v_sq_t) / v_sqrt_t;
    double cash_prob_d2 = (d2_fact + b_t + half_v_sq_t) / v_sqrt_t;
    double asset_bar_prob_d3 = (d3_fact + b_t + half_v_sq_t) / v_sqrt_t;
    double cash_bar_prob_d4 = (d4_fact + b_t + half_v_sq_t) / v_sqrt_t;

    double two_b_vsq_1 = 2.0 * b / v_sq + 1.0;
    double expected_value = fi * S * exp_b_t * 
        (rq_pricing_cumul_norm_dist(asset_prob_d1) - rq_pricing_cumul_norm_dist(cash_prob_d2));
    double expected_bar_cost = fi * S * exp_b_t * pow(L / S, two_b_vsq_1) * 
        (rq_pricing_cumul_norm_dist(asset_bar_prob_d3) - rq_pricing_cumul_norm_dist(cash_bar_prob_d4));
    double agreed_value = fi * X * 
        (rq_pricing_cumul_norm_dist(asset_prob_d1 - v_sqrt_t) - 
         rq_pricing_cumul_norm_dist(cash_prob_d2 - v_sqrt_t));
    double agreed_bar_cost = fi * X * pow(L / S, two_b_vsq_1 - 2.0) *
        (rq_pricing_cumul_norm_dist(asset_bar_prob_d3 - v_sqrt_t) - 
         rq_pricing_cumul_norm_dist(cash_bar_prob_d4 - v_sqrt_t));

    double ko = exp(-r_dom * tau_d) * (expected_value - expected_bar_cost  - agreed_value + agreed_bar_cost);
    if (in)
        return ko;
    else
        return rq_pricing_blackscholes(
            call,
            S,
            X,
            r_dom,
            r_for,
            sigma,
            tau_e,
            tau_d) - ko;
}

RQ_EXPORT double 
rq_pricing_partial_double_barrier_delayed_start_greeks(
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
    double tau_bar,
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
	double base_price = rq_pricing_partial_double_barrier_delayed_start(in, call, S, X, L, H, r_dom, r_for, sigma, tau_e, tau_d, tau_bar); 
	double delta_price = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price = rq_pricing_partial_double_barrier_delayed_start(in, call, S + delta_shift, X, L, H, r_dom, r_for, sigma, tau_e, tau_d, tau_bar);
		d1 = delta_price - base_price;
		
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_partial_double_barrier_delayed_start(in, call, S + 2 * delta_shift, X, L, H, r_dom, r_for, sigma, tau_e, tau_d, tau_bar);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_partial_double_barrier_delayed_start(in, call, S, X, L, H, r_dom, r_for, sigma + vega_shift, tau_e, tau_d, tau_bar);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_partial_double_barrier_delayed_start(in, call, S, X, L, H, r_dom, r_for, sigma, tau_e - theta_shift, tau_d - theta_shift, tau_bar);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_partial_double_barrier_delayed_start(in, call, S, X, L, H, r_dom + r_dom_shift, r_for, sigma, tau_e, tau_d, tau_bar);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_partial_double_barrier_delayed_start(in, call, S, X, L, H, r_dom, r_for + r_for_shift, sigma, tau_e, tau_d, tau_bar);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}

RQ_EXPORT double 
rq_pricing_partial_double_barrier_early_end_greeks(
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
    double tau_bar,
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
	double base_price = rq_pricing_partial_double_barrier_early_end(in, call, S, X, L, H, r_dom, r_for, sigma, tau_e, tau_d, tau_bar); 
	double delta_price = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price = rq_pricing_partial_double_barrier_early_end(in, call, S + delta_shift, X, L, H, r_dom, r_for, sigma, tau_e, tau_d, tau_bar);
		d1 = delta_price - base_price;
		
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_partial_double_barrier_early_end(in, call, S + 2 * delta_shift, X, L, H, r_dom, r_for, sigma, tau_e, tau_d, tau_bar);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_partial_double_barrier_early_end(in, call, S, X, L, H, r_dom, r_for, sigma + vega_shift, tau_e, tau_d, tau_bar);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_partial_double_barrier_early_end(in, call, S, X, L, H, r_dom, r_for, sigma, tau_e - theta_shift, tau_d - theta_shift, tau_bar);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_partial_double_barrier_early_end(in, call, S, X, L, H, r_dom + r_dom_shift, r_for, sigma, tau_e, tau_d, tau_bar);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_partial_double_barrier_early_end(in, call, S, X, L, H, r_dom, r_for + r_for_shift, sigma, tau_e, tau_d, tau_bar);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}
