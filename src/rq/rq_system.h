/**
 * \file rq_system.h
 * \author Brett Hutley
 *
 * \brief The rq_system object contains the various objects and
 * container classes in a financial system that are generally needed.
 */
/*
** rq_system.h
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
#ifndef rq_system_h
#define rq_system_h

#include "rq_config.h"
#include "rq_object.h"
#include "rq_defs.h"
#include "rq_asset_mgr.h"
#include "rq_bootstrap_config_mgr.h"
#include "rq_calendar_mgr.h"
#include "rq_object_schema_mgr.h"
#include "rq_product_handler_mgr.h"
#include "rq_product_termstruct_mapping_mgr.h"
#include "rq_type_id_mgr.h"
#include "rq_termstruct_mapping_mgr.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_system {
    rq_object_mgr_t object_mgr;
    rq_asset_mgr_t asset_mgr;
    rq_bootstrap_config_mgr_t bootstrap_config_mgr;
    rq_type_id_mgr_t date_event_type_mgr;
    rq_calendar_mgr_t calendar_mgr;
    rq_object_schema_mgr_t object_schema_mgr;
    rq_product_handler_mgr_t product_handler_mgr;
    rq_product_termstruct_mapping_mgr_t product_termstruct_mapping_mgr;
    rq_termstruct_mapping_mgr_t termstruct_mapping_mgr;
} *rq_system_t;


/** Test whether the rq_system is NULL */
RQ_EXPORT int rq_system_is_null(rq_system_t obj);

/** Allocate a new system object
 */
RQ_EXPORT rq_system_t rq_system_alloc();

/** Free the allocated system object
 */
RQ_EXPORT void rq_system_free(rq_system_t system);

/** Get the default object manager.
 *
 */
RQ_EXPORT rq_object_mgr_t rq_system_get_object_mgr(rq_system_t system);

/** Get the asset manager out of the system object.
 */
RQ_EXPORT rq_asset_mgr_t rq_system_get_asset_mgr(const rq_system_t s);

/** Get the bootstrapping configuration manager out of the system object
 */
RQ_EXPORT rq_bootstrap_config_mgr_t rq_system_get_bootstrap_config_mgr(const rq_system_t s);

/** Get the type_id_mgr that identifies the different date events in the
 * system.
 */
RQ_EXPORT rq_type_id_mgr_t rq_system_get_date_event_type_mgr(const rq_system_t s);

/** Get the calendar manager out of the system object.
 */
RQ_EXPORT rq_calendar_mgr_t rq_system_get_calendar_mgr(const rq_system_t s);

/** Get the object schema manager.
 */
RQ_EXPORT rq_object_schema_mgr_t rq_system_get_object_schema_mgr(const rq_system_t s);

/** Get the product handler manager.
 */
RQ_EXPORT rq_product_handler_mgr_t rq_system_get_product_handler_mgr(const rq_system_t s);

/** Get the product to term structure mapping manager.
 */
RQ_EXPORT rq_product_termstruct_mapping_mgr_t rq_system_get_product_termstruct_mapping_mgr(const rq_system_t s);

/** Get the term structure mapping manager object from the system.
 */
RQ_EXPORT rq_termstruct_mapping_mgr_t rq_system_get_termstruct_mapping_mgr(const rq_system_t system);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
