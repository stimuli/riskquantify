/*
** rq_business_center_time.c
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
#include "rq_business_center_time.h"
#include <stdlib.h>

RQ_EXPORT void
rq_business_center_time_init(struct rq_business_center_time *p)
{
    p->hour_minute_time = 0;
    p->business_center = NULL;
}

RQ_EXPORT void
rq_business_center_time_clear(struct rq_business_center_time *p)
{
    p->hour_minute_time = 0;
    if (p->business_center)
    {
       RQ_FREE((char *)p->business_center);
        p->business_center = NULL;
    }
}

RQ_EXPORT int
rq_business_center_time_write_xml(struct rq_business_center_time *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<businessCenterTime>");

    rq_stream_write_string(output_stream, "<hourMinuteTime>");
    rq_stream_write_integer_as_string(output_stream, p->hour_minute_time);
    rq_stream_write_string(output_stream, "</hourMinuteTime>");
    rq_stream_write_string(output_stream, "<businessCenter>");
    rq_stream_write_string(output_stream, p->business_center);
    rq_stream_write_string(output_stream, "</businessCenter>");

    rq_stream_write_string(output_stream, "</businessCenterTime>");

    return 0;
}

