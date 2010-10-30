/*
** rq_pricing_binomial.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002-2008 Brett Hutley
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
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <math.h>

RQ_EXPORT double
rq_pricing_binomial(
    short call,
    double S, /* assume this rate is passed in domestic over foreign terms */
    double X, /* assume this rate is passed in domestic over foreign terms */
    double r_dom, /* aka the numerator rate (continuously compounded) */
    double r_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
    double tau_e,
    double tau_d,
    int num_iters,
    double *passed_vals /* being an array of num_iters+1 doubles */
    )
{
    double *vals = (passed_vals ? passed_vals : (double *)RQ_MALLOC(sizeof(double) * (num_iters+1)));

	double xy_d = tau_d / num_iters;
	double xy_e = tau_e / num_iters;

    /* the cost of carry growth factor per period */
    double rr = pow(1.0 + (r_dom - r_for), xy_d);
    double rd = pow(1.0 + r_dom, xy_d);

	/* double u = exp(sigma * sqrt(xy_e)); */

	/* alternative calculation of u that prevents 
       p or q being greater than 1.
    */

    double rr_sqr = rr * rr;
	double bb = rr_sqr * (exp(sigma * sigma * xy_e) - 1.0);
	double ab = rr_sqr + bb + 1.0;
	double u = (ab + sqrt((ab * ab) - (4.0 * rr_sqr))) / (2.0 * rr);

	double d = 1 / u;

	double p = (rr - d) / (u - d);
    double q = 1 - p;

    double upr = pow(d, num_iters);
    double uinc = u / d;

    double value = 0.0;

    int i;

    /* printf("--- start array ---\n"); */

    for (i = 0; i <= num_iters; i++)
    {
        double oval = (call ? ((S * upr) - X) : (X - (S * upr)));
        if (oval < 0.0)
            oval = 0.0;
        vals[i] = oval;
        /* printf("%f\n", oval); */
        upr *= uinc;
    }

    for (num_iters--; num_iters >= 0 ; num_iters--)
    {
        /* printf("\n--- iteration %d ---\n", num_iters); */

        for (i = 0; i <= num_iters; i++)
        {
            vals[i] = ((p * vals[i+1]) + (q * vals[i])) / rd;
            if (vals[i] < 0)
                vals[i] = 0.0;
            /* printf("%f\n", vals[i]); */
        }
    }

    value = vals[0];

    if (!passed_vals)
       RQ_FREE(vals);
    
	return value;
}

