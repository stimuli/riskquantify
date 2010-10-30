/*
** rq_side_rate.c
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
#include "rq_side_rate.h"
#include <stdlib.h>

RQ_EXPORT void
rq_side_rate_init(struct rq_side_rate *p)
{
    p->currency = NULL;
    p->side_rate_basis = NULL;
    p->rate = 0;
    p->spot_rate = NULL;
    p->forward_points = NULL;
}

RQ_EXPORT void
rq_side_rate_clear(struct rq_side_rate *p)
{
    if (p->currency)
    {
        free((char *)p->currency);
        p->currency = NULL;
    }
    if (p->side_rate_basis)
    {
        free((char *)p->side_rate_basis);
        p->side_rate_basis = NULL;
    }
    p->rate = 0;
    if (p->spot_rate)
    {
        free((char *)p->spot_rate);
        p->spot_rate = NULL;
    }
    if (p->forward_points)
    {
        free((char *)p->forward_points);
        p->forward_points = NULL;
    }
}

RQ_EXPORT int
rq_side_rate_write_xml(struct rq_side_rate *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<sideRate>");

    rq_stream_write_string(output_stream, "<currency>");
    rq_stream_write_string(output_stream, p->currency);
    rq_stream_write_string(output_stream, "</currency>");
    rq_stream_write_string(output_stream, "<sideRateBasis>");
    rq_stream_write_string(output_stream, p->side_rate_basis);
    rq_stream_write_string(output_stream, "</sideRateBasis>");
    rq_stream_write_string(output_stream, "<rate>");
    rq_stream_write_double_as_string(output_stream, p->rate, 0);
    rq_stream_write_string(output_stream, "</rate>");
    rq_stream_write_string(output_stream, "<spotRate>");
    rq_stream_write_string(output_stream, p->spot_rate);
    rq_stream_write_string(output_stream, "</spotRate>");
    rq_stream_write_string(output_stream, "<forwardPoints>");
    rq_stream_write_string(output_stream, p->forward_points);
    rq_stream_write_string(output_stream, "</forwardPoints>");

    rq_stream_write_string(output_stream, "</sideRate>");

    return 0;
}

RQ_EXPORT void
rq_side_rate_free(struct rq_side_rate *p)
{
    rq_side_rate_clear(p);
    free(p);
}
