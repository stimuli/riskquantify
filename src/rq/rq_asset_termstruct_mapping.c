/*
** rq_asset_termstruct_mapping.c
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
#include "rq_asset_termstruct_mapping.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT rq_asset_termstruct_mapping_t
rq_asset_termstruct_mapping_alloc()
{
    struct rq_asset_termstruct_mapping *atm = (struct rq_asset_termstruct_mapping *)RQ_CALLOC(1, sizeof(struct rq_asset_termstruct_mapping));
    return atm;
}

RQ_EXPORT void  
rq_asset_termstruct_mapping_free(rq_asset_termstruct_mapping_t atm)
{
    if (atm->asset_id)
        RQ_FREE((char *)atm->asset_id);
    if (atm->termstruct_id)
        RQ_FREE((char *)atm->termstruct_id);
    RQ_FREE(atm);
}

RQ_EXPORT int
rq_asset_termstruct_mapping_is_null(rq_asset_termstruct_mapping_t obj)
{
    return obj == NULL;
}

RQ_EXPORT void 
rq_asset_termstruct_mapping_set_asset_id(rq_asset_termstruct_mapping_t atm, const char *asset_id)
{
    if (atm->asset_id)
       RQ_FREE((char *)atm->asset_id);
    atm->asset_id = (const char *)RQ_STRDUP(asset_id);
}

RQ_EXPORT const char *
rq_asset_termstruct_mapping_get_asset_id(rq_asset_termstruct_mapping_t atm)
{
    return atm->asset_id;
}

RQ_EXPORT void 
rq_asset_termstruct_mapping_set_termstruct_type(rq_asset_termstruct_mapping_t atm, enum rq_termstruct_type termstruct_type)
{
    atm->termstruct_type = termstruct_type;
}

RQ_EXPORT enum rq_termstruct_type 
rq_asset_termstruct_mapping_get_termstruct_type(rq_asset_termstruct_mapping_t atm)
{
    return atm->termstruct_type;
}

RQ_EXPORT void 
rq_asset_termstruct_mapping_set_termstruct_id(rq_asset_termstruct_mapping_t atm, const char *termstruct_id)
{
    if (atm->termstruct_id)
       RQ_FREE((char *)atm->termstruct_id);
    atm->termstruct_id = (const char *)RQ_STRDUP(termstruct_id);
}

RQ_EXPORT const char *
rq_asset_termstruct_mapping_get_termstruct_id(rq_asset_termstruct_mapping_t atm)
{
    return atm->termstruct_id;
}
