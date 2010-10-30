/*
** rq_fx_leg.h
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
#ifndef rq_fx_leg_h
#define rq_fx_leg_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_fx_cash_settlement.h"
#include "rq_fx_rate.h"
#include "rq_currency_flow.h"
#include "rq_stream.h"

/* -- structures --------------------------------------------------- */
/** either value_date or currency1_value_date and currency2_value_date */
struct rq_fx_leg {
	struct rq_currency_flow exchanged_currency1;
	struct rq_currency_flow exchanged_currency2;
	rq_date value_date;
	rq_date currency2_value_date;
	struct rq_fx_rate exchange_rate;
	struct rq_fx_cash_settlement *non_deliverable_forward;
	const char *confirmation_sender_party_reference;
};

/* -- prototypes -------------------------------------------------- */
/**
 * Allocate a new fx leg structure 
 */
RQ_EXPORT struct rq_fx_leg *rq_fx_leg_alloc();

/**
 * Write an XML representation of the FX Leg structure
 */
RQ_EXPORT int rq_fx_leg_write_xml(struct rq_fx_leg *p, rq_stream_t output_stream);

/**
 * Initialize an fx leg structure
 */
RQ_EXPORT void rq_fx_leg_init(struct rq_fx_leg *p);

/**
 * Clear an fx leg structure
 */
RQ_EXPORT void rq_fx_leg_clear(struct rq_fx_leg *p);

/**
 * Free an fx leg structure
 */
RQ_EXPORT void rq_fx_leg_free(struct rq_fx_leg *p);

#endif
