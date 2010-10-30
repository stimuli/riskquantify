/*
** rq_business_day_adjustments.c
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
#include "rq_business_day_adjustments.h"
#include <stdlib.h>

RQ_EXPORT void 
rq_business_day_adjustments_init(struct rq_business_day_adjustments *bda)
{
    bda->business_day_convention = RQ_BUSINESS_DAY_CONVENTION_NONE;
}

RQ_EXPORT void 
rq_business_day_adjustments_clear(struct rq_business_day_adjustments *bda)
{
    bda->business_day_convention = RQ_BUSINESS_DAY_CONVENTION_NONE;
}

RQ_EXPORT int
rq_business_day_adjustments_write_xml(struct rq_business_day_adjustments *p, rq_stream_t output_stream)
{
    rq_stream_write_string(output_stream, "<businessDayAdjustments>");

    rq_stream_write_string(output_stream, "<businessDayConvention>");
    rq_stream_write_string(output_stream, rq_business_day_convention_to_string(p->business_day_convention));
    rq_stream_write_string(output_stream, "</businessDayConvention>");

    rq_stream_write_string(output_stream, "</businessDayAdjustments>");

    return 0;
}

