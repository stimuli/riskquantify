/**
 * \file rq_rate_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_rate_mgr object provides a manager class for rq_rate
 * objects.
 */
/*
** rq_rate_mgr.h
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
#ifndef rq_rate_mgr_h
#define rq_rate_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_rate.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_rate_mgr {
    rq_tree_rb_t rates;
	rq_perturbation_mgr_t perturbation_mgr;
} * rq_rate_mgr_t;

typedef struct rq_rate_mgr_iterator {
    rq_tree_rb_iterator_t rate_it;
} * rq_rate_mgr_iterator_t;


/** Test whether the rq_rate_mgr is NULL */
RQ_EXPORT int rq_rate_mgr_is_null(rq_rate_mgr_t obj);

/** Allocate a new rate manager object.
 */
RQ_EXPORT rq_rate_mgr_t rq_rate_mgr_alloc();

/** Free an allocated rate manager object.
 */
RQ_EXPORT void rq_rate_mgr_free(rq_rate_mgr_t rate_mgr);

/** Deep copy a rate manager object.
 */
RQ_EXPORT rq_rate_mgr_t rq_rate_mgr_clone(rq_rate_mgr_t rate_mgr);

/** Copy the rates from one rate manager to another.
 */
RQ_EXPORT void rq_rate_mgr_copy(rq_rate_mgr_t rate_mgr_dst, rq_rate_mgr_t rate_mgr_src);

/** Clear all the rates from the rate manager.
 */
RQ_EXPORT void rq_rate_mgr_clear(rq_rate_mgr_t rate_mgr);

/** Add a rate to the rate manager.
 */
RQ_EXPORT void rq_rate_mgr_add(rq_rate_mgr_t rate_mgr, rq_rate_t rate);

/** Find a rate by rate class ID within the rate manager.
 */
RQ_EXPORT rq_rate_t rq_rate_mgr_find(const rq_rate_mgr_t rate_mgr, const char *rate_class_id);

/** Set a days adjustment on all rate value and observation dates.
 */
RQ_EXPORT void rq_rate_mgr_set_perturbation_mgr(const rq_rate_mgr_t rate_mgr, rq_perturbation_mgr_t perturbation_mgr);

/** Get the perturbation manager.
 */
RQ_EXPORT rq_perturbation_mgr_t rq_rate_mgr_get_perturbation_mgr(const rq_rate_mgr_t rate_mgr);

/** Set the date adjustments after a clone or copy.
 */
RQ_EXPORT void rq_rate_mgr_sync_perturbation_mgr(rq_rate_mgr_t rate_mgr);

/** Allocate a rate iterator.
 */
RQ_EXPORT rq_rate_mgr_iterator_t rq_rate_mgr_iterator_alloc();

/** Free the rate iterator.
 */
RQ_EXPORT void rq_rate_mgr_iterator_free(rq_rate_mgr_iterator_t it);

/** Initialize the rate iterator to the beginning of the rate list.
 */
RQ_EXPORT void rq_rate_mgr_begin(rq_rate_mgr_t tree, rq_rate_mgr_iterator_t it);

/** Test whether the iterator has passed the end of the rate list.
 */
RQ_EXPORT int rq_rate_mgr_at_end(rq_rate_mgr_iterator_t it);

/** Move to the next rate in the rate list.
 */
RQ_EXPORT void rq_rate_mgr_next(rq_rate_mgr_iterator_t i);

/** Get the rate that the iterator is currently pointing to.
 */
RQ_EXPORT rq_rate_t rq_rate_mgr_iterator_deref(rq_rate_mgr_iterator_t i);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
