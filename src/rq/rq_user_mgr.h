/**
 * \file rq_user_mgr.h
 * \author Brett Hutley
 *
 * \brief This object manages users of the Risk Quantify system
 */
/*
** rq_user_mgr.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA 
*/
#ifndef rq_user_mgr_h
#define rq_user_mgr_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_date.h"
#include "rq_enum.h"
#include "rq_user.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- typedefs ---------------------------------------------------- */
/** Define the user manager object. */
typedef struct rq_user_mgr {
    rq_tree_rb_t tree;
} *rq_user_mgr_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_user_mgr is NULL */
RQ_EXPORT int rq_user_mgr_is_null(rq_user_mgr_t obj);

/**
 * Allocate a new user manager
 */
RQ_EXPORT rq_user_mgr_t rq_user_mgr_alloc();

/**
 * Free an allocated user mgr.
 */
RQ_EXPORT void rq_user_mgr_free(rq_user_mgr_t user_mgr);

/**
 * Add a user to the user mgr
 */
RQ_EXPORT void rq_user_mgr_add(rq_user_mgr_t user_mgr, rq_user_t user);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
