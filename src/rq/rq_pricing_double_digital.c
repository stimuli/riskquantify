/*
** rq_pricing_double_digital.c
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
#include "rq_pricing_digital.h"
#include "rq_pricing_double_digital.h"

RQ_EXPORT double 
rq_pricing_double_digital_knockout(
    short cash,
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
    double expected_asset_value = (cash ? 0.0 : (fi * S * exp_b_t * 
        (rq_pricing_cumul_norm_dist(asset_prob_d1) - rq_pricing_cumul_norm_dist(cash_prob_d2))));
    double expected_asset_bar_cost = (cash ? 0.0 : (fi * S * exp_b_t * pow(L / S, two_b_vsq_1) * 
        (rq_pricing_cumul_norm_dist(asset_bar_prob_d3) - rq_pricing_cumul_norm_dist(cash_bar_prob_d4))));
    double expected_cash_value = (cash ? (-fi * X * 
        (rq_pricing_cumul_norm_dist(asset_prob_d1 - v_sqrt_t) - 
         rq_pricing_cumul_norm_dist(cash_prob_d2 - v_sqrt_t))) : 0.0);
    double expected_cash_bar_cost = (cash ? (-fi * X * pow(L / S, two_b_vsq_1 - 2.0) *
        (rq_pricing_cumul_norm_dist(asset_bar_prob_d3 - v_sqrt_t) - 
         rq_pricing_cumul_norm_dist(cash_bar_prob_d4 - v_sqrt_t))) : 0.0);

    return exp(-r_dom * tau_d) * (expected_asset_value - expected_asset_bar_cost  - expected_cash_value + expected_cash_bar_cost);
}

RQ_EXPORT double 
rq_pricing_double_digital_knockin(
    short cash,
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
    return
        rq_pricing_digital(
            cash,
            call,
            S,
            X,
            r_dom,
            r_for,
            sigma,
            tau_e,
            tau_d) -
        rq_pricing_double_digital_knockout(
            cash,
            call,
            S,
            X,
            L,
            H,
            r_dom,
            r_for,
            sigma,
            tau_e,
            tau_d
            );
}
