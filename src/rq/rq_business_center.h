/**
 * \file rq_business_center.h
 * \author Brett Hutley
 *
 * \brief The rq_business_center is an object that represents a
 * business center.
 */
/*
** rq_business_center.h
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
#ifndef rq_business_center_h
#define rq_business_center_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"

/* -- typedefs ----------------------------------------------------- */
typedef struct rq_business_center {
    const char *id;
    const char *region_id;
} *rq_business_center_t;


/* -- prototypes --------------------------------------------------- */

/** Test whether the rq_business_center is NULL */
RQ_EXPORT int rq_business_center_is_null(rq_business_center_t obj);

/**
 * Allocate a new business center object
 *
 * @return	The newly allocated business center
 */
RQ_EXPORT rq_business_center_t rq_business_center_alloc();


/**
 * Free a business center object
 *
 * @param	business_center	The business center object to free
 */
RQ_EXPORT void rq_business_center_free(rq_business_center_t business_center);


/**
 * Get a business center's ID
 *
 * @param	business_center	The business center
 * @return	The ID associated with this business center
 */
RQ_EXPORT const char *rq_business_center_get_id(rq_business_center_t business_center);


/**
 * Set the ID associated with a business center.
 *
 * @param	business_center	The business center.
 * @param	id	The ID of the business center
 */
RQ_EXPORT void rq_business_center_set_id(rq_business_center_t business_center, const char *id);


/**
 * Get the region ID of a business center. This is usually a 2 character 
 * code like "AU" or "US".
 *
 * @param	business_center	The business center
 * @return	The region ID
 */
RQ_EXPORT const char *rq_business_center_get_region_id(rq_business_center_t business_center);


/**
 * Set the region ID for a business center.
 *
 * @param	business_center	The business center
 * @param	region_id	The region ID (usually a 2 character code like "AU" or "US")
 */
RQ_EXPORT void rq_business_center_set_region_id(rq_business_center_t business_center, const char *region_id);

#endif
