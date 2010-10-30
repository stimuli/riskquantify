/*
** rq_bootstrap_config.c
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
#include "rq_bootstrap_config.h"
#include <stdlib.h>
#include <string.h>

static unsigned int s_max_rate_class_ids = 60;


struct rq_bootstrap_config *
rq_bootstrap_config_alloc()
{
    unsigned int i;
    /* use RQ_CALLOC to zero out the memory */
    struct rq_bootstrap_config *c = 
        (struct rq_bootstrap_config *)RQ_CALLOC(1, sizeof(struct rq_bootstrap_config));

    c->max_rate_class_ids = s_max_rate_class_ids;
    c->rate_class_ids = (const char **)RQ_CALLOC(s_max_rate_class_ids, sizeof(const char *));

    c->rate_class_offsets = (long *)RQ_CALLOC(s_max_rate_class_ids, sizeof(long)); 
    c->basis_points_to_add = (double *)RQ_CALLOC(s_max_rate_class_ids, sizeof(double)); 
    c->percentages_to_add = (double *)RQ_CALLOC(s_max_rate_class_ids, sizeof(double)); 
    for (i = 0; i < s_max_rate_class_ids; i++)
    {
        c->rate_class_offsets[i] = -1;
        c->basis_points_to_add[i] = 0.0;
        c->percentages_to_add[i] = 0.0;
    }

    c->bootstrap_method_id = NULL;
    c->curve_id1 = NULL;
    c->curve_id2 = NULL;
    c->asset_id = NULL;

    return c;
}

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
    enum rq_day_count_convention default_day_count_convention
    )
{
    struct rq_bootstrap_config *config = rq_bootstrap_config_alloc();

    config->curve_id = (const char *)RQ_STRDUP(curve_id);
    config->asset_id = (const char *)RQ_STRDUP(asset_id);
    config->termstruct_type = termstruct_type;

    config->interpolation_method = interpolation_method;
    config->extrapolation_method_start = extrapolation_method_start;
    config->extrapolation_method_end = extrapolation_method_end;
	config->zero_method = zero_method;
	if (zero_method_compound_frequency > 1) config->zero_method_compound_frequency = zero_method_compound_frequency;
	else config->zero_method_compound_frequency = 1;
    config->default_day_count_convention = default_day_count_convention;

    return config;
}

RQ_EXPORT void
rq_bootstrap_config_free(rq_bootstrap_config_t config)
{
    if (config->rate_class_ids)
    {
        unsigned int i = 0;
        while (i < config->num_rate_class_ids)
        {
            if (config->rate_class_ids[i])
                RQ_FREE((char *)config->rate_class_ids[i]);
            i++;
        }
        RQ_FREE((char **)config->rate_class_ids);
    }

    if (config->rate_class_offsets)
        RQ_FREE(config->rate_class_offsets);

    if (config->basis_points_to_add)
        RQ_FREE(config->basis_points_to_add);

    if (config->percentages_to_add)
        RQ_FREE(config->percentages_to_add);
    if (config->curve_id)
        RQ_FREE((char *)config->curve_id);

    if (config->bootstrap_method_id)
        RQ_FREE((char *)config->bootstrap_method_id);
    if (config->curve_id1)
        RQ_FREE((char *)config->curve_id1);
    if (config->curve_id2)
        RQ_FREE((char *)config->curve_id2);
    if (config->asset_id)
        RQ_FREE((char *)config->asset_id);

    RQ_FREE(config);
}

RQ_EXPORT const char *
rq_bootstrap_config_get_curve_id(const rq_bootstrap_config_t config)
{
    return config->curve_id;
}

RQ_EXPORT enum rq_termstruct_type 
rq_bootstrap_config_get_termstruct_type(const rq_bootstrap_config_t config)
{
    return config->termstruct_type;
}

RQ_EXPORT unsigned int 
rq_bootstrap_config_get_num_rate_class_ids(const rq_bootstrap_config_t config)
{
    return config->num_rate_class_ids;
}

RQ_EXPORT const char *
rq_bootstrap_config_get_rate_class_id_at(const rq_bootstrap_config_t config, unsigned int offset)
{
    return config->rate_class_ids[offset];
}

RQ_EXPORT void 
rq_bootstrap_config_add_rate_class_id(rq_bootstrap_config_t config, const char *rate_class_id)
{
    if (config->num_rate_class_ids == config->max_rate_class_ids)
    {
        unsigned int new_max_rate_class_ids = config->max_rate_class_ids * 2;
        config->rate_class_ids = realloc((char *)config->rate_class_ids, new_max_rate_class_ids * sizeof(const char *));
        config->rate_class_offsets = realloc(config->rate_class_offsets, new_max_rate_class_ids * sizeof(long));
        config->basis_points_to_add = realloc(config->basis_points_to_add, new_max_rate_class_ids * sizeof(double));
        config->percentages_to_add = realloc(config->percentages_to_add, new_max_rate_class_ids * sizeof(double));
        config->max_rate_class_ids = new_max_rate_class_ids;
    }

    config->rate_class_ids[config->num_rate_class_ids] = (const char *)RQ_STRDUP(rate_class_id);
    config->num_rate_class_ids++;
}

RQ_EXPORT long 
rq_bootstrap_config_get_rate_class_offset_at(const rq_bootstrap_config_t config, unsigned int index)
{
    return config->rate_class_offsets[index];
}

RQ_EXPORT void 
rq_bootstrap_config_set_rate_class_offset_at(const rq_bootstrap_config_t config, unsigned int index, long offset)
{
    config->rate_class_offsets[index] = offset;
}

RQ_EXPORT double 
rq_bootstrap_config_get_basis_points_to_add_at(const rq_bootstrap_config_t config, unsigned int index)
{
    return config->basis_points_to_add[index];
}

RQ_EXPORT void 
rq_bootstrap_config_set_basis_points_to_add_at(const rq_bootstrap_config_t config, unsigned int index, double basis_points_to_add)
{
    config->basis_points_to_add[index] = basis_points_to_add;
}

RQ_EXPORT double 
rq_bootstrap_config_get_percentage_to_add_at(const rq_bootstrap_config_t config, unsigned int index)
{
    return config->percentages_to_add[index];
}

RQ_EXPORT void 
rq_bootstrap_config_set_percentage_to_add_at(const rq_bootstrap_config_t config, unsigned int index, double percentage_to_add)
{
    config->percentages_to_add[index] = percentage_to_add;
}

RQ_EXPORT int
rq_bootstrap_config_is_null(rq_bootstrap_config_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void
rq_bootstrap_config_set_bootstrap_method_id(rq_bootstrap_config_t c, const char *bootstrap_method_id)
{
    if (c->bootstrap_method_id)
       RQ_FREE((char *)c->bootstrap_method_id);
    c->bootstrap_method_id = RQ_STRDUP(bootstrap_method_id);
}

RQ_EXPORT const char *
rq_bootstrap_config_get_bootstrap_method_id(rq_bootstrap_config_t c)
{
    return c->bootstrap_method_id;
}

RQ_EXPORT void
rq_bootstrap_config_set_curve_id1(rq_bootstrap_config_t c, const char *curve_id1)
{
    if (c->curve_id1)
       RQ_FREE((char *)c->curve_id1);
    c->curve_id1 = RQ_STRDUP(curve_id1);
}

RQ_EXPORT const char *
rq_bootstrap_config_get_curve_id1(rq_bootstrap_config_t c)
{
    return c->curve_id1;
}

RQ_EXPORT void
rq_bootstrap_config_set_curve_id2(rq_bootstrap_config_t c, const char *curve_id2)
{
    if (c->curve_id2)
       RQ_FREE((char *)c->curve_id2);
    c->curve_id2 = RQ_STRDUP(curve_id2);
}

RQ_EXPORT const char *
rq_bootstrap_config_get_curve_id2(rq_bootstrap_config_t c)
{
    return c->curve_id2;
}

RQ_EXPORT void
rq_bootstrap_config_set_asset_id(rq_bootstrap_config_t c, const char *asset_id)
{
    if (c->asset_id)
       RQ_FREE((char *)c->asset_id);
    c->asset_id = RQ_STRDUP(asset_id);
}

RQ_EXPORT const char *
rq_bootstrap_config_get_asset_id(rq_bootstrap_config_t c)
{
    return c->asset_id;
}

RQ_EXPORT enum rq_interpolation_method 
rq_bootstrap_config_get_interpolation_method(const rq_bootstrap_config_t config)
{
    return config->interpolation_method;
}

RQ_EXPORT enum rq_extrapolation_method
rq_bootstrap_config_get_extrapolation_method_start(const rq_bootstrap_config_t config)
{
    return config->extrapolation_method_start;
}

RQ_EXPORT enum rq_extrapolation_method
rq_bootstrap_config_get_extrapolation_method_end(const rq_bootstrap_config_t config)
{
    return config->extrapolation_method_end;
}

RQ_EXPORT enum rq_zero_method 
rq_bootstrap_config_get_zero_method(const rq_bootstrap_config_t config)
{
    return config->zero_method;
}

RQ_EXPORT enum rq_billFuture_method 
rq_bootstrap_config_get_billFuture_method(const rq_bootstrap_config_t config)
{
    return config->billFuture_method;
}

RQ_EXPORT void
rq_bootstrap_config_set_billFuture_method(rq_bootstrap_config_t config, enum rq_billFuture_method b)
{
    config->billFuture_method = b;
}

RQ_EXPORT enum rq_swapParInterpolation_method
rq_bootstrap_config_get_swapParInterpolation_method(const rq_bootstrap_config_t config)
{
    return config->swapParInterpolation_method;
}

RQ_EXPORT void
rq_bootstrap_config_set_swapParInterpolation_method(rq_bootstrap_config_t config, enum rq_swapParInterpolation_method method)
{
    config->swapParInterpolation_method = method;
}

RQ_EXPORT enum rq_swapDayCount_method
rq_bootstrap_config_get_swapDayCount_method(const rq_bootstrap_config_t config)
{
    return config->swapDayCount_method;
}

RQ_EXPORT void
rq_bootstrap_config_set_swapDayCount_method(rq_bootstrap_config_t config, enum rq_swapDayCount_method method)
{
    config->swapDayCount_method = method;
}

RQ_EXPORT unsigned int
rq_bootstrap_config_get_zero_method_compound_frequency(const rq_bootstrap_config_t config)
{
    return config->zero_method_compound_frequency;
}

RQ_EXPORT enum rq_day_count_convention
rq_bootstrap_config_get_default_day_count_convention(const rq_bootstrap_config_t config)
{
    return config->default_day_count_convention;
}
