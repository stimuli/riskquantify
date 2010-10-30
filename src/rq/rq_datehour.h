/**
 * \file rq_datehour.h
 * \author Brett Hutley
 *
 * \brief Provides date/hour helper functions and the rq_datehour
 * type.
 */
/*
** rq_datehour.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2006-2008 Brett Hutley
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
#ifndef rq_datehour_h
#define rq_datehour_h

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


typedef struct {
    rq_date date;
    short hour;
} rq_datehour;

/** Construct a datehour from the day, month, year, and hour parameters.
 *
 * The hour must be between 0-23.
 */
RQ_EXPORT rq_datehour rq_datehour_from_dmyh(short day, short month, short year, short hour);

/** Convert a datehour into a day, month, year, hour.
 */
RQ_EXPORT short 
rq_datehour_to_dmyh(
    rq_datehour dt, 
    short *day, 
    short *mon, 
    short *year,
    short *hour
    );

/** Get the current date/hour.
 */
rq_datehour rq_datehour_now();

/** Get the day.
 */
short rq_datehour_get_day(rq_datehour dh);

/** Get the month.
 */
short rq_datehour_get_month(rq_datehour dh);

/** Get the year.
 */
short rq_datehour_get_year(rq_datehour dh);

/** Get the hour.
 */
short rq_datehour_get_hour(rq_datehour dh);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
