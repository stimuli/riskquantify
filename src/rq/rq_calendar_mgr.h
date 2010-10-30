/**
 * \file rq_calendar_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_calendar_mgr provides a manager class for rq_calendar
 * objects.
 */
/*
** rq_calendar_mgr.h
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
#ifndef rq_calendar_mgr_h
#define rq_calendar_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_calendar.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_calendar_mgr {
    rq_tree_rb_t calendars;
} *rq_calendar_mgr_t;


/** Test whether the rq_calendar_mgr is NULL */
RQ_EXPORT int rq_calendar_mgr_is_null(rq_calendar_mgr_t obj);

/**
 * Allocate a new calendar manager object
 */
RQ_EXPORT rq_calendar_mgr_t rq_calendar_mgr_alloc();

/**
 * Free an allocated calendar manager object
 */
RQ_EXPORT void rq_calendar_mgr_free(rq_calendar_mgr_t calendar_mgr);

/**
 * Add a calendar to the calendar manager
 */
RQ_EXPORT void rq_calendar_mgr_add(rq_calendar_mgr_t calendar_mgr, rq_calendar_t cal);

/**
 * Get a calendar from the calendar manager
 */
RQ_EXPORT rq_calendar_t rq_calendar_mgr_get(const rq_calendar_mgr_t calendar_mgr, const char *id);

/** Clear the calendars from the calendar manager */
RQ_EXPORT void rq_calendar_mgr_clear(rq_calendar_mgr_t m);

/** Get an iterator for the calendar. */
RQ_EXPORT rq_iterator_t rq_calendar_mgr_get_iterator(rq_calendar_mgr_t m);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
