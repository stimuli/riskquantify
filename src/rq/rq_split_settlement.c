/*
** rq_split_settlement.c
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
#include "rq_split_settlement.h"

void
rq_split_settlement_init(struct rq_split_settlement *p)
{
    rq_money_init(&p->split_settlement_amount);
    rq_routing_init(&p->beneficiary_bank);
    rq_routing_init(&p->beneficiary);
}

void
rq_split_settlement_clear(struct rq_split_settlement *p)
{
    rq_money_clear(&p->split_settlement_amount);
    rq_routing_clear(&p->beneficiary_bank);
    rq_routing_clear(&p->beneficiary);
}

int
rq_split_settlement_write_xml(struct rq_split_settlement *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<splitSettlement>");

    rq_money_write_xml(&p->split_settlement_amount, output_stream);
    rq_routing_write_xml(&p->beneficiary_bank, output_stream);
    rq_routing_write_xml(&p->beneficiary, output_stream);

    rq_stream_write_string(output_stream, "</splitSettlement>");

    return 0;
}

