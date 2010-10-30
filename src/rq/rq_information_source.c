/*
** rq_information_source.c
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
#include "rq_information_source.h"
#include <stdlib.h>

RQ_EXPORT void
rq_information_source_init(struct rq_information_source *p)
{
    p->rate_source = NULL;
    p->rate_source_page = NULL;
    p->rate_source_page_heading = NULL;
}

RQ_EXPORT void
rq_information_source_clear(struct rq_information_source *p)
{
    if (p->rate_source)
    {
       RQ_FREE((char *)p->rate_source);
        p->rate_source = NULL;
    }
    if (p->rate_source_page)
    {
       RQ_FREE((char *)p->rate_source_page);
        p->rate_source_page = NULL;
    }
    if (p->rate_source_page_heading)
    {
       RQ_FREE((char *)p->rate_source_page_heading);
        p->rate_source_page_heading = NULL;
    }
}

RQ_EXPORT int
rq_information_source_write_xml(struct rq_information_source *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<informationSource>");

    rq_stream_write_string(output_stream, "<rateSource>");
    rq_stream_write_string(output_stream, p->rate_source);
    rq_stream_write_string(output_stream, "</rateSource>");
    rq_stream_write_string(output_stream, "<rateSourcePage>");
    rq_stream_write_string(output_stream, p->rate_source_page);
    rq_stream_write_string(output_stream, "</rateSourcePage>");
    rq_stream_write_string(output_stream, "<rateSourcePageHeading>");
    rq_stream_write_string(output_stream, p->rate_source_page_heading);
    rq_stream_write_string(output_stream, "</rateSourcePageHeading>");

    rq_stream_write_string(output_stream, "</informationSource>");

    return 0;
}

