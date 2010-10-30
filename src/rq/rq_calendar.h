/**
 * \file rq_calendar.h
 * \author Brett Hutley
 *
 * \brief The rq_calendar object represents a calendar of event dates
 * (like holidays)
 */
/*
** rq_calendar.h
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
#ifndef rq_calendar_h
#define rq_calendar_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date_event.h"
#include "rq_object_schema_mgr.h"
#include "rq_stream.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

enum rq_calendar_type {
    RQ_CALENDAR_TYPE_BASE,
    RQ_CALENDAR_TYPE_COMPOSITE
};

#define MAX_COMPOSITE_CALENDAR_SIZE 5

#define RQ_CALENDAR_WEEKEND_MASK_SUNDAY 0x01
#define RQ_CALENDAR_WEEKEND_MASK_MONDAY 0x02
#define RQ_CALENDAR_WEEKEND_MASK_TUESDAY 0x04
#define RQ_CALENDAR_WEEKEND_MASK_WEDNESDAY 0x08
#define RQ_CALENDAR_WEEKEND_MASK_THURSDAY 0x10
#define RQ_CALENDAR_WEEKEND_MASK_FRIDAY 0x20
#define RQ_CALENDAR_WEEKEND_MASK_SATURDAY 0x40

typedef struct rq_calendar {
    const char *id;
    unsigned int max_date_events;
    unsigned int num_date_events;
    unsigned short growth_factor;
    struct rq_date_event *date_events;

    unsigned char weekend_mask;
    enum rq_calendar_type calendar_type;
    unsigned int num_composites;
    struct rq_calendar *base_calendars[MAX_COMPOSITE_CALENDAR_SIZE];
} *rq_calendar_t;


/** Test whether the rq_calendar is NULL */
RQ_EXPORT int rq_calendar_is_null(rq_calendar_t obj);

/** Allocate a new calendar with a particular ID. The ID may be NULL.
 */
RQ_EXPORT rq_calendar_t rq_calendar_alloc(const char *id);

/** Free an allocated calendar.
 */
RQ_EXPORT void rq_calendar_free(rq_calendar_t cal);

/** Clone the calendar.
 */
RQ_EXPORT rq_calendar_t rq_calendar_clone(rq_calendar_t calendar);

/** Add an event to the calendar.
 *
 * @param cal the calendar to add the event to.
 * @param date the date of the event.
 * @param event the event mask representing the type of event occuring on this date.
 */
RQ_EXPORT void rq_calendar_add_event(rq_calendar_t cal, rq_date date, long event_mask);

/** Get the number of date events in the calendar.
 */
RQ_EXPORT unsigned int rq_calendar_size(const rq_calendar_t cal);

/** Get the date event at a particular zero-based offset.
 */
RQ_EXPORT const struct rq_date_event *rq_calendar_element_at(const rq_calendar_t cal, unsigned int offset);

/** Set the calendar ID.
 */
RQ_EXPORT void rq_calendar_set_id(rq_calendar_t calendar, const char *id);

/** Get the calendar ID.
 */
RQ_EXPORT const char *rq_calendar_get_id(const rq_calendar_t cal);

/** Get the date event mask for a particular date.
 */
RQ_EXPORT long rq_calendar_get_event_mask(const rq_calendar_t cal, rq_date date);

/** Determine whether a particular date is a holiday or not.
 */
RQ_EXPORT short rq_calendar_is_holiday(const rq_calendar_t cal, rq_date date);

/** Determine whether a particular date is a weekend or not.
 */
RQ_EXPORT short rq_calendar_is_weekend(const rq_calendar_t cal, rq_date date);

/** Determine whether a particular date is a business date or not.
 */
RQ_EXPORT short rq_calendar_is_good_date(const rq_calendar_t cal, rq_date date);

/** Counts the number of business days between 2 inclusive dates.
 */
RQ_EXPORT long rq_calendar_businessday_count(const rq_calendar_t cal, rq_date from_date, rq_date to_date);

/** Initialize the object schema for the Calendar and DateEvent types.
 */
RQ_EXPORT void rq_calendar_init_object_schemas(rq_object_schema_mgr_t schema_mgr);

/** Write the calendar to a stream.
 */
RQ_EXPORT rq_error_code rq_calendar_write_to_stream(const rq_calendar_t cal, rq_stream_t stream);

/** Read the calendar from a stream. */
RQ_EXPORT rq_error_code rq_calendar_read_from_stream(rq_calendar_t cal, rq_stream_t stream);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
