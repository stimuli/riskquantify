/*
** rq_market_requirements.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#include "rq_market_requirements.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_market_requirements_t
rq_market_requirements_alloc()
{
    unsigned int i;
    struct rq_market_requirements *request = (struct rq_market_requirements *)
        RQ_MALLOC(sizeof(struct rq_market_requirements));
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
    {
        request->req_hdrs[i] = (struct rq_termstruct_req_hdr *)
            RQ_CALLOC(1, sizeof(struct rq_termstruct_req_hdr));
    }

    request->cur_spot_price_asset_num = 0;

    return request;
}

RQ_EXPORT void  
rq_market_requirements_free(rq_market_requirements_t request)
{
    unsigned int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        RQ_FREE(request->req_hdrs[i]);
    RQ_FREE(request);
}

RQ_EXPORT int
rq_market_requirements_is_null(rq_market_requirements_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void
rq_market_requirements_termstruct_add(rq_market_requirements_t request, enum rq_termstruct_type termstruct_type, const char *termstruct_group_id, const char *asset_id, rq_date maturity_date)
{
    strcpy(
        request->req_hdrs[termstruct_type]->requests[request->req_hdrs[termstruct_type]->num_requests].termstruct_group_id, 
        termstruct_group_id
        );
    strcpy(
        request->req_hdrs[termstruct_type]->requests[request->req_hdrs[termstruct_type]->num_requests].asset_id, 
        asset_id
        );
    request->req_hdrs[termstruct_type]->requests[request->req_hdrs[termstruct_type]->num_requests].maturity_date = maturity_date;
    request->req_hdrs[termstruct_type]->num_requests++;
}

RQ_EXPORT unsigned int
rq_market_requirements_termstruct_size(rq_market_requirements_t request, enum rq_termstruct_type termstruct_type)
{
    return request->req_hdrs[termstruct_type]->num_requests;
}

RQ_EXPORT struct rq_termstruct_req *
rq_market_requirements_termstruct_get_at(rq_market_requirements_t request, enum rq_termstruct_type termstruct_type, unsigned int offset)
{
    return &request->req_hdrs[termstruct_type]->requests[offset];
}

RQ_EXPORT void 
rq_market_requirements_clear(rq_market_requirements_t request)
{
    unsigned int i;
    for (i = 0; i < RQ_TERMSTRUCT_TYPE_MAX_ENUM; i++)
        request->req_hdrs[i]->num_requests = 0;

    request->cur_spot_price_asset_num = 0;
}

RQ_EXPORT void
rq_market_requirements_spot_price_asset_add(rq_market_requirements_t request, const char *asset_id)
{
    unsigned int i;

    for (i = 0; i < RQ_MAX_SPOT_PRICE_ASSET_NUM && i < request->cur_spot_price_asset_num; i++)
        if (!strcmp(request->spot_price_assets + (i * RQ_MAX_ASSET_ID_LEN), asset_id))
            return;

    if (i < RQ_MAX_SPOT_PRICE_ASSET_NUM)
    {
        strcpy(request->spot_price_assets + (request->cur_spot_price_asset_num * RQ_MAX_ASSET_ID_LEN), asset_id);
        request->cur_spot_price_asset_num++;
    }
}

RQ_EXPORT unsigned int
rq_market_requirements_spot_price_asset_size(rq_market_requirements_t request)
{
    return request->cur_spot_price_asset_num;
}

RQ_EXPORT const char *
rq_market_requirements_spot_price_asset_get_at(rq_market_requirements_t request, unsigned int offset)
{
    return request->spot_price_assets + (offset * RQ_MAX_ASSET_ID_LEN);
}
