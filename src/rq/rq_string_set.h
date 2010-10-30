/**
 * @file
 *
 * This file implements functions for managing a set of strings.
 */
/*
** rq_string_set.h
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
#ifndef rq_string_set_h
#define rq_string_set_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_set_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the rq_string_set */
typedef struct rq_string_set {
    rq_set_rb_t strings;
} * rq_string_set_t;


/** Test whether the rq_string_set is NULL */
int rq_string_set_is_null(rq_string_set_t obj);

/**
 * Allocate a new string_set
 */
rq_string_set_t rq_string_set_alloc();

/**
 * Free an object of type rq_string_set
 */
void rq_string_set_free(rq_string_set_t string_set);

/** Add a new copy of the string s to the set.
 *
 * This function adds a new copy of the string s to the set.
 */
void rq_string_set_add(rq_string_set_t ss, const char *s);

/** Test whether a string exists in the set.
 */
int rq_string_set_exists(rq_string_set_t ss, const char *s);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
