/**
 * \file rq_user.h
 * \author Brett Hutley
 *
 * \brief This object represents users of the Risk Quantify system
 */
/*
** rq_user.h
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
#ifndef rq_user_h
#define rq_user_h

/* -- includes ---------------------------------------------------- */
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

/* -- typedefs ---------------------------------------------------- */
/** Define a handle to a user object */
typedef struct rq_user {
    const char *id;
    const char *name;
} *rq_user_t;

/* -- prototypes -------------------------------------------------- */

/** Test whether the rq_user is NULL */
RQ_EXPORT int rq_user_is_null(rq_user_t obj);

/**
 * Allocate a new user
 */
RQ_EXPORT rq_user_t rq_user_alloc(const char *user_id);

/**
 * Free an allocated user.
 */
RQ_EXPORT void rq_user_free(rq_user_t user);

/**
 * Get the user ID
 */
RQ_EXPORT const char *rq_user_get_id(rq_user_t user);

/**
 * Set the user name
 */
RQ_EXPORT void rq_user_set_name(rq_user_t user, const char *name);

/**
 * Get the user name
 */
RQ_EXPORT const char *rq_user_get_name(rq_user_t user);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
