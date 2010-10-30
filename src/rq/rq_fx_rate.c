/*
** rq_fx_rate.c
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
#include "rq_fx_rate.h"
#include <stdlib.h>

RQ_EXPORT void
rq_fx_rate_init(struct rq_fx_rate *p)
{
    rq_quoted_currency_pair_init(&p->quoted_currency_pair);
    p->rate = 0;
    p->spot_rate = 0;
    p->forward_points = 0;
    p->side_rates = NULL;
}

RQ_EXPORT void
rq_fx_rate_clear(struct rq_fx_rate *p)
{
    rq_quoted_currency_pair_clear(&p->quoted_currency_pair);
    p->rate = 0;
    p->spot_rate = 0;
    p->forward_points = 0;
    if (p->side_rates)
    {
        rq_side_rates_free(p->side_rates);
        p->side_rates = NULL;
    }
}

RQ_EXPORT int
rq_fx_rate_write_xml(struct rq_fx_rate *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<fxRate>");

    rq_quoted_currency_pair_write_xml(&p->quoted_currency_pair, output_stream);
    rq_stream_write_string(output_stream, "<rate>");
    rq_stream_write_double_as_string(output_stream, p->rate, 0);
    rq_stream_write_string(output_stream, "</rate>");

    if (p->spot_rate != 0.0)
    {
        rq_stream_write_string(output_stream, "<spotRate>");
        rq_stream_write_double_as_string(output_stream, p->spot_rate, 0);
        rq_stream_write_string(output_stream, "</spotRate>");
    }

    if (p->forward_points != 0.0)
    {
        rq_stream_write_string(output_stream, "<forwardPoints>");
        rq_stream_write_double_as_string(output_stream, p->forward_points, 0);
        rq_stream_write_string(output_stream, "</forwardPoints>");
    }

    if (p->side_rates)
        rq_side_rates_write_xml(p->side_rates, output_stream);

    rq_stream_write_string(output_stream, "</fxRate>");

    return 0;
}

