/*
** rq_business_centers.c
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
#include "rq_business_centers.h"
#include <stdlib.h>

RQ_EXPORT void
rq_business_centers_init(struct rq_business_centers *p)
{
    p->business_center_list = rq_array_alloc(free);
}

RQ_EXPORT void
rq_business_centers_clear(struct rq_business_centers *p)
{
    rq_array_free(p->business_center_list);
    p->business_center_list = NULL;
}

RQ_EXPORT int
rq_business_centers_write_xml(struct rq_business_centers *p, rq_stream_t output_stream)
{
    unsigned int i = 0;

    rq_stream_write_string(output_stream, "<businessCenters>");

    for (i = 0; i < rq_array_size(p->business_center_list); i++)
    {
        const char *business_center = (const char *)rq_array_get_at(p->business_center_list, i);
        rq_stream_write_string(output_stream, "<businessCenter>");
        rq_stream_write_string(output_stream, business_center);
        rq_stream_write_string(output_stream, "</businessCenter>");
    }

    rq_stream_write_string(output_stream, "</businessCenters>");

    return 0;
}

