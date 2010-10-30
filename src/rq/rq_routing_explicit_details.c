/*
** rq_routing_explicit_details.c
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
#include "rq_routing_explicit_details.h"
#include <stdlib.h>

RQ_EXPORT void
rq_routing_explicit_details_init(struct rq_routing_explicit_details *p)
{
    p->routing_name = NULL;
    p->routing_address = NULL;
    p->routing_account_number = NULL;
    p->routing_reference_text = rq_array_alloc(free);
}

RQ_EXPORT void
rq_routing_explicit_details_clear(struct rq_routing_explicit_details *p)
{
    if (p->routing_name)
    {
        RQ_FREE((char *)p->routing_name);
        p->routing_name = NULL;
    }

    if (p->routing_address)
    {
        rq_address_free(p->routing_address);
        p->routing_address = NULL;
    }

    if (p->routing_account_number)
    {
        RQ_FREE((char *)p->routing_account_number);
        p->routing_account_number = NULL;
    }

    rq_array_clear(p->routing_reference_text);
}

RQ_EXPORT int
rq_routing_explicit_details_write_xml(struct rq_routing_explicit_details *p, rq_stream_t output_stream)
{
    unsigned int i;

    rq_stream_write_string(output_stream, "<routingExplicitDetails>");

    rq_stream_write_string(output_stream, "<routingName>");
    rq_stream_write_string(output_stream, p->routing_name);
    rq_stream_write_string(output_stream, "</routingName>");

    if (p->routing_address)
        rq_address_write_xml(p->routing_address, output_stream);

    if (p->routing_account_number)
    {
        rq_stream_write_string(output_stream, "<routingAccountNumber>");
        rq_stream_write_string(output_stream, p->routing_account_number);
        rq_stream_write_string(output_stream, "</routingAccountNumber>");
    }

    for (i = 0; i < rq_array_size(p->routing_reference_text); i++)
    {
        const char *text = (const char *)rq_array_get_at(p->routing_reference_text, i);
        rq_stream_write_string(output_stream, "<routingReferenceText>");
        rq_stream_write_string(output_stream, text);
        rq_stream_write_string(output_stream, "</routingReferenceText>");
    }

    rq_stream_write_string(output_stream, "</routingExplicitDetails>");

    return 0;
}

RQ_EXPORT void 
rq_routing_explicit_details_free(struct rq_routing_explicit_details *p)
{
    rq_routing_explicit_details_clear(p);
    rq_array_free(p->routing_reference_text);
    RQ_FREE(p);
}
