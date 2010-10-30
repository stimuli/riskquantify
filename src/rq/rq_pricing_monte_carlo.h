/**
 * \file rq_pricing_monte_carlo.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_monte_carlo files provide pricing routines
 * based on Monte Carlo simulation.
 */
/*
** rq_pricing_monte_carlo.h
** 
** Written by Brett Hutley - brett@hutley.net
** 
** Copyright (C) 2002-2008 Brett Hutley
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

#ifndef rq_monte_carlo_h
#define rq_monte_carlo_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#include "rq_config.h"

struct rq_pricing_monte_carlo_timestep {
    int timestep;
    int num_timesteps;
    int path;
    int num_paths;
    double tau;
    double tau_d;
    double initial_S;
    double r_dom;
    double r_for;
    double dt;
    double log_S;
    double weiner;
};

RQ_EXPORT double
rq_pricing_monte_carlo(
    double S, /* assume this rate is passed in domestic over foreign terms */
    double r_dom, /* aka the numerator rate (continuously compounded) */
    double r_for, /* aka the denominator rate (continuously compounded) */
    double sigma,
    double tau_d,
    int num_iters,
    double *terminal_distribution, /* being an array of num_iters doubles, this returns the distribution */
    int num_timesteps,
    double *timestep_vals, /* being an array of num_timesteps + 1 doubles, this is used WITHIN the model for antithetic purposes */
    void *user_defined,
    void (*user_defined_init)(void *user_defined),
    double (*calc_timestep)(void *user_defined, struct rq_pricing_monte_carlo_timestep *timestep, double prev_value),
    double (*calc_terminal)(void *user_defined, double log_S, double prev_value),
    void (*user_defined_clear)(void *user_defined),
    void (*user_defined_free)(void *user_defined)
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif


#endif
