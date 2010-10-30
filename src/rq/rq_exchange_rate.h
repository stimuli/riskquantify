/**
 * \file rq_exchange_rate.h
 * \author Brett Hutley
 *
 * \brief The rq_exchange_rate object implements an exchange rate.
 */
/*
** rq_exchange_rate.h
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
#ifndef rq_exchange_rate_h
#define rq_exchange_rate_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#define CCY_CODE_EQUAL(a, b) (strcmp(a, b) == 0)

typedef struct rq_exchange_rate_key {
    const char* ccy_code_from;
    const char* ccy_code_to;
} * rq_exchange_rate_key_t;

typedef struct rq_exchange_rate {
    rq_exchange_rate_key_t key;
    char ccy_code_from[4];
    char ccy_code_to[4];
    double exchange_rate;
} *rq_exchange_rate_t;


/** Test whether the rq_exchange_rate is NULL */
RQ_EXPORT int rq_exchange_rate_is_null(rq_exchange_rate_t obj);

/**
 * Build an exchange rate object from the two currency codes and the
 * exchange rate.  
 */
RQ_EXPORT rq_exchange_rate_t 
rq_exchange_rate_build(
    const char *ccy_code_from,
    const char *ccy_code_to,
    double exchange_rate
    );

/**
 * Free an allocated exchange rate object
 */
RQ_EXPORT void rq_exchange_rate_free(rq_exchange_rate_t er);


/**
 * Compare exchange rate objects for storage in indexed containers
 */
RQ_EXPORT int 
rq_exchange_rate_cmp(const void *lhs, const void *rhs);

/**
 * Clone an exchange rate
 */
RQ_EXPORT rq_exchange_rate_t rq_exchange_rate_clone(const rq_exchange_rate_t er);

/* -- getters ----------------------------------------------------- */
/**
 * Get the exchange rate key
 */
RQ_EXPORT rq_exchange_rate_key_t rq_exchange_rate_get_key(const rq_exchange_rate_t er);

/**
 * Get the currency code from
 */
RQ_EXPORT const char *rq_exchange_rate_get_ccy_code_from(const rq_exchange_rate_t er);

/**
 * Get the currency code to
 */
RQ_EXPORT const char *rq_exchange_rate_get_ccy_code_to(const rq_exchange_rate_t er);

/**
 * Get the exchange rate from the exchange rate object
 */
RQ_EXPORT double rq_exchange_rate_get_exchange_rate(const rq_exchange_rate_t er);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
