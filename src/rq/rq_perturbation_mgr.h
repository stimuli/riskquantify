/**
 * \file rq_perturbation_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_perturbation_mgr files contain functions for implementing
 * red-black trees.
 */
/*
** rq_perturbation_mgr.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA 
*/
#ifndef rq_perturbation_mgr_h
#define rq_perturbation_mgr_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"


#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */

typedef struct rq_perturbation_mgr {
    struct rq_perturbation_mgr_node *head;
    double (*perturb_value_func)(void *, void *);
    void (*override_perturbation_func)(void *, void *, double);
    void (*adjust_perturbation_func)(void *, void *, double);
	void *data;
	int date_perturbation;
} *rq_perturbation_mgr_t;

/* -- prototypes -------------------------------------------------- */

/** Allocate a new perturbation manager.
 *
 * Allocate a new perturbation manager. Pass in the perturb value callback function
 * and the application data pointer.
 */
RQ_EXPORT rq_perturbation_mgr_t rq_perturbation_mgr_alloc(void *data);

/**
 * Free the perturbation manager
 */
RQ_EXPORT void rq_perturbation_mgr_free(rq_perturbation_mgr_t mgr);

/**
 * Set the perturbation callback function
 */
RQ_EXPORT void rq_perturbation_mgr_set_perturb_value_func(rq_perturbation_mgr_t mgr, double (*perturb_value_func)(void *, void *));

/**
 * Set the override perturbation callback function
 */
RQ_EXPORT void rq_perturbation_mgr_set_override_perturbation_func(rq_perturbation_mgr_t mgr, void (*override_perturbation_func)(void *, void *, double));

/**
 * Set the adjust perturbation callback function
 */
RQ_EXPORT void rq_perturbation_mgr_set_adjust_perturbation_func(rq_perturbation_mgr_t mgr, void (*adjust_perturbation_func)(void *, void *, double));

/**
 * Perturb the value for a rate.
 */
RQ_EXPORT double rq_perturbation_mgr_perturb_value(rq_perturbation_mgr_t mgr, void *rate);

/**
 * Override the perturbed value for a rate.
 */
RQ_EXPORT void rq_perturbation_mgr_override_perturbation(rq_perturbation_mgr_t mgr, void *rate, double perturbed_value);

/**
 * Adjust the perturbed value for a rate.
 */
RQ_EXPORT void rq_perturbation_mgr_adjust_perturbation(rq_perturbation_mgr_t mgr, void *rate, double perturbed_value);

/**
 * Perturb the value for a rate.
 */
RQ_EXPORT rq_date rq_perturbation_mgr_perturb_date(rq_perturbation_mgr_t mgr, rq_date date);

/**
 * Get the current date perturbation
 */
RQ_EXPORT int rq_perturbation_mgr_get_date_perturbation(rq_perturbation_mgr_t mgr);

/**
 * Set the current date perturbation
 */
RQ_EXPORT void rq_perturbation_mgr_set_date_perturbation(rq_perturbation_mgr_t mgr, int date_perturbation);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
