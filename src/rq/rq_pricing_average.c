/*
** rq_pricing_average.c
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
#include "rq_pricing_blackscholes.h"
#include "rq_pricing_average.h"
#include "rq_math.h"

RQ_EXPORT double 
rq_pricing_average_rate_geometric(
    short call, 
    double S, 
    double SA, 
    double X,
    double r, 
    double rf, 
    double v,
    double T, 
    double T2 
    )
{
    double t1 = T - T2; /* Observed or realized time period */
    double b = r - rf; 
    double bA = 1.0 / 2.0 * (b - (v * v) / 6.0);
    double vA = v / sqrt(3);

    double ret = 0.0;
    
    if (t1 > 0)
    {
        X = (t1 + T2) / T2 * X - t1 / T2 * SA;
        ret = rq_pricing_blackscholes_gen(call, S, X, T2, r, bA, vA) * T2 / (t1 + T2);
    }
    else if (t1 == 0)
        ret = rq_pricing_blackscholes_gen(call, S, SA, T, r, bA, vA);

    return ret;
}

RQ_EXPORT double 
rq_pricing_average_rate_geometric_greeks(
    short call, 
    double S, 
    double SA, 
    double X,
    double r, 
    double rf, 
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
	double rho_shift)
{
	double delta_up = 0.0;
	double delta_down = 0.0;
	double gamma_up = 0.0;
	double gamma_down = 0.0;
	double vega_up = 0.0;
	double vega_down = 0.0;
	double theta_up = 0.0;
	double rho_up = 0.0;
	double rho_down = 0.0;
	double price = 0.0;

	price = rq_pricing_average_rate_geometric(call, S, SA, X, r, rf, v, T, T2);

	if (delta)
	{
		delta_up = rq_pricing_average_rate_geometric(call, S + delta_shift, SA, X, r, rf, v, T, T2);
		delta_down = rq_pricing_average_rate_geometric(call, S - delta_shift, SA, X, r, rf, v, T, T2);

		*delta = (delta_up - delta_down)/ (2.0 * delta_shift);
	
		if (gamma)
		{
			gamma_up =  rq_pricing_average_rate_geometric(call, S + 2 * gamma_shift, SA, X, r, rf, v, T, T2);
			gamma_down =  rq_pricing_average_rate_geometric(call, S - 2 * gamma_shift, SA, X, r, rf, v, T, T2);

			gamma_up = (gamma_up - 2 * delta_up + price);
			gamma_down = (gamma_down -  2 * delta_down + price);

			*gamma = (fabs(gamma_up) + fabs(gamma_down)) / (2.0 * gamma_shift * gamma_shift);
		}
	}

	if (theta)
	{
		theta_up = rq_pricing_average_rate_geometric(call, S, SA, X, r, rf, v, T + theta_shift, T2);

		*theta = (price - theta_up) / price;
	}
		
	if (rho)
	{
		rho_up = rq_pricing_average_rate_geometric(call, S, SA, X, r + rho_shift, rf, v, T, T2);
		rho_down = rq_pricing_average_rate_geometric(call, S, SA, X, r - rho_shift, rf, v, T, T2);

		*rho = (rho_up - rho_down) / (2.0 * rho_shift);
	}
			
	if (vega)
	{
		vega_up = rq_pricing_average_rate_geometric(call, S, SA, X, r, rf, v + vega_shift, T, T2);
		vega_down = rq_pricing_average_rate_geometric(call, S, SA, X, r, rf, v - vega_shift, T, T2);

		*vega = (vega_up - vega_down) / (2.0 * vega_shift);
	}

	return price;
}

RQ_EXPORT double 
rq_pricing_average_rate_turnbullwakeman(
    short call, 
    double S, 
    double SA, 
    double X,
    double r, 
    double rf, 
    double v,
    double T, 
    double T2, 
    double T_period
    )
{
    double v_sqr = v * v;
    double b = r - rf;
    double two_b_plus_v_sqr = 2.0 * b + v_sqr;
    double T_minus_tau = T - T_period;
    double m1 = (exp(b * T) - exp(b * T_period)) / (b * T_minus_tau);
    double m2 = 
        2.0 * exp(two_b_plus_v_sqr * T) / 
        ((b + v_sqr) * two_b_plus_v_sqr * (T_minus_tau * T_minus_tau))
        + 2.0 * exp(two_b_plus_v_sqr * T_period) / 
        (b * T_minus_tau * T_minus_tau) * (1 / two_b_plus_v_sqr - exp(b * T_minus_tau) / (b + v_sqr));
    double bA = log(m1) / T;
    double vA = sqrt(log(m2) / T - 2 * bA);
    double t1 = T - T2;
    double ret = 0.0;
    
    if (t1 > 0)
    {
        X = T / T2 * X - t1 / T2 * SA;
        ret = rq_pricing_blackscholes_gen(call, S, X, T2, r, bA, vA) * T2 / T;
    }
    else
        ret = rq_pricing_blackscholes_gen(call, S, X, T2, r, bA, vA);

    return ret;
}

RQ_EXPORT double 
rq_pricing_average_rate_turnbullwakeman_greeks(
    short call, 
    double S, 
    double SA, 
    double X,
    double r, 
    double rf, 
    double v,
    double T, 
    double T2, 
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
	)
{
	double delta_up = 0.0;
	double delta_down = 0.0;
	double gamma_up = 0.0;
	double gamma_down = 0.0;
	double vega_up = 0.0;
	double vega_down = 0.0;
	double theta_up = 0.0;
	double rho_up = 0.0;
	double rho_down = 0.0;
	double price = 0.0;

	price = rq_pricing_average_rate_turnbullwakeman(call, S, SA, X, r, rf, v, T, T2, T_period);

	if (delta)
	{
		delta_up = rq_pricing_average_rate_turnbullwakeman(call, S + delta_shift, SA, X, r, rf, v, T, T2, T_period);
		delta_down = rq_pricing_average_rate_turnbullwakeman(call, S - delta_shift, SA, X, r, rf, v, T, T2, T_period);

		*delta = (delta_up - delta_down)/ (2.0 * delta_shift);
	
		if (gamma)
		{
			gamma_up =  rq_pricing_average_rate_turnbullwakeman(call, S + 2 * gamma_shift, SA, X, r, rf, v, T, T2, T_period);
			gamma_down =  rq_pricing_average_rate_turnbullwakeman(call, S - 2 * gamma_shift, SA, X, r, rf, v, T, T2, T_period);

			gamma_up = (gamma_up - 2 * delta_up + price);
			gamma_down = (gamma_down -  2 * delta_down + price);

			*gamma = (fabs(gamma_up) + fabs(gamma_down)) / (2.0 * gamma_shift * gamma_shift);
		}
	}

	if (theta)
	{
		theta_up = rq_pricing_average_rate_turnbullwakeman(call, S, SA, X, r, rf, v, T + theta_shift, T2, T_period);

		*theta = (price - theta_up) / price;
	}
		
	if (rho)
	{
		rho_up = rq_pricing_average_rate_turnbullwakeman(call, S, SA, X, r + rho_shift, rf, v, T, T2, T_period);
		rho_down = rq_pricing_average_rate_turnbullwakeman(call, S, SA, X, r - rho_shift, rf, v, T, T2, T_period);

		*rho = (rho_up - rho_down) / (2.0 * rho_shift);
	}
			
	if (vega)
	{
		vega_up = rq_pricing_average_rate_turnbullwakeman(call, S, SA, X, r, rf, v + vega_shift, T, T2, T_period);
		vega_down = rq_pricing_average_rate_turnbullwakeman(call, S, SA, X, r, rf, v - vega_shift, T, T2, T_period);

		*vega = (vega_up - vega_down) / (2.0 * vega_shift);
	}

	return price;
}

RQ_EXPORT double 
rq_pricing_average_rate_levy(
    short call, 
    double S, 
    double SA, 
    double X, 
    double r, 
    double rf, 
    double v,
    double T, 
    double T2 
    )
{
    double b = r - rf;
    double ret = 0.0;
    double SE = S / (T * b) * (exp((b - r) * T2) - exp(-r * T2));
    double m = 2 * (S * S) / (b + (v * v)) * ((exp((2 * b + (v * v)) * T2) - 1) / (2 * b + (v * v)) - (exp(b * T2) - 1) / b);
    double d = m / (T * T);
    double Sv = log(d) - 2 * (r * T2 + log(SE));
    double XStar = X - (T - T2) / T * SA;
    double d1 = 1 / sqrt(Sv) * (log(d) / 2 - log(XStar));
    double d2 = d1 - sqrt(Sv);
    
    if (call)
        ret = 
            SE * rq_pricing_cumul_norm_dist(d1) 
            - XStar * exp(-r * T2) * rq_pricing_cumul_norm_dist(d2);
    else
        ret = (SE * rq_pricing_cumul_norm_dist(d1) - XStar * exp(-r * T2) * rq_pricing_cumul_norm_dist(d2)) - SE + XStar * exp(-r * T2);

    return ret;
}

RQ_EXPORT double 
rq_pricing_average_rate_levy_greeks(
    short call, 
    double S, 
    double SA, 
    double X, 
    double r, 
    double rf, 
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
    )
{
	double delta_up = 0.0;
	double delta_down = 0.0;
	double gamma_up = 0.0;
	double gamma_down = 0.0;
	double vega_up = 0.0;
	double vega_down = 0.0;
	double theta_up = 0.0;
	double rho_up = 0.0;
	double rho_down = 0.0;
	double price = 0.0;

	price = rq_pricing_average_rate_levy(call, S, SA, X, r, rf, v, T, T2);

	if (delta)
	{
		delta_up = rq_pricing_average_rate_levy(call, S + delta_shift, SA, X, r, rf, v, T, T2);
		delta_down = rq_pricing_average_rate_levy(call, S - delta_shift, SA, X, r, rf, v, T, T2);

		*delta = (delta_up - delta_down)/ (2.0 * delta_shift);
	
		if (gamma)
		{
			gamma_up =  rq_pricing_average_rate_levy(call, S + 2 * gamma_shift, SA, X, r, rf, v, T, T2);
			gamma_down =  rq_pricing_average_rate_levy(call, S - 2 * gamma_shift, SA, X, r, rf, v, T, T2);

			gamma_up = (gamma_up - 2 * delta_up + price);
			gamma_down = (gamma_down -  2 * delta_down + price);

			*gamma = (fabs(gamma_up) + fabs(gamma_down)) / (2.0 * gamma_shift * gamma_shift);
		}
	}

	if (theta)
	{
		theta_up = rq_pricing_average_rate_levy(call, S, SA, X, r, rf, v, T + theta_shift, T2);

		*theta = (price - theta_up) / price;
	}
		
	if (rho)
	{
		rho_up = rq_pricing_average_rate_levy(call, S, SA, X, r + rho_shift, rf, v, T, T2);
		rho_down = rq_pricing_average_rate_levy(call, S, SA, X, r - rho_shift, rf, v, T, T2);

		*rho = (rho_up - rho_down) / (2.0 * rho_shift);
	}
			
	if (vega)
	{
		vega_up = rq_pricing_average_rate_levy(call, S, SA, X, r, rf, v + vega_shift, T, T2);
		vega_down = rq_pricing_average_rate_levy(call, S, SA, X, r, rf, v - vega_shift, T, T2);

		*vega = (vega_up - vega_down) / (2.0 * vega_shift);
	}

	return price;
}

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
 )
{
	double as = 0.0;
    double var_as = 0.0;
    double vx = 0.0;
    int i;

    double b = r_dom - r_for;

    for (i = 0; i < num_ratesets; i++)
    {
        int j;
		double ti = rateset_dates[i] / 2.0;
        double weight_i = rateset_weights[i];
        
		as += weight_i * exp(b * ti);

		for (j = 0; j < num_ratesets; j++)
		{
			double tj = rateset_dates[j] / 2.0;
            double weight_j = rateset_weights[j];

			var_as += weight_i * weight_j * exp(b * (ti + tj)) * 
                (exp(sigma * sigma * MIN(ti, tj)) - 1.0);
		}
    }

    as *= S;
    var_as *= S * S;
    vx = sqrt(log(1 + var_as / (as * as)) / tau_e);

    return rq_pricing_blackscholes(
        call,
        as,
        X,
        r_dom,
        r_for,
        vx,
        tau_e,
        tau_d
        );
}


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
 )
{
    double ax = 0.0;
    double var_ax = 0.0;
    double vx;
    double rho = 0.0;
    int i;

    double b = r_dom - r_for;
    

    for (i = 0; i < num_ratesets; i++)
    {
		double ti = rateset_dates[i] / 2.0;
        double weight_i = rateset_weights[i];
        int j;

        double weight_f = weight_i * exp(b * ti);

        ax += weight_f;
	    rho += weight_f * (exp(sigma * sigma * ti) - 1.0);

        for (j = 0; j < num_ratesets; j++)
        {
            double tj = rateset_dates[j] / 2.0;
            double weight_j = rateset_weights[j];

            var_ax += weight_i * weight_j *exp(b * (ti+tj)) * (exp(sigma * sigma * MIN(ti, tj)) - 1.0);
        }
    }

    ax *= S;
    var_ax *= S * S;
    vx = sqrt(log(var_ax / (ax * ax)+1) / tau_e);
	rho *= S / sqrt(var_ax * (exp(sigma * sigma * tau_e) - 1.0));
    vx = sqrt(vx * vx + sigma * sigma - 2 * rho * vx * sigma);

    return rq_pricing_blackscholes(
        call,
        S * exp(b * tau_d),
        ax,
        r_dom,
        r_for,
        vx,
        tau_e,
        tau_d
        );
}
