/*
** rq_spot_price.c
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
#include "rq_spot_price.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_spot_price_t
rq_spot_price_alloc(const char *asset_id, double spot_price)
{
    struct rq_spot_price *sp = (struct rq_spot_price *)RQ_MALLOC(sizeof(struct rq_spot_price));
    sp->asset_id = (const char *)RQ_STRDUP(asset_id);
    sp->price = spot_price;

    return sp;
}

RQ_EXPORT void  
rq_spot_price_free(rq_spot_price_t sp)
{
    if (sp->asset_id)
        RQ_FREE((char *)sp->asset_id);
    RQ_FREE(sp);
}

RQ_EXPORT rq_spot_price_t 
rq_spot_price_clone(rq_spot_price_t sp)
{
    return rq_spot_price_alloc(
        sp->asset_id,
        sp->price
        );
}

RQ_EXPORT int
rq_spot_price_is_null(rq_spot_price_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT const char *
rq_spot_price_get_asset_id(rq_spot_price_t sp)
{
    return sp->asset_id;
}

RQ_EXPORT double 
rq_spot_price_get_price(rq_spot_price_t sp)
{
    return sp->price;
}

RQ_EXPORT void 
rq_spot_price_set_price(rq_spot_price_t sp, double price)
{
    sp->price = price;
}
