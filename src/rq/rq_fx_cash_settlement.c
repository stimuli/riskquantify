/*
** rq_fx_cash_settlement.c
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
#include "rq_fx_cash_settlement.h"
#include <stdlib.h>

RQ_EXPORT void
rq_fx_cash_settlement_init(struct rq_fx_cash_settlement *p)
{
    p->settlement_currency = NULL;
    rq_fx_fixing_init(&p->fixing);
}

RQ_EXPORT void
rq_fx_cash_settlement_clear(struct rq_fx_cash_settlement *p)
{
    if (p->settlement_currency)
    {
       RQ_FREE((char *)p->settlement_currency);
        p->settlement_currency = NULL;
    }
    rq_fx_fixing_clear(&p->fixing);
}

RQ_EXPORT int
rq_fx_cash_settlement_write_xml(struct rq_fx_cash_settlement *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<fxCashSettlement>");

    if (p->settlement_currency)
    {
        rq_stream_write_string(output_stream, "<settlementCurrency>");
        rq_stream_write_string(output_stream, p->settlement_currency);
        rq_stream_write_string(output_stream, "</settlementCurrency>");
    }

    rq_fx_fixing_write_xml(&p->fixing, output_stream);

    rq_stream_write_string(output_stream, "</fxCashSettlement>");

    return 0;
}

