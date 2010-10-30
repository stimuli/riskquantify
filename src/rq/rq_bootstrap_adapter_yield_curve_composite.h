/**
 * \file rq_bootstrap_adapter_yield_curve_composite.h
 * \author Brett Hutley
 *
 * \brief A composite yield curve bootstrap adapter
 */
/*
** rq_bootstrap_adapter_yield_curve_composite.h
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
#ifndef rq_bootstrap_adapter_yield_curve_composite_h
#define rq_bootstrap_adapter_yield_curve_composite_h

#include "rq_config.h"
#include "rq_defs.h"
#include "rq_bootstrap_adapter.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

extern const char * const rq_bootstrap_adapter_yield_curve_composite_id;

/**
 * Allocate a new bootstrap_adapter_yield_curve_composite
 */
rq_bootstrap_adapter_t 
rq_bootstrap_adapter_yield_curve_composite_alloc();


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
