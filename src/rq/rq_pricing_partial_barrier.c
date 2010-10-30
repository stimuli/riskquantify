/*
** rq_pricing_partial_barrier.c
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
**
** Heynan and Kat (1994) - Haug 'The Complete Guide to Option Pricing Formula' 
**					       2nd Edition - p160-162.
**
*/
#include "rq_pricing_partial_barrier.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

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
    double t2 
    )
{
    double ret = 0.0;
    short up = H > S;
    double b = r - rf;
    double d1 = (log(S / X) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double d2 = d1 - v * sqrt(t2);
    double f1 = (log(S / X) + 2 * log(H / S) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double f2 = f1 - v * sqrt(t2);
    double e1 = (log(S / H) + (b + (v * v) / 2) * t1) / (v * sqrt(t1));
    double e2 = e1 - v * sqrt(t1);
    double e3 = e1 + 2 * log(H / S) / (v * sqrt(t1));
    double e4 = e3 - v * sqrt(t1);
    double mu = (b - (v * v) / 2) / (v * v);
    double rho = sqrt(t1 / t2);
    double g1 = (log(S / H) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double g2 = g1 - v * sqrt(t2);
    double g3 = g1 + 2 * log(H / S) / (v * sqrt(t2));
    double g4 = g3 - v * sqrt(t2);
    
/*
    double z3 = rq_pricing_cumul_bivar_norm_dist(g2, e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(g4, -e4, -rho);
    double z4 = rq_pricing_cumul_bivar_norm_dist(-g2, -e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(-g4, e4, -rho);
    double z7 = rq_pricing_cumul_bivar_norm_dist(g1, e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(g3, -e3, -rho);
    double z8 = rq_pricing_cumul_bivar_norm_dist(-g1, -e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(-g3, e3, -rho);
*/
    
    if (call && !in)
    {
        double eta = (up ? -1.0 : 1.0);
        ret = S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(d1, eta * e1, eta * rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(f1, eta * e3, eta * rho)) 
            - X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(d2, eta * e2, eta * rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(f2, eta * e4, eta * rho));
    }
    else if (!call && !up && !in)
    {
        double z1 = rq_pricing_norm_dist(e2) - pow((H / S), (2 * mu)) * rq_pricing_norm_dist(e4);
        double z5 = rq_pricing_norm_dist(e1) - pow((H / S), (2 * (mu + 1))) * rq_pricing_norm_dist(e3);
        ret = rq_pricing_partial_barrier_early_end(0, 1, S, X, H, r, b, v, t1, t2) - S * exp((b - r) * t2) * z5 + X * exp(-r * t2) * z1;
    }
    else if (!call && up && !in)
    {
        double z2 = rq_pricing_norm_dist(-e2) - pow((H / S), (2 * mu)) * rq_pricing_norm_dist(-e4);
        double z6 = rq_pricing_norm_dist(-e1) - pow((H / S), (2 * (mu + 1))) * rq_pricing_norm_dist(-e3);
        ret = rq_pricing_partial_barrier_early_end(0, 1, S, X, H, r, b, v, t1, t2) - S * exp((b - r) * t2) * z6 + X * exp(-r * t2) * z2;
    }

    return ret;
}

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
    )
{
    double ret = 0.0;
    
    double b = r - rf;

    double d1 = (log(S / X) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double d2 = d1 - v * sqrt(t2);
    double f1 = (log(S / X) + 2 * log(H / S) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double f2 = f1 - v * sqrt(t2);
    double e1 = (log(S / H) + (b + (v * v) / 2) * t1) / (v * sqrt(t1));
    double e2 = e1 - v * sqrt(t1);
    double e3 = e1 + 2 * log(H / S) / (v * sqrt(t1));
    double e4 = e3 - v * sqrt(t1);
    double mu = (b - (v * v) / 2) / (v * v);
    double rho = sqrt(t1 / t2);
    double g1 = (log(S / H) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double g2 = g1 - v * sqrt(t2);
    double g3 = g1 + 2 * log(H / S) / (v * sqrt(t2));
    double g4 = g3 - v * sqrt(t2);
    
    double z1 = rq_pricing_norm_dist(e2) - pow((H / S), (2 * mu)) * rq_pricing_norm_dist(e4);
    double z2 = rq_pricing_norm_dist(-e2) - pow((H / S), (2 * mu)) * rq_pricing_norm_dist(-e4);
    double z3 = rq_pricing_cumul_bivar_norm_dist(g2, e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(g4, -e4, -rho);
    double z4 = rq_pricing_cumul_bivar_norm_dist(-g2, -e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(-g4, e4, -rho);
    double z5 = rq_pricing_norm_dist(e1) - pow((H / S), (2 * (mu + 1))) * rq_pricing_norm_dist(e3);
    double z6 = rq_pricing_norm_dist(-e1) - pow((H / S), (2 * (mu + 1))) * rq_pricing_norm_dist(-e3);
    double z7 = rq_pricing_cumul_bivar_norm_dist(g1, e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(g3, -e3, -rho);
    double z8 = rq_pricing_cumul_bivar_norm_dist(-g1, -e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(-g3, e3, -rho);
    
    if (call && X > H) /* call out type B1 */
    {
        ret = S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(d1, e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(f1, -e3, -rho))
            - X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(d2, e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(f2, -e4, -rho));
    }
    else if (call && X < H)
    {
        ret = S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(-g1, -e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(-g3, e3, -rho)) 
            - X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(-g2, -e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(-g4, e4, -rho)) 
            - S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(-d1, -e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(-f1, e3, -rho)) 
            + X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(-d2, -e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(-f2, e4, -rho)) 
            + S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(g1, e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(g3, -e3, -rho)) 
            - X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(g2, e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(g4, -e4, -rho));
    }
    else if (!call) /* put out type B1 */
    {
        ret = rq_pricing_partial_barrier_delayed_start_up_or_down(0, 1, S, X, H, r, b, v, t1, t2) - S * exp((b - r) * t2) * z8 + X * exp(-r * t2) * z4 - S * exp((b - r) * t2) * z7 + X * exp(-r * t2) * z3;
    }

    return ret;
}

RQ_EXPORT double 
rq_pricing_partial_barrier_delayed_start(
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
    )
{
    double ret = 0.0;
    double b = r - rf;

    short up = H > S;
    
    double d1 = (log(S / X) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double d2 = d1 - v * sqrt(t2);
    double f1 = (log(S / X) + 2 * log(H / S) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double f2 = f1 - v * sqrt(t2);
    double e1 = (log(S / H) + (b + (v * v) / 2) * t1) / (v * sqrt(t1));
    double e2 = e1 - v * sqrt(t1);
    double e3 = e1 + 2 * log(H / S) / (v * sqrt(t1));
    double e4 = e3 - v * sqrt(t1);
    double mu = (b - (v * v) / 2) / (v * v);
    double rho = sqrt(t1 / t2);
    double g1 = (log(S / H) + (b + (v * v) / 2) * t2) / (v * sqrt(t2));
    double g2 = g1 - v * sqrt(t2);
    double g3 = g1 + 2 * log(H / S) / (v * sqrt(t2));
    double g4 = g3 - v * sqrt(t2);
    
    double z1 = rq_pricing_norm_dist(e2) - pow((H / S), (2 * mu)) * rq_pricing_norm_dist(e4);
    double z2 = rq_pricing_norm_dist(-e2) - pow((H / S), (2 * mu)) * rq_pricing_norm_dist(-e4);
    double z3 = rq_pricing_cumul_bivar_norm_dist(g2, e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(g4, -e4, -rho);
    double z4 = rq_pricing_cumul_bivar_norm_dist(-g2, -e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(-g4, e4, -rho);
    double z5 = rq_pricing_norm_dist(e1) - pow((H / S), (2 * (mu + 1))) * rq_pricing_norm_dist(e3);
    double z6 = rq_pricing_norm_dist(-e1) - pow((H / S), (2 * (mu + 1))) * rq_pricing_norm_dist(-e3);
    double z7 = rq_pricing_cumul_bivar_norm_dist(g1, e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(g3, -e3, -rho);
    double z8 = rq_pricing_cumul_bivar_norm_dist(-g1, -e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(-g3, e3, -rho);
    
    if (!in && !up && call && X < H) /* call down-and-out type B2 */
    {
        ret = S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(g1, e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(g3, -e3, -rho)) 
            - X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(g2, e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(g4, -e4, -rho));
    }
    else if (call && !up && !in && X > H)
    {
        ret = rq_pricing_partial_barrier_delayed_start_up_or_down(0, 1, S, X, H, r, b, v, t1, t2);
    }
    else if (call && up && !in && X < H) /* call up-and-out type B2 */
    {
        ret = S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(-g1, -e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(-g3, e3, -rho)) 
            - X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(-g2, -e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(-g4, e4, -rho)) 
            - S * exp((b - r) * t2) * (rq_pricing_cumul_bivar_norm_dist(-d1, -e1, rho) - pow((H / S), (2 * (mu + 1))) * rq_pricing_cumul_bivar_norm_dist(e3, -f1, -rho)) 
            + X * exp(-r * t2) * (rq_pricing_cumul_bivar_norm_dist(-d2, -e2, rho) - pow((H / S), (2 * mu)) * rq_pricing_cumul_bivar_norm_dist(e4, -f2, -rho));
    }
    else if (!call && !up && !in) /* put down-and-out type B2 */
    {
        ret = rq_pricing_partial_barrier_delayed_start(0, 1, S, X, H, r, b, v, t1, t2) - S * exp((b - r) * t2) * z7 + X * exp(-r * t2) * z3;
    }
    else if (!call && up && !in) /* put up-and-out type B2 */
    {
        ret = rq_pricing_partial_barrier_delayed_start(0, 1, S, X, H, r, b, v, t1, t2) - S * exp((b - r) * t2) * z8 + X * exp(-r * t2) * z4;
    }

    return ret;
}

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
    )
{
	double base_price = rq_pricing_partial_barrier_early_end(in, call, S, X, H, r, rf, v, t1, t2); 
	double delta_price = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price = rq_pricing_partial_barrier_early_end(in, call, S + delta_shift, X, H, r, rf, v, t1, t2);
		d1 = delta_price - base_price;
		
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_partial_barrier_early_end(in, call, S + 2 * delta_shift, X, H, r, rf, v, t1, t2);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_partial_barrier_early_end(in, call, S, X, H, r, rf, v + vega_shift, t1, t2);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_partial_barrier_early_end(in, call, S, X, H, r, rf, v, t1 - theta_shift, t2 - theta_shift);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_partial_barrier_early_end(in, call, S, X, H, r + r_dom_shift, rf, v, t1, t2);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_partial_barrier_early_end(in, call, S, X, H, r, rf + r_for_shift, v, t1, t2);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}

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
    )
{
	double base_price = rq_pricing_partial_barrier_delayed_start(in, call, S, X, H, r, rf, v, t1, t2); 
	double delta_price = 0;
	double d1 = 0, d2 = 0;

	if (delta || gamma)
	{
		delta_price = rq_pricing_partial_barrier_delayed_start(in, call, S + delta_shift, X, H, r, rf, v, t1, t2);
		d1 = delta_price - base_price;
		
		if (delta) (*delta) = d1;
	}

	if (gamma)
	{
		double gamma_price = rq_pricing_partial_barrier_delayed_start(in, call, S + 2 * delta_shift, X, H, r, rf, v, t1, t2);
		double d2 = gamma_price - delta_price;
		(*gamma) = d2 - d1;
	}

	if (vega)
	{
		double vol_price = rq_pricing_partial_barrier_delayed_start(in, call, S, X, H, r, rf, v + vega_shift, t1, t2);	
		(*vega) = vol_price - base_price;
	}

	if (theta)
	{
		double theta_price = rq_pricing_partial_barrier_delayed_start(in, call, S, X, H, r, rf, v, t1 - theta_shift, t2 - theta_shift);	
		(*theta) = theta_price - base_price;	
	}

	if (rho_dom)
	{
		double rho_dom_price = rq_pricing_partial_barrier_delayed_start(in, call, S, X, H, r + r_dom_shift, rf, v, t1, t2);	
		(*rho_dom) = rho_dom_price - base_price;	
	}

	if (rho_for)
	{
		double rho_for_price = rq_pricing_partial_barrier_delayed_start(in, call, S, X, H, r, rf + r_for_shift, v, t1, t2);	
		(*rho_for) = rho_for_price - base_price;	
	}

	return base_price;
}


