/*
** rq_business_centers.h
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
#ifndef rq_business_centers_h
#define rq_business_centers_h

/* -- includes ----------------------------------------------------- */
#include "rq_config.h"
#include "rq_stream.h"
#include "rq_array.h"

/* -- structures --------------------------------------------------- */
struct rq_business_centers {
    rq_array_t business_center_list;
};

RQ_EXPORT int rq_business_centers_write_xml(struct rq_business_centers *p, rq_stream_t output_stream);

RQ_EXPORT void rq_business_centers_init(struct rq_business_centers *p);

RQ_EXPORT void rq_business_centers_clear(struct rq_business_centers *p);


#endif
