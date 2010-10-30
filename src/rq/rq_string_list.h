/**
 * \file rq_string_list.h
 * \author Brett Hutley
 *
 * \brief This file implements a vector of strings
 */
/*
** rq_string_list.h
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
#ifndef rq_string_list_h
#define rq_string_list_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_array.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** the handle to the rq_string_list */
typedef struct rq_string_list {
    rq_array_t strings;
} * rq_string_list_t;


/** Test whether the rq_string_list is NULL */
RQ_EXPORT int rq_string_list_is_null(rq_string_list_t obj);

/**
 * Allocate a new string_list
 */
RQ_EXPORT rq_string_list_t rq_string_list_alloc();

/**
 * Free an object of type rq_string_list
 */
RQ_EXPORT void rq_string_list_free(rq_string_list_t string_list);

/**
 * Add a string to the list of strings
 */
RQ_EXPORT void rq_string_list_add(rq_string_list_t string_list, const char *string);

/**
 * Get the number of strings managed by the list
 */
RQ_EXPORT unsigned long rq_string_list_size(rq_string_list_t string_list);

/**
 * Get the string at a zero-based offset
 */
RQ_EXPORT const char *rq_string_list_get_at(rq_string_list_t string_list, unsigned long offset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
