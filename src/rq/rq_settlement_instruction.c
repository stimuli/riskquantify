/*
** rq_settlement_instruction.c
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
#include "rq_settlement_instruction.h"
#include <stdlib.h>

RQ_EXPORT void
rq_settlement_instruction_init(struct rq_settlement_instruction *p)
{
    p->settlement_method = NULL;
    rq_routing_init(&p->correspondent_information);
    rq_intermediary_information_init(&p->intermediary_information);
    rq_routing_init(&p->beneficiary_bank);
    rq_routing_init(&p->beneficiary);
    rq_split_settlement_init(&p->split_settlement);
}

RQ_EXPORT void
rq_settlement_instruction_clear(struct rq_settlement_instruction *p)
{
    if (p->settlement_method)
    {
        RQ_FREE((char *)p->settlement_method);
        p->settlement_method = NULL;
    }
    rq_routing_clear(&p->correspondent_information);
    rq_intermediary_information_clear(&p->intermediary_information);
    rq_routing_clear(&p->beneficiary_bank);
    rq_routing_clear(&p->beneficiary);
    rq_split_settlement_clear(&p->split_settlement);
}

RQ_EXPORT int
rq_settlement_instruction_write_xml(struct rq_settlement_instruction *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<settlementInstruction>");

    rq_stream_write_string(output_stream, "<settlementMethod>");
    rq_stream_write_string(output_stream, p->settlement_method);
    rq_stream_write_string(output_stream, "</settlementMethod>");
    rq_routing_write_xml(&p->correspondent_information, output_stream);
    rq_intermediary_information_write_xml(&p->intermediary_information, output_stream);
    rq_routing_write_xml(&p->beneficiary_bank, output_stream);
    rq_routing_write_xml(&p->beneficiary, output_stream);
    rq_split_settlement_write_xml(&p->split_settlement, output_stream);

    rq_stream_write_string(output_stream, "</settlementInstruction>");

    return 0;
}

