/*
** rq_pricing_monte_carlo.c
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
#include "rq_pricing_monte_carlo.h"
#include "rq_pricing_normdist.h"
#include "rq_random.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

RQ_EXPORT double
rq_pricing_monte_carlo(
    double S,
    double r_dom,
    double r_for,
    double sigma,
    double tau_d,
    int num_paths,
    double *terminal_distribution,
    int num_timesteps,
    double *timestep_vals,
    void *user_defined,
    void (*user_defined_init)(void *user_defined),
    double (*calc_timestep)(void *user_defined, struct rq_pricing_monte_carlo_timestep *timestep, double prev_value),
    double (*calc_terminal)(void *user_defined, double log_S, double prev_value),
    void (*user_defined_clear)(void *user_defined),
    void (*user_defined_free)(void *user_defined)
    ) 
{
    double dt = tau_d / num_timesteps;
    double b = r_dom - r_for;

    /* from Ito's Lemma the process followed by ln S has these factors */
    double drift_factor = (b - 0.5 * sigma * sigma) * dt;
    double weiner_factor = sqrt(dt) * sigma;

    double log_S = log(S);

    double value = 0.0;
    int i;
    struct rq_pricing_monte_carlo_timestep timestep;

    timestep.num_timesteps = num_timesteps;
    timestep.num_paths = num_paths;
    timestep.tau_d = tau_d;
    timestep.initial_S = S;
    timestep.r_dom = r_dom;
    timestep.r_for = r_for;
    timestep.dt = dt;

    for (i = 0; i < num_paths; i++)
    {
        double s = log_S; /* reinitialize log(S) */
        int j;
        double terminal_value;
        double prev_value = 0.0;

        timestep.path = i+1;

        if (user_defined_init)
            (*user_defined_init)(user_defined);

		for (j = 1; j <= num_timesteps; j++)
		{
            double weiner_process = rq_random_normal();
            s += drift_factor + (weiner_factor * weiner_process);
            if (calc_timestep)
            {
                timestep.timestep = j;
                timestep.tau = j * dt;
                timestep.log_S = s;
                timestep.weiner = weiner_process;
                prev_value = (*calc_timestep)(user_defined, &timestep, prev_value);
            }
		}

        terminal_value = (*calc_terminal)(user_defined, s, 0.0);
        terminal_distribution[i] = terminal_value;
        value += terminal_value;

        if (user_defined_clear)
            (*user_defined_clear)(user_defined);
    }

    /* value is average of end results */
	value /= num_paths;

    /* discounted back to today */
    value *= exp(-r_dom * tau_d);

    if (user_defined_free)
        (*user_defined_free)(user_defined);

    return value;
}
