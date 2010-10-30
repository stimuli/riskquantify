/*
** rq_currency_flow.c
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
#include "rq_currency_flow.h"
#include <stdlib.h>

RQ_EXPORT void
rq_currency_flow_init(struct rq_currency_flow *p)
{
    p->payer_party_id = NULL;
    p->receiver_party_id = NULL;
    rq_money_init(&p->payment_amount);
    p->payment_date = NULL;
    p->adjusted_payment_date = 0;
    p->settlement_information = NULL;
}

RQ_EXPORT void
rq_currency_flow_clear(struct rq_currency_flow *p)
{
    if (p->payer_party_id)
    {
       RQ_FREE((char *)p->payer_party_id);
        p->payer_party_id = NULL;
    }
    if (p->receiver_party_id)
    {
       RQ_FREE((char *)p->receiver_party_id);
        p->receiver_party_id = NULL;
    }
    rq_money_clear(&p->payment_amount);
    if (p->payment_date)
    {
        rq_adjustable_date_free(p->payment_date);
        p->payment_date = NULL;
    }
    p->adjusted_payment_date = 0;
    if (p->settlement_information)
    {
        rq_settlement_information_free(p->settlement_information);
        p->settlement_information = NULL;
    }
}

RQ_EXPORT int
rq_currency_flow_write_xml(struct rq_currency_flow *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<currencyFlow>");

    if (p->payer_party_id)
    {
        rq_stream_write_string(output_stream, "<payerParty href=\"");
        rq_stream_write_string(output_stream, p->payer_party_id);
        rq_stream_write_string(output_stream, "\"/>");
    }

    if (p->receiver_party_id)
    {
        rq_stream_write_string(output_stream, "<receiverParty href=\"");
        rq_stream_write_string(output_stream, p->receiver_party_id);
        rq_stream_write_string(output_stream, "\"/>");
    }

    rq_money_write_xml(&p->payment_amount, output_stream);

    if (p->payment_date)
        rq_adjustable_date_write_xml(p->payment_date, output_stream);

    if (p->adjusted_payment_date > 0)
    {
        char buf[50];
        rq_stream_write_string(output_stream, "<adjustedPaymentDate>");
        rq_date_to_string(buf, "yyyy-mm-dd", p->adjusted_payment_date);
        rq_stream_write_string(output_stream, buf);
        rq_stream_write_string(output_stream, "</adjustedPaymentDate>");
    }

    if (p->settlement_information)
        rq_settlement_information_write_xml(p->settlement_information, output_stream);

    rq_stream_write_string(output_stream, "</currencyFlow>");

    return 0;
}

RQ_EXPORT void
rq_currency_flow_free(struct rq_currency_flow *p)
{
    rq_currency_flow_clear(p);
   RQ_FREE(p);
}
