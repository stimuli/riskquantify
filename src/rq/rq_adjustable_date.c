/*
** rq_adjustable_date.c
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
#include "rq_adjustable_date.h"
#include <stdlib.h>

RQ_EXPORT
void 
rq_adjustable_date_init(struct rq_adjustable_date *ad)
{
    ad->unadjusted_date = 0;
    rq_business_day_adjustments_init(&ad->date_adjustments);
}

RQ_EXPORT 
void 
rq_adjustable_date_clear(struct rq_adjustable_date *ad)
{
    ad->unadjusted_date = 0;
    rq_business_day_adjustments_clear(&ad->date_adjustments);
}

RQ_EXPORT
void  
rq_adjustable_date_free(struct rq_adjustable_date *ad)
{
    rq_business_day_adjustments_clear(&ad->date_adjustments);
    RQ_FREE(ad);
}

RQ_EXPORT
int 
rq_adjustable_date_write_xml(struct rq_adjustable_date *p, rq_stream_t output_stream)
{
    char buf[30];

    rq_stream_write_string(output_stream, "<adjustableDate>");

    rq_stream_write_string(output_stream, "<unadjustedDate>");
    rq_date_to_string(buf, "yyyy-mm-dd", p->unadjusted_date);
    rq_stream_write_string(output_stream, buf);
    rq_stream_write_string(output_stream, "</unadjustedDate>");
    rq_business_day_adjustments_write_xml(&p->date_adjustments, output_stream);

    rq_stream_write_string(output_stream, "</adjustableDate>");

    return 0;
}

