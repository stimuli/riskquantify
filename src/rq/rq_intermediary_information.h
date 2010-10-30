/*
** rq_intermediary_information.h
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
#ifndef rq_intermediary_information_h
#define rq_intermediary_information_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_routing_explicit_details.h"
#include "rq_routing_ids.h"
#include "rq_stream.h"

/* -- structures --------------------------------------------------- */
/** either 	routing_ids or routing_explicit_details or 
    routing_ids_and_explicit_details;
*/
struct rq_intermediary_information {
	int intermediary_sequence_number;
	struct rq_routing_ids *routing_ids;
	struct rq_routing_explicit_details *routing_explicit_details;
};

/* -- prototypes -------------------------------------------------- */
RQ_EXPORT int rq_intermediary_information_write_xml(struct rq_intermediary_information *p, rq_stream_t output_stream);

RQ_EXPORT void rq_intermediary_information_init(struct rq_intermediary_information *p);

RQ_EXPORT void rq_intermediary_information_clear(struct rq_intermediary_information *p);


#endif
