/*
** rq_perturbation_mgr.c
**
** Written by Nigel Slack-Smith - nigel@slack-smith.com
**
** Copyright (C) 2008
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
/* -- includes ---------------------------------------------------- */
#include "rq_perturbation_mgr.h"
#include "rq_rate.h"
#include <stdlib.h>

/* -- functions --------------------------------------------------- */

RQ_EXPORT rq_perturbation_mgr_t 
rq_perturbation_mgr_alloc(void *data)
{
	struct rq_perturbation_mgr *mgr = (struct rq_perturbation_mgr *)RQ_MALLOC(sizeof(struct rq_perturbation_mgr));
	
	mgr->data = data;
	mgr->perturb_value_func = NULL;
	mgr->date_perturbation = 0;

	return mgr;
}

RQ_EXPORT void
rq_perturbation_mgr_free(rq_perturbation_mgr_t mgr)
{
    RQ_FREE(mgr);
}

RQ_EXPORT void 
rq_perturbation_mgr_set_perturb_value_func(rq_perturbation_mgr_t mgr, double (*perturb_value_func)(void *, void *))
{
	mgr->perturb_value_func = perturb_value_func;
}

RQ_EXPORT void 
rq_perturbation_mgr_set_override_perturbation_func(rq_perturbation_mgr_t mgr, void (*override_perturbation_func)(void *, void *, double))
{
	mgr->override_perturbation_func = override_perturbation_func;
}

RQ_EXPORT void 
rq_perturbation_mgr_set_adjust_perturbation_func(rq_perturbation_mgr_t mgr, void (*adjust_perturbation_func)(void *, void *, double))
{
	mgr->adjust_perturbation_func = adjust_perturbation_func;
}

RQ_EXPORT double 
rq_perturbation_mgr_perturb_value(rq_perturbation_mgr_t mgr, void *rate)
{
	if (mgr->perturb_value_func)
		return (*mgr->perturb_value_func)(mgr->data, rate);
	else
		return ((rq_rate_t)rate)->value;
}

RQ_EXPORT void 
rq_perturbation_mgr_override_perturbation(rq_perturbation_mgr_t mgr, void *rate, double perturbed_value)
{
	(*mgr->override_perturbation_func)(mgr->data, rate, perturbed_value);
}

RQ_EXPORT void 
rq_perturbation_mgr_adjust_perturbation(rq_perturbation_mgr_t mgr, void *rate, double perturbed_value)
{
	(*mgr->adjust_perturbation_func)(mgr->data, rate, perturbed_value);
}

RQ_EXPORT rq_date 
rq_perturbation_mgr_perturb_date(rq_perturbation_mgr_t mgr, rq_date date)
{
	return date + mgr->date_perturbation;
}

RQ_EXPORT int rq_perturbation_mgr_get_date_perturbation(rq_perturbation_mgr_t mgr)
{
	return mgr->date_perturbation;
}

RQ_EXPORT void rq_perturbation_mgr_set_date_perturbation(rq_perturbation_mgr_t mgr, int date_perturbation)
{
	mgr->date_perturbation = date_perturbation;
}
