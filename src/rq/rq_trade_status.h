/**
 * \file rq_trade_status.h
 * \author Brett Hutley
 *
 * \brief Defines various status's for different kinds of trades
 */
/*
** rq_trade_status.h
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
#ifndef rq_trade_status_h
#define rq_trade_status_h

#include "rq_config.h"
#include "rq_defs.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

typedef unsigned short rq_trade_status;

/* -- deal creation events ---------------------------------------- */
#define RQ_TRADE_STATUS_CREATED                 0x0001
#define RQ_TRADE_STATUS_QUOTED                  0x0002
#define RQ_TRADE_STATUS_QUOTED_LOST             0x0003
#define RQ_TRADE_STATUS_DEALT                   0x0004
#define RQ_TRADE_STATUS_SETTLED                 0x0005
#define RQ_TRADE_STATUS_EXERCISED               0x0006
#define RQ_TRADE_STATUS_EXPIRED                 0x0007
#define RQ_TRADE_STATUS_KNOCKED_IN              0x0008
#define RQ_TRADE_STATUS_KNOCKED_OUT             0x0009
#define RQ_TRADE_STATUS_OVERDUE                 0x000a

/* -- confirmation status ----------------------------------------- */
#define RQ_TRADE_STATUS_UNCONFIRMED				0x0010
#define RQ_TRADE_STATUS_CONFIRMATION_SENT		0x0020
#define RQ_TRADE_STATUS_CONFIRMATION_RECEIVED	0x0030
#define RQ_TRADE_STATUS_CONFIRMED				0x0040
#define RQ_TRADE_STATUS_CONFIRMATION_UNMATCHED	0x0050

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
