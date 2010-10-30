/**
 * \file rq_electricity_curve.h
 * \author Brett Hutley
 *
 */
/*
** rq_electricity_curve.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA 
*/
#ifndef rq_electricity_curve_h
#define rq_electricity_curve_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_tree_rb.h"
#include "rq_termstruct.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif


struct hourly_prices_for_day {
    rq_date date;
    double prices[25];
};

typedef struct rq_electricity_curve {
    struct rq_termstruct termstruct;

    rq_tree_rb_t price_curve;
} *rq_electricity_curve_t;

/** Allocate a new electricity forward curve.
 */
RQ_EXPORT rq_electricity_curve_t rq_electricity_curve_alloc();

/** Free an allocated electricity forward curve.
 */
RQ_EXPORT void rq_electricity_curve_free(rq_electricity_curve_t curve);

/** Add a price for a certain date and hour on the forward curve.
 */
RQ_EXPORT void rq_electricity_curve_add_price(rq_electricity_curve_t curve, rq_date date, unsigned char hour, double price);

/** Get the price for a certain date and hour from the forward curve.
 */
RQ_EXPORT double rq_electricity_curve_get_price(rq_electricity_curve_t curve, rq_date date, unsigned char hour);

/** Get an iterator from the forward curve.
 */
RQ_EXPORT rq_iterator_t rq_electricity_curve_get_iterator(rq_electricity_curve_t curve);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
