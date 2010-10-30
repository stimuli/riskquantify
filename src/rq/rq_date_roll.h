/**
 * \file rq_date_roll.h
 * \author Brett Hutley
 *
 * \brief This file contains functions for rolling forward or backward
 * through time, while taking account of market conventions and
 * holiday calendars.
 */
/*
** rq_date_roll.h
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
#ifndef rq_date_roll_h
#define rq_date_roll_h

#include "rq_config.h"
#include "rq_date.h"
#include "rq_term.h"
#include "rq_calendar.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** Determine if a roll date is good.
 * The roll date is good if it is not a weekend or a holiday on any calendar.
 */
RQ_EXPORT short
rq_date_roll_is_good_date(
    rq_date date,
    enum rq_date_roll_convention date_roll_convention, 
    const rq_calendar_t *cals,
    unsigned short num_cals
    );

/** Get the offset date in the date roll. Days field only is business days.
 */
RQ_EXPORT rq_date
rq_date_roll_offset_term(
    rq_date start_date,
    enum rq_day_type day_type, /* day type used to adjust initial offset before adding term,
                                  currently only supports WEEK and BUSINESS */
    const struct rq_term *term,
    const rq_calendar_t *cals,
    unsigned short num_cals,
    enum rq_date_roll_convention date_roll_convention
    );

/** Get the offset date in the date roll.
 * Get the offset date in the date roll, passing in the current date,
 * the offset, and current calendar(s).  
 */
RQ_EXPORT rq_date
rq_date_roll_days_offset(
    rq_date start_date,
    int offset,
    const rq_calendar_t *cals,
    unsigned short num_cals,
    enum rq_date_roll_convention date_roll_convention
    );
/* Old call syntax without bus_day convention. */
RQ_EXPORT rq_date 
rq_date_roll_get_offset_date(
    rq_date start_date, 
    int offset,
    const rq_calendar_t *cals,
    unsigned short num_cals
    );

/**
 * Adjust the date according to the date roll convention until
 * it is "Good".
 */
/* DS (IT&E) - made this an export to be available for start and end
 * dates in generate cashflows from params. 
*/
RQ_EXPORT rq_date
rq_date_roll_adjust_date(
    rq_date date, 
    enum rq_date_roll_convention date_roll_convention, 
    const rq_calendar_t *cals,
    unsigned short num_cals
    );

/** Get the next date in the date roll.
 * Get the next date in the date roll, passing in the current date,
 * the term, the date roll convention and the current calendar.  
 */
RQ_EXPORT rq_date 
rq_date_roll_get_date(
    rq_date start_date, 
    const struct rq_term *term,
    enum rq_roll_convention roll_convention,
    enum rq_date_roll_convention date_roll_convention,
    const rq_calendar_t *cals,
    unsigned short num_cals
    );

/** Generate the list of date rolls based on the date roll convention.
 */
RQ_EXPORT int
rq_date_roll_generate_dates(
    rq_date *dates,
    unsigned int max_dates,
    rq_date start_date,
    rq_date end_date,
    const struct rq_term *term,
    enum rq_roll_convention roll_convention,
    enum rq_date_roll_convention date_roll_convention,
    const rq_calendar_t *calendars,
    unsigned short num_calendars,
    enum rq_date_roll_stub_position stub_position,
    unsigned allowable_days_matching_error,
	int *startStubIndex
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
