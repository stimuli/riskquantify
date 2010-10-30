/*
** rq_pricing_result.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#include "rq_pricing_result.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

RQ_EXPORT struct rq_pricing_result *
rq_pricing_result_alloc()
{
    struct rq_pricing_result *pricing_result = (struct rq_pricing_result *)calloc(1, sizeof(struct rq_pricing_result));

    return pricing_result;
}

RQ_EXPORT void
rq_pricing_result_free_exposure(struct rq_pricing_result *pricing_result)
{
    if (pricing_result->exposure_profile.dates)
    {
        RQ_FREE(pricing_result->exposure_profile.dates);
        pricing_result->exposure_profile.dates = NULL;
    }
    if (pricing_result->exposure_profile.exposures)
    {
        RQ_FREE(pricing_result->exposure_profile.exposures);
        pricing_result->exposure_profile.exposures = NULL;
    }
    if (pricing_result->exposure_profile.fixed_exposures)
    {
        RQ_FREE(pricing_result->exposure_profile.fixed_exposures);
        pricing_result->exposure_profile.fixed_exposures = NULL;
    }
}

RQ_EXPORT void
rq_pricing_result_exposure_alloc(struct rq_pricing_result *pricing_result, long numNodes, int fixed)
{
    rq_pricing_result_free_exposure(pricing_result);
    pricing_result->exposure_profile.num_exposure_dates = numNodes;
    pricing_result->exposure_profile.dates = (rq_date *)calloc(numNodes, sizeof(rq_date));
    pricing_result->results_need_freeing = 1;

    if (fixed)
    {
        pricing_result->exposure_profile.fixed_exposures = (double *)calloc(numNodes, sizeof(double));
    }
    else
    {
        pricing_result->exposure_profile.exposures = (double *)calloc(numNodes, sizeof(double));
    }
}

RQ_EXPORT void
rq_pricing_result_free(struct rq_pricing_result *pricing_result)
{
    rq_pricing_result_free_data(pricing_result);
	/* rq_named_variant_mgr_free(pricing_result->named_value_results); */

    free(pricing_result);
}

#if 0
RQ_EXPORT void 
rq_pricing_result_init(struct rq_pricing_result *pricing_result)
{
    pricing_result->results_returned = 0;
    pricing_result->results_need_freeing = 0;
    pricing_result->valid = 0;
    pricing_result->face_value = 0;
    pricing_result->value = 0;
    pricing_result->delta = 0;
    pricing_result->gamma = 0;
    pricing_result->theta = 0;
    pricing_result->vega = 0;
    pricing_result->rho = 0;
    pricing_result->exposure = 0;

    pricing_result->exposure_profile.num_exposure_dates = 0;
    pricing_result->exposure_profile.dates = 0;
    pricing_result->exposure_profile.exposures = 0;
    pricing_result->exposure_profile.fixed_exposures = 0;

    pricing_result->messages.num_messages = 0;
    pricing_result->messages.message_severity = 0;
    pricing_result->messages.message_result_types = 0;
    pricing_result->messages.messages = 0;

    pricing_result->assetflow_list = NULL;

    pricing_result->object_mgr = rq_object_mgr_alloc();
    pricing_result->named_value_results = rq_object_alloc_tree(pricing_result->object_mgr);

    pricing_result->trade_details = NULL;
}
#endif

RQ_EXPORT void
rq_pricing_result_free_data(struct rq_pricing_result *pricing_result)
{
    rq_pricing_result_free_exposure(pricing_result);

    if (pricing_result->messages.messages)
    {
		unsigned int i = 0;
		for (i = 0; i < pricing_result->messages.num_messages; i++)
		{
			RQ_FREE(pricing_result->messages.messages[i]);	
		}

        RQ_FREE(pricing_result->messages.messages);
        pricing_result->messages.messages = NULL;
    }
    if (pricing_result->messages.message_severity)
    {
        RQ_FREE(pricing_result->messages.message_severity);
        pricing_result->messages.message_severity = NULL;
    }
    if (pricing_result->messages.message_result_types)
    {
        RQ_FREE(pricing_result->messages.message_result_types);
        pricing_result->messages.message_result_types = NULL;
    }
    pricing_result->messages.num_messages = 0;

    if (!rq_assetflow_list_is_null(pricing_result->assetflow_list))
    {
        rq_assetflow_list_free(pricing_result->assetflow_list);
        pricing_result->assetflow_list = NULL;
    }

	if (!rq_named_variant_mgr_is_null(pricing_result->named_value_results))
	{
		rq_named_variant_mgr_free(pricing_result->named_value_results);
		pricing_result->named_value_results = NULL;
	}

	if (!rq_named_variant_mgr_is_null(pricing_result->pricing_value_results))
	{
		rq_named_variant_mgr_free(pricing_result->pricing_value_results);
		pricing_result->pricing_value_results = NULL;
	}

	if (!rq_named_variant_mgr_is_null(pricing_result->tracing_value_results))
	{
		rq_named_variant_mgr_free(pricing_result->tracing_value_results);
		pricing_result->tracing_value_results = NULL;
	}

	if (!rq_named_variant_mgr_is_null(pricing_result->calibration_results))
	{
		rq_named_variant_mgr_free(pricing_result->calibration_results);
		pricing_result->calibration_results = NULL;
	}
}

RQ_EXPORT void
rq_pricing_result_add_message(struct rq_pricing_result *pricing_result, int result_requested, enum rq_error_severity error_severity, const char *message)
{
    if ( pricing_result->messages.num_messages == 0 )
    {
        pricing_result->messages.message_severity = (int *)RQ_MALLOC(sizeof(int) * RQ_PRICING_RESULTS_MAX_MESSAGES);
        pricing_result->messages.message_result_types = (int *)RQ_MALLOC(sizeof(int) * RQ_PRICING_RESULTS_MAX_MESSAGES);
        pricing_result->messages.messages = (char **)RQ_MALLOC(sizeof(char *) * RQ_PRICING_RESULTS_MAX_MESSAGES);
        pricing_result->results_need_freeing = 1;
    }
	if ( pricing_result->messages.num_messages == RQ_PRICING_RESULTS_MAX_MESSAGES - 1 )
	{	
		pricing_result->messages.message_result_types[pricing_result->messages.num_messages] = result_requested;
		pricing_result->messages.message_severity[pricing_result->messages.num_messages] = error_severity;
		pricing_result->messages.messages[pricing_result->messages.num_messages] = RQ_STRDUP("Maximum error count reached");
		pricing_result->messages.num_messages++;
	}
	else if ( pricing_result->messages.num_messages < RQ_PRICING_RESULTS_MAX_MESSAGES - 1 )
	{
		pricing_result->messages.message_result_types[pricing_result->messages.num_messages] = result_requested;
		pricing_result->messages.message_severity[pricing_result->messages.num_messages] = error_severity;
		pricing_result->messages.messages[pricing_result->messages.num_messages] = RQ_STRDUP(message);
		pricing_result->messages.num_messages++;
	}
}

RQ_EXPORT void
rq_pricing_result_add_messagefmt(struct rq_pricing_result *pricing_result, int result_requested, enum rq_error_severity error_severity, char *buf, const char *message, ...)
{
    va_list arglist;

    va_start(arglist, message);
    vsprintf(buf, message, arglist);
    rq_pricing_result_add_message(
        pricing_result, 
        result_requested, 
        error_severity, 
        buf
        );
    va_end(arglist);
}

RQ_EXPORT void 
rq_pricing_result_set_assetflow_list(struct rq_pricing_result *pricing_result, rq_assetflow_list_t assetflow_list)
{
    if (!rq_assetflow_list_is_null(pricing_result->assetflow_list))
        rq_assetflow_list_free(pricing_result->assetflow_list);
    pricing_result->assetflow_list = assetflow_list;
}

RQ_EXPORT void 
rq_pricing_result_add_named_value_double(struct rq_pricing_result *pricing_result, const char *name, double d)
{
    rq_named_variant_mgr_add_double(pricing_result->named_value_results, name, d);
}

RQ_EXPORT void 
rq_pricing_result_add_named_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str)
{
    rq_named_variant_mgr_add_string(pricing_result->named_value_results, name, str);
}

RQ_EXPORT rq_variant_t 
rq_pricing_result_get_named_value(struct rq_pricing_result *pricing_result, const char *name)
{
    return rq_named_variant_mgr_get(pricing_result->named_value_results, name);
}

RQ_EXPORT void 
rq_pricing_result_add_pricing_value_double(struct rq_pricing_result *pricing_result, const char *name, double d)
{
    rq_named_variant_mgr_add_double(pricing_result->pricing_value_results, name, d);
}

RQ_EXPORT void 
rq_pricing_result_add_pricing_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str)
{
    rq_named_variant_mgr_add_string(pricing_result->pricing_value_results, name, str);
}

RQ_EXPORT rq_variant_t 
rq_pricing_result_get_pricing_value(struct rq_pricing_result *pricing_result, const char *name)
{
    return rq_named_variant_mgr_get(pricing_result->pricing_value_results, name);
}

RQ_EXPORT void 
rq_pricing_result_add_tracing_value_double(struct rq_pricing_result *pricing_result, const char *name, double d)
{
    rq_named_variant_mgr_add_double(pricing_result->tracing_value_results, name, d);
}

RQ_EXPORT void 
rq_pricing_result_add_tracing_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str)
{
    rq_named_variant_mgr_add_string(pricing_result->tracing_value_results, name, str);
}

RQ_EXPORT rq_variant_t 
rq_pricing_result_get_tracing_value(struct rq_pricing_result *pricing_result, const char *name)
{
    return rq_named_variant_mgr_get(pricing_result->tracing_value_results, name);
}

RQ_EXPORT void 
rq_pricing_result_add_calibration_value_double(struct rq_pricing_result *pricing_result, const char *name, double d)
{
    rq_named_variant_mgr_add_double(pricing_result->calibration_results, name, d);
}

RQ_EXPORT void 
rq_pricing_result_add_calibration_value_string(struct rq_pricing_result *pricing_result, const char *name, const char *str)
{
    rq_named_variant_mgr_add_string(pricing_result->calibration_results, name, str);
}

RQ_EXPORT rq_variant_t 
rq_pricing_result_get_calibration_value(struct rq_pricing_result *pricing_result, const char *name)
{
    return rq_named_variant_mgr_get(pricing_result->calibration_results, name);
}
