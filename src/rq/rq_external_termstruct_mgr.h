/**
 * @file
 *
 * A file that manages externally created term structures
 */
/*
** rq_external_termstruct_mgr.h
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
#ifndef rq_external_termstruct_mgr_h
#define rq_external_termstruct_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the rq_external_termstruct_mgr */
typedef struct rq_external_termstruct {
    const char *id;
    void *external_termstruct;
    void (*free_func) (void *);
    void *(*clone_func) (void *);
} *rq_external_termstruct_t;

typedef struct rq_external_termstruct_mgr {
    rq_tree_rb_t tree;
} *rq_external_termstruct_mgr_t;


/** Test whether the rq_external_termstruct_mgr is NULL */
RQ_EXPORT int rq_external_termstruct_mgr_is_null(rq_external_termstruct_mgr_t obj);

/**
 * Allocate a new external_termstruct_mgr
 */
RQ_EXPORT rq_external_termstruct_mgr_t rq_external_termstruct_mgr_alloc();

/**
 * Free an object of type rq_external_termstruct_mgr
 */
RQ_EXPORT void rq_external_termstruct_mgr_free(rq_external_termstruct_mgr_t external_termstruct_mgr);

/**
 * Free all the external curves managed by the external curve manager
 */
RQ_EXPORT void rq_external_termstruct_mgr_clear(rq_external_termstruct_mgr_t m);

/**
 * Clone the external termstructure manager.
 */
RQ_EXPORT rq_external_termstruct_mgr_t rq_external_termstruct_mgr_clone(const rq_external_termstruct_mgr_t external_termstruct_mgr);

/**
 * Add an external term structure to the manager
 */
/* RQ_EXPORT void rq_external_termstruct_mgr_add(rq_external_termstruct_mgr_t external_termstruct_mgr, const char *id, void *external_termstruct, void (*free_func)(void *), void *(*clone_func)(void *)); */
RQ_EXPORT void rq_external_termstruct_mgr_add(rq_external_termstruct_mgr_t et_mgr, rq_external_termstruct_t et);

/**
 * Get an external term structure from the manager
 */
RQ_EXPORT void *rq_external_termstruct_mgr_get(rq_external_termstruct_mgr_t external_termstruct_mgr, const char *id);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

