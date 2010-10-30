/**
 * \file rq_symbol_table.h
 * \author Brett Hutley
 *
 * \brief Implements a symbol table that maps strings to rq_objects.
 */
/*
** rq_symbol_table.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#ifndef rq_symbol_table_h
#define rq_symbol_table_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_object.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_symbol_table_node {
    const char *id;
    rq_object_t object;
};

typedef struct rq_symbol_table {
    rq_tree_rb_t tree;
} *rq_symbol_table_t;

typedef struct rq_symbol_table_iterator {
    rq_tree_rb_iterator_t symbol_it;
} * rq_symbol_table_iterator_t;

/**
 * Determine if an rq_symbol_table_t object is NULL. (ie if the object
 * hasn't been allocated).
 */
RQ_EXPORT int rq_symbol_table_is_null(rq_symbol_table_t obj);

/**
 * Allocate an object which initially has a NIL type
 * 
 * @param free_objects if non-zero means to call rq_object_free() on
 * the objects in the symbol table when freeing the symbol table.
 */
RQ_EXPORT rq_symbol_table_t rq_symbol_table_alloc(unsigned short free_objects);

/**
 * Free an allocated rq_symbol_table_t
 */
RQ_EXPORT void rq_symbol_table_free(rq_symbol_table_t obj);

/**
 * Clear the rq_symbol_table_t
 */
RQ_EXPORT void rq_symbol_table_clear(rq_symbol_table_t obj);

/**
 * Add an ID and object to the symbol table
 */
RQ_EXPORT void rq_symbol_table_add(rq_symbol_table_t obj, const char *id, rq_object_t object);

/**
 * Get an object from the symbol table
 */
RQ_EXPORT rq_object_t rq_symbol_table_get(rq_symbol_table_t obj, const char *id);

/**
 * Start an iteration of the symbol table
 */
RQ_EXPORT rq_symbol_table_iterator_t rq_symbol_table_begin(rq_symbol_table_t obj);

/**
 * test to see whether we are at the end of the list of symbols
 */
RQ_EXPORT int rq_symbol_table_at_end(rq_symbol_table_iterator_t it);

/**
 * Move to the next symbol in the symbol table
 */
RQ_EXPORT void rq_symbol_table_next(rq_symbol_table_iterator_t it);

/**
 * Free the symbol table iterator
 */
RQ_EXPORT void rq_symbol_table_iterator_free(rq_symbol_table_iterator_t it);

/**
 * Get the identifier associated with the current iterator
 */
RQ_EXPORT const char *rq_symbol_table_iterator_get_identifier(rq_symbol_table_iterator_t it);

/**
 * Get the object associated with the current iterator
 */
RQ_EXPORT rq_object_t rq_symbol_table_iterator_get_object(rq_symbol_table_iterator_t it);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
