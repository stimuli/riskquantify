/*
** rq_assetflow.c
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
/* -- includes ---------------------------------------------------- */
#include "rq_assetflow.h"
#include <stdlib.h>
#include <string.h>

/* -- code -------------------------------------------------------- */
RQ_EXPORT rq_assetflow_t 
rq_assetflow_alloc()
{
    struct rq_assetflow *assetflow = (struct rq_assetflow *)RQ_CALLOC(
        1,
        sizeof(struct rq_assetflow)
        );
    return assetflow;
}

RQ_EXPORT void 
rq_assetflow_free(rq_assetflow_t assetflow)
{
    if (assetflow->asset_id)
        RQ_FREE((char *)assetflow->asset_id);

    RQ_FREE(assetflow);
}

RQ_EXPORT const char *
rq_assetflow_get_asset_id(rq_assetflow_t assetflow)
{
    return assetflow->asset_id;
}

RQ_EXPORT void 
rq_assetflow_set_asset_id(rq_assetflow_t assetflow, const char *asset_id)
{
    if (assetflow->asset_id)
        RQ_FREE((char *)assetflow->asset_id);

    assetflow->asset_id = RQ_STRDUP(asset_id);
}

RQ_EXPORT rq_date 
rq_assetflow_get_payment_date(rq_assetflow_t assetflow)
{
    return assetflow->payment_date;
}

RQ_EXPORT void 
rq_assetflow_set_payment_date(rq_assetflow_t assetflow, rq_date payment_date)
{
    assetflow->payment_date = payment_date;
}

RQ_EXPORT double 
rq_assetflow_get_amount(rq_assetflow_t assetflow)
{
    return assetflow->amount;
}

RQ_EXPORT void 
rq_assetflow_set_amount(rq_assetflow_t assetflow, double amount)
{
    assetflow->amount = amount;
}

RQ_EXPORT int
rq_assetflow_is_null(rq_assetflow_t obj)
{
    return obj == NULL;
}
