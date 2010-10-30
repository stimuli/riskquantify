/**
 * \file rq_vol_surface.h
 * \author Brett Hutley
 *
 * \brief The rq_vol_surface object implements a volatility surface.
 */
/*
** rq_vol_surface.h
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
#ifndef rq_vol_surface_h
#define rq_vol_surface_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_vol_curve.h"
#include "rq_termstruct.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/**
 * The volatility surface handle.
 */
typedef struct rq_vol_surface {
    struct rq_termstruct termstruct;
    unsigned short num_curves;
    unsigned short max_curves;
    unsigned short is_strike_vol;
    double *deltas;
    rq_vol_curve_t *vol_curves;
} *rq_vol_surface_t;


/** Test whether the rq_vol_surface is NULL */
RQ_EXPORT int rq_vol_surface_is_null(rq_vol_surface_t obj);

/** Allocate a new vol surface.
 */
RQ_EXPORT rq_vol_surface_t rq_vol_surface_alloc(const char *termstruct_id);

/** Free an allocated vol surface.
 */
RQ_EXPORT void rq_vol_surface_free(rq_vol_surface_t vol_surface);

/** Get the termstruct id for this vol surface.
 */
RQ_EXPORT const char *rq_vol_surface_get_termstruct_id(const rq_vol_surface_t s);

/** Get the underlying asset ID associated with this volatility surface.
 */
RQ_EXPORT const char *
rq_vol_surface_get_underlying_asset_id(const rq_vol_surface_t yc);

/** Set the underlying asset ID associated with this volatility surface.
 */
RQ_EXPORT void
rq_vol_surface_set_underlying_asset_id(rq_vol_surface_t yc, const char *asset_id);

/** Add a new curve for the specified delta or strike to this vol surface.
 */
RQ_EXPORT void rq_vol_surface_add(rq_vol_surface_t s, double delta_or_strike, rq_vol_curve_t c);

/** Get the volatility from the surface for the specified delta or strike.
 */
RQ_EXPORT int
rq_vol_surface_get_volatility(
    const rq_vol_surface_t s,
    double delta_or_strike,
    rq_date date,
    double *vol
    );

/** Set a volatility in the vol surface for the specified delta or strike.
 */
RQ_EXPORT void
rq_vol_surface_set_volatility(
    rq_vol_surface_t s,
    double delta_or_strike,
    rq_date date,
    double vol
    );

/** Deep copy this vol surface.
 */
RQ_EXPORT rq_vol_surface_t
rq_vol_surface_clone(
    const rq_vol_surface_t s
    );

/** Return a count of the number of curves in the vol surface.
 */
RQ_EXPORT unsigned rq_vol_surface_count(const rq_vol_surface_t s);

/** Get the curve at the specified offset in the vol surface.
 */
RQ_EXPORT const rq_vol_curve_t rq_vol_surface_element_at(const rq_vol_surface_t s, unsigned offset);

/** Get the delta of the curve at the specified offset in the vol surface.
 */
RQ_EXPORT double rq_vol_surface_delta_at(const rq_vol_surface_t s, unsigned offset);

/** Test whether the rq_vol_surface is in terms of strike instead of delta */
RQ_EXPORT unsigned short rq_vol_surface_is_strike_vol(rq_vol_surface_t obj);

/** Set the is_strike_vol flag of the volatility surface.
 */
RQ_EXPORT void rq_vol_surface_set_is_strike_vol(rq_vol_surface_t s, unsigned short is_strike_vol);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
