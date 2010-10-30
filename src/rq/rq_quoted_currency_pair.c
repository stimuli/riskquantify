/*
** rq_quoted_currency_pair.c
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
#include "rq_quoted_currency_pair.h"
#include <stdlib.h>

RQ_EXPORT void
rq_quoted_currency_pair_init(struct rq_quoted_currency_pair *p)
{
    p->currency1 = NULL;
    p->currency2 = NULL;
    p->quote_basis = NULL;
}

RQ_EXPORT void
rq_quoted_currency_pair_clear(struct rq_quoted_currency_pair *p)
{
    if (p->currency1)
    {
       RQ_FREE((char *)p->currency1);
        p->currency1 = NULL;
    }
    if (p->currency2)
    {
       RQ_FREE((char *)p->currency2);
        p->currency2 = NULL;
    }
    if (p->quote_basis)
    {
       RQ_FREE((char *)p->quote_basis);
        p->quote_basis = NULL;
    }
}

RQ_EXPORT int
rq_quoted_currency_pair_write_xml(struct rq_quoted_currency_pair *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<quotedCurrencyPair>");

    rq_stream_write_string(output_stream, "<currency1>");
    rq_stream_write_string(output_stream, p->currency1);
    rq_stream_write_string(output_stream, "</currency1>");
    rq_stream_write_string(output_stream, "<currency2>");
    rq_stream_write_string(output_stream, p->currency2);
    rq_stream_write_string(output_stream, "</currency2>");
    rq_stream_write_string(output_stream, "<quoteBasis>");
    rq_stream_write_string(output_stream, p->quote_basis);
    rq_stream_write_string(output_stream, "</quoteBasis>");

    rq_stream_write_string(output_stream, "</quotedCurrencyPair>");

    return 0;
}

