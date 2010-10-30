/**
 * \file rq_object_builder_xml.h
 * \author Brett Hutley
 *
 * \brief The rq_object_builder_xml type reconstitutes Risk Quantify
 * objects from XML streams.
 */
/*
** rq_object_builder_xml.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#ifndef rq_object_builder_xml_h
#define rq_object_builder_xml_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_object_builder.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

extern const char * const rq_object_builder_xml_type_name;

/**
 * Allocate a new XML object builder. 
 *
 */
RQ_EXPORT rq_object_builder_t rq_object_builder_xml_alloc();

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
