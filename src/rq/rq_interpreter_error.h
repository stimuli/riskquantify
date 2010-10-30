/**
 * \file rq_interpreter_error.h
 * \author Brett Hutley
 *
 * \brief A type representing an error fired in the interpreter
 */
/*
** rq_interpreter_error.h
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
#ifndef rq_interpreter_error_h
#define rq_interpreter_error_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_interpreter_error.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The private interpreter_error structure.
 */
typedef struct rq_interpreter_error {
    const char *error_message;
} *rq_interpreter_error_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_interpreter_error is NULL */
RQ_EXPORT int rq_interpreter_error_is_null(rq_interpreter_error_t obj);

/**
 * Allocate a new interpreter_error
 */
RQ_EXPORT rq_interpreter_error_t rq_interpreter_error_alloc();

/**
 * Free an object of type rq_interpreter_error
 */
RQ_EXPORT void rq_interpreter_error_free(rq_interpreter_error_t interpreter_error);
RQ_EXPORT void _rq_interpreter_error_free(void *interpreter_error);

/**
 * Get the error message out of the interpreter error.
 */
RQ_EXPORT const char *rq_interpreter_error_get_error_message(rq_interpreter_error_t interpreter_error);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

