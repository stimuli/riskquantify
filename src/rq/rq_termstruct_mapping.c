/*
** rq_termstruct_mapping.c
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
#include "rq_termstruct_mapping.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_termstruct_mapping_t
rq_termstruct_mapping_alloc()
{
    rq_termstruct_mapping_t termstruct_mapping = (rq_termstruct_mapping_t)
        RQ_CALLOC(1, sizeof(struct rq_termstruct_mapping));
    return termstruct_mapping;
}

RQ_EXPORT rq_termstruct_mapping_t
rq_termstruct_mapping_build(
    const char *asset_id,
    const char *termstruct_group_id,
    const char *curve_id
    )
{
    rq_termstruct_mapping_t termstruct_mapping = (rq_termstruct_mapping_t)
        RQ_MALLOC(sizeof(struct rq_termstruct_mapping));

    termstruct_mapping->termstruct_group_id = RQ_STRDUP(termstruct_group_id);
    termstruct_mapping->asset_id = RQ_STRDUP(asset_id);
    termstruct_mapping->curve_id = RQ_STRDUP(curve_id);

    return termstruct_mapping;
}

RQ_EXPORT void  
rq_termstruct_mapping_free(rq_termstruct_mapping_t termstruct_mapping)
{
    if (termstruct_mapping->asset_id)
        RQ_FREE((char *)termstruct_mapping->asset_id);
    if (termstruct_mapping->termstruct_group_id)
        RQ_FREE((char *)termstruct_mapping->termstruct_group_id);
    if (termstruct_mapping->curve_id)
        RQ_FREE((char *)termstruct_mapping->curve_id);
    RQ_FREE(termstruct_mapping);
}

RQ_EXPORT int
rq_termstruct_mapping_is_null(rq_termstruct_mapping_t obj)
{
    return RQ_IS_NULL(obj);
}
