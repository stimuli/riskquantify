/**
 * \file rq_cds_curve.h
 * \author Brett Hutley
 *
 * \brief The rq_cds_curve object implements a CDS curve.
 */
/*
** rq_cds_curve.h
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
#ifndef rq_cds_curve_h
#define rq_cds_curve_h

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
/** This is the main controlling structure for a cds curve
 */
typedef struct rq_cds_curve {
    struct rq_termstruct termstruct;
	double frequency; /**< The CDS fee frequency */
	double recovery_rate; /**< The recovery rate upon default */
    unsigned int num_elements; /**< the number of elements managed by this structure */
    unsigned int max_elements; /**< max number of elements before this structure needs to grow */
    struct rq_cds_curve_elem *spreads; /**< the actual survival rates */
	double *alpha;
	double *psi;
    enum rq_interpolation_method interpolation_method; /**< the interpolation method used to get a value between points */
} *rq_cds_curve_t;

/* -- structures -------------------------------------------------- */
/**
 * This structure defines an individual term structure element.
 */
struct rq_cds_curve_elem {
    double term;
    double survival_rate;
};

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_cds_curve is NULL */
RQ_EXPORT int rq_cds_curve_is_null(rq_cds_curve_t obj);

/** Get the date out of the curve element.
 */
RQ_EXPORT double rq_cds_curve_elem_get_term(const struct rq_cds_curve_elem *e);

/** Get the survival_rate out of the curve element.
 */
RQ_EXPORT double rq_cds_curve_elem_get_survival_rate(const struct rq_cds_curve_elem *e);

/** Allocate a new CDS curve.
 *
 * @param termstruct_id the termstruct ID of the CDS curve to build
 * @param from_date the date the CDS curve is based from
 * @return the newly allocated CDS curve. Caller is responsible for 
 * freeing.
 */
RQ_EXPORT rq_cds_curve_t 
rq_cds_curve_build(
    const char *termstruct_id
    );

/** Deallocate the memory for a cds curve.
 */
RQ_EXPORT void rq_cds_curve_free(rq_cds_curve_t cscrv);

/** Get the term structure ID associated with this CDS curve.
 */
RQ_EXPORT const char *rq_cds_curve_get_termstruct_id(const rq_cds_curve_t cscrv);

/** Get the underlying asset ID associated with this CDS curve.
 */
RQ_EXPORT const char *
rq_cds_curve_get_underlying_asset_id(const rq_cds_curve_t yc);

/** Set the underlying asset ID associated with this CDS curve.
 */
RQ_EXPORT void
rq_cds_curve_set_underlying_asset_id(rq_cds_curve_t yc, const char *asset_id);

/** Get the recovery rate from the CDS curve.
 */
RQ_EXPORT double rq_cds_curve_get_recovery_rate(const rq_cds_curve_t yc);

/** Set the recovery rate from the CDS curve.
 */
RQ_EXPORT void
rq_cds_curve_set_recovery_rate(rq_cds_curve_t yc, double recovery_rate);

/** Get the frequency from the CDS curve.
 */
RQ_EXPORT double rq_cds_curve_get_frequency(const rq_cds_curve_t yc);

/** Set the frequency from the CDS curve.
 */
RQ_EXPORT void
rq_cds_curve_set_frequency(rq_cds_curve_t yc, double recovery_rate);

/** Get the maximum date on our term structure.
 */
RQ_EXPORT double rq_cds_curve_get_longest_term(const rq_cds_curve_t cscrv);

/** Get the survival rate for the specified term (in years).
 */
RQ_EXPORT double rq_cds_curve_get_survival_rate(const rq_cds_curve_t ts, double term);

/** Set the survival rate for a particular date.
 */
RQ_EXPORT void rq_cds_curve_set_survival_rate(rq_cds_curve_t spread_curve, double term, double survival_rate);

/** Clone a CDS curve.
 *
 * Allocate a new CDS curve object and fill it with the
 * contents of the CDS curve passed as the argument.  
 */
RQ_EXPORT rq_cds_curve_t rq_cds_curve_clone(const rq_cds_curve_t cscrv);

/** Return the number of points on the CDS curve.
 */
RQ_EXPORT unsigned int rq_cds_curve_size(const rq_cds_curve_t cscrv);

/** Get a point on the CDS curve by offset. 
 *
 * The offset is zero based.  
 */
RQ_EXPORT struct rq_cds_curve_elem *
rq_cds_curve_element_at(
    const rq_cds_curve_t cscrv,
    unsigned int offset
    );


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

