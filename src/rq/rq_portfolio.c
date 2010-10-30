/*
** rq_portfolio.c
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
#include "rq_portfolio.h"
#include <stdlib.h>


RQ_EXPORT int
rq_portfolio_is_null(rq_portfolio_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_portfolio_t 
rq_portfolio_alloc()
{
    struct rq_portfolio *p = (struct rq_portfolio *)RQ_CALLOC(1, sizeof(struct rq_portfolio));

    p->id = RQ_ID_UNASSIGNED;

    /* name is set to zero in the RQ_CALLOC call */

    return p;
}

RQ_EXPORT void
rq_portfolio_free(rq_portfolio_t portfolio)
{
    if (portfolio->name)
        RQ_FREE((char *)portfolio->name);
    RQ_FREE(portfolio);
}
