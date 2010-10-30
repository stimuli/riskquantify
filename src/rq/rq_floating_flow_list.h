/**
 * \file rq_floating_flow_list.h
 * \author Brett Hutley
 *
 * \brief The rq_floating_flow_list class represents the payment of am asset
 * based on a floating rate of interest.
 */
/*
** rq_floating_flow_list.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#ifndef rq_floating_flow_list_h
#define rq_floating_flow_list_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_array.h"
#include "rq_floating_flow.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */

typedef struct rq_floating_flow_list {
    rq_array_t floating_flow_list;
} *rq_floating_flow_list_t;

typedef struct rq_floating_flow** rq_floating_flow_list_iterator;

/* -- prototypes -------------------------------------------------- */
/** Test whether the rq_floating_flow_list is NULL */
RQ_EXPORT int rq_floating_flow_list_is_null(rq_floating_flow_list_t obj);

/** Allocate a new floating flow list. This list will free it's floating flows
 * when deleted.
 */
RQ_EXPORT rq_floating_flow_list_t rq_floating_flow_list_alloc();

/** Allocate a new floating flow list. This list WILL NOT free it's floating
 * flows when deleted.
 */
RQ_EXPORT rq_floating_flow_list_t rq_floating_flow_list_alloc_unmanaged();

/** Free a previously allocated floating flow.
 */
RQ_EXPORT void rq_floating_flow_list_free(rq_floating_flow_list_t ffl);

/** Clone a floating flow list.
 */
RQ_EXPORT rq_floating_flow_list_t rq_floating_flow_list_clone(rq_floating_flow_list_t ffl);

/** Clone a floating flow list from given date.
 */
RQ_EXPORT rq_floating_flow_list_t rq_floating_flow_list_clone_from(rq_floating_flow_list_t ffl, rq_date from_date);

/** Clear all the elements from a floating flow list.
 */
RQ_EXPORT void rq_floating_flow_list_clear(rq_floating_flow_list_t ffl);

/** Revert all rate sets to an unset state.
 */
RQ_EXPORT void rq_floating_flow_list_revert(rq_floating_flow_list_t ffl);

/** Add a floating flow to the list.
 */
RQ_EXPORT void 
rq_floating_flow_list_add(
    rq_floating_flow_list_t ffl, 
    rq_floating_flow_t f
    );

/** Get the number of floating flows in the list.
 */
RQ_EXPORT unsigned long
rq_floating_flow_list_size(
    rq_floating_flow_list_t ffl
    );

/** Get the floating flow at the specified zero-based offset.
 */
RQ_EXPORT rq_floating_flow_t
rq_floating_flow_list_get_at(
    rq_floating_flow_list_t ffl,
    unsigned long offset
    );

/** This function inverts the pay/receive flags on all the floating
 * flows in the list.
 */
RQ_EXPORT void
rq_floating_flow_list_invert_pay_receive(
    rq_floating_flow_list_t ffl
    );

/** Get an iterator at the begining of the flow list
 *
 */
RQ_EXPORT rq_floating_flow_list_iterator
rq_floating_flow_list_begin(rq_floating_flow_list_t ffl);

/** Get an iterator at the end of the flow list - one past last item
 *
 */
RQ_EXPORT rq_floating_flow_list_iterator
rq_floating_flow_list_end(rq_floating_flow_list_t ffl);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
