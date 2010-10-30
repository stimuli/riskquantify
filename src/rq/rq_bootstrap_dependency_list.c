/*
** rq_bootstrap_dependency_list.c
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
#include "rq_bootstrap_dependency_list.h"
#include "rq_error.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT int 
rq_bootstrap_dependency_list_is_null(rq_bootstrap_dependency_list_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_bootstrap_dependency_list_t 
rq_bootstrap_dependency_list_alloc()
{
    rq_bootstrap_dependency_list_t dl = (rq_bootstrap_dependency_list_t)RQ_CALLOC(1, sizeof(struct rq_bootstrap_dependency_list));
    return dl;
}

RQ_EXPORT void 
rq_bootstrap_dependency_list_init(rq_bootstrap_dependency_list_t bdl)
{
    bdl->curve_ids_need_freeing = 0;
    bdl->num_dependencies = 0;
}

RQ_EXPORT void 
rq_bootstrap_dependency_list_free(rq_bootstrap_dependency_list_t bdl)
{
    if (bdl->curve_ids_need_freeing)
    {
        unsigned short i;

        for (i = 0; i < bdl->num_dependencies; i++)
        {
            if (bdl->curve_ids[i])
                RQ_FREE((char *)bdl->curve_ids[i]);
        }
    }

    RQ_FREE(bdl);
}

RQ_EXPORT unsigned short
rq_bootstrap_dependency_list_size(rq_bootstrap_dependency_list_t bdl)
{
    return bdl->num_dependencies;
}

RQ_EXPORT rq_error_code 
rq_bootstrap_dependency_list_get_at(
    rq_bootstrap_dependency_list_t bdl, 
    unsigned int offset,
    enum rq_termstruct_type *termstruct_type,
    const char **curve_id
    )
{
    rq_error_code status = RQ_FAILED;
    if (offset < bdl->num_dependencies)
    {
        *termstruct_type = bdl->termstruct_types[offset];
        *curve_id = bdl->curve_ids[offset];
        status = RQ_OK;
    }

    return status;
}

RQ_EXPORT rq_error_code
rq_bootstrap_dependency_list_add(
    rq_bootstrap_dependency_list_t bdl, 
    enum rq_termstruct_type termstruct_type,
    const char *curve_id
    )
{
    rq_error_code status = RQ_FAILED;
    if (bdl->num_dependencies < RQ_MAX_BOOTSTRAP_DEPENDENCIES)
    {
        bdl->termstruct_types[bdl->num_dependencies] = termstruct_type;
        bdl->curve_ids[bdl->num_dependencies] = curve_id;
        bdl->num_dependencies++;
        status = RQ_OK;
    }

    return status;
}

RQ_EXPORT void
rq_bootstrap_dependency_list_clear(
    rq_bootstrap_dependency_list_t bdl
    )
{
    if (bdl->curve_ids_need_freeing)
    {
        unsigned short i;

        for (i = 0; i < bdl->num_dependencies; i++)
        {
            if (bdl->curve_ids[i])
                RQ_FREE((char *)bdl->curve_ids[i]);
        }
    }

    bdl->num_dependencies = 0;
}
