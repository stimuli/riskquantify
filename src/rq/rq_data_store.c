/*
** rq_data_store.c
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
#include <stdlib.h>

#include "rq_data_store.h"
#include "rq_error.h"

RQ_EXPORT rq_data_store_t 
_rq_data_store_alloc(
    void (*free_func)(void *store_data),
    rq_error_code (*create_store)(void *, const char *store_url),
    rq_error_code (*open_store)(void *, const char *store_url),
    rq_error_code (*close_store)(void *),
    rq_error_code (*load_system)(void *, rq_system_t),
    rq_error_code (*save_system)(void *, rq_system_t),
    rq_error_code (*load_market)(void *, rq_market_t),
    rq_error_code (*save_market)(void *, rq_market_t),
    void *store_data
    )
{
    rq_data_store_t store = (rq_data_store_t)RQ_MALLOC(sizeof(struct rq_data_store));
    store->free_func = free_func;
    store->create_store = create_store;
    store->open_store = open_store;
    store->close_store = close_store;
    store->load_system = load_system;
    store->save_system = save_system;
    store->load_market = load_market;
    store->save_market = save_market;

    store->store_data = store_data;

    return store;
}

RQ_EXPORT void 
rq_data_store_free(rq_data_store_t store)
{
    if (store->free_func)
        (*store->free_func)(store->store_data);
    RQ_FREE(store);
}

RQ_EXPORT rq_error_code 
rq_data_store_create(rq_data_store_t store, const char *filename)
{
    if (store->create_store)
        return (*store->create_store)(store->store_data, filename);

    return RQ_ERROR_CAPABILITY_NOT_AVAILABLE;
}

RQ_EXPORT rq_error_code 
rq_data_store_open(rq_data_store_t store, const char *filename)
{
    if (store->open_store)
        return (*store->open_store)(store->store_data, filename);

    return RQ_ERROR_CAPABILITY_NOT_AVAILABLE;
}

RQ_EXPORT rq_error_code
rq_data_store_close(rq_data_store_t store)
{
    if (store->close_store)
        return (*store->close_store)(store->store_data);

    return RQ_ERROR_CAPABILITY_NOT_AVAILABLE;
}

RQ_EXPORT rq_error_code
rq_data_store_system_load(rq_data_store_t store, rq_system_t system)
{
    if (store->load_system)
        return (*store->load_system)(store->store_data, system);

    return RQ_ERROR_CAPABILITY_NOT_AVAILABLE;
}

RQ_EXPORT rq_error_code 
rq_data_store_system_save(rq_data_store_t store, rq_system_t system)
{
    if (store->save_system)
        return (*store->save_system)(store->store_data, system);

    return RQ_ERROR_CAPABILITY_NOT_AVAILABLE;
}

RQ_EXPORT rq_error_code
rq_data_store_market_load(rq_data_store_t store, rq_market_t market)
{
    if (store->load_market)
        return (*store->load_market)(store->store_data, market);

    return RQ_ERROR_CAPABILITY_NOT_AVAILABLE;
}

RQ_EXPORT rq_error_code 
rq_data_store_market_save(rq_data_store_t store, rq_market_t market)
{
    if (store->save_market)
        return (*store->save_market)(store->store_data, market);

    return RQ_ERROR_CAPABILITY_NOT_AVAILABLE;
}
