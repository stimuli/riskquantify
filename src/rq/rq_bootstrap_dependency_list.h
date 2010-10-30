/**
 * @file
 *
 * This file defines a structure for returning a list of curves a
 * particular term structure is dependent on.
 */
/*
** rq_bootstrap_dependency_list.h
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
#ifndef rq_bootstrap_dependency_list_h
#define rq_bootstrap_dependency_list_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#define RQ_MAX_BOOTSTRAP_DEPENDENCIES 5

/** This structure defines a list of term structure dependencies.
 *
 * This structure defines a list of term structure dependencies. These
 * are particular curves that this term structure needs to have built
 * before it can be built.
 */
typedef struct rq_bootstrap_dependency_list {
    unsigned short num_dependencies;
    short curve_ids_need_freeing; /**< This flag determines whether the curve_ids are freed when the dependency list is freed or not. */
    enum rq_termstruct_type termstruct_types[RQ_MAX_BOOTSTRAP_DEPENDENCIES];
    const char *curve_ids[RQ_MAX_BOOTSTRAP_DEPENDENCIES];
} *rq_bootstrap_dependency_list_t;


/** Test whether the rq_bootstrap_dependency_list is NULL. 
*/
RQ_EXPORT int rq_bootstrap_dependency_list_is_null(rq_bootstrap_dependency_list_t obj);

/** Allocate a new bootstrap_dependency_list.
 */
RQ_EXPORT rq_bootstrap_dependency_list_t rq_bootstrap_dependency_list_alloc();

/** Initialize a stack-allocated dependency list structure.
 */
RQ_EXPORT void rq_bootstrap_dependency_list_init(rq_bootstrap_dependency_list_t bootstrap_dependency_list);

/** Free an object of type rq_bootstrap_dependency_list.
 */
RQ_EXPORT void rq_bootstrap_dependency_list_free(rq_bootstrap_dependency_list_t bootstrap_dependency_list);

/** Get the number of dependencies in the list.
 */
RQ_EXPORT unsigned short rq_bootstrap_dependency_list_size(rq_bootstrap_dependency_list_t bootstrap_dependency_list);

/** Get a particular dependency in the list.
 *
 * @return zero on success, non-zero on failure.
 */
RQ_EXPORT rq_error_code
rq_bootstrap_dependency_list_get_at(
    rq_bootstrap_dependency_list_t bootstrap_dependency_list, 
    unsigned int offset, /**< The zero-based offset in the list. */
    enum rq_termstruct_type *termstruct_type,  /**< The termstruct type is returned here. */
    const char **curve_id /**< The curve ID is returned here. */
    );

/** Add a dependency to the list.
 */
RQ_EXPORT rq_error_code
rq_bootstrap_dependency_list_add(
    rq_bootstrap_dependency_list_t bootstrap_dependency_list, 
    enum rq_termstruct_type termstruct_type,
    const char *curve_id
    );

/** Clear all bootstrap dependencies from the list.
 */
RQ_EXPORT void
rq_bootstrap_dependency_list_clear(
    rq_bootstrap_dependency_list_t bootstrap_dependency_list
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
