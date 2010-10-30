/**
 * \file rq_date_event.h
 * \author Brett Hutley
 *
 * \brief The rq_date_event object implements an event that occurs on
 * a date.  An example of an event is a holiday, but this object could
 * also be used to store elections or other type of events that might
 * affect volatility interpolation.
 */
/*
** rq_date_event.h
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
#ifndef rq_date_event_h
#define rq_date_event_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_date.h"

/* -- defines ----------------------------------------------------- */

#define RQ_DATE_EVENT_GEN_HOLIDAY 0x80000000

/* -- structs ----------------------------------------------------- */

/**
 * \brief The data event structure defines a date, and an event that
 * occurs on that date.
 *
 * The data event structure defines a date, and an event that occurs
 * on that date. Events are represented by an event type ID allowing
 * us to store recurring events or non-recurring events efficiently.
 * The highest order bit represents whether the event type is a
 * non-trading day or not, so we can quickly determine this by whether
 * the event type id is negative.
 */

struct rq_date_event {
    rq_date date;
    long event_mask;
};

/* -- prototypes -------------------------------------------------- */

RQ_EXPORT long 
rq_date_event_is_holiday(long event_mask);


#endif
