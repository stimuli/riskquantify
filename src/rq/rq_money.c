/*
** rq_money.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
#include "rq_money.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT void 
rq_money_init(struct rq_money *money)
{
    money->currency = NULL;
    money->amount = 0;
}

RQ_EXPORT void 
rq_money_clear(struct rq_money *money)
{
    if (money->currency)
    {
       RQ_FREE((char *)money->currency);
        money->currency = NULL;
    }

    money->amount = 0;
}

RQ_EXPORT void 
rq_money_fill(struct rq_money *money, const char *currency, double amount)
{
    if (money->currency)
       RQ_FREE((char *)money->currency);
    money->currency = (const char *)RQ_STRDUP(currency);
    money->amount = amount;
}

RQ_EXPORT int
rq_money_write_xml(struct rq_money *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<money>");

    rq_stream_write_string(output_stream, "<currency>");
    rq_stream_write_string(output_stream, p->currency);
    rq_stream_write_string(output_stream, "</currency>");
    rq_stream_write_string(output_stream, "<amount>");
    rq_stream_write_double_as_string(output_stream, p->amount, 0);
    rq_stream_write_string(output_stream, "</amount>");

    rq_stream_write_string(output_stream, "</money>");

    return 0;
}

RQ_EXPORT void 
rq_money_free(struct rq_money *p)
{
    rq_money_clear(p);
   RQ_FREE(p);
}
