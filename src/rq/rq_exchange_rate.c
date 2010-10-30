/*
** rq_exchange_rate.c
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
#include "rq_exchange_rate.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_exchange_rate_t 
rq_exchange_rate_build(
    const char *ccy_code_from,
    const char *ccy_code_to,
    double exchange_rate
    )
{
    struct rq_exchange_rate *r = (struct rq_exchange_rate *)RQ_MALLOC(sizeof(struct rq_exchange_rate));
    r->key = (struct rq_exchange_rate_key *)RQ_MALLOC(sizeof(struct rq_exchange_rate_key));

    memcpy(r->ccy_code_from, ccy_code_from, 3);
    r->ccy_code_from[3] = '\0';

    memcpy(r->ccy_code_to, ccy_code_to, 3);
    r->ccy_code_to[3] = '\0';

    r->exchange_rate = exchange_rate;

	r->key->ccy_code_from = r->ccy_code_from;
	r->key->ccy_code_to = r->ccy_code_to;

    return r;
}

RQ_EXPORT void 
rq_exchange_rate_free(rq_exchange_rate_t r)
{
    RQ_FREE(r->key);
    RQ_FREE(r);
}

RQ_EXPORT int 
rq_exchange_rate_cmp(const void *lhs, const void *rhs)
{
    struct rq_exchange_rate_key *lkey = (struct rq_exchange_rate_key *)lhs;
    struct rq_exchange_rate_key *rkey = (struct rq_exchange_rate_key *)rhs;

	int from_cmp = strcmp(lkey->ccy_code_from, rkey->ccy_code_from);
	if (from_cmp != 0) return from_cmp;

	return strcmp(lkey->ccy_code_to, rkey->ccy_code_to);
}

RQ_EXPORT rq_exchange_rate_t 
rq_exchange_rate_clone(const rq_exchange_rate_t r)
{
    struct rq_exchange_rate *c = (struct rq_exchange_rate *)RQ_MALLOC(sizeof(struct rq_exchange_rate));
    c->key = (struct rq_exchange_rate_key *)RQ_MALLOC(sizeof(struct rq_exchange_rate_key));

    strcpy(c->ccy_code_from, r->ccy_code_from);
    strcpy(c->ccy_code_to, r->ccy_code_to);
    c->exchange_rate = r->exchange_rate;
    memcpy(c->key, r->key, sizeof(struct rq_exchange_rate_key));

    return c;
}

RQ_EXPORT rq_exchange_rate_key_t
rq_exchange_rate_get_key(const rq_exchange_rate_t r)
{
    return r->key;
}

RQ_EXPORT const char *
rq_exchange_rate_get_ccy_code_from(const rq_exchange_rate_t r)
{
    return r->ccy_code_from;
}

RQ_EXPORT const char *
rq_exchange_rate_get_ccy_code_to(const rq_exchange_rate_t r)
{
    return r->ccy_code_to;
}

RQ_EXPORT double
rq_exchange_rate_get_exchange_rate(const rq_exchange_rate_t r)
{
    return r->exchange_rate;
}

RQ_EXPORT int
rq_exchange_rate_is_null(rq_exchange_rate_t obj)
{
    return (obj == NULL);
}
