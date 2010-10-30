/**
 * \file rq_ir_vol_surface.h
 * \author Brett Hutley
 *
 * \brief The rq_ir_vol_surface object implements a volatility surface.
 */
/*
** rq_ir_vol_surface.h
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
#ifndef rq_ir_vol_surface_h
#define rq_ir_vol_surface_h

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
typedef struct rq_ir_vol_surface {
    struct rq_termstruct termstruct;
    unsigned short num_curves;
    unsigned short max_curves;

    double *tenors;
    rq_vol_curve_t *vol_curves;
} *rq_ir_vol_surface_t;


/** Test whether the rq_ir_vol_surface is NULL */
RQ_EXPORT int rq_ir_vol_surface_is_null(rq_ir_vol_surface_t obj);

/** Allocate a new vol surface.
 */
RQ_EXPORT rq_ir_vol_surface_t rq_ir_vol_surface_alloc(const char *termstruct_id);

/** Free an allocated vol surface.
 */
RQ_EXPORT void rq_ir_vol_surface_free(rq_ir_vol_surface_t ir_vol_surface);

/** Get the termstruct id for this vol surface.
 */
RQ_EXPORT const char *rq_ir_vol_surface_get_termstruct_id(const rq_ir_vol_surface_t s);

/** Get the underlying asset ID associated with this volatility surface.
 */
RQ_EXPORT const char *
rq_ir_vol_surface_get_underlying_asset_id(const rq_ir_vol_surface_t yc);

/** Set the underlying asset ID associated with this volatility surface.
 */
RQ_EXPORT void
rq_ir_vol_surface_set_underlying_asset_id(rq_ir_vol_surface_t yc, const char *asset_id);

/** Add a new curve for the specified delta to this vol surface.
 */
RQ_EXPORT void rq_ir_vol_surface_add(rq_ir_vol_surface_t s, double tenor, rq_vol_curve_t c);

/** Get the volatility from the surface for the specified delta.
 */
RQ_EXPORT int
rq_ir_vol_surface_get_volatility(
    const rq_ir_vol_surface_t s,
    double underlyingTenor,
    rq_date optionMaturity,
    double *vol
    );

/** Set a volatility in the vol surface for the specified delta.
 */
RQ_EXPORT void
rq_ir_vol_surface_set_volatility(
    rq_ir_vol_surface_t s,
    double underlyingTenor,
    rq_date optionMaturity,
    double vol
    );

/** Deep copy this vol surface.
 */
RQ_EXPORT rq_ir_vol_surface_t
rq_ir_vol_surface_clone(
    const rq_ir_vol_surface_t s
    );

/** Return a count of the number of curves in the vol surface.
 */
RQ_EXPORT unsigned rq_ir_vol_surface_count(const rq_ir_vol_surface_t s);

/** Get the curve at the specified offset in the vol surface.
 */
RQ_EXPORT const rq_vol_curve_t rq_ir_vol_surface_element_at(const rq_ir_vol_surface_t s, unsigned offset);

/** Get the tenor (in years) of the curve at the specified offset in the vol surface.
 */
RQ_EXPORT const double rq_ir_vol_surface_tenor_at(const rq_ir_vol_surface_t s, unsigned offset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
