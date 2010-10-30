/*
** rq_pricing_normdist.c
** 
** Written by Brett Hutley - brett@hutley.net
** 
** Copyright (C) 2001-2008 Brett Hutley
** 
** This file is part of the Risk Quantify Library
** 
** Risk Quantify is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public License
** as published by the Free Software Foundation; either version 2 of
** the License, or (at your option) any later version.
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
#include <assert.h>
#include "rq_config.h"
#include "rq_pricing_normdist.h"

#ifndef PI 
#define PI 3.141592653589793238462643 
#endif 

RQ_EXPORT double 
rq_pricing_norm_dist(double z)
{
    return 0.39894228040143 * exp(-(z*z) / 2.0);
}

RQ_EXPORT double 
rq_pricing_cumul_norm_dist(double z) 
{
	double y = fabs(z);
	double cnd, e, sumA, sumB;

	/* Hart Algorithm */
	if (y > 37)
		cnd = 0;
	else
	{
		e = exp(- pow(y, 2.0) / 2);
		if (y < 7.07106781186547)
		{
			sumA = 3.52624965998911E-02 * y + 0.700383064443688;
            sumA = sumA * y + 6.37396220353165;
            sumA = sumA * y + 33.912866078383;
            sumA = sumA * y + 112.079291497871;
            sumA = sumA * y + 221.213596169931;
            sumA = sumA * y + 220.206867912376;
            sumB = 8.83883476483184E-02 * y + 1.75566716318264;
            sumB = sumB * y + 16.064177579207;
            sumB = sumB * y + 86.7807322029461;
            sumB = sumB * y + 296.564248779674;
            sumB = sumB * y + 637.333633378831;
            sumB = sumB * y + 793.826512519948;
            sumB = sumB * y + 440.413735824752;
            cnd = e * sumA / sumB;
		}
		else
		{
			sumA = y + 0.65;
            sumA = y + 4 / sumA;
            sumA = y + 3 / sumA;
            sumA = y + 2 / sumA;
            sumA = y + 1 / sumA;
            cnd = e / (sumA * 2.506628274631);
		}
	}
	
	return (z > 0.0 ? 1.0 - cnd : cnd);
}

RQ_EXPORT double 
rq_pricing_density(double x, double y, double ad, double bd, double rho) 
{
    double r = ad * (2 * x - ad) + bd * (2 * y - bd)
        + 2.0 * rho * (x - ad) * (y - bd);
    return exp(r);
}

RQ_EXPORT double 
rq_pricing_norm_density(double x)
{
    return 0.39894228040143 * exp(-(x*x) / 2.0);
}

#define SGN(x) ((x) >= 0.0 ? 1.0 : -1.0)

RQ_EXPORT double 
rq_pricing_cumul_bivar_norm_dist(double a, double b, double rho) 
{
    double x[] = {
        0.24840615, 
        0.39233107, 
        0.21141819, 
        0.03324666, 
        0.00082485334
    };
    double y[] = {
        0.10024215, 
        0.48281397, 
        1.0609498, 
        1.7797294, 
        2.6697604
    };
    double rho1;
    double rho2;
    double delta;
    int i;
    int j;
    double r = 0;
    if (a <= 0 && b <= 0 && rho <= 0)
    {
        double rho_sqr = rho * rho;
        double two_x_1_minus_rho_sqr = 2.0 * (1.0 - rho_sqr);
        double sqrt_2_x_1_minus_rho_sqr;
        double a1;
        double b1;
        double sum = 0;

/*
        if (two_x_1_minus_rho_sqr <= 0.0)
            return 0.0;
*/

        sqrt_2_x_1_minus_rho_sqr = sqrt(two_x_1_minus_rho_sqr);
    
        a1 = a / sqrt_2_x_1_minus_rho_sqr;
        b1 = b / sqrt_2_x_1_minus_rho_sqr;

        for (i = 0; i <  5; i++)
        {
            for (j = 0; j < 5; j++)
            {
                sum = sum + x[i] * x[j] * exp(a1 * (2 * y[i] - a1) + b1 * (2 * y[j] - b1) + 2 * rho * (y[i] - a1) * (y[j] - b1));
            }
        }

        r = sqrt(1 - pow(rho, 2)) / PI * sum;
    }
    else if (a <= 0 && b >= 0 && rho >= 0)
        r = rq_pricing_cumul_norm_dist(a) - rq_pricing_cumul_bivar_norm_dist(a, -b, -rho);
    else if (a >= 0 && b <= 0 && rho >= 0)
        r = rq_pricing_cumul_norm_dist(b) - rq_pricing_cumul_bivar_norm_dist(-a, b, -rho);
    else if (a >= 0 && b >= 0 && rho <= 0)
        r = rq_pricing_cumul_norm_dist(a) + rq_pricing_cumul_norm_dist(b) - 1 + rq_pricing_cumul_bivar_norm_dist(-a, -b, rho);
    else if (a * b * rho > 0)
    {
        double b_sqr = b * b;
        double a_sqr = a * a;
        double a_sqr_b_sqr = a_sqr - 2 * rho * a * b + b_sqr;
        rho1 = (rho * a - b) * SGN(a) / sqrt(a_sqr_b_sqr);
        rho2 = (rho * b - a) * SGN(b) / sqrt(a_sqr_b_sqr);
        delta = (1 - SGN(a) * SGN(b)) / 4;
        r = rq_pricing_cumul_bivar_norm_dist(a, 0, rho1) + rq_pricing_cumul_bivar_norm_dist(b, 0, rho2) - delta;
    }

    return r;
}

RQ_EXPORT double 
rq_pricing_cumul_bivar_norm_dist2(double a, double b, double r) 
{
	double ret = 0.0;

	double x[] = {0.04691008, 0.23076534, 0.5, 0.76923466, 0.95308992};
	double W[] = {0.018854042, 0.038088059, 0.0452707394, 0.038088059, 0.018854042};

	double h1 = a;
	double h2 = b;
	double h12 = (h1 * h1 + h2 * h2) / 2.0;

	int i;

	if (fabs(r) >= 0.7) {
		double r2 = 1.0 - r * r;
		double r3 = sqrt(r2);
		if (r < 0.0) { 
			double h2 = -b;
			double h3 = h1 * h2;
			double h7 = exp(-h3 / 2.0);
			double LH = 0.0;

			if (fabs(r) < 1.0) {
				double h6 = fabs(h1 - h2);
				double h5 = h6 * h6 / 2.0;
				double AA = 0.5 - h3 / 8.0;
				double ab = 3.0 - 2.0 * AA * h5;
				
				h6 = h6 / r3;
				LH = 0.13298076 * h6 * ab * (1.0 - rq_pricing_cumul_norm_dist(h6)) - exp(-h5 / r2) * (ab + AA * r2) * 0.053051647;
				for (i = 0; i<5; i++) {
					double r1 = r3 * x[i];
					double rr = r1 * r1;
					double r2 = sqrt(1 - rr);
					double h8 = h7 == 0.0 ? 0.0 : exp(-h3 / (1 + r2)) / r2 / h7;
					LH = LH - W[i] * exp(-h5 / rr) * (h8 - 1 - AA * rr);
				}
			}
			ret = LH * r3 * h7 + rq_pricing_cumul_norm_dist((h1 < h2) ? h1 : h2);
			if (r < 0.0) {
				ret = rq_pricing_cumul_norm_dist(h1) - ret;
			}
		}
	}
	else {
		double h3 = h1 * h2;
		double LH = 0.0;
		if (r != 0.0) {
			for (i = 0; i<5; i++) {
				double r1 = r * x[i];
				double r2 = 1.0 - r1 * r1;
				LH = LH + W[i] * exp((r1 * h3 - h12) / r2) / sqrt(r2);
			}
		}
		ret = rq_pricing_cumul_norm_dist(h1) * rq_pricing_cumul_norm_dist(h2) + r * LH;
	}

	return ret;
}

/** This function was taken from the book "Numerical Recipes in C" */
RQ_EXPORT double
rq_log_gamma(double xx)
{
    double cof[] = {
        76.18009172947146,
        -86.50532032941677,
        24.01409824083091,
        -1.231739572450155,
        0.1208650973866179e-2,
        -0.5395239384953e-5
    };

    double x;
    double y = x = xx;
    double tmp = x + 5.5;
    double ser = 1.000000000190015;

    unsigned j;

    tmp -= (x + 0.5) * log(tmp);

    for (j = 0; j < 6; j++)
        ser += cof[j] / ++y;

    return -tmp + log(2.5066282746310005 * ser / x);
}
