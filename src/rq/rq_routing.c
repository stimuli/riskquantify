/*
** rq_routing.c
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
#include "rq_routing.h"
#include <stdlib.h>

RQ_EXPORT void
rq_routing_init(struct rq_routing *p)
{
    p->routing_ids = NULL;
    p->routing_explicit_details = NULL;
}

RQ_EXPORT void
rq_routing_clear(struct rq_routing *p)
{
    if (p->routing_ids)
    {
        rq_routing_ids_free(p->routing_ids);
        p->routing_ids = NULL;
    }

    if (p->routing_explicit_details)
    {
        rq_routing_explicit_details_free(p->routing_explicit_details);
        p->routing_explicit_details = NULL;
    }
}

RQ_EXPORT int
rq_routing_write_xml(struct rq_routing *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<routing>");

    if (p->routing_ids)
        rq_routing_ids_write_xml(p->routing_ids, output_stream);

    if (p->routing_explicit_details)
        rq_routing_explicit_details_write_xml(p->routing_explicit_details, output_stream);

    rq_stream_write_string(output_stream, "</routing>");

    return 0;
}

RQ_EXPORT void
rq_routing_free(struct rq_routing *p)
{
    rq_routing_clear(p);
   RQ_FREE(p);
}
