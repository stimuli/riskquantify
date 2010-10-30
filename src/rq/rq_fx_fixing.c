/*
** rq_fx_fixing.c
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
#include "rq_fx_fixing.h"

RQ_EXPORT void
rq_fx_fixing_init(struct rq_fx_fixing *p)
{
    rq_quoted_currency_pair_init(&p->quoted_currency_pair);
    rq_information_source_init(&p->primary_rate_source);
    rq_information_source_init(&p->secondary_rate_source);
    p->fixing_date = 0;
    rq_business_center_time_init(&p->fixing_time);
}

RQ_EXPORT void
rq_fx_fixing_clear(struct rq_fx_fixing *p)
{
    rq_quoted_currency_pair_clear(&p->quoted_currency_pair);
    rq_information_source_clear(&p->primary_rate_source);
    rq_information_source_clear(&p->secondary_rate_source);
    p->fixing_date = 0;
    rq_business_center_time_clear(&p->fixing_time);
}

RQ_EXPORT int
rq_fx_fixing_write_xml(struct rq_fx_fixing *p, rq_stream_t output_stream)
{
    char buf[30];

    rq_stream_write_string(output_stream, "<fxFixing>");

    rq_quoted_currency_pair_write_xml(&p->quoted_currency_pair, output_stream);
    rq_information_source_write_xml(&p->primary_rate_source, output_stream);
    rq_information_source_write_xml(&p->secondary_rate_source, output_stream);
    rq_stream_write_string(output_stream, "<fixingDate>");
    rq_date_to_string(buf, "yyyy-mm-dd", p->fixing_date);
    rq_stream_write_string(output_stream, buf);
    rq_stream_write_string(output_stream, "</fixingDate>");
    rq_business_center_time_write_xml(&p->fixing_time, output_stream);

    rq_stream_write_string(output_stream, "</fxFixing>");

    return 0;
}

