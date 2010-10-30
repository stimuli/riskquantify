/**
 * \file rq_datetime.h
 * \author Brett Hutley
 *
 * \brief Provides date/time helper functions and the rq_datetime
 * type.
 */
/*
** rq_datetime.h
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
#ifndef rq_datetime_h
#define rq_datetime_h

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

/* -- enums ------------------------------------------------------- */

enum rq_datetime_accuracy {
    RQ_DATETIME_ACCURACY_5_MIN = 5,
    RQ_DATETIME_ACCURACY_10_MIN = 10,
    RQ_DATETIME_ACCURACY_15_MIN = 15,
    RQ_DATETIME_ACCURACY_20_MIN = 20,
    RQ_DATETIME_ACCURACY_30_MIN = 30,
    RQ_DATETIME_ACCURACY_60_MIN = 60
};

/* -- structs ----------------------------------------------------- */

typedef struct {
    rq_date date;
    rq_time time;
} rq_datetime;

/* -- prototypes -------------------------------------------------- */

/** Construct a datetime from the day, month, year, hour, min, second
 * parameters.
 */
RQ_EXPORT rq_datetime rq_datetime_from_dmyhms(short day, short month, short year, rq_hour hour, rq_minute minute, rq_second second);

/** Convert a datetime into a day, month, year, hour, minute and second.
 */
RQ_EXPORT short 
rq_datetime_to_dmyhms(
    rq_datetime dt, 
    short *day, 
    short *mon, 
    short *year,
    rq_hour *hour,
    rq_minute *minute,
    rq_second *second
    );

/** Get the current date/time.
 */
rq_datetime rq_datetime_now();

/** Get the next date/time, rounded forward to the next x minute interval.
 * For example, if the current date/time is 2007-10-24 10:06 and we
 * pass in a 5 minute interval, then the next date/time will be
 * 2007-10-24 10:10.
 */
rq_datetime rq_datetime_next_with_accuracy(rq_datetime from, enum rq_datetime_accuracy accuracy);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
