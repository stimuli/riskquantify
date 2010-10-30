/*
** rq_system.c
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
#include "rq_system.h"
#include <stdlib.h>


RQ_EXPORT rq_system_t 
rq_system_alloc()
{
    struct rq_system *system = (struct rq_system *)RQ_CALLOC(
        1, 
        sizeof(struct rq_system)
        );

    system->object_mgr = rq_object_mgr_alloc();

    system->asset_mgr = rq_asset_mgr_alloc();
    system->bootstrap_config_mgr = rq_bootstrap_config_mgr_alloc();
    system->date_event_type_mgr = rq_type_id_mgr_alloc(RQ_ID_TYPE_USER_DEFINED);
    system->calendar_mgr = rq_calendar_mgr_alloc();
    system->object_schema_mgr = rq_object_schema_mgr_alloc();
    system->product_handler_mgr = rq_product_handler_mgr_alloc();
    system->product_termstruct_mapping_mgr = rq_product_termstruct_mapping_mgr_alloc();
    system->termstruct_mapping_mgr = rq_termstruct_mapping_mgr_alloc();

    rq_object_schema_mgr_init_object_schemas(system->object_schema_mgr);

    return system;
}

RQ_EXPORT void 
rq_system_free(rq_system_t system)
{
    rq_object_mgr_free(system->object_mgr);

    rq_asset_mgr_free(system->asset_mgr);
    rq_bootstrap_config_mgr_free(system->bootstrap_config_mgr);
    rq_type_id_mgr_free(system->date_event_type_mgr);
    rq_calendar_mgr_free(system->calendar_mgr);
    rq_object_schema_mgr_free(system->object_schema_mgr);
    rq_product_handler_mgr_free(system->product_handler_mgr);
    rq_product_termstruct_mapping_mgr_free(system->product_termstruct_mapping_mgr);
    rq_termstruct_mapping_mgr_free(system->termstruct_mapping_mgr);

    RQ_FREE(system);
}

RQ_EXPORT rq_object_mgr_t 
rq_system_get_object_mgr(rq_system_t system)
{
    return system->object_mgr;
}

RQ_EXPORT rq_asset_mgr_t 
rq_system_get_asset_mgr(const rq_system_t system)
{
    return system->asset_mgr;
}

RQ_EXPORT rq_bootstrap_config_mgr_t 
rq_system_get_bootstrap_config_mgr(const rq_system_t system)
{
    return system->bootstrap_config_mgr;
}

RQ_EXPORT rq_type_id_mgr_t 
rq_system_get_date_event_type_mgr(const rq_system_t system)
{
    return system->date_event_type_mgr;
}

RQ_EXPORT rq_calendar_mgr_t 
rq_system_get_calendar_mgr(const rq_system_t system)
{
    return system->calendar_mgr;
}

RQ_EXPORT int
rq_system_is_null(rq_system_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_object_schema_mgr_t 
rq_system_get_object_schema_mgr(const rq_system_t system)
{
    return system->object_schema_mgr;
}

RQ_EXPORT rq_product_handler_mgr_t 
rq_system_get_product_handler_mgr(const rq_system_t system)
{
    return system->product_handler_mgr;
}

RQ_EXPORT rq_product_termstruct_mapping_mgr_t 
rq_system_get_product_termstruct_mapping_mgr(const rq_system_t system)
{
    return system->product_termstruct_mapping_mgr;
}

RQ_EXPORT rq_termstruct_mapping_mgr_t
rq_system_get_termstruct_mapping_mgr(const rq_system_t system)
{
    return system->termstruct_mapping_mgr;
}
