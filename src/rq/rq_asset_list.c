/*
** rq_asset_list.c
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
#include "rq_asset_list.h"
#include "rq_array.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_asset_list_t
rq_asset_list_alloc()
{
    struct rq_asset_list *asset_list = (struct rq_asset_list *)RQ_MALLOC(sizeof(struct rq_asset_list));
    asset_list->assets = rq_array_alloc(NULL);

    return asset_list;
}

RQ_EXPORT void  
rq_asset_list_free(rq_asset_list_t al)
{
    rq_array_free(al->assets);
    RQ_FREE(al);
}

RQ_EXPORT int
rq_asset_list_is_null(rq_asset_list_t obj)
{
    return obj == NULL;
}

RQ_EXPORT unsigned int 
rq_asset_list_size(rq_asset_list_t al)
{
    return rq_array_size(al->assets);
}

RQ_EXPORT rq_asset_t 
rq_asset_list_get_at(rq_asset_list_t al, unsigned int offset)
{
    return (rq_asset_t)rq_array_get_at(al->assets, offset);
}

RQ_EXPORT void
rq_asset_list_push_back(rq_asset_list_t al, rq_asset_t asset)
{
    rq_array_push_back(al->assets, asset);
}
