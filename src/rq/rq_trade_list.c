/*
** rq_trade_list.c
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
#include "rq_trade_list.h"
#include "rq_array.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_trade_list_t
rq_trade_list_alloc()
{
    struct rq_trade_list *trade_list = (struct rq_trade_list *)RQ_MALLOC(sizeof(struct rq_trade_list));

    trade_list->trade_list = rq_array_alloc(NULL);

    return trade_list;
}

RQ_EXPORT void  
rq_trade_list_free(rq_trade_list_t tl)
{
    rq_array_free(tl->trade_list);

    RQ_FREE(tl);
}

RQ_EXPORT int
rq_trade_list_is_null(rq_trade_list_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_trade_list_add(rq_trade_list_t tl, rq_trade_t trade)
{
    rq_array_push_back(tl->trade_list, trade);
}

RQ_EXPORT unsigned int 
rq_trade_list_size(rq_trade_list_t tl)
{
    return rq_array_size(tl->trade_list);
}

RQ_EXPORT rq_trade_t 
rq_trade_list_get_at(rq_trade_list_t tl, unsigned int offset)
{
    return (rq_trade_t) rq_array_get_at(tl->trade_list, offset);
}
