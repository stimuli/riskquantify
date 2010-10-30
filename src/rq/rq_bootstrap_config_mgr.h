/**
 * \file rq_bootstrap_config_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_bootstrap_config_mgr class manages a list of term
 * structure bootstrapping configurations keyed on the ID of the term
 * structure they build.
 */
/*
** rq_bootstrap_config_mgr.h
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
#ifndef rq_bootstrap_config_mgr_h
#define rq_bootstrap_config_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_enum.h"
#include "rq_bootstrap_config.h"
#include "rq_string_list.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_bootstrap_config_mgr {
    rq_tree_rb_t termstruct_configs[RQ_TERMSTRUCT_TYPE_MAX_ENUM];
} *rq_bootstrap_config_mgr_t;

typedef struct rq_bootstrap_config_mgr_iterator {
    rq_tree_rb_iterator_t bootstrap_config_it;
} * rq_bootstrap_config_mgr_iterator_t;


/** Test whether the rq_bootstrap_config_mgr is NULL */
RQ_EXPORT int rq_bootstrap_config_mgr_is_null(rq_bootstrap_config_mgr_t obj);

/**
 * Allocate a new bootstrap configuration manager
 */
RQ_EXPORT rq_bootstrap_config_mgr_t rq_bootstrap_config_mgr_alloc();

/**
 * Free an allocated bootstrap configuration manager
 */
RQ_EXPORT void rq_bootstrap_config_mgr_free(rq_bootstrap_config_mgr_t m);

/**
 * Remove and free the bootstrap configuration manager's
 * bootstrap configurations.
 */
RQ_EXPORT void rq_bootstrap_config_mgr_clear(rq_bootstrap_config_mgr_t m);

/**
 * Add a new bootstrap configuration to the manager
 */
RQ_EXPORT void rq_bootstrap_config_mgr_add(rq_bootstrap_config_mgr_t m, rq_bootstrap_config_t c);

/**
 * Find the bootstrap configuration for a particular curve
 */
RQ_EXPORT rq_bootstrap_config_t rq_bootstrap_config_mgr_find(rq_bootstrap_config_mgr_t m, const char *curve_id, enum rq_termstruct_type termstruct_type);

/**
 * Get the list of curve ID's given the bootstrap config manager and
 * the term structure type.
 * @return The list of curve ID's. This should be freed after use.
 */
RQ_EXPORT rq_string_list_t rq_bootstrap_config_mgr_get_curve_id_list(rq_bootstrap_config_mgr_t m, enum rq_termstruct_type termstruct_type);

RQ_EXPORT rq_bootstrap_config_mgr_iterator_t
rq_bootstrap_config_mgr_begin(rq_bootstrap_config_mgr_t bootstrap_config_mgr, enum rq_termstruct_type termstruct_type);

RQ_EXPORT int 
rq_bootstrap_config_mgr_at_end(rq_bootstrap_config_mgr_iterator_t it);

RQ_EXPORT void
rq_bootstrap_config_mgr_next(rq_bootstrap_config_mgr_iterator_t it);

RQ_EXPORT rq_bootstrap_config_t
rq_bootstrap_config_mgr_iterator_deref(rq_bootstrap_config_mgr_iterator_t it);

RQ_EXPORT void
rq_bootstrap_config_mgr_iterator_free(rq_bootstrap_config_mgr_iterator_t it);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
