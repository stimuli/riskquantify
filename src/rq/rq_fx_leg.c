/*
** rq_fx_leg.c
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
#include "rq_fx_leg.h"
#include <stdlib.h>

RQ_EXPORT struct rq_fx_leg *
rq_fx_leg_alloc()
{
    struct rq_fx_leg *fx_leg = RQ_MALLOC(sizeof(struct rq_fx_leg));
    rq_fx_leg_init(fx_leg);
    return fx_leg;
}

RQ_EXPORT void
rq_fx_leg_init(struct rq_fx_leg *p)
{
    rq_currency_flow_init(&p->exchanged_currency1);
    rq_currency_flow_init(&p->exchanged_currency2);
    p->value_date = 0;
    p->currency2_value_date = 0;
    rq_fx_rate_init(&p->exchange_rate);
    p->non_deliverable_forward = NULL;
    p->confirmation_sender_party_reference = NULL;
}

RQ_EXPORT void
rq_fx_leg_clear(struct rq_fx_leg *p)
{
    rq_currency_flow_clear(&p->exchanged_currency1);
    rq_currency_flow_clear(&p->exchanged_currency2);
    p->value_date = 0;
    p->currency2_value_date = 0;
    rq_fx_rate_clear(&p->exchange_rate);
    if (p->non_deliverable_forward)
    {
        rq_fx_cash_settlement_clear(p->non_deliverable_forward);
        p->non_deliverable_forward = NULL;
    }
    if (p->confirmation_sender_party_reference)
    {
       RQ_FREE((char *)p->confirmation_sender_party_reference);
        p->confirmation_sender_party_reference = NULL;
    }
}

RQ_EXPORT int
rq_fx_leg_write_xml(struct rq_fx_leg *p, rq_stream_t output_stream)
{
    char buf[30];

    rq_stream_write_string(output_stream, "<fxLeg>");

    rq_currency_flow_write_xml(&p->exchanged_currency1, output_stream);
    rq_currency_flow_write_xml(&p->exchanged_currency2, output_stream);
    if (!p->currency2_value_date)
    {
        rq_stream_write_string(output_stream, "<valueDate>");
        rq_date_to_string(buf, "yyyy-mm-dd", p->value_date);
        rq_stream_write_string(output_stream, buf);
        rq_stream_write_string(output_stream, "</valueDate>");
    }
    else
    {
        rq_stream_write_string(output_stream, "<currency1ValueDate>");
        rq_date_to_string(buf, "yyyy-mm-dd", p->value_date);
        rq_stream_write_string(output_stream, buf);
        rq_stream_write_string(output_stream, "</currency1ValueDate>");
        rq_stream_write_string(output_stream, "<currency2ValueDate>");
        rq_date_to_string(buf, "yyyy-mm-dd", p->currency2_value_date);
        rq_stream_write_string(output_stream, buf);
        rq_stream_write_string(output_stream, "</currency2ValueDate>");
    }

    rq_fx_rate_write_xml(&p->exchange_rate, output_stream);
    if (p->non_deliverable_forward)
        rq_fx_cash_settlement_write_xml(p->non_deliverable_forward, output_stream);
    if (p->confirmation_sender_party_reference)
    {
        rq_stream_write_string(output_stream, "<confirmationSenderPartyReference href='");
        rq_stream_write_string(output_stream, p->confirmation_sender_party_reference);
        rq_stream_write_string(output_stream, "'/>");
    }

    rq_stream_write_string(output_stream, "</fxLeg>");

    return 0;
}

RQ_EXPORT void 
rq_fx_leg_free(struct rq_fx_leg *p)
{
    rq_fx_leg_clear(p);
    RQ_FREE(p);
}
