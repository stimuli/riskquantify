/*
** rq_pricing_blackscholes.c
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA 
*/

#include <math.h>
#include <stdio.h>
#include "rq_config.h"
#include "rq_pricing_normdist.h"
#include "rq_pricing_blackscholes.h"

RQ_EXPORT double 
rq_pricing_blackscholes(
    int call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
    double r_dom, /* aka the numerator rate (continuously compounded) */
    double r_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
    double tau_e,
    double tau_d
    ) 
{
    double m = (call ? 1.0 : -1.0);
    double f = S * exp(r_dom * tau_d) / exp(r_for * tau_d);

    double sigma_tau_sqrt = sigma * sqrt(tau_e);

    double d1 = 
        (log(f / X) + (0.5 * sigma * sigma * tau_e)) / sigma_tau_sqrt;

    double d2 = (d1 - sigma_tau_sqrt) * m;
    d1 *= m;

    return
        exp(-r_dom * tau_d) * ((f * rq_pricing_cumul_norm_dist(d1) * m) + 
        (X * rq_pricing_cumul_norm_dist(d2) * -m));
}

/* Black (1976)  S(spot)=F(fwd) and b = 0
   Merton (1973) b=r-q where q is continous dividend yield
*/
RQ_EXPORT double 
rq_pricing_blackscholes_gen(
    int call, 
    double S, 
    double X, 
    double tau, 
    double r, 
    double b, 
    double sigma
    )
{
/* ideally the input 'call' should be 1.0 (for call) and -1.0 (for put).  But need changes at many places. */

    double sigma_tau_sqrt, d1, d2;

    double m = call ? 1.0 : -1.0;
    if (tau <= 0.0)
        return max((S - X) * m, 0.0);

	sigma_tau_sqrt = sigma * sqrt(tau);

    d1 = (log(S / X) + (b + 0.5 * sigma * sigma) * tau) / sigma_tau_sqrt;
    d2 = d1 - sigma_tau_sqrt;

    return (m * S * exp((b - r) * tau) * rq_pricing_cumul_norm_dist(d1*m) - m * X * exp(-r * tau) * rq_pricing_cumul_norm_dist(d2*m));
}

/* Black (1976)  S(spot)=F(fwd) and b = 0
   Merton (1973) b=r-q where q is continous dividend yield
*/
RQ_EXPORT double 
rq_pricing_blackscholes_gen_greeks(
    int call, 
    double S, 
    double X, 
    double tau, 
    double r, 
    double b, 
    double sigma,
	double *delta,
	double *gamma,
	double *vega,
	double *rho,
	double *theta
    )
{
	double sigma_tau_sqrt = sigma * sqrt(tau);
	double tau_sqrt = sqrt(tau);

    double d1 = (log(S / X) + (b + 0.5 * sigma * sigma * tau)) / sigma_tau_sqrt;

    double d2 = d1 - sigma * sqrt(tau);
    double ret = 0.0;

	double df_spot = exp((b - r) * tau);
	double df_strike = exp(-r * tau);
	double df = call ? df_spot : df_strike;

    double m = call ? 1.0 : -1.0;

	double nd1 = rq_pricing_norm_density(d1);
	double cnd1 = rq_pricing_cumul_norm_dist(m * d1);
	double cnd2 = rq_pricing_cumul_norm_dist(m * d2);

    ret = m * S * df_spot * cnd1 - m * X * df_strike * cnd2;

	if (delta)
		(*delta) = df * cnd1 * m;

	if (gamma)
		(*gamma) = df * nd1 / (S * sigma_tau_sqrt);

	if (vega)
		(*vega) = S * tau_sqrt * df * nd1;

	if (rho)
		(*rho) = m * X * df_strike * cnd2;

	if (theta)
		(*theta) = -sigma * S * df_spot * nd1 / (2 * tau_sqrt) - m * (b - r) * S * cnd1 * df_spot - m * r * X * df_strike * cnd2;

    return ret;
}

/* Return PV of option. */
RQ_EXPORT double 
rq_pricing_garmankhol(
    int call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
    double df_dom, /* aka the numerator rate (continuously compounded) */
    double df_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
    double tau_e,
    double tau_d
    ) 
{
    double d1, d2, m = (call ? 1.0 : -1.0);
    double sigma_tau_sqrt;
    double f = S * df_for / df_dom;

    /* Price zero vol - useful for getting intrinsic value and for model verification. */
    /* Also price when only strike < 0.0, needed for swap floor and caps. */
    if(sigma <= 0.0 || X == 0.0 || f/X <= 0.0 || tau_e <= 0.0)
        return df_dom * max(m * (f - X), 0.0);
    sigma_tau_sqrt = sigma * sqrt(tau_e);
    d1 = (log(f / X) + (0.5 * sigma * sigma * tau_e)) / sigma_tau_sqrt;

    d2 = (d1 - sigma_tau_sqrt) * m;
    d1 *= m;

	//call
	//delta = df_for * rq_pricing_cumul_norm_dist(d1)
	//theta = 

    return
        df_dom * m * (f * rq_pricing_cumul_norm_dist(d1) - X * rq_pricing_cumul_norm_dist(d2));
}

RQ_EXPORT double 
rq_greeks_garmankhol(
    int call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
    double df_dom, /* aka the numerator rate (continuously compounded) */
    double df_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
    double tau_e,
    double tau_d,
	double *delta,
	double *gamma,
	double *vega,
	double *rho_dom,
	double *rho_for,
	double *theta
    ) 
{
    double m = (call ? 1.0 : -1.0);
    double f = S * df_for / df_dom;

	double tau_sqrt = sqrt(tau_e);
    double sigma_tau_sqrt = sigma * tau_sqrt;

	/* Hull ed 4. forumla 12.11, 12.12 */
    double d1tmp = (log(f / X) + (0.5 * sigma * sigma * tau_e)) / sigma_tau_sqrt;

    double d2 = (d1tmp - sigma_tau_sqrt) * m;
    double d1 = d1tmp * m;

	double cnd1 = rq_pricing_cumul_norm_dist(d1);
	double cnd2 = rq_pricing_cumul_norm_dist(d2);
	double nd1 = rq_pricing_norm_density(d1);
	double nd1Neg = rq_pricing_norm_density(-d1);

	if (delta)
		(*delta) = df_for * cnd1 * m;
	/*	(*delta) = df_for * ( cnd1 - (call ? 0.0 : 1.0) ); */

	if (gamma)
		(*gamma) = df_for * nd1 / (S * sigma_tau_sqrt);

	if (vega)
		(*vega) = S * tau_sqrt * df_for * nd1;

	/* Incorrect
       if (rho)
           (*rho) = (call ? 1.0 : -1.0) * tau_e * S * df_for * (call ? cnd2 : rq_pricing_cumul_norm_dist(-d2));
    */

	if (rho_dom)
		(*rho_dom) = (call ? 1.0 : -1.0) * tau_e * X * df_dom * cnd2;

	if (rho_for)
		(*rho_for) = (call ? -1.0 : 1.0) * tau_e * S * df_for * cnd1;

	if (theta)
	{
		double rf_for = -log(df_for) / tau_e;
		double rf_dom = -log(df_dom) / tau_e;

        /*
		  (*theta) = -sigma * S * df_for * (call ? nd1 : nd1Neg) / (2 * tau_sqrt) + (call ? 1.0 : -1.0) * rf_for * S * cnd1 * df_for - (call ? 1.0 : -1.0) * rf_dom * X * df_dom * cnd2;
        */
		(*theta) = -sigma * S * df_for * nd1 / (2 * tau_sqrt) - (-m) * rf_for * S * cnd1 * df_for - m * rf_dom * X * df_dom * cnd2;
	}

    return ( df_dom * ((f * cnd1 * m) + (X * cnd2 * -m)) );
}

RQ_EXPORT double
rq_pricing_blackscholes_delta(
    int call,
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
 )
{
    double d1 = (log(S/X) + b*td + (v*v)/2.0*tv) / (v*sqrt(tv));

	if (tv <= 0.0 || td <= 0.0)
		return 0.0;

    if (call)
		return exp((b-r)*td) * rq_pricing_cumul_norm_dist(d1);
    else
		return exp((b-r)*td) * (rq_pricing_cumul_norm_dist(d1) - 1.0);
}

RQ_EXPORT double
rq_pricing_blackscholes_theta(
    int call,
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
 )
{
    double fi = (call ? 1.0: -1.0);
    double d1 = (log(S/X) + b*td + (v*v)/2.0*tv) / (v*sqrt(tv));
    double d2 = d1 - v * sqrt(tv);	
	double D = r - b;

	if (tv <= 0.0 || td <= 0.0)
		return 0.0;

	return 
		-v * S * exp(-D * td) * rq_pricing_norm_density(fi * d1) / (2 * sqrt(tv) )
		+ fi * D * S * rq_pricing_cumul_norm_dist(fi * d1) * exp(-D * td)
		- fi * r * X * rq_pricing_cumul_norm_dist(fi * d2) * exp(-r * td);

		/*
        exp(-r *td) *
        (exp(b *td) *
         S * 
         rq_pricing_norm_density(fi * ((log(S / X) + (b * td) + (((v * v) / 2.0) * tv))  / (v * sqrt(tv)))) * v / (2.0 * sqrt(tv)) + 
         (b - r) * exp(b * td) * S * 
         rq_pricing_cumul_norm_dist(fi * ((log(S / X) + (b * td) + (((v * v) / 2.0) * tv))  / (v * sqrt(tv)))) +
         r * X * rq_pricing_cumul_norm_dist(fi * (((log(S / X) + (b * td) + (((v * v) / 2.0) * tv))  / (v * sqrt(tv))) - v * sqrt(tv))));
		 */
}

RQ_EXPORT double
rq_pricing_blackscholes_gamma(
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
 )
{
	double D = r - b;
	double norm_density;

	if (tv <= 0.0 || td <= 0.0)
		return 0.0;

	norm_density = rq_pricing_norm_density((log(S / X) + (b * td) + (((v * v) / 2.0) * tv))  / (v * sqrt(tv)));

	return exp(-D * td) * norm_density/(S*v*sqrt(tv));
}

RQ_EXPORT double
rq_pricing_blackscholes_vega(
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
 )
{
    double d1 = (log(S/X) + b*td + (v*v)/2.0*tv) / (v*sqrt(tv));

	if (tv <= 0.0 || td <= 0.0)
		return 0.0;

	return 
        exp(-r * td) *
        exp(b * td) *
        S * 
        rq_pricing_norm_density(d1) * 
        sqrt(tv);
}

RQ_EXPORT double
rq_pricing_blackscholes_rho(
    int call,
	double	S,
	double	X,
	double	r,
	double	b,
	double	v,
	double	tv,
	double	td
 )
{
	double m = (call ? 1.0 : -1.0);
	if (tv <= 0.0 || td <= 0.0)
		return 0.0;

	return m * X * td * exp(-r*td) * rq_pricing_cumul_norm_dist(m * ((log(S / X) + (b * td) - (((v * v) / 2.0) * tv))  / (v * sqrt(tv))));
}
