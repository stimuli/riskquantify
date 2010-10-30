/**
 * \file rq_interpreter.h
 * \author Brett Hutley
 *
 * \brief This implements an lisp-like interpreter for rq_object types
 */
/*
** rq_interpreter.h
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
#ifndef rq_interpreter_h
#define rq_interpreter_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_object.h"
#include "rq_interpreter_error.h"
#include "rq_stream.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_interpreter {
    rq_object_mgr_t object_mgr;
    rq_double_linked_list_t error_list;
    enum rq_error_severity error_severity;

    char *parse_buffer; /**< The buffer to read the stream into */
    unsigned parse_buffer_len; /**< The current length of the buffer */
    unsigned parse_max_position; /**< The maximum char offset in the parse buffer */
    unsigned parse_position; /**< The current parse offset char */

    char *token_buffer; /**< The buffer to read the current token into */
    unsigned token_buffer_len; /**< The current token buffer length */
    unsigned token_position; /**< the current token offset char */

    int parse_state;

    unsigned lineno;
    int token_type;
} *rq_interpreter_t;


/* -- interpreter functions --------------------------------------- */
/**
 * Determine if an rq_interpreter is NULL. (ie if the object
 * hasn't been allocated).
 */
RQ_EXPORT int rq_interpreter_is_null(rq_interpreter_t obj);

/**
 * Allocate a new interpreter with it's own symbol table, etc
 */
RQ_EXPORT rq_interpreter_t rq_interpreter_alloc();

/**
 * Free the interpreter.
 */
RQ_EXPORT void rq_interpreter_free(rq_interpreter_t obj);

/**
 * Evaluate an object.
 */
RQ_EXPORT rq_object_t rq_interpreter_eval(rq_interpreter_t interp, rq_object_t obj);

/**
 * Get the object manager from the interpreter
 */
RQ_EXPORT rq_object_mgr_t rq_interpreter_get_object_mgr(rq_interpreter_t interp);

/**
 * Get an object from the symbol table, looking through the symbol
 * table stack until the identifier is found. Returns rq_object_nil
 * if not found.
 */
RQ_EXPORT rq_object_t rq_interpreter_symbol_find(rq_interpreter_t interp, const char *id);

/**
 * Set the value of a symbol in the interpreter
 */
RQ_EXPORT void rq_interpreter_symbol_set(rq_interpreter_t interp, const char *id, rq_object_t value);

/**
 * Set a local variable in the interpreter
 */
RQ_EXPORT void rq_interpreter_set_local(rq_interpreter_t interp, const char *id, rq_object_t value);

/**
 * Create a new object.
 */
RQ_EXPORT rq_object_t rq_interpreter_new_object(rq_interpreter_t interp);

/**
 * Parse a stream, returning the tokenized byte-code
 */
RQ_EXPORT rq_object_t rq_interpreter_parse(rq_interpreter_t interp, rq_stream_t stream);

/**
 * Add a builtin function to the interpreter
 */
RQ_EXPORT void rq_interpreter_add_builtin(rq_interpreter_t interp, const char *function_id, rq_object_t (*func)(void *, rq_object_t));

/**
 * Perform garbage collection
 */
RQ_EXPORT void rq_interpreter_garbage_collect(rq_interpreter_t interp);

/**
 * Signal an error in the interpreter.
 * @param interp A pointer to the private interpreter structure
 * @param severity The severity of the error
 * @param format A printf-type format buffer
 */
RQ_EXPORT void
rq_interpreter_signal_error(
    rq_interpreter_t interp, 
    enum rq_error_severity severity, 
    const char *format, 
    ...
    );

/**
 * Enter a new variable block...
 */
RQ_EXPORT void rq_interpreter_variable_block_enter(rq_interpreter_t interp);

/**
 * Leave the last entered variable block...
 */
RQ_EXPORT void rq_interpreter_variable_block_leave(rq_interpreter_t interp);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
