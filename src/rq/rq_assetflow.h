/**
 * \file rq_assetflow.h
 * \author Brett Hutley
 *
 * \brief The rq_assetflow class represents the movement of an asset
 * on a particular date.
 */
/*
** rq_assetflow.h
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
#ifndef rq_assetflow_h
#define rq_assetflow_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
typedef struct rq_assetflow {
    const char *asset_id;
    rq_date payment_date;
    double amount;
} *rq_assetflow_t;


/** Test whether the rq_assetflow is NULL */
RQ_EXPORT int rq_assetflow_is_null(rq_assetflow_t obj);

/* -- prototypes -------------------------------------------------- */
RQ_EXPORT rq_assetflow_t rq_assetflow_alloc();

RQ_EXPORT void rq_assetflow_free(rq_assetflow_t aflow);

RQ_EXPORT const char *rq_assetflow_get_asset_id(rq_assetflow_t aflow);
RQ_EXPORT void rq_assetflow_set_asset_id(rq_assetflow_t aflow, const char *asset_id);

RQ_EXPORT rq_date rq_assetflow_get_payment_date(rq_assetflow_t aflow);
RQ_EXPORT void rq_assetflow_set_payment_date(rq_assetflow_t aflow, rq_date payment_date);

RQ_EXPORT double rq_assetflow_get_amount(rq_assetflow_t aflow);
RQ_EXPORT void rq_assetflow_set_amount(rq_assetflow_t aflow, double amount);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
