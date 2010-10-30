/*
** rq_pricing_spread.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2006-2008 Brett Hutley
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
#include "rq_pricing_spread.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <math.h>

RQ_EXPORT double 
rq_pricing_spread(
    int call,
    double f1,
    double f2,
    double k,
    double T, 
    double r, 
    double v1,
    double v2,
    double rho
    )
{
    double f = f1 / (f2 + k);
    double f2_part = f2 / (f2 + k);
    double v2_f2 = v2 * f2_part;
    double v = sqrt(
        (v1 * v1) + 
        (v2_f2 * v2_f2) - 
        (2 * rho * v1 * v2 * f2_part)
        );
    double v_sqrt_t = v * sqrt(T);
    double d1 = (log(f) + ((v * v) / 2.0) * T) / v_sqrt_t;
    double d2 = d1 - v_sqrt_t;

    double ret;
    if (call)
        ret = (f2 + k) * (exp(-r * T) * (f * rq_pricing_cumul_norm_dist(d1) - rq_pricing_cumul_norm_dist(d2)));
    else
        ret = (f2 + k) * (exp(-r * T) * (rq_pricing_cumul_norm_dist(-d2) - f * rq_pricing_cumul_norm_dist(-d1)));

    return ret;
}

RQ_EXPORT double
rq_pricing_spread_numeric(
    int call,
    double f1,
    double f2,
    double k,
    double T, 
    double r, 
    double v1,
    double v2,
    double rho,
    unsigned grid_size
    )
{
    int i;
    double stddev = 4.0;

    double low = -(2.0 * stddev) / 2.0;
    double dx = (2.0 * stddev) / (grid_size - 1.0);
    double v1_sigma_sqrt_t = v1 * sqrt(T);
    double v2_sigma_sqrt_t = v2 * sqrt(T);

    double **s1_payoff_grid;
    double **s2_payoff_grid;
    double *z;
    double *probs;

    double result = 0.0;

    if (T == 0)
    {
        double payoff;

        if (call)
            payoff = f1 - f2 - k;
        else
            payoff = -f1 + f2 + k;

        return payoff > 0 ? payoff : 0.0;
    }

    s1_payoff_grid = (double **)calloc(grid_size, sizeof(double));
    s2_payoff_grid = (double **)calloc(grid_size, sizeof(double));
    z = (double *)calloc(grid_size, sizeof(double));
    probs = (double *)calloc(grid_size, sizeof(double));


    for (i = 0; i < grid_size; i++)
    {
        s1_payoff_grid[i] = (double *)calloc(grid_size, sizeof(double));
        s2_payoff_grid[i] = (double *)calloc(grid_size, sizeof(double));
    }

    for (i = 0; i < grid_size; ++i)
        z[i] = low + i * dx;

    for (i = 0; i < grid_size; ++i)
    { 
        int j;
        for (j = 0; j < grid_size; ++j)
        { 
            s1_payoff_grid[i][j] = exp(z[i] * v1_sigma_sqrt_t - 0.5 * v1 * v1 * T);
            s2_payoff_grid[i][j] = exp((rho * z[i] + sqrt((1.0 - rho * rho)) * z[j]) * v2_sigma_sqrt_t - 0.5 * v2 * v2 * T);
            
        }
    }
    
    probs[0] = rq_pricing_cumul_norm_dist(-stddev + dx / 2.0);
    probs[grid_size - 1] = 1.0 - rq_pricing_cumul_norm_dist(stddev - dx / 2.0);
    for(i = 1; i < grid_size - 1; ++i)
    { 
        double x = -stddev + i * dx;
        probs[i] = 
            rq_pricing_cumul_norm_dist(x + dx / 2.0) - 
            rq_pricing_cumul_norm_dist(x - dx / 2.0);
    }

    for (i = 0; i < grid_size; ++i)
    {
        int j;
        for (j = 0; j < grid_size; ++j)
        {
            double payoff;
            double prob;

            if (call)
                payoff = (f1 * s1_payoff_grid[i][j] - f2 * s2_payoff_grid[i][j] - k);
            else
                payoff = (-f1 * s1_payoff_grid[i][j] + f2 * s2_payoff_grid[i][j] + k);

            if (payoff < 0) payoff = 0.0;
            prob = probs[i] * probs[j];
            result += prob * payoff;
        }
    }

    RQ_FREE(probs);
    RQ_FREE(z);

    for (i = 0; i < grid_size; i++)
    {
        RQ_FREE(s1_payoff_grid[i]);
        RQ_FREE(s2_payoff_grid[i]);
    }

    RQ_FREE(s1_payoff_grid);
    RQ_FREE(s2_payoff_grid);

    return result;
}
