/*
** rq_address.h
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
#ifndef rq_address_h
#define rq_address_h

#include "rq_street_address.h"
#include "rq_stream.h"

struct rq_address {
    struct rq_street_address *street_address;
    const char *city; /* optional */
    const char *state; /* optional */
    const char *country; /* optional */
    const char *postal_code; /* optional */
};

RQ_EXPORT void rq_address_init(struct rq_address *p);
RQ_EXPORT void rq_address_clear(struct rq_address *p);
RQ_EXPORT int rq_address_write_xml(struct rq_address *p, rq_stream_t output_stream);
RQ_EXPORT void rq_address_free(struct rq_address *p);

#endif
