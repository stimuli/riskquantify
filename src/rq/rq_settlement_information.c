/*
** rq_settlement_information.c
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
#include "rq_settlement_information.h"
#include <stdlib.h>

RQ_EXPORT void
rq_settlement_information_init(struct rq_settlement_information *p)
{
    p->standard_settlement_style = NULL;
    rq_settlement_instruction_init(&p->settlement_instruction);
}

RQ_EXPORT void
rq_settlement_information_clear(struct rq_settlement_information *p)
{
    if (p->standard_settlement_style)
    {
       RQ_FREE((char *)p->standard_settlement_style);
        p->standard_settlement_style = NULL;
    }
    rq_settlement_instruction_clear(&p->settlement_instruction);
}

RQ_EXPORT int
rq_settlement_information_write_xml(struct rq_settlement_information *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<settlementInformation>");

    rq_stream_write_string(output_stream, "<standardSettlementStyle>");
    rq_stream_write_string(output_stream, p->standard_settlement_style);
    rq_stream_write_string(output_stream, "</standardSettlementStyle>");
    rq_settlement_instruction_write_xml(&p->settlement_instruction, output_stream);

    rq_stream_write_string(output_stream, "</settlementInformation>");

    return 0;
}

RQ_EXPORT void 
rq_settlement_information_free(struct rq_settlement_information *p)
{
    rq_settlement_information_clear(p);
    free(p);
}
