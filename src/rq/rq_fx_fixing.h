/*
** rq_fx_fixing.h
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
#ifndef rq_fx_fixing_h
#define rq_fx_fixing_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_date.h"
#include "rq_business_center_time.h"
#include "rq_information_source.h"
#include "rq_quoted_currency_pair.h"
#include "rq_stream.h"

/* -- structures --------------------------------------------------- */
struct rq_fx_fixing {
	struct rq_quoted_currency_pair quoted_currency_pair;
	struct rq_information_source primary_rate_source;
	struct rq_information_source secondary_rate_source;
	rq_date fixing_date;
	struct rq_business_center_time fixing_time;
};

/* -- prototypes -------------------------------------------------- */
RQ_EXPORT int rq_fx_fixing_write_xml(struct rq_fx_fixing *p, rq_stream_t output_stream);

RQ_EXPORT void rq_fx_fixing_init(struct rq_fx_fixing *p);

RQ_EXPORT void rq_fx_fixing_clear(struct rq_fx_fixing *p);


#endif
