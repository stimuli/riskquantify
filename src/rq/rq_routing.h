/**
 * \file rq_routing.h
 * \author Brett Hutley
 *
 * \brief An entity that provides three alternative ways of
 * identifying a party involved in the routing of a payment. The
 * identification may use payment system identifiers only; actual
 * name, address and other reference information; or a combination of
 * both.
 */
/*
** rq_routing.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */
#ifndef rq_routing_h
#define rq_routing_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_routing_ids.h"
#include "rq_routing_explicit_details.h"
#include "rq_stream.h"

/* -- structures --------------------------------------------------- */
/**
   EITHER:
	routing_ids;
   OR:
	routing_explicit_details;
   OR:
    BOTH
*/

struct rq_routing {
	struct rq_routing_ids *routing_ids;
	struct rq_routing_explicit_details *routing_explicit_details;
};

RQ_EXPORT void rq_routing_init(struct rq_routing *p);

RQ_EXPORT void rq_routing_clear(struct rq_routing *p);

RQ_EXPORT int rq_routing_write_xml(struct rq_routing *p, rq_stream_t output_stream);

RQ_EXPORT void rq_routing_free(struct rq_routing *p);

#endif
