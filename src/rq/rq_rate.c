/*
** rq_rate.c
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
#include "rq_rate.h"
#include "rq_day_count.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT double
rq_simple_rate_to_discount_factor(double rate, rq_date from_date, rq_date to_date, double days_per_year)
{
    return 1.0 / (1.0 + (rate * (to_date - from_date) / days_per_year));
}

RQ_EXPORT double
rq_simple_rate_from_discount_factor(double df, rq_date from_date, rq_date to_date, enum rq_day_count_convention dayCountConv)
{
    return
        ((1.0 / df) - 1.0) /* effective rate */
        / rq_day_count_get_year_fraction(dayCountConv, from_date, to_date);
}

RQ_EXPORT double 
rq_discount_factor_from_par_rate(
    double par_rate, 
    unsigned short num_discount_factors, 
    double *discount_factors
    )
{
    unsigned short i = 0;
    double sum_dfs = 0.0;
    while (i < num_discount_factors)
    {
        sum_dfs += discount_factors[i];
        ++i;
    }

    return (1.0 - (par_rate * sum_dfs)) / (1.0 + par_rate);
}

static struct rq_rate *
rq_rate_alloc()
{
    /* use RQ_CALLOC to zero the memory */
    struct rq_rate *r = (struct rq_rate *)RQ_CALLOC(1, sizeof(struct rq_rate));
    return r;
}

RQ_EXPORT rq_rate_t 
rq_rate_build(
    const char *rate_class_id,
    const char *asset_id,
    enum rq_rate_type rate_type,
    rq_date observation_date,
    rq_date value_date,
    double value
    )
{
    struct rq_rate *r = rq_rate_alloc();

    r->rate_class_id = RQ_STRDUP(rate_class_id);
    r->asset_id = RQ_STRDUP(asset_id);
    r->rate_type = rate_type;
    r->observation_date = observation_date;
    r->value_date = value_date;
    r->value = value;
	r->perturbation_mgr = NULL;
	r->perturbation_offset = -1;

    return r;
}

RQ_EXPORT void 
rq_rate_free(rq_rate_t r)
{
    if (r->rate_class_id)
        RQ_FREE((char *)r->rate_class_id);
    if (r->asset_id)
        RQ_FREE((char *)r->asset_id);
    RQ_FREE(r);
}

RQ_EXPORT rq_rate_t 
rq_rate_clone(const rq_rate_t r)
{
    struct rq_rate *c = (struct rq_rate *)rq_rate_alloc();

    if (r->rate_class_id)
        c->rate_class_id = RQ_STRDUP(r->rate_class_id);
    if (r->asset_id)
        c->asset_id = RQ_STRDUP(r->asset_id);
    c->rate_type = r->rate_type;
    c->observation_date = r->observation_date;
    c->value_date = r->value_date;
    c->value = r->value;
	c->perturbation_mgr = r->perturbation_mgr;
	c->perturbation_offset = r->perturbation_offset;

    return c;
}

RQ_EXPORT const char *
rq_rate_get_asset_id(const rq_rate_t r)
{
    return r->asset_id;
}

RQ_EXPORT const char *
rq_rate_get_rate_class_id(const rq_rate_t r)
{
    return r->rate_class_id;
}

RQ_EXPORT enum rq_rate_type 
rq_rate_get_rate_type(const rq_rate_t r)
{
    return r->rate_type;
}

RQ_EXPORT rq_date 
rq_rate_get_observation_date(const rq_rate_t r)
{
	return r->perturbation_mgr ? 
		rq_perturbation_mgr_perturb_date(r->perturbation_mgr, r->observation_date) : r->observation_date;
}

RQ_EXPORT rq_date 
rq_rate_get_value_date(const rq_rate_t r)
{
	return r->perturbation_mgr ? 
		rq_perturbation_mgr_perturb_date(r->perturbation_mgr, r->value_date) : r->value_date;
}

RQ_EXPORT double
rq_rate_get_value(const rq_rate_t r)
{
	return r->perturbation_mgr ? rq_perturbation_mgr_perturb_value(r->perturbation_mgr, r) : r->value;
}

RQ_EXPORT double
rq_rate_get_unperturbed_value(const rq_rate_t r)
{
	return r->value;
}

RQ_EXPORT void 
rq_rate_set_observation_date(rq_rate_t r, rq_date observation_date)
{
    r->observation_date = observation_date;
}

RQ_EXPORT void 
rq_rate_set_value_date(rq_rate_t r, rq_date value_date)
{
    r->value_date = value_date;
}

RQ_EXPORT void 
rq_rate_set_value(rq_rate_t r, double value)
{
	if (r->perturbation_mgr && r->perturbation_mgr->adjust_perturbation_func)
		rq_perturbation_mgr_adjust_perturbation(r->perturbation_mgr, r, value);
	else
		r->value = value;
}

RQ_EXPORT void 
rq_rate_override_value(rq_rate_t r, double value)
{
	if (r->perturbation_mgr && r->perturbation_mgr->override_perturbation_func)
		rq_perturbation_mgr_override_perturbation(r->perturbation_mgr, r, value);
	else
		r->value = value;
}

RQ_EXPORT int
rq_rate_is_null(rq_rate_t obj)
{
    return (obj == NULL);
}
