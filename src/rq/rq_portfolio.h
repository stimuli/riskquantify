/**
 * \file rq_portfolio.h
 * \author Brett Hutley
 *
 * \brief The rq_portfolio object provides a container class for a set
 * of deals.
 */
/*
** rq_portfolio.h
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
#ifndef rq_portfolio_h
#define rq_portfolio_h

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

typedef struct rq_portfolio {
    rq_id id;
    const char *name;
} *rq_portfolio_t;

/** Test whether the rq_portfolio is NULL */
RQ_EXPORT int rq_portfolio_is_null(rq_portfolio_t obj);

RQ_EXPORT rq_portfolio_t rq_portfolio_alloc();

RQ_EXPORT void rq_portfolio_free(rq_portfolio_t portfolio);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
