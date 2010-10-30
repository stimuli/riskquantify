/**
 * \file rq_error.h
 * \author Brett Hutley
 *
 * \brief rq_error.h provides error code definitions
 */
/*
** rq_error.h
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
#ifndef rq_error_h
#define rq_error_h

#include "rq_defs.h"

#include <errno.h>

#define RQ_OK                 0
#define RQ_FAILED			  -1

#define RQ_ERROR_CAPABILITY_NOT_AVAILABLE -2

#define RQ_ENOENT  ENOENT

/* -- rq_xml_parser error codes -- */
#define RQ_ERR_XML_PARSER_STREAM_NOT_ASSIGNED -100

#define RQ_PRICING_FINITE_DIFFERENCES_SOR_DID_NOT_CONVERGE -10

#endif

