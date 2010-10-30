/**
 * \file rq_pricing_normdist.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_normdist files provide functions based around
 * various distributions.
 */
/*
** rq_pricing_normdist.h
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

#ifndef rq_pricing_normdist_h
#define rq_pricing_normdist_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#include "rq_config.h"

RQ_EXPORT double rq_pricing_norm_dist(double z);

/** Calculate the CDF for Normal distribution with double precision
   - adopted from "Option Pricing Formulas" by E.G. Haug 
*/
RQ_EXPORT double rq_pricing_cumul_norm_dist(double z);

RQ_EXPORT double rq_pricing_density(double x, double y, double ad, double bd, double rho);

RQ_EXPORT double rq_pricing_norm_density(double x);

/** The Drezner 1978 Algorithm */
RQ_EXPORT double rq_pricing_cumul_bivar_norm_dist(double a, double b, double r);

/** The Genz 2004 Algorithm (Quasi Double Precision) */
RQ_EXPORT double rq_pricing_cumul_bivar_norm_dist2(double a, double b, double r);

/** Return the natural log of the gamma function for xx.
 * 
 * This function was taken from "Numerical Recipes in C"
 */
RQ_EXPORT double rq_log_gamma(double xx);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif


#endif
