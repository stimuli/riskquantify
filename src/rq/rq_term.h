/**
 * \file rq_term.h
 * \author Brett Hutley
 *
 * \brief The rq_term object encapsulates a period of time.
 */
/*
** rq_term.h
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
#ifndef rq_term_h
#define rq_term_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#include "rq_stream.h"

/**
 * This structure defines a term, as in a length of time.
 */
typedef struct rq_term {
    short days; /**< The number of days in the term (max. sizeof short) */
    short weeks; /**< The number of weeks in the term (max. sizeof short) */
    short months; /**< The number of months in the term (max. sizeof short) */
    short years; /**< The number of years in the term (max. sizeof short) */
	long date; /**< An absolute date used to define a futures term */
} *rq_term_t;

/** Allocate a new term structure.
 */
RQ_EXPORT rq_term_t rq_term_alloc();

/** Allocate a new term structure setting the various periods.
 */
RQ_EXPORT rq_term_t rq_term_build(short days, short weeks, short months, short years);

/** Clone the calendar.
 */
RQ_EXPORT rq_term_t rq_term_clone(rq_term_t term);

/** Initialize a term structure to zero.
 */
RQ_EXPORT void rq_term_init(struct rq_term *term);

/** Fill out a term object.
 */
RQ_EXPORT void 
rq_term_fill(
    struct rq_term *term, 
    short days, 
    short weeks,
    short months,
    short years
    );

/** Copy the values from one term object to another.
 */
RQ_EXPORT void
rq_term_copy(
    struct rq_term *term_dst,
    const struct rq_term *term_src
    );

/** Add one term to another term.
 */
RQ_EXPORT void
rq_term_add_term(
    struct rq_term *term,
    const struct rq_term *term_to_add
    );

/** Subtract one term from another term.
 */

RQ_EXPORT void
rq_term_subtract_term(
    struct rq_term *term,
    const struct rq_term *term_to_subtract
    );

/** Write the term out to an XML stream.
 */
RQ_EXPORT int rq_term_write_xml(struct rq_term *term, rq_stream_t stream);

/** Get the number of days in the term.
 */
RQ_EXPORT short rq_term_get_days(const struct rq_term *term);

/** Get the number of weeks in the term.
 */
RQ_EXPORT short rq_term_get_weeks(const struct rq_term *term);

/** Get the number of months in the term.
 */
RQ_EXPORT short rq_term_get_months(const struct rq_term *term);

/** Get the number of years in the term.
 */
RQ_EXPORT short rq_term_get_years(const struct rq_term *term);

/** Determine if terms are exactly equal.
 */
RQ_EXPORT short rq_term_is_equal(const struct rq_term *term1, const struct rq_term *term2);

/** If results of comparions such that: <0 => term1 < term 2, =0 => term1 = term 2, >0 => term1 > term 2.
 */
RQ_EXPORT short rq_term_compare(struct rq_term *term1, struct rq_term *term2);

/** Free an allocated term structure.
 */
RQ_EXPORT void rq_term_free(rq_term_t term);

/** Invert a term by setting all it's periods to the negative.
 */
RQ_EXPORT void rq_term_invert(struct rq_term *term);

/** Return the term as an annualized period (ie as a percentage of a year).
 */
RQ_EXPORT double rq_term_get_annualized_period(const struct rq_term *term);

/** Return the term as a total number of days (adding all components).
 */
RQ_EXPORT int rq_term_get_days_period(const struct rq_term *term);

/** Fill out a rq_term structure from a string.
 */
RQ_EXPORT void
rq_term_parse(
    struct rq_term *term, /**< The term to get filled from the parse. */
    const char *str, /**< The string to parse. Must be in the form 1M/1D/1Y, etc */
    const char *delims /**< delimiters to use when parsing. */
    ); 

/** Convert a term to a string.
 *
 * Convert a term to a string, putting the result in the specified buffer.
 * This function takes the maximum length of the buffer as an argument and
 * tries not to overwrite the buffer.
 * The format specifier can have any characters in it, which are copied
 * to the result buffer. Characters which have special significance are:
 *
 * %d - the number of days in the term.
 * %w - the number of weeks in the term.
 * %m - the number of months in the term.
 * %y - the number of years in the term.
 */
RQ_EXPORT char *
rq_term_to_string(
    char *buf, /**< The result buffer. */
    unsigned int max_len, /**< The maximum length of the result buffer. */
    const char *format, /**< The format string. */
    struct rq_term *term /**< The term to output. */
    );

/** Fill out a rq_term structure from a tenor.
 */
RQ_EXPORT void 
rq_term_set(struct rq_term* term, const char* period, const int multiplier);

/** Return the time difference between two terms (as a percentage of a year).
 */
RQ_EXPORT double
rq_term_diff(struct rq_term* termBase, struct rq_term* termDiff);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
