/**
 * \file rq_future_price.h
 * \author Brett Hutley
 *
 * \brief The rq_future_price class implements a future price object
 */
/*
** rq_future_price.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#ifndef rq_future_price_h
#define rq_future_price_h

#include "rq_config.h"
#include "rq_date.h"
#include "rq_asset_future.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

struct rq_future_price {
    char *asset_id;
    double price;
	int quote;
};

/*
  typedef void *rq_future_price_t;

  rq_future_price_t rq_future_price_alloc();
  void rq_future_price_free(rq_future_price_t fr);

  void rq_future_price_set_date(rq_future_price_t future_price, rq_date date);
  rq_date rq_future_price_get_date(const rq_future_price_t future_price);
  void rq_future_price_set_price(rq_future_price_t future_price, double price);
  double rq_future_price_get_price(const rq_future_price_t future_price);
*/

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
