/**
 * \file rq_bootstrap_config.h
 * \author Brett Hutley
 *
 * \brief The rq_bootstrap_config class stores the list of asset class
 * IDs that are used to bootstrap a term structure.
 */
/*
** rq_bootstrap_config.h
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
#ifndef rq_bootstrap_config_h
#define rq_bootstrap_config_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_bootstrap_config {
    const char **rate_class_ids;
    long *rate_class_offsets;
    double *basis_points_to_add; /**< An array of basis points to add to the rate */
    double *percentages_to_add; /**< An array of percentages to add to the rate */
    unsigned int num_rate_class_ids;
    unsigned int max_rate_class_ids;

    const char *curve_id;
    enum rq_termstruct_type termstruct_type;
    enum rq_interpolation_method interpolation_method;
    enum rq_extrapolation_method extrapolation_method_start; /**< extrapolation method prior to set of points */
    enum rq_extrapolation_method extrapolation_method_end; /**< extrapolation method post set of points */
	enum rq_zero_method zero_method; /**< definition of zero rate as specified by user **/
    enum rq_billFuture_method billFuture_method; /**< Affects ts date, strip(default)/tenor. */
    enum rq_swapParInterpolation_method swapParInterpolation_method; /**< Linear or spline. */
    enum rq_swapDayCount_method swapDayCount_method; /**< Simple (use frequency) or daycount. */
	unsigned int zero_method_compound_frequency; /**< frequency (#times in a year) compounding is done */
    enum rq_day_count_convention default_day_count_convention;

    const char *bootstrap_method_id;
    const char *curve_id1;
    const char *curve_id2;
    const char *asset_id;
} *rq_bootstrap_config_t;


/** Test whether the rq_bootstrap_config is NULL */
RQ_EXPORT int rq_bootstrap_config_is_null(rq_bootstrap_config_t obj);

/**
 * Build a new bootstrap configuration
 */
RQ_EXPORT rq_bootstrap_config_t 
rq_bootstrap_config_build(
    const char *curve_id, 
    const char *asset_id,
    enum rq_termstruct_type termstruct_type,
    enum rq_interpolation_method interpolation_method,
    enum rq_extrapolation_method extrapolation_method_start,
    enum rq_extrapolation_method extrapolation_method_end,
	enum rq_zero_method zero_method,
	unsigned int zero_method_compound_frequency,
    enum rq_day_count_convention default_day_count_convention);

/**
 * Free a bootstrap configuration
 */
RQ_EXPORT void rq_bootstrap_config_free(rq_bootstrap_config_t config);

/**
 * Get the curve ID that the bootstrap configuration builds
 */
RQ_EXPORT const char *rq_bootstrap_config_get_curve_id(const rq_bootstrap_config_t c);

/**
 * Get the type of term structure this bootstrap configuration is for
 */
RQ_EXPORT enum rq_termstruct_type rq_bootstrap_config_get_termstruct_type(const rq_bootstrap_config_t c);

/**
 * Get the number of rate class IDs in the bootstrap configuration
 */
RQ_EXPORT unsigned int rq_bootstrap_config_get_num_rate_class_ids(const rq_bootstrap_config_t c);

/**
 * Get the rate class ID at a particular offset (zero based)
 */
RQ_EXPORT const char *rq_bootstrap_config_get_rate_class_id_at(const rq_bootstrap_config_t c, unsigned int index);

/**
 * Add a new rate class ID to the bootstrap configuration
 */
RQ_EXPORT void rq_bootstrap_config_add_rate_class_id(rq_bootstrap_config_t c, const char *rate_class_id);

RQ_EXPORT long rq_bootstrap_config_get_rate_class_offset_at(const rq_bootstrap_config_t c, unsigned int index);

RQ_EXPORT void rq_bootstrap_config_set_rate_class_offset_at(const rq_bootstrap_config_t c, unsigned int index, long offset);


/**
 * Gets the number of basis points to add to the rate while bootstrapping
 */
RQ_EXPORT double 
rq_bootstrap_config_get_basis_points_to_add_at(
    const rq_bootstrap_config_t c, 
    unsigned int index
    );

/**
 * Sets the number of basis points to add to the rate while bootstrapping
 */
RQ_EXPORT void 
rq_bootstrap_config_set_basis_points_to_add_at(
    const rq_bootstrap_config_t c, 
    unsigned int index, 
    double basis_points_to_add
    );

/**
 * Gets a percentage to add on to the rate while bootstrapping the curve
 */
RQ_EXPORT double 
rq_bootstrap_config_get_percentage_to_add_at(
    const rq_bootstrap_config_t c, 
    unsigned int index
    );

/**
 * Set the percentage to add on to the rate while bootstrapping the curve
 */
RQ_EXPORT void 
rq_bootstrap_config_set_percentage_to_add_at(
    const rq_bootstrap_config_t c, 
    unsigned int index, 
    double percentage_to_add
    );

/**
 * Set the bootstrapping method ID
 */
RQ_EXPORT void rq_bootstrap_config_set_bootstrap_method_id(rq_bootstrap_config_t config, const char *bootstrap_method_id);

/**
 * Get the bootstrapping method ID
 */
RQ_EXPORT const char *rq_bootstrap_config_get_bootstrap_method_id(rq_bootstrap_config_t config);

/**
 * Set the curve ID for the first curve used in implying this curve
 */
RQ_EXPORT void rq_bootstrap_config_set_curve_id1(rq_bootstrap_config_t config, const char *curve_id1);

/**
 * Get the curve ID for the first curve used in implying this curve
 */
RQ_EXPORT const char *rq_bootstrap_config_get_curve_id1(rq_bootstrap_config_t config);

/**
 * Set the curve ID for the second curve used in implying this curve
 */
RQ_EXPORT void rq_bootstrap_config_set_curve_id2(rq_bootstrap_config_t config, const char *curve_id2);

/**
 * Get the curve ID for the second curve used in implying this curve
 */
RQ_EXPORT const char *rq_bootstrap_config_get_curve_id2(rq_bootstrap_config_t config);

/**
 * Set the asset ID indicating the underlying asset in impied curves
 */
RQ_EXPORT void
rq_bootstrap_config_set_asset_id(rq_bootstrap_config_t config, const char *asset_id);

/**
 * Get the asset ID indicating the underlying asset in impied curves
 */
RQ_EXPORT const char *rq_bootstrap_config_get_asset_id(rq_bootstrap_config_t config);

/**
 * Get the interpolation method
 */
RQ_EXPORT enum rq_interpolation_method rq_bootstrap_config_get_interpolation_method(const rq_bootstrap_config_t config);

/**
 * Get the start extrapolation method
 */
RQ_EXPORT enum rq_extrapolation_method rq_bootstrap_config_get_extrapolation_method_start(const rq_bootstrap_config_t config);

/**
 * Get the end extrapolation method
 */
RQ_EXPORT enum rq_extrapolation_method rq_bootstrap_config_get_extrapolation_method_end(const rq_bootstrap_config_t config);

/**
 * Get the zero method
 **/
RQ_EXPORT enum rq_zero_method rq_bootstrap_config_get_zero_method(const rq_bootstrap_config_t config);

/**
 * Get the Bill Future method
 **/
RQ_EXPORT enum rq_billFuture_method rq_bootstrap_config_get_billFuture_method(const rq_bootstrap_config_t config);
RQ_EXPORT void
rq_bootstrap_config_set_billFuture_method(rq_bootstrap_config_t config, enum rq_billFuture_method);

/**
 * Get the Swap Par Rate Interpolation method
 **/
RQ_EXPORT enum rq_swapParInterpolation_method rq_bootstrap_config_get_swapParInterpolation_method(const rq_bootstrap_config_t config);
RQ_EXPORT void
rq_bootstrap_config_set_swapParInterpolation_method(rq_bootstrap_config_t config, enum rq_swapParInterpolation_method);

/**
 * Get the Swap Day Count method
 **/
RQ_EXPORT enum rq_swapDayCount_method rq_bootstrap_config_get_swapDayCount_method(const rq_bootstrap_config_t config);
RQ_EXPORT void
rq_bootstrap_config_set_swapDayCount_method(rq_bootstrap_config_t config, enum rq_swapDayCount_method);

/**
 * Get the compound frequency
 **/
RQ_EXPORT unsigned int
rq_bootstrap_config_get_zero_method_compound_frequency(const rq_bootstrap_config_t config);

/**
 * Get the default day count convention 
 */
RQ_EXPORT enum rq_day_count_convention rq_bootstrap_config_get_default_day_count_convention(const rq_bootstrap_config_t config);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
