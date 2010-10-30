/*
** rq_routing_ids.c
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
#include "rq_routing_ids.h"
#include <stdlib.h>

RQ_EXPORT void
rq_routing_ids_init(struct rq_routing_ids *p)
{
    p->routing_id_list = rq_array_alloc(free);
}

RQ_EXPORT void
rq_routing_ids_clear(struct rq_routing_ids *p)
{
    if (p->routing_id_list)
    {
        rq_array_free(p->routing_id_list);
        p->routing_id_list = NULL;
    }
}

RQ_EXPORT int
rq_routing_ids_write_xml(struct rq_routing_ids *p, rq_stream_t output_stream)
{
    unsigned int i;
    rq_stream_write_string(output_stream, "<routingIds>");

    for (i = 0; i < rq_array_size(p->routing_id_list); i++)
    {
        const char *routing_id = (const char *)rq_array_get_at(p->routing_id_list, i);
        rq_stream_write_string(output_stream, "<routingId>");
        rq_stream_write_string(output_stream, routing_id);
        rq_stream_write_string(output_stream, "</routingId>");
    }

    rq_stream_write_string(output_stream, "</routingIds>");

    return 0;
}

RQ_EXPORT void 
rq_routing_ids_free(struct rq_routing_ids *p)
{
    rq_routing_ids_clear(p);
    free(p);
}
