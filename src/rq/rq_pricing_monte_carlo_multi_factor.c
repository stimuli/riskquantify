/*
** rq_pricing_monte_carlo_multi_factor.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include "rq_pricing_monte_carlo_multi_factor.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

RQ_EXPORT double
rq_pricing_monte_carlo_multi_factor_get_dt(
    double tau_d,
    unsigned long num_timesteps
    )
{
    return tau_d / (double)num_timesteps;
}

RQ_EXPORT short
rq_pricing_monte_carlo_multi_factor(
    double *values,
    double tau_d,
    rq_matrix_t correl_matrix,
    unsigned long num_paths,
    unsigned long num_timesteps,
    double *timestep_vals,
    double *random_factors,
    double *terminal_distribution,
    double (*random_func)(),
    void *user_defined,
    void (*user_defined_init)(void *user_defined),
    void (*user_defined_path_init)(void *user_defined, unsigned long path, double *values),
    double (*calc_timestep)(void *user_defined, unsigned long step, double *values, double *factors, double *timestep_vals),
    double (*calc_payoff)(void *user_defined, unsigned long path, double *values, double *timestep_vals, unsigned long num_timesteps),
    void (*user_defined_free)(void *user_defined),
    struct rq_simulation_results *sim_results
    )
{
    double dt = tau_d / (double)num_timesteps;
    double sqrt_dt = sqrt(dt);
    unsigned long path;
    unsigned long num_factors = rq_matrix_get_rows(correl_matrix);
    double value = 0.0;
    short failed = 0;

    /* build a matrix to hold the cholesky results */
    rq_matrix_t cholesky_matrix = rq_matrix_build(
        rq_matrix_get_rows(correl_matrix),
        rq_matrix_get_columns(correl_matrix)
        );

    if ((failed = rq_matrix_cholesky(correl_matrix, cholesky_matrix)) != 0)
    {
        rq_matrix_free(cholesky_matrix);
        return failed;
    }

    /* rq_matrix_print(cholesky_matrix); */

    if (user_defined_init)
        (*user_defined_init)(user_defined);

    for (path = 0; path < num_paths; path++)
    {
        unsigned long step;
        double terminal_value;

        if (user_defined_path_init)
            (*user_defined_path_init)(user_defined, path, values);
        
		for (step = 0; step < num_timesteps; step++)
		{
            unsigned long i;
            unsigned long r;
            unsigned long c;

            for (i = 0; i < num_factors; i++)
                random_factors[i] = (*random_func)() * sqrt_dt;

            /* multiply by matrix */
            for (r = 0; r < num_factors; r++)
            {
                double sum = 0.0;

                for (c = 0; c < num_factors; c++)
                    sum += rq_matrix_get(cholesky_matrix, r, c) * random_factors[c];

                random_factors[r] = sum;
            }
/*
            for (i = 1; i < num_factors; i++)
            {
                double rho = rq_matrix_get(correl_matrix, 0, i);
                random_factors[i] = rho * random_factors[0] + sqrt(1.0 - rho * rho) * random_factors[i];
            }
*/
            timestep_vals[step] = (*calc_timestep)(user_defined, step, values, random_factors, timestep_vals);
		}

        terminal_value = (*calc_payoff)(user_defined, path, values, timestep_vals, num_timesteps);
        terminal_distribution[path] = terminal_value;
        value += terminal_value;
    }

    /* value is average of end results */
	value /= (double)num_paths;

    /* discounted back to today */
    /* value *= exp(-r_dom * tau_d); */

    if (user_defined_free)
        (*user_defined_free)(user_defined);

    sim_results->mean = value;

    return 0;
}
