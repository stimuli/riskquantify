/**
 * \file rq_trade_list.h
 * \author Brett Hutley
 *
 * \brief A structure representing a list of trades. This object WON'T
 * delete it's trades upon destruction!
 */
/*
** rq_trade_list.h
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
#ifndef rq_trade_list_h
#define rq_trade_list_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_trade.h"
#include "rq_array.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/** The trade_list structure.
 */
typedef struct rq_trade_list {
    rq_array_t trade_list;
} *rq_trade_list_t;


/** Test whether the rq_trade_list is NULL */
RQ_EXPORT int rq_trade_list_is_null(rq_trade_list_t obj);

/**
 * Allocate a new trade_list
 */
RQ_EXPORT rq_trade_list_t rq_trade_list_alloc();

/**
 * Free an object of type rq_trade_list
 */
RQ_EXPORT void rq_trade_list_free(rq_trade_list_t trade_list);

/**
 * Add a trade to the trade list
 */
RQ_EXPORT void rq_trade_list_add(rq_trade_list_t trade_list, rq_trade_t trade);

/**
 * Get the number of trades in the list
 */
RQ_EXPORT unsigned int rq_trade_list_size(rq_trade_list_t trade_list);

/**
 * Get the trade at a particular zero-based offset in the list
 */
RQ_EXPORT rq_trade_t rq_trade_list_get_at(rq_trade_list_t trade_list, unsigned int offset);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
