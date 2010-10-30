/**
 * \file rq_pricing_fx.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_fx files provide pricing routines for
 * physical FX
 */
/*
** rq_pricing_fx.h
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
#ifndef rq_pricing_fx_h
#define rq_pricing_fx_h

#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double 
rq_pricing_fx_ccir(
	double S, /* quoted in domestic over foreign terms */
	double K, /* quoted in domestic over foreign terms */
    double r_dom, /* continuously compounded, the numerator currency */
    double r_for, /* continuously compounded, the denominator currency */
    double tau_d /* days from spot to delivery, over 365.0 */
    );


RQ_EXPORT double 
rq_pricing_fx(
	double S, /**< The spot rate, quoted in domestic over foreign terms */
	double K, /**< The agreed exchange rate, quoted in domestic over foreign terms */
    double df_dom, /**< discount factor, the numerator currency */
    double df_for /**< discount factor, the denominator currency */
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
