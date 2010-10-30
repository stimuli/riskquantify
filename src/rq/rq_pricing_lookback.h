/**
 * \file rq_pricing_lookback.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_lookback files provide lookback option
 * pricing routines.
 */
/*
** rq_pricing_lookback.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#ifndef rq_pricing_lookback_h
#define rq_pricing_lookback_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double 
rq_pricing_lookback(
    short call,
    double S,
    double Smin,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d
    );

RQ_EXPORT double 
rq_pricing_lookback_floating_strike(
    short call, 
    double S, 
    double SMin, 
    double SMax, 
    double T, 
    double r, 
    double b, 
    double v
    );

RQ_EXPORT double 
rq_pricing_lookback_fixed_strike(
    short call, 
    double S, 
    double SMin, 
    double SMax, 
    double X,
    double T, 
    double r, 
    double b, 
    double v
    );

RQ_EXPORT double
rq_pricing_lookback_partial_time_floating_strike(
    short call, 
    double S, 
    double SMin, 
    double SMax, 
    double t1,
    double T2, 
    double r, 
    double b, 
    double v, 
    double lambda
    );

RQ_EXPORT double 
rq_pricing_lookback_partial_time_fixed_strike(
    short call, 
    double S, 
    double X, 
    double t1,
    double T2, 
    double r, 
    double b, 
    double v
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
