/**
 * \file rq_currency_flow.h
 * \author Brett Hutley
 *
 * \brief A structure representing an FpML currency flow
 */
/*
** rq_currency_flow.h
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
#ifndef rq_currency_flow_h
#define rq_currency_flow_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_money.h"
#include "rq_adjustable_date.h"
#include "rq_settlement_information.h"
#include "rq_stream.h"

/* -- structures --------------------------------------------------- */
struct rq_currency_flow {
	const char *payer_party_id; /**< The payer party ID */
	const char *receiver_party_id; /**< The receiver party ID */
	struct rq_money payment_amount; /**< The amount to be payed/received */
	struct rq_adjustable_date *payment_date; /**< The unadjusted date the payment occurs on */
	rq_date adjusted_payment_date; /**< The date the payment occurs on, after any adjustments */
	struct rq_settlement_information *settlement_information; /**< Any settlement information for the currency flow */
};

/* -- prototypes -------------------------------------------------- */
/**
 * Write the currency flow to an XML stream
 */
RQ_EXPORT int rq_currency_flow_write_xml(struct rq_currency_flow *p, rq_stream_t output_stream);

/**
 * Initialize the currency flow structure (this doesn't free any memory, 
 * merely zeros the structure).
 */
RQ_EXPORT void rq_currency_flow_init(struct rq_currency_flow *p);

/**
 * Clear the currency flow structure, freeing any memory that has been 
 * allocated
 */
RQ_EXPORT void rq_currency_flow_clear(struct rq_currency_flow *p);

/**
 * Free all memory allocated for the currency flow structure
 */
RQ_EXPORT void rq_currency_flow_free(struct rq_currency_flow *p);


#endif
