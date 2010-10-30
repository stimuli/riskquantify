/**
 * @file
 *
 * This file implements a manager for a collection of named variants.
 */
/*
** rq_named_variant_mgr.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2004-2008 Brett Hutley
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
#ifndef rq_named_variant_mgr_h
#define rq_named_variant_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_variant.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_named_variant {
    const char *name;
    rq_variant_t value;
};

/** the handle to the rq_named_variant_mgr */
typedef struct rq_named_variant_mgr {
    rq_tree_rb_t named_variants;
} * rq_named_variant_mgr_t;

typedef struct rq_named_variant_iterator {
    rq_tree_rb_iterator_t named_variant_it;
} * rq_named_variant_iterator_t;


/** Test whether the rq_named_variant_mgr is NULL. */
RQ_EXPORT int rq_named_variant_mgr_is_null(rq_named_variant_mgr_t obj);

/** Allocate a new named_variant_mgr.
 */
RQ_EXPORT rq_named_variant_mgr_t rq_named_variant_mgr_alloc();

/** Free an object of type rq_named_variant_mgr.
 */
RQ_EXPORT void rq_named_variant_mgr_free(rq_named_variant_mgr_t named_variant_mgr);

/** Add a string to the manager.
 */
RQ_EXPORT void rq_named_variant_mgr_add_string(rq_named_variant_mgr_t named_variant_mgr, const char *name, const char *value);

/** Add a double to the manager.
 */
RQ_EXPORT void rq_named_variant_mgr_add_double(rq_named_variant_mgr_t named_variant_mgr, const char *name, double d);

/** Add an integer to the manager.
 */
RQ_EXPORT void rq_named_variant_mgr_add_integer(rq_named_variant_mgr_t named_variant_mgr, const char *name, int i);

/** Add an date to the manager.
 */
RQ_EXPORT void rq_named_variant_mgr_add_date(rq_named_variant_mgr_t named_variant_mgr, const char *name, rq_date dt);

/** Get the value for a variable from the manager.
 */
RQ_EXPORT rq_variant_t rq_named_variant_mgr_get(rq_named_variant_mgr_t named_variant_mgr, const char *name);

/** Clear the named variant manager of all values.
 */
RQ_EXPORT void rq_named_variant_mgr_clear(rq_named_variant_mgr_t named_variant_mgr);

/** Allocate an iterator to walk the named variant manager.
 */
RQ_EXPORT rq_named_variant_iterator_t rq_named_variant_iterator_alloc();

/** Free an allocated iterator.
 */
RQ_EXPORT void rq_named_variant_iterator_free(rq_named_variant_iterator_t it);

/** Start an in-order walk of the tree.
 */
RQ_EXPORT void rq_named_variant_begin(rq_named_variant_mgr_t tree, rq_named_variant_iterator_t it);

/** Test to see if we have passed the last node in the tree.
 */
RQ_EXPORT int rq_named_variant_at_end(rq_named_variant_iterator_t it);

/** Move to the next node in the tree.
 */
RQ_EXPORT void rq_named_variant_next(rq_named_variant_iterator_t i);

/** Get the name associated with the current named value in the iterator.
 */
RQ_EXPORT const char *rq_named_variant_iterator_get_name(rq_named_variant_iterator_t it);

/** Get the value associated with the current named value in the iterator.
 */
RQ_EXPORT rq_variant_t rq_named_variant_iterator_get_value(rq_named_variant_iterator_t it);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
