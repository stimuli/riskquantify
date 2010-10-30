/**
 * @file
 *
 * This file defines data that all term structures have.
 */
/*
** rq_termstruct.h
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
#ifndef rq_termstruct_h
#define rq_termstruct_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_enum.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_termstruct {
    enum rq_termstruct_type termstruct_type;
    const char *termstruct_id;
    const char *underlying_asset_id;
};

/** Free the objects managed by a termstruct structure.
 *
 * Free the objects managed by a termstruct structure. This function
 * does NOT free the termstruct structure itself.
 */
RQ_EXPORT void rq_termstruct_clear(struct rq_termstruct *termstruct);

/** Get the term structure type.
 *
 * This function should work with any term structure.
 */
RQ_EXPORT enum rq_termstruct_type rq_termstruct_get_type(struct rq_termstruct *termstruct);

/** Set the underlying asset ID associated with a term structure.
 */
RQ_EXPORT void rq_termstruct_set_underlying_asset_id(struct rq_termstruct *termstruct, const char *asset_id);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
