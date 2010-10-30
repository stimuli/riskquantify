/*
** rq_address.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002,2003,2004,2005,2006,2007,2008 Brett Hutley
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
#include "rq_address.h"
#include <stdlib.h>

RQ_EXPORT 
void 
rq_address_init(struct rq_address *p)
{
    p->street_address = NULL;
    p->city = NULL;
    p->state = NULL;
    p->country = NULL;
    p->postal_code = NULL;
}

RQ_EXPORT 
void 
rq_address_clear(struct rq_address *p)
{
    if (p->street_address)
    {
        rq_street_address_free(p->street_address);
        p->street_address = NULL;
    }
    if (p->city)
    {
        RQ_FREE((char *)p->city);
        p->city = NULL;
    }
    if (p->state)
    {
        RQ_FREE((char *)p->state);
        p->state = NULL;
    }
    if (p->country)
    {
        RQ_FREE((char *)p->country);
        p->country = NULL;
    }
    if (p->postal_code)
    {
        RQ_FREE((char *)p->postal_code);
        p->postal_code = NULL;
    }
}

RQ_EXPORT
int 
rq_address_write_xml(struct rq_address *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<address>");

    if (p->street_address)
        rq_street_address_write_xml(p->street_address, output_stream);

    rq_stream_write_string(output_stream, "<city>");
    rq_stream_write_string(output_stream, p->city);
    rq_stream_write_string(output_stream, "</city>");
    rq_stream_write_string(output_stream, "<state>");
    rq_stream_write_string(output_stream, p->state);
    rq_stream_write_string(output_stream, "</state>");
    rq_stream_write_string(output_stream, "<country>");
    rq_stream_write_string(output_stream, p->country);
    rq_stream_write_string(output_stream, "</country>");
    rq_stream_write_string(output_stream, "<postalCode>");
    rq_stream_write_string(output_stream, p->postal_code);
    rq_stream_write_string(output_stream, "</postalCode>");

    rq_stream_write_string(output_stream, "</address>");

    return 0;
}

RQ_EXPORT
void 
rq_address_free(struct rq_address *p)
{
    rq_address_clear(p);
    RQ_FREE(p);
}
