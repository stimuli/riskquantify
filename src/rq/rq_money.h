/*
** rq_money.h
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
#ifndef rq_money_h
#define rq_money_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_stream.h"

/* -- structures --------------------------------------------------- */

struct rq_money {
    const char *currency;
    double amount;
};

/* -- prototypes --------------------------------------------------- */
/**
 * zero the memory in the money structure
 *
 * @param	money	The money structure to initialize
 */
RQ_EXPORT void rq_money_init(struct rq_money *money);

/**
 * clear the memory managed by the money structure.
 *
 * @param	money	the money structure to clear.
 */
RQ_EXPORT void rq_money_clear(struct rq_money *money);


/**
 * Fill the members of the money structure. If a currency has already
 * been assigned to the money structure it will be freed. A copy will
 * be made of currency parameter.
 *
 * @param	money	The money structure to fill
 * @param	currency	The currency code
 * @param amount The amount 
 */
RQ_EXPORT void rq_money_fill(struct rq_money *money, const char *currency, double amount);

/**
 * Write out the money structure to an XML file.
 */
RQ_EXPORT int rq_money_write_xml(struct rq_money *p, rq_stream_t output_stream);

/**
 * Free a previously allocated money structure
 */
RQ_EXPORT void rq_money_free(struct rq_money *p);

#endif
