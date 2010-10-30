/**
 * \file rq_business_center_mgr.h
 * \author Brett Hutley
 *
 * \brief The rq_business_center_mgr object is a manager class for
 * business centers.
 */
/*
** rq_business_center_mgr.h
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
#ifndef rq_business_center_mgr_h
#define rq_business_center_mgr_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_business_center.h"

/* -- typedefs ----------------------------------------------------- */
struct rq_business_center_mgr_node {
    struct rq_business_center_mgr_node *left;
    struct rq_business_center_mgr_node *right;

    rq_business_center_t business_center;
};

typedef struct rq_business_center_mgr {
    struct rq_business_center_mgr_node *head;
} *rq_business_center_mgr_t;


/* -- prototypes --------------------------------------------------- */

/** Test whether the rq_business_center_mgr is NULL */
RQ_EXPORT int rq_business_center_mgr_is_null(rq_business_center_mgr_t obj);

/**
 * Allocate a new business center manager.
 *
 * @return	The newly allocated business center manager
 */
RQ_EXPORT rq_business_center_mgr_t rq_business_center_mgr_alloc();


/**
 * free the memory allocated by the rq_business_center_mgr_alloc() call
 *
 * @param	business_center_mgr	The business center manager to free
 */
RQ_EXPORT void rq_business_center_mgr_free(rq_business_center_mgr_t business_center_mgr);


/**
 * Add a new business center to the manager. The business center manager
 * takes ownership of this business center.
 *
 * @param	business_center_mgr	The business center manager
 * @param	business_center	The business center to add
 */
RQ_EXPORT void rq_business_center_mgr_add(rq_business_center_mgr_t business_center_mgr, rq_business_center_t business_center);

#endif
