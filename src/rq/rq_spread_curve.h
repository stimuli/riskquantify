/**
 * \file rq_spread_curve.h
 * \author Brett Hutley
 *
 * \brief The rq_spread_curve object implements a spread curve.
 */
/*
** rq_spread_curve.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#ifndef rq_spread_curve_h
#define rq_spread_curve_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_rate.h"
#include "rq_enum.h"
#include "rq_termstruct.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
/** This is the main controlling structure for a spread curve
 */
typedef struct rq_spread_curve {
    struct rq_termstruct termstruct;
    rq_date from_date; /**< the date all the spreads are based from */
    unsigned int num_spreads; /**< the number of spreads managed by this structure */
    unsigned int max_spreads; /**< max number of spreads before this structure needs to grow */
    struct rq_spread_curve_elem *spreads; /**< the actual spreads */
    enum rq_interpolation_method interpolation_method; /**< the interpolation method used to get a spread between points */
} *rq_spread_curve_t;

/* -- structures -------------------------------------------------- */
/**
 * This structure defines an individual term structure element.
 */
struct rq_spread_curve_elem {
    rq_date date;
    double spread;
};

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_spread_curve is NULL */
RQ_EXPORT int rq_spread_curve_is_null(rq_spread_curve_t obj);

/** Get the date out of the spread curve element.
 */
RQ_EXPORT rq_date rq_spread_curve_elem_get_date(const struct rq_spread_curve_elem *e);

/** Get the spread out of the spread curve element.
 */
RQ_EXPORT double rq_spread_curve_elem_get_spread(const struct rq_spread_curve_elem *e);

/** Allocate a new spread curve.
 *
 * @param termstruct_id the termstruct ID of the spread curve to build
 * @param from_date the date the spread curve is based from
 * @return the newly allocated spread curve. Caller is responsible for 
 * freeing.
 */
RQ_EXPORT rq_spread_curve_t 
rq_spread_curve_build(
    const char *termstruct_id,
    rq_date from_date
    );

/** Deallocate the memory for a spread curve.
 */
RQ_EXPORT void rq_spread_curve_free(rq_spread_curve_t cscrv);

/** Get the term structure ID associated with this spread curve.
 */
RQ_EXPORT const char *rq_spread_curve_get_termstruct_id(const rq_spread_curve_t cscrv);

/** Get the underlying asset ID associated with this spread curve.
 */
RQ_EXPORT const char *
rq_spread_curve_get_underlying_asset_id(const rq_spread_curve_t yc);

/** Set the underlying asset ID associated with this spread curve.
 */
RQ_EXPORT void
rq_spread_curve_set_underlying_asset_id(rq_spread_curve_t yc, const char *asset_id);

/** Get the date from which the spread curve is based.
 */
RQ_EXPORT rq_date rq_spread_curve_get_curve_date(const rq_spread_curve_t yc);

/** Get the maximum date on our term structure.
 */
RQ_EXPORT rq_date rq_spread_curve_get_last_date(const rq_spread_curve_t cscrv);

/** Get a spread from the spread curve curve. The spread
 * is for the for_date specified.
 */
RQ_EXPORT double rq_spread_curve_get_spread(const rq_spread_curve_t ts, rq_date for_date);

/** Set the spread for a particular date.
 */
RQ_EXPORT void rq_spread_curve_set_spread(rq_spread_curve_t spread_curve, rq_date for_date, double spread);

/** Clone a spread curve.
 *
 * Allocate a new spread curve object and fill it with the
 * contents of the spread curve passed as the argument.  
 */
RQ_EXPORT rq_spread_curve_t rq_spread_curve_clone(const rq_spread_curve_t cscrv);

/** Return the number of points on the spread curve.
 */
RQ_EXPORT unsigned int rq_spread_curve_size(const rq_spread_curve_t cscrv);

/** Get a point on the spread curve by offset. 
 *
 * The offset is zero based.  
 */
RQ_EXPORT struct rq_spread_curve_elem *
rq_spread_curve_element_at(
    const rq_spread_curve_t cscrv,
    unsigned int offset
    );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

