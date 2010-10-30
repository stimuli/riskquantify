/**
 * \file rq_bootstrap_ir_vol_surface.h
 * \author Todd Cooper
 *
 * \brief A simple function to bootstrap a volatility surface
 */
/*
** rq_bootstrap_ir_vol_surface.h
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
#ifndef rq_bootstrap_ir_vol_surface_h
#define rq_bootstrap_ir_vol_surface_h

/* -- includes ---------------------------------------------------- */
#include "rq_config.h"
#include "rq_ir_vol_surface.h"
#include "rq_asset_mgr.h"
#include "rq_rate_mgr.h"
#include "rq_bootstrap_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- prototypes -------------------------------------------------- */

RQ_EXPORT rq_ir_vol_surface_t
rq_bootstrap_ir_vol_surface(
    rq_date date, 
    const rq_rate_mgr_t rate_mgr, 
    const rq_bootstrap_config_t config, 
    const rq_asset_mgr_t asset_mgr,
    double *rate_adjust_factors, /**< if non-NULL, the factors to multiply the rate by before bootstrapping */
    rq_date last_date_to_bootstrap    
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
