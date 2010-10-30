/**
 * \file rq_asset_correlation.h
 * \author Hendra
 *
 * \brief The rq_asset_correlation object implements a correlation between two assets.
 */
/*
** rq_asset_correlation.h
**
** Written by Hendra
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
#ifndef rq_asset_correlation_h
#define rq_asset_correlation_h

#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef struct rq_asset_correlation 
{
    const char* key;
    double correlation;
}* rq_asset_correlation_t;


/** Test whether the rq_asset_correlation is NULL */
RQ_EXPORT int rq_asset_correlation_is_null(rq_asset_correlation_t obj);

/**
 * Build an asset correlation object given the two assets and their corrleation.
 */
RQ_EXPORT rq_asset_correlation_t 
rq_asset_correlation_build(
    const char* asset1,
    const char* asset2,
    double correlation
    );

/**
 * Build the index name associated with this asset correlation.
 */
RQ_EXPORT char* rq_asset_correlation_build_assetKey(const char* asset1, const char* asset2);

/**
 * Free an allocated asset correlation object
 */
RQ_EXPORT void rq_asset_correlation_free(rq_asset_correlation_t er);

/**
 * Clone an asset correlation
 */
RQ_EXPORT rq_asset_correlation_t rq_asset_correlation_clone(const rq_asset_correlation_t er);

/* -- getters ----------------------------------------------------- */
/**
 * Get the asset correlation key
 */
RQ_EXPORT const char *rq_asset_correlation_get_key(const rq_asset_correlation_t er);

/**
 * Get the correlation from the asset correlation object
 */
RQ_EXPORT double rq_asset_correlation_get_correlation(const rq_asset_correlation_t er);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif
#endif
