/**
 * \file rq_pricing_result.h
 * \author Brett Hutley
 *
 * \brief The rq_pricing_result structure allows us to return results
 * from the pricing adapters in a standard way.
 */
/*
** rq_pricing_result.h
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
#ifndef rq_pricing_result_h
#define rq_pricing_result_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_enum.h"
#include "rq_date.h"
#include "rq_assetflow_list.h"
#include "rq_named_variant_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- structs ----------------------------------------------------- */
struct rq_pricing_result {
    unsigned long results_returned;
    short results_need_freeing; /**< There is data that has been alloc'd from heap */
    short valid;
    double face_value;
    double value;
    double delta;
    double gamma;
    double theta;
    double vega;
    double rho;
    double exposure;
    struct {
        unsigned num_exposure_dates;
        rq_date *dates;
        double *exposures;
        double *fixed_exposures;
    } exposure_profile;
    struct {
        unsigned num_messages; 
        int *message_severity;
        int *message_result_types;
        char **messages;
    } messages;
    rq_assetflow_list_t assetflow_list;
    rq_named_variant_mgr_t named_value_results;
    rq_named_variant_mgr_t pricing_value_results;
    rq_named_variant_mgr_t tracing_value_results;
	rq_named_variant_mgr_t calibration_results;
    void *trade_details;
};

/** Allocate a new pricing result structure.
 */
RQ_EXPORT struct rq_pricing_result *rq_pricing_result_alloc();

/** Free a pricing result structure that was previously allocated by
 * rq_pricing_result_alloc.
 */
RQ_EXPORT void rq_pricing_result_free(struct rq_pricing_result *pricing_result);

/** Allocate exposure_profile space, freeing previous is necessary.
 */
RQ_EXPORT void rq_pricing_result_exposure_alloc(struct rq_pricing_result *pricing_result, long numNodes, int fixed);

/**
 * Initialize the pricing_result structure will all values set to zero
 */
/* RQ_EXPORT void rq_pricing_result_init(struct rq_pricing_result *pricing_result); */

/**
 * Free any exposure_profile data or failure_reasons data contained by
 * the pricing result
 */
RQ_EXPORT void rq_pricing_result_free_data(struct rq_pricing_result *pricing_result);

/**
 * Add a reason for the pricing failure
 */
RQ_EXPORT void rq_pricing_result_add_message(struct rq_pricing_result *pricing_result, int result_requested, enum rq_error_severity error_severity, const char *message);

/**
 * Add a reason for the pricing failure using a printf style
 * syntax. buf is a temporary buffer used to write the finished string
 * into.
 */
RQ_EXPORT void
rq_pricing_result_add_messagefmt(struct rq_pricing_result *pricing_result, int result_requested, enum rq_error_severity error_severity, char *buf, const char *message, ...);

/** Set the assetflow list member of the pricing result structure.
 */
RQ_EXPORT void rq_pricing_result_set_assetflow_list(struct rq_pricing_result *pricing_result, rq_assetflow_list_t assetflow_list);

/** Add a new double result to the named value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_named_value_double(struct rq_pricing_result *pricing_result, const char *name, double d);

/** Add a new string result to the named value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_named_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str);

/** Get the specific named value.
 *
 */
RQ_EXPORT rq_variant_t rq_pricing_result_get_named_value(struct rq_pricing_result *pricing_result, const char *name);

/** Add a new double result to the pricing value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_pricing_value_double(struct rq_pricing_result *pricing_result, const char *name, double d);

/** Add a new string result to the pricing value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_pricing_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str);

/** Get the specific pricing value.
 *
 */
RQ_EXPORT rq_variant_t rq_pricing_result_get_pricing_value(struct rq_pricing_result *pricing_result, const char *name);

/** Add a new double result to the tracing value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_tracing_value_double(struct rq_pricing_result *pricing_result, const char *name, double d);

/** Add a new string result to the tracing value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_tracing_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str);

/** Get the specific tracing value.
 *
 */
RQ_EXPORT rq_variant_t rq_pricing_result_get_tracing_value(struct rq_pricing_result *pricing_result, const char *name);

/** Add a new double result to the calibration value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_calibration_value_double(struct rq_pricing_result *pricing_result, const char *name, double d);

/** Add a new string result to the tracing value list.
 *
 */
RQ_EXPORT void rq_pricing_result_add_calibration_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str);

/** Get the specific tracing value.
 *
 */
RQ_EXPORT rq_variant_t rq_pricing_result_get_calibration_value(struct rq_pricing_result *pricing_result, const char *name);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
