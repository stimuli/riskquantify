/**
 * @file
 *
 * A Monte Carlo pricing model using stochastic volatility
 */
/*
** rq_pricing_monte_carlo_multi_factor.h
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
#ifndef rq_pricing_monte_carlo_multi_factor_h
#define rq_pricing_monte_carlo_multi_factor_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_matrix.h"
#include "rq_pricing_monte_carlo.h"
#include "rq_simulation_results.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT short
rq_pricing_monte_carlo_multi_factor(
    double *values, /**< N values, should be filled with the starting values */
    double tau_d, /**< time to expiry/delivery in years. */
    rq_matrix_t correl_matrix, /**< The correlation matrix (NxN) */
    unsigned long num_paths, /**< The number of paths */
    unsigned long num_timesteps, /**< The number of timesteps */
    double *timestep_vals, /**< An array that is filled out with the values returned by the calc_timestep function, on this path */
    double *random_factors, /**< An array of N random factors. */
    double *terminal_distribution, /**< An array that is filled with the terminal distribution. */
    double (*random_func)(), /**< A function for providing random numbers. */
    void *user_defined, /**< A pointer to user defined data, that is passed to the callback functions */
    void (*user_defined_init)(void *user_defined), /** < A call-back function for initializing the pricing. May be NULL. */
    void (*user_defined_path_init)(void *user_defined, unsigned long path, double *values), /**< A callback function that is called before each path. May be NULL. */
    double (*calc_timestep)(void *user_defined, unsigned long step, double *values, double *factors, double *timestep_vals), /**< A callback function that is called on each timestep. Returns the current value for the timestep. */
    double (*calc_payoff)(void *user_defined, unsigned long path, double *values, double *timestep_vals, unsigned long num_timesteps), /** A callback function to calculate the payoff. Called at the end of the path */
    void (*user_defined_free)(void *user_defined) /**< A callback function to free any user-defined data at the end of the pricing function. May be NULL. */,
    struct rq_simulation_results *sim_results /* Used to return the results */
    );


/** A function to calculate exactly the same dt as the model uses.
 */
RQ_EXPORT double
rq_pricing_monte_carlo_multi_factor_get_dt(
    double tau_d,
    unsigned long num_timesteps
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
