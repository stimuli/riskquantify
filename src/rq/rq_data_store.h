/**
 * @file
 *
 * This file defines the functions for storing and loading the
 * Risk Quantify system.
 */
/*
** rq_data_store.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2009 Brett Hutley
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
#ifndef rq_data_store_h
#define rq_data_store_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_type.h"
#include "rq_system.h"
#include "rq_market.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- structs ----------------------------------------------------- */
typedef struct rq_data_store {
    void (*free_func)(void *store_data);

    rq_error_code (*create_store)(void *store_data, const char *store_url);
    rq_error_code (*open_store)(void *store_data, const char *store_url);
    rq_error_code (*close_store)(void *store_data);
    rq_error_code (*load_system)(void *, rq_system_t);
    rq_error_code (*save_system)(void *, rq_system_t);
    rq_error_code (*load_market)(void *, rq_market_t);
    rq_error_code (*save_market)(void *, rq_market_t);

    void *store_data;
} *rq_data_store_t;

/* -- prototypes -------------------------------------------------- */

/** Data store allocation function. Should be called by a derived data store */
RQ_EXPORT rq_data_store_t 
_rq_data_store_alloc(
    void (*free_func)(void *store_data),
    rq_error_code (*create_store)(void *store_data, const char *store_url),
    rq_error_code (*open_store)(void *store_data, const char *store_url),
    rq_error_code (*close_store)(void *store_data),
    rq_error_code (*load_system)(void *store_data, rq_system_t system),
    rq_error_code (*save_system)(void *store_data, rq_system_t system),
    rq_error_code (*load_market)(void *store_data, rq_market_t market),
    rq_error_code (*save_market)(void *store_data, rq_market_t market),
    void *store_data
    );


/** Free a previously allocated data store. */
RQ_EXPORT void rq_data_store_free(rq_data_store_t store);

RQ_EXPORT rq_error_code rq_data_store_create(rq_data_store_t store, const char *filename);

RQ_EXPORT rq_error_code rq_data_store_open(rq_data_store_t store, const char *filename);

RQ_EXPORT rq_error_code rq_data_store_close(rq_data_store_t store);

RQ_EXPORT rq_error_code rq_data_store_system_load(rq_data_store_t, rq_system_t);
RQ_EXPORT rq_error_code rq_data_store_system_save(rq_data_store_t, rq_system_t);
RQ_EXPORT rq_error_code rq_data_store_market_load(rq_data_store_t, rq_market_t);
RQ_EXPORT rq_error_code rq_data_store_market_save(rq_data_store_t, rq_market_t);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
