/*
** rq_street_address.c
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
#include "rq_street_address.h"
#include <stdlib.h>

RQ_EXPORT void
rq_street_address_init(struct rq_street_address *p)
{
    p->street_line_list = rq_array_alloc(free);
}

RQ_EXPORT void
rq_street_address_clear(struct rq_street_address *p)
{
    rq_array_clear(p->street_line_list);
}

RQ_EXPORT int
rq_street_address_write_xml(struct rq_street_address *p, rq_stream_t output_stream)
{
    unsigned int i;

    rq_stream_write_string(output_stream, "<streetAddress>");

    for (i = 0; i < rq_array_size(p->street_line_list); i++)
    {
        const char *street_line = (const char *)rq_array_get_at(p->street_line_list, i);
        rq_stream_write_string(output_stream, "<streetLine>");
        rq_stream_write_string(output_stream, street_line);
        rq_stream_write_string(output_stream, "</streetLine>");
    }

    rq_stream_write_string(output_stream, "</streetAddress>");

    return 0;
}

RQ_EXPORT void
rq_street_address_free(struct rq_street_address *p)
{
    rq_array_free(p->street_line_list);
    RQ_FREE(p);
}
