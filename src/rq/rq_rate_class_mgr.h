/**
 * \file rq_rate_class_mgr.h
 * \author Brett Hutley
 *
 * \brief This object manages rate classes
 */
/*
** rq_rate_class_mgr.h
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
#ifndef rq_rate_class_mgr_h
#define rq_rate_class_mgr_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_rate_class.h"
#include "rq_tree_rb.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_rate_class_mgr_node {
    const char *type_id;
    rq_tree_rb_t rate_classes;
};

/** The rate_class_mgr structure.
 */
typedef struct rq_rate_class_mgr {
    rq_tree_rb_t rate_classes;
    rq_tree_rb_t rate_class_types;
} *rq_rate_class_mgr_t;


/** Test whether the rq_rate_class_mgr is NULL */
RQ_EXPORT int rq_rate_class_mgr_is_null(rq_rate_class_mgr_t obj);

/**
 * Allocate a new rate_class_mgr
 */
RQ_EXPORT rq_rate_class_mgr_t rq_rate_class_mgr_alloc();

/**
 * Free an object of type rq_rate_class_mgr
 */
RQ_EXPORT void rq_rate_class_mgr_free(rq_rate_class_mgr_t rate_class_mgr);

/**
 * Add a new rate class to the manager.
 */
RQ_EXPORT void rq_rate_class_mgr_add(rq_rate_class_mgr_t rate_class_mgr, rq_rate_class_t rate_class);

/**
 * Find a rate class by ID.
 */
RQ_EXPORT rq_rate_class_t rq_rate_class_mgr_find(rq_rate_class_mgr_t rate_class_mgr, const char *id);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif

