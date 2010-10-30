/*
** rq_termstruct.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include "rq_termstruct.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT void 
rq_termstruct_clear(struct rq_termstruct *termstruct)
{
    if (termstruct->termstruct_id)
    {
        RQ_FREE((char *)termstruct->termstruct_id);
        termstruct->termstruct_id = NULL;
    }

    if (termstruct->underlying_asset_id)
    {
        RQ_FREE((char *)termstruct->underlying_asset_id);
        termstruct->underlying_asset_id = NULL;
    }
}

RQ_EXPORT enum rq_termstruct_type 
rq_termstruct_get_type(struct rq_termstruct *termstruct)
{
    return termstruct->termstruct_type;
}

RQ_EXPORT void 
rq_termstruct_set_underlying_asset_id(struct rq_termstruct *termstruct, const char *asset_id)
{
    if (termstruct->underlying_asset_id)
    {
        RQ_FREE((char *)termstruct->underlying_asset_id);
    }
    termstruct->underlying_asset_id = RQ_STRDUP(asset_id);
}
