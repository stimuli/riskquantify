/*
** rq_business_center.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
/* -- includes ----------------------------------------------------- */
#include "rq_business_center.h"
#include <stdlib.h>
#include <string.h>


/* -- code --------------------------------------------------------- */
RQ_EXPORT rq_business_center_t 
rq_business_center_alloc()
{
    struct rq_business_center *bc = 
        (struct rq_business_center *)RQ_CALLOC(1, sizeof(struct rq_business_center));
    return bc;
}

RQ_EXPORT void 
rq_business_center_free(rq_business_center_t bc)
{
    if (bc->id)
        RQ_FREE((char *)bc->id);
    if (bc->region_id)
        RQ_FREE((char *)bc->region_id);
    RQ_FREE(bc);
}

RQ_EXPORT const char *
rq_business_center_get_id(rq_business_center_t bc)
{
    return bc->id;
}

RQ_EXPORT void 
rq_business_center_set_id(rq_business_center_t bc, const char *id)
{
    if (bc->id)
       RQ_FREE((char *)bc->id);
    bc->id = (const char *)RQ_STRDUP(id);
}

RQ_EXPORT const char *
rq_business_center_get_region_id(rq_business_center_t bc)
{
    return bc->region_id;
}

RQ_EXPORT void 
rq_business_center_set_region_id(rq_business_center_t bc, const char *region_id)
{
    if (bc->region_id)
       RQ_FREE((char *)bc->region_id);
    bc->region_id = (const char *)RQ_STRDUP(region_id);
}

RQ_EXPORT int
rq_business_center_is_null(rq_business_center_t obj)
{
    return (obj == NULL);
}
