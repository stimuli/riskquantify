/**
 * \file rq_vol_curve.h
 * \author Brett Hutley
 *
 * \brief The rq_vol_curve object implements a volatility curve.
 */
/*
** rq_vol_curve.h
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
#ifndef rq_vol_curve_h
#define rq_vol_curve_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_volatility.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/**
 * The volatility curve handle.
 */
typedef struct rq_vol_curve {
    unsigned int num_vols;
    unsigned int max_vols;
    struct rq_volatility *vols;
} *rq_vol_curve_t;


/** Test whether the rq_vol_curve is NULL */
RQ_EXPORT int rq_vol_curve_is_null(rq_vol_curve_t obj);

/**
 * Allocate a new vol curve
 */
RQ_EXPORT rq_vol_curve_t rq_vol_curve_alloc();

/**
 * Free an allocated vol curve
 */
RQ_EXPORT void rq_vol_curve_free(rq_vol_curve_t vol_curve);

/**
 * Add a volatility for the specified date to the vol curve
 */
RQ_EXPORT void rq_vol_curve_add(rq_vol_curve_t c, rq_date date, double vol);

/**
 * Get the volatility for the specified date from the vol curve
 */
RQ_EXPORT int 
rq_vol_curve_get_volatility(
    const rq_vol_curve_t c,
    rq_date date,
    double *vol
    );

/**
 * Deep copy the vol curve
 */
RQ_EXPORT rq_vol_curve_t rq_vol_curve_clone(const rq_vol_curve_t vol_curve);

/**
 * Get the number of points in the vol curve
 */
RQ_EXPORT unsigned rq_vol_curve_count(const rq_vol_curve_t vol_curve);

/**
 * Get the volatility and date at a specific offset
 */
RQ_EXPORT struct rq_volatility *rq_vol_curve_element_at(const rq_vol_curve_t vol_curve, unsigned offset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
