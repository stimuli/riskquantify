/**
 * \file rq_forward_rate.h
 * \author Brett Hutley
 *
 * \brief The rq_forward_rate class implements a forward rate object
 */
/*
** rq_forward_rate.h
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
#ifndef rq_forward_rate_h
#define rq_forward_rate_h

#include "rq_config.h"
#include "rq_date.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_forward_rate {
    rq_date date;
    double rate;
	int quote;
};


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
