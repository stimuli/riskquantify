/*
** rq_side_rates.c
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
#include "rq_side_rates.h"
#include <stdlib.h>

RQ_EXPORT void
rq_side_rates_init(struct rq_side_rates *p)
{
    p->base_currency = NULL;
    rq_side_rate_init(&p->currency1_side_rate);
    rq_side_rate_init(&p->currency2_side_rate);
}

RQ_EXPORT void
rq_side_rates_clear(struct rq_side_rates *p)
{
    if (p->base_currency)
    {
        RQ_FREE((char *)p->base_currency);
        p->base_currency = NULL;
    }
    rq_side_rate_clear(&p->currency1_side_rate);
    rq_side_rate_clear(&p->currency2_side_rate);
}

RQ_EXPORT int
rq_side_rates_write_xml(struct rq_side_rates *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<sideRates>");

    rq_stream_write_string(output_stream, "<baseCurrency>");
    rq_stream_write_string(output_stream, p->base_currency);
    rq_stream_write_string(output_stream, "</baseCurrency>");
    rq_side_rate_write_xml(&p->currency1_side_rate, output_stream);
    rq_side_rate_write_xml(&p->currency2_side_rate, output_stream);

    rq_stream_write_string(output_stream, "</sideRates>");

    return 0;
}

RQ_EXPORT void 
rq_side_rates_free(struct rq_side_rates *p)
{
    rq_side_rates_clear(p);
    free(p);
}
