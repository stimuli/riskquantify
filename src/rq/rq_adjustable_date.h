/**
 * @file 
 * 
 * From FpML version 3 reference:
 * An entity for defining a date that shall be subject to adjustment
 * if it would otherwise fall on a day that is not a business day in
 * the specified business centers, together with the convention for
 * adjusting the date.  
 */
/*
** rq_adjustable_date.h
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
#ifndef rq_adjustable_date_h
#define rq_adjustable_date_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_date.h"
#include "rq_business_day_adjustments.h"
#include "rq_stream.h"

/* -- structures --------------------------------------------------- */
struct rq_adjustable_date {
    rq_date unadjusted_date;
    struct rq_business_day_adjustments date_adjustments;
};

/* -- prototypes --------------------------------------------------- */
RQ_EXPORT void rq_adjustable_date_init(struct rq_adjustable_date *ad);

RQ_EXPORT void rq_adjustable_date_clear(struct rq_adjustable_date *ad);

RQ_EXPORT void rq_adjustable_date_free(struct rq_adjustable_date *ad);

RQ_EXPORT int rq_adjustable_date_write_xml(struct rq_adjustable_date *p, rq_stream_t output_stream);

#endif
