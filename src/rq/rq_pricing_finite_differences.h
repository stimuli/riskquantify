/* \file rq_pricing_finite_differences.h
 * \author Brett Hutley
 *
 * \brief An implementation of a finite differences method for option
 * pricing.  This is just the start of the implementation, and will be
 * worked on.  At the moment it just prices European Vanilla options.
 *
 * @author Brett Hutley
 */
/*
** rq_pricing_finite_differences.h
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
#ifndef rq_pricing_finite_differences_h
#define rq_pricing_finite_differences_h

#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif


RQ_EXPORT double
rq_pricing_finite_differences_equity_american(
    short call,
    double S,
    double X,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d,
    unsigned num_timesteps,
    unsigned num_values,
    double *mesh, /* num_values + 1 */
    double *value,  /* num_values + 1 */
    double *vold,  /* num_values + 1 */
    double *ivalue,  /* num_values + 1 */
    double *a,  /* num_values + 1 */
    double *b,  /* num_values + 1 */
    double *c,  /* num_values + 1 */
    double *tvalue,  /* num_timesteps + 1 */
    int *err /* error return */
    );

RQ_EXPORT double
rq_pricing_finite_differences_equity_bermudan(
    short call,
    double S,
    double X,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d,
    double tau_ex_start,
    double tau_ex_end,
    unsigned num_timesteps,
    unsigned num_values,
    double *mesh, /* num_values + 1 */
    double *value,  /* num_values + 1 */
    double *vold,  /* num_values + 1 */
    double *ivalue,  /* num_values + 1 */
    double *a,  /* num_values + 1 */
    double *b,  /* num_values + 1 */
    double *c,  /* num_values + 1 */
    double *tvalue,  /* num_timesteps + 1 */
    int *err
    );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
