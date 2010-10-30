/*
** rq_calendar.c
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
#include <stdlib.h>
#include <string.h>

#include "rq_calendar.h"
#include "rq_date_event.h"
#include "rq_object_schema_node.h"
#include "rq_error.h"
#include "rq_dom_parser.h"

static unsigned short s_growth_factor = 40;

RQ_EXPORT rq_calendar_t 
rq_calendar_alloc(const char *id)
{
    struct rq_calendar *cal = 
        (struct rq_calendar *)RQ_MALLOC(sizeof(struct rq_calendar));
    cal->id = (const char *)(id ? RQ_STRDUP(id) : NULL);
    cal->max_date_events = s_growth_factor;
    cal->num_date_events = 0;
    cal->growth_factor = s_growth_factor;
    cal->date_events = RQ_MALLOC(s_growth_factor * sizeof(struct rq_date_event));

    cal->weekend_mask = (RQ_CALENDAR_WEEKEND_MASK_SATURDAY | RQ_CALENDAR_WEEKEND_MASK_SUNDAY);

    cal->calendar_type = RQ_CALENDAR_TYPE_BASE;
    cal->num_composites = 0;
    memset(cal->base_calendars, '\0', sizeof(struct rq_calendar *) * MAX_COMPOSITE_CALENDAR_SIZE);
    
    return cal;
}

RQ_EXPORT void 
rq_calendar_free(rq_calendar_t cal)
{
    if (cal->id)
        RQ_FREE((char *)cal->id);

    RQ_FREE(cal->date_events);
    RQ_FREE(cal);
}

RQ_EXPORT rq_calendar_t 
rq_calendar_clone(rq_calendar_t cal)
{
    struct rq_calendar *clone = 
        (struct rq_calendar *)RQ_MALLOC(sizeof(struct rq_calendar));
    clone->id = (const char *)(cal->id ? RQ_STRDUP(cal->id) : NULL);
    clone->max_date_events = cal->max_date_events;
    clone->num_date_events = cal->num_date_events;
    clone->growth_factor = cal->growth_factor;
    clone->date_events = RQ_MALLOC(clone->max_date_events * sizeof(struct rq_date_event));
    memcpy(clone->date_events, cal->date_events, cal->num_date_events * sizeof(struct rq_date_event));

    return clone;
}

static struct rq_date_event *
rq_calendar_find_date_event(const struct rq_calendar *cal, rq_date date, int *lb, int *ub)
{
	int	cmp;

	*lb = -1;
	*ub = cal->num_date_events;
	while (*ub - *lb > 1)
	{
		int mid = (*lb + *ub) >> 1;
		if ((cmp = (date - cal->date_events[mid].date)) > 0)
			*lb = mid;
		else if (cmp < 0)
			*ub = mid;
		else
			return &cal->date_events[mid];
	}

	return NULL;
}

RQ_EXPORT void 
rq_calendar_add_event(rq_calendar_t cal, rq_date date, long event_mask)
{
    if (cal->num_date_events == 0)
    {
        cal->date_events[0].date = date;
        cal->date_events[0].event_mask = event_mask;
        cal->num_date_events = 1;
    }
    else
    {
        int lb;
        int ub;

        struct rq_date_event *date_event = 
            rq_calendar_find_date_event(cal, date, &lb, &ub);

        /* printf("lb: %d ub: %d\n", lb, ub); */

        if (date_event)
            date_event->event_mask |= event_mask;
        else
        {
            /* check whether I need to grow the calendar. */
            if (cal->num_date_events == cal->max_date_events)
            {
                cal->max_date_events += cal->growth_factor;
                cal->date_events = (struct rq_date_event *)RQ_REALLOC(cal->date_events, cal->max_date_events * sizeof(struct rq_date_event));
            }

            if (lb >= 0 && cal->date_events[lb].date > date)
            {
                int i;

                /* move all dates from lb up one */
                for (i = cal->num_date_events; i > lb; i--)
                    memcpy(&cal->date_events[i], &cal->date_events[i-1], sizeof(struct rq_date_event));

                cal->date_events[lb].date = date;
                cal->date_events[lb].event_mask = event_mask;
                cal->num_date_events++;

            }
            else /* if (cal->date_events[ub].date < date) */
            {
                int i;
                /* move all dates from ub up one */
                for (i = cal->num_date_events; i > ub; i--)
                    memcpy(&cal->date_events[i], &cal->date_events[i-1], sizeof(struct rq_date_event));


                cal->date_events[ub].date = date;
                cal->date_events[ub].event_mask = event_mask;
                cal->num_date_events++;
            }
        }
    }
}

RQ_EXPORT unsigned int 
rq_calendar_size(const rq_calendar_t cal)
{
    return cal->num_date_events;
}

RQ_EXPORT const struct rq_date_event *
rq_calendar_element_at(const rq_calendar_t cal, unsigned int offset)
{
    return &cal->date_events[offset];
}

RQ_EXPORT void
rq_calendar_set_id(rq_calendar_t cal, const char *id)
{
    if (cal->id)
       RQ_FREE((char *)cal->id);
    cal->id = RQ_STRDUP(id);
}

RQ_EXPORT const char *
rq_calendar_get_id(const rq_calendar_t cal)
{
    return cal->id;
}

RQ_EXPORT long
rq_calendar_get_event_mask(const rq_calendar_t cal, rq_date date)
{
    int lb;
    int ub;

    struct rq_date_event *date_event = 
        rq_calendar_find_date_event(cal, date, &lb, &ub);
    if (date_event)
        return date_event->event_mask;
    return 0;
}

RQ_EXPORT short 
rq_calendar_is_holiday(const rq_calendar_t cal, rq_date date)
{
    int lb;
    int ub;

    struct rq_date_event *date_event = 
        rq_calendar_find_date_event(cal, date, &lb, &ub);
    if (date_event)
        return (date_event->event_mask & RQ_DATE_EVENT_GEN_HOLIDAY) != 0L;
    return 0;
}

RQ_EXPORT int
rq_calendar_is_null(rq_calendar_t obj)
{
    return (obj == NULL);
}

/* -- object schema support -- */
struct _date_event {
    rq_date date;
    short eventtype;
};

static void *
rq_calendar_constructor()
{
    return rq_calendar_alloc(NULL);
}

static void
rq_calendar_destructor(void *p)
{
    rq_calendar_free((rq_calendar_t)p);
}

static void *
rq_calendar_cloner(void *p)
{
    return rq_calendar_clone((rq_calendar_t)p);
}

static void
rq_calendar_set_value_string(void *p, const char *property_name, const char *value)
{
    if (!strcmp(property_name, "@id"))
        rq_calendar_set_id((rq_calendar_t)p, value);
}

static void
rq_calendar_set_value_object(void *p, const char *property_name, void *value)
{
    if (!strcmp(property_name, "dateEvent"))
    {
        struct _date_event *dateevent = (struct _date_event *)value;
        rq_calendar_add_event((rq_calendar_t)p, dateevent->date, dateevent->eventtype);
        RQ_FREE(dateevent);
    }
}

static void *
rq_dateevent_constructor()
{
    struct _date_event *dateevent = (struct _date_event *)
        RQ_CALLOC(1, sizeof(struct _date_event));
    return dateevent;
}

static void *
rq_dateevent_cloner(void *de)
{
    struct _date_event *dateevent = (struct _date_event *)de;
    struct _date_event *clone = (struct _date_event *)
        RQ_CALLOC(1, sizeof(struct _date_event));

    memcpy(clone, dateevent, sizeof(struct _date_event));

    return clone;
}

static void 
dateevent_set_value_string(void *parent, const char *prop_name, const char *value)
{
    struct _date_event *dateevent = (struct _date_event *)parent;
    if (!strcmp(prop_name, "@type"))
    {
        if (!strcmp(value, "HOLIDAY"))
        {
            dateevent->eventtype |= RQ_DATE_EVENT_GEN_HOLIDAY;
        }
    }
}

static void 
dateevent_set_value_date(void *parent, const char *prop_name, rq_date value)
{
    struct _date_event *dateevent = (struct _date_event *)parent;
    if (!strcmp(prop_name, "."))
        dateevent->date = value;
}

void 
rq_calendar_init_object_schemas(rq_object_schema_mgr_t schema_mgr)
{
    rq_object_schema_t cal_schema = rq_object_schema_alloc(
        "Calendar",
        rq_calendar_constructor,
        rq_calendar_destructor,
        rq_calendar_cloner,
        rq_calendar_set_value_string,
        NULL,
        NULL,
        NULL,
        rq_calendar_set_value_object
        );
    rq_object_schema_t dateevent_schema = rq_object_schema_alloc(
        "DateEvent",
        rq_dateevent_constructor,
        free,
        rq_dateevent_cloner,
        dateevent_set_value_string,
        NULL,
        NULL,
        dateevent_set_value_date,
        NULL
        );

    rq_object_schema_node_t cal_dateevent = 
        rq_object_schema_node_alloc(
            "dateEvent",
            "DateEvent"
            );

    rq_object_schema_node_t dateevent_value = 
        rq_object_schema_node_alloc(
            ".",
            "date"
            );

    rq_object_schema_add_property(cal_schema, cal_dateevent);
    rq_object_schema_add_property(dateevent_schema, dateevent_value);

    rq_object_schema_mgr_add(schema_mgr, cal_schema);
    rq_object_schema_mgr_add(schema_mgr, dateevent_schema);
}

RQ_EXPORT short 
rq_calendar_is_weekend(const rq_calendar_t cal, rq_date date)
{
    short is_weekend = 0;
    if (cal->weekend_mask != 0)
    {
        short dow = rq_date_get_day_of_week(date);
        char mask = 0x01;
        if (dow > 0)
            mask <<= dow;

        is_weekend = (cal->weekend_mask & mask);
    }

    return is_weekend;
}

RQ_EXPORT short
rq_calendar_is_good_date(const rq_calendar_t cal, rq_date date)
{
    if (!rq_calendar_is_null(cal))
        return !rq_calendar_is_weekend(cal, date) && !rq_calendar_is_holiday(cal, date);
    else
        return !rq_date_is_weekend(date);
}

RQ_EXPORT long 
rq_calendar_businessday_count(const rq_calendar_t cal, rq_date from_date, rq_date to_date)
{
	long count = 0;

	rq_date d;
	for (d = from_date; d <= to_date; ++d)
		if (rq_calendar_is_good_date(cal, d))
			++count;

	return count;
}

RQ_EXPORT rq_error_code
rq_calendar_write_to_stream(const rq_calendar_t cal, rq_stream_t stream)
{
    int i;

    rq_stream_printf(stream, "<calendar id='%s' ", cal->id);
    if (cal->calendar_type == RQ_CALENDAR_TYPE_BASE)
    {
        rq_stream_printf(stream, "type='base'>\n");

        rq_stream_printf(stream, " <dateEvents>\n");
        for (i = 0; i < cal->num_date_events; i++)
        {
            char datebuf[50];

            rq_stream_printf(stream, "  <dateEvent>\n");
            rq_stream_printf(stream, "   <date>%s</date>\n", rq_date_to_string(datebuf, "yyyy-mm-dd", cal->date_events[i].date));
            rq_stream_printf(stream, "   <eventMask>%ld</eventMask>\n", cal->date_events[i].event_mask);
            rq_stream_printf(stream, "  </dateEvent>\n");
        }
        rq_stream_printf(stream, " </dateEvents>\n");
    }
    else
    {
        rq_stream_printf(stream, "type='composite'>\n");

        rq_stream_printf(stream, " <baseCalendars>\n");
        for (i = 0; i < cal->num_composites; i++)
            rq_stream_printf(stream, "  <baseCalendar>%s</baseCalendar>\n", cal->base_calendars[i]);
        rq_stream_printf(stream, " </baseCalendars>\n");
    }
    
    rq_stream_printf(stream, "</calendar>\n");

    return RQ_OK;
}


RQ_EXPORT rq_error_code
rq_calendar_read_from_stream(rq_calendar_t cal, rq_stream_t stream)
{
    rq_error_code errcode = RQ_FAILED;

    struct rq_xml_node *node_top = rq_dom_parser_parse_stream(stream);
    if (node_top)
    {
        struct rq_xml_node *node_cal = rq_xml_node_find_element(node_top, "calendar", 0);
        if (node_cal)
        {
            char cal_id[512];

            if (rq_xml_node_find_text(node_cal, "@id", cal_id, 512) > 0)
            {
                rq_calendar_set_id(cal, cal_id);
            }
        }

        rq_xml_node_free(node_top);
    }

    return errcode;
}
