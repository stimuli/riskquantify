/**
 * \file rq_market_requirements.h
 * \author Brett Hutley
 *
 * \brief Allows the pricing adapter to specify what market content it
 * needs in order to fulfill pricing requests.
 */
/*
** rq_market_requirements.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#ifndef rq_market_requirements_h
#define rq_market_requirements_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- defines ----------------------------------------------------- */
#define RQ_MAX_ASSET_ID_LEN 100
#define RQ_MAX_SPOT_PRICE_ASSET_NUM 20


/* -- structs ----------------------------------------------------- */
struct rq_termstruct_req {
    char termstruct_group_id[RQ_LENGTH_TERMSTRUCT_GROUP_ID+1];
    char asset_id[RQ_LENGTH_ASSET_ID+1];
    rq_date maturity_date;
};

struct rq_termstruct_req_hdr {
    unsigned int num_requests;
    struct rq_termstruct_req requests[RQ_MAX_TERMSTRUCT_SPECIFICATIONS];
};

/** the handle to the rq_market_requirements */
typedef struct rq_market_requirements {
    struct rq_termstruct_req_hdr *req_hdrs[RQ_TERMSTRUCT_TYPE_MAX_ENUM];
    char spot_price_assets[RQ_MAX_ASSET_ID_LEN * RQ_MAX_SPOT_PRICE_ASSET_NUM];
    unsigned cur_spot_price_asset_num;
} *rq_market_requirements_t;


/* -- protos ------------------------------------------------------ */

/** Test whether the rq_market_requirements is NULL */
RQ_EXPORT int rq_market_requirements_is_null(rq_market_requirements_t obj);

/**
 * Allocate a new market_requirements
 */
RQ_EXPORT rq_market_requirements_t rq_market_requirements_alloc();

/**
 * Free an object of type rq_market_requirements
 */
RQ_EXPORT void rq_market_requirements_free(rq_market_requirements_t market_requirements);

/**
 * Clear the list of requested market items
 */
RQ_EXPORT void rq_market_requirements_clear(rq_market_requirements_t market_requirements);

/**
 * Add a termstructure request to the list
 */
RQ_EXPORT void
rq_market_requirements_termstruct_add(rq_market_requirements_t market_requirements, enum rq_termstruct_type termstruct_type, const char *termstruct_group_id, const char *asset_id, rq_date maturity_date);

/**
 * Get the number of termstructures in the list by termstructure type
 */
RQ_EXPORT unsigned int
rq_market_requirements_termstruct_size(rq_market_requirements_t market_requirements, enum rq_termstruct_type termstruct_type);

/**
 * Get the termstructure_req structure at a particular offset
 */
RQ_EXPORT struct rq_termstruct_req *
rq_market_requirements_termstruct_get_at(rq_market_requirements_t market_requirements, enum rq_termstruct_type termstruct_type, unsigned int offset);

RQ_EXPORT void
rq_market_requirements_spot_price_asset_add(rq_market_requirements_t market_requirements, const char *asset_id);

RQ_EXPORT unsigned int
rq_market_requirements_spot_price_asset_size(rq_market_requirements_t market_requirements);

RQ_EXPORT const char *
rq_market_requirements_spot_price_asset_get_at(rq_market_requirements_t market_requirements, unsigned int offset);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

