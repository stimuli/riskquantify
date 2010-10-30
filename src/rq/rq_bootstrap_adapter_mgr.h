/**
 * \file rq_bootstrap_adapter_mgr.h
 * \author Brett Hutley
 *
 * \brief A manager class for bootstrap adapters.
 */
/*
** rq_bootstrap_adapter_mgr.h
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
#ifndef rq_bootstrap_adapter_mgr_h
#define rq_bootstrap_adapter_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_enum.h"
#include "rq_tree_rb.h"
#include "rq_bootstrap_adapter.h"
#include "rq_yield_curve.h"
#include "rq_vol_surface.h"
#include "rq_forward_curve.h"
#include "rq_system.h"
#include "rq_market.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private bootstrap_adapter_mgr structure.
 */
typedef struct rq_bootstrap_adapter_mgr {
    rq_tree_rb_t adapters[RQ_TERMSTRUCT_TYPE_MAX_ENUM];
} *rq_bootstrap_adapter_mgr_t;


/** Test whether the rq_bootstrap_adapter_mgr is NULL. */
RQ_EXPORT int rq_bootstrap_adapter_mgr_is_null(rq_bootstrap_adapter_mgr_t obj);

/** Allocate a new bootstrap_adapter_mgr.
 */
RQ_EXPORT rq_bootstrap_adapter_mgr_t rq_bootstrap_adapter_mgr_alloc();

/** Clone the requested bootstrap_adapter_mgr.
 *
 * This function builds a new bootstrap_adapter_mgr object, and just
 * copies all the bootstrap_adapter pointers to the new object.  Since
 * bootstrap adapters are purely interface pointers, with no
 * destructor, there shouldn't be ownership issues.
 */
RQ_EXPORT rq_bootstrap_adapter_mgr_t rq_bootstrap_adapter_mgr_clone(rq_bootstrap_adapter_mgr_t mgr);

/**
 * Free an object of type rq_bootstrap_adapter_mgr
 */
RQ_EXPORT void rq_bootstrap_adapter_mgr_free(rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr);

/**
 * Add a bootstrap adapter to the bootstrap adapter manager.
 */
RQ_EXPORT void
rq_bootstrap_adapter_mgr_add(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    rq_bootstrap_adapter_t bootstrap_adapter
    );

/** Find a bootstrap adapter in the bootstrap adapter manager.
 */
RQ_EXPORT rq_bootstrap_adapter_t
rq_bootstrap_adapter_mgr_find(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    enum rq_termstruct_type termstruct_type,
    const char *adapter_id
    );

/** Bootstrap a curve or surface of the requested type.
 */
RQ_EXPORT void *
rq_bootstrap_adapter_mgr_build(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    enum rq_termstruct_type termstruct_type,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    );

/** Bootstrap a yield curve.
 */
RQ_EXPORT rq_yield_curve_t
rq_bootstrap_adapter_mgr_build_yield_curve(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    );

/** Build a forward curve.
 */
RQ_EXPORT rq_forward_curve_t
rq_bootstrap_adapter_mgr_build_forward_curve(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    );

/** Build a vol surface.
 */
RQ_EXPORT rq_vol_surface_t
rq_bootstrap_adapter_mgr_build_vol_surface(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    );

/** Build a future curve.
 */
RQ_EXPORT rq_future_curve_t
rq_bootstrap_adapter_mgr_build_future_curve(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    );

/** Build via external adapter.
 */
RQ_EXPORT rq_external_termstruct_t
rq_bootstrap_adapter_mgr_build_external(
    rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr,
    const char *adapter_id,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    );

/** Add the standard bootstrap adapters to the manager.
 */
void
rq_bootstrap_adapter_mgr_add_standard_adapters(rq_bootstrap_adapter_mgr_t bootstrap_adapter_mgr);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

