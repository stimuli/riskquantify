/**
 * \file rq_time.h
 * \author Brett Hutley
 *
 * \brief Provides time helper functions and the rq_time type.
 */
/*
** rq_time.h
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
#ifndef rq_time_h
#define rq_time_h

#include "rq_config.h"
#include "rq_enum.h"
#include "rq_term.h"
#include "rq_date.h"
#include "rq_time.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* format is:

    + hour
    |  + minute
    |  |  + second
    01:48:37
    || || |+- 6th nybble 
    || || +---5th nybble 
    || ||
    || |+-----4th nybble
    || +------3rd nybble
    ||
    |+--------2nd nybble
    +---------1st nybble

    7th & 8th nybble are currently 0x01 in a VALID time.
    In an INVALID time, the last byte is 0x00.

 */

typedef struct {
    unsigned char t[4];
} rq_time;

typedef unsigned short rq_hour;
typedef unsigned short rq_minute;
typedef unsigned short rq_second;

/* -- prototypes -- */

/** Initialize a time structure to an INVALID valid.
 */
RQ_EXPORT void rq_time_init(rq_time *tm);

/** Test whether a time is valid.
 */
RQ_EXPORT short rq_time_is_valid(rq_time tm);

/** Construct a rq_time from the hour, min, second parameters.
 */
RQ_EXPORT rq_time rq_time_from_hms(rq_hour hour, rq_minute minute, rq_second second);

/** Convert a rq_time into a hour, minute and second.
 */
RQ_EXPORT short 
rq_time_to_hms(
    rq_time time, 
    rq_hour *hour,
    rq_minute *minute,
    rq_second *second
    );

/** Parse a time value from a string.
 *
 * The string must be in the format HH:MM:SS
 */
RQ_EXPORT rq_time rq_time_parse(const char *s);

/** Convert a time value to a string.
 *
 * This function uses the following format specifiers:
 *   %%   - percent sign
 *   %H   - 2 digit hour of the day, 24 hour day
 *   %M   - 2 digit minute
 *   %S   - 2 digit second
 */
RQ_EXPORT char *rq_time_to_string(char *buf, unsigned int buflen, const char *format, rq_time tm);

/** Get the current time.
 */
RQ_EXPORT rq_time rq_time_now();

/** Get the hour (0 - 23)
 */
RQ_EXPORT unsigned rq_time_get_hour(rq_time tm);

/** Get the minute (0-59)
 */
RQ_EXPORT unsigned rq_time_get_minute(rq_time tm);

/** Get the second (0-59)
 */
RQ_EXPORT unsigned rq_time_get_second(rq_time tm);

/** Compare two times, return a positive number if tm1 is greater than
    tm2, a negative number if less or zero if they are the same.
 */
RQ_EXPORT int rq_time_compare(rq_time tm1, rq_time tm2);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
