/**
 * \file rq_type_id_mgr.h
 * \author Brett Hutley
 *
 * \brief A manager for name/ID pairs
 */
/*
** rq_type_id_mgr.h
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
#ifndef rq_type_id_mgr_h
#define rq_type_id_mgr_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_defs.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- types ------------------------------------------------------- */
/**
 * This enum defines the different ways that the ID can be assigned.
 */
enum rq_id_type {
    RQ_ID_TYPE_LONG, /**< This value means that the ID is a long and incremented by the manager */
    RQ_ID_TYPE_BITMAP, /**< This value means that the ID is a member of a bitmap and is allocated by the manager */
    RQ_ID_TYPE_USER_DEFINED /**< This value means that the ID is assigned by the user */
};

struct rq_type_id {
    long id;
    const char *name;
};

/** The private type_id_mgr structure.
 */
typedef struct rq_type_id_mgr {
    rq_tree_rb_t id_tree;
    rq_tree_rb_t name_tree;
    long next_id;
    enum rq_id_type id_type;
} *rq_type_id_mgr_t;

/* -- prototypes -------------------------------------------------- */
/** Test whether the rq_type_id_mgr is NULL */
RQ_EXPORT int rq_type_id_mgr_is_null(rq_type_id_mgr_t obj);

/**
 * Allocate a new type_id_mgr
 */
RQ_EXPORT rq_type_id_mgr_t rq_type_id_mgr_alloc(enum rq_id_type id_type);

/**
 * Free an object of type rq_type_id_mgr
 */
RQ_EXPORT void rq_type_id_mgr_free(rq_type_id_mgr_t type_id_mgr);

/**
 * Add an ID and type name to the rq_type_id_mgr object. This function
 * should only be called if the id_type of the type_id_mgr is
 * RQ_ID_TYPE_USER_DEFINED.
 */
RQ_EXPORT rq_boolean rq_type_id_mgr_add(rq_type_id_mgr_t type_id_mgr, const char *name, long id);

/**
 * Add a new type to the manager and (potentially) assign an id. This
 * function should NOT be called if the id_type of the type_id_mgr is
 * RQ_ID_TYPE_USER_DEFINED.
 */
RQ_EXPORT long rq_type_id_mgr_add_type(rq_type_id_mgr_t type_id_mgr, const char *name);

/**
 * Get the ID for a particular type name
 */
RQ_EXPORT long rq_type_id_mgr_get_id_for_type(rq_type_id_mgr_t type_id_mgr, const char *name);
/**
 * Get a type name for a particular type ID.
 */
RQ_EXPORT const char *rq_type_id_mgr_get_type_for_id(rq_type_id_mgr_t type_id_mgr, long id);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
