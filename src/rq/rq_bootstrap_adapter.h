/**
 * \file rq_bootstrap_adapter.h
 * \author Brett Hutley
 *
 * \brief Acts as a standard interface to bootstrapping functions
 */
/*
** rq_bootstrap_adapter.h
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
#ifndef rq_bootstrap_adapter_h
#define rq_bootstrap_adapter_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_system.h"
#include "rq_market.h"
#include "rq_bootstrap_dependency_list.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The bootstrap_adapter structure.
 */
typedef struct rq_bootstrap_adapter {
    const char *adapter_id;
    enum rq_termstruct_type termstruct_type;
    void *(*bootstrap_func)(struct rq_bootstrap_adapter *, const char *, const rq_system_t, rq_market_t);
    rq_error_code (*get_bootstrap_dependency_list)(struct rq_bootstrap_adapter *, const char *, const rq_system_t, rq_market_t, rq_bootstrap_dependency_list_t);
    void (*free_func)(void *);
    void *(*clone_func)(void *);
	const char *debug_filename;
} *rq_bootstrap_adapter_t;


/** Test whether the rq_bootstrap_adapter is NULL */
int rq_bootstrap_adapter_is_null(rq_bootstrap_adapter_t obj);

/** Allocate a new bootstrap_adapter object.
 *
 * Allocate a new bootstrap_adapter object. This method should only be
 * called by bootstrap adapter derived class.
 */
rq_bootstrap_adapter_t 
_rq_bootstrap_adapter_alloc(
    const char *adapter_id,
    enum rq_termstruct_type termstruct_type,
    void *(*bootstrap_func)(rq_bootstrap_adapter_t, const char *, const rq_system_t, rq_market_t)
    );

/** Free an object of type rq_bootstrap_adapter.
 */
void rq_bootstrap_adapter_free(rq_bootstrap_adapter_t bootstrap_adapter);

/** Clone a rq_bootstrap_adapter object.
 *
 * This function makes a clone of the rq_bootstrap_adapter object. The
 * function pointer that actually does the bootstrapping is just
 * shallow-copied.
 */
rq_bootstrap_adapter_t rq_bootstrap_adapter_clone(rq_bootstrap_adapter_t adapter);

/** Build the requested curve.
 */
void *
rq_bootstrap_adapter_build(
    rq_bootstrap_adapter_t bootstrap_adapter,
    const char *curve_id,
    const rq_system_t system,
    rq_market_t market
    );

void rq_bootstrap_adaptor_set_debug_filename(rq_bootstrap_adapter_t bootstrap_adapter, const char *filename);

const char *rq_bootstrap_adaptor_get_debug_filename(const rq_bootstrap_adapter_t bootstrap_adapter);

/** Get the bootstrap adapter's ID.
 */
const char *rq_bootstrap_adapter_get_adapter_id(rq_bootstrap_adapter_t bootstrap_adapter);

/** Get the bootstrap adapter's termstructure type.
 */
enum rq_termstruct_type rq_bootstrap_adapter_get_termstruct_type(rq_bootstrap_adapter_t bootstrap_adapter);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

