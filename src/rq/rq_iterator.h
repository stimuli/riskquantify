/**
 * @file
 *
 * This file implements a generic iterator object.
 */
/*
** rq_iterator.h
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
#ifndef rq_iterator_h
#define rq_iterator_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_variant.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

enum rq_iterator_seek_position {
    RQ_ITERATOR_SEEK_START, /**< Start the seek from the first object in the iteration */
    RQ_ITERATOR_SEEK_END, /**< Start the seek from the last object in the iteration. */
    RQ_ITERATOR_SEEK_CUR /**< Start the seek from the current object in the iteration. */
};

/** The iterator structure.
 */
typedef struct rq_iterator {
    void (*freefunc)(void *iterdata);
    void (*incrfunc)(void *iterdata);
    void (*decrfunc)(void *iterdata);
    short (*atendfunc)(void *iterdata);
    short (*atbeginfunc)(void *iterdata);
    short (*seekfunc)(void *iterdata, enum rq_iterator_seek_position seek_pos, long offset);
    struct rq_variant (*getvaluefunc)(void *iterdata);
    void *iterdata;
} *rq_iterator_t;


/** Test whether the rq_iterator is NULL */
RQ_EXPORT int rq_iterator_is_null(rq_iterator_t obj);

/** Allocate a new iterator. This function should only be called by the
 * classes that return an iterator object (ie container implementations).
 */
RQ_EXPORT rq_iterator_t 
_rq_iterator_alloc(
    void (*freefunc)(void *iterdata),
    void (*incrfunc)(void *iterdata),
    void (*decrfunc)(void *iterdata),
    short (*atendfunc)(void *iterdata),
    short (*atbeginfunc)(void *iterdata),
    short (*seekfunc)(void *iterdata, enum rq_iterator_seek_position seek_pos, long offset),
    struct rq_variant (*getvaluefunc)(void *iterdata),
    void *iterdata
    );

/** Free an object of type rq_iterator
 */
RQ_EXPORT void rq_iterator_free(rq_iterator_t iterator);

/** Determine whether an iterator is bi-directional or not.
 */
RQ_EXPORT short rq_iterator_is_bidirectional(rq_iterator_t iterator);

/** Increment an iterator - moves to the next object in the iteration.
 */
RQ_EXPORT void rq_iterator_incr(rq_iterator_t iterator);

/** Decrement an iterator - moves to the previous object in the iteration.
 */
RQ_EXPORT void rq_iterator_decr(rq_iterator_t iterator);

/** Test to see whether we are at the last node on an iteration.
 */
RQ_EXPORT short rq_iterator_at_end(rq_iterator_t iterator);

/** Test to see whether we are at the first node on an iteration.
 */
RQ_EXPORT short rq_iterator_at_beginning(rq_iterator_t iterator);

/** Get the value pointed to by this iterator.
 */
RQ_EXPORT struct rq_variant rq_iterator_get_value(rq_iterator_t iterator);

/** Seek to a particular position within the container class.
 */
RQ_EXPORT short rq_iterator_seek(rq_iterator_t it, enum rq_iterator_seek_position seek_pos, long offset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

