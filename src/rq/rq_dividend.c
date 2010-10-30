/*
** rq_dividend.c
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
#include "rq_dividend.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_dividend_t
rq_dividend_alloc()
{
    struct rq_dividend *d = (struct rq_dividend *)RQ_CALLOC(1, sizeof(struct rq_dividend));
    return d;
}

RQ_EXPORT rq_dividend_t 
rq_dividend_build(rq_date date, double value, const char *currency)
{
    struct rq_dividend *d = (struct rq_dividend *)RQ_MALLOC(sizeof(struct rq_dividend));
    d->date = date;
    d->value = value;
	d->currency = currency;
    return d;
}

RQ_EXPORT void  
rq_dividend_free(rq_dividend_t d)
{
    RQ_FREE(d);
}

RQ_EXPORT int
rq_dividend_is_null(rq_dividend_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_date 
rq_dividend_get_date(rq_dividend_t d)
{
    return d->date;
}

RQ_EXPORT void 
rq_dividend_set_date(rq_dividend_t d, rq_date date)
{
    d->date = date;
}

RQ_EXPORT double 
rq_dividend_get_value(rq_dividend_t d)
{
    return d->value;
}

RQ_EXPORT void 
rq_dividend_set_value(rq_dividend_t d, double value)
{
    d->value = value;
}

RQ_EXPORT const char *
rq_dividend_get_currency(rq_dividend_t d)
{
    return d->currency;
}

RQ_EXPORT void 
rq_dividend_set_currency(rq_dividend_t d, const char *currency)
{
    if (d->currency)
        RQ_FREE((char *)d->currency);
    d->currency = currency;
}

RQ_EXPORT rq_dividend_t 
rq_dividend_clone(rq_dividend_t d)
{
    struct rq_dividend *d_clone = (struct rq_dividend *)RQ_MALLOC(sizeof(struct rq_dividend));
    d_clone->date = d->date;
    d_clone->value = d->value;
	d_clone->currency = d->currency;

    return d_clone;
}
