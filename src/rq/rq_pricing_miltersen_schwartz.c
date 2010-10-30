/*
** rq_pricing_miltersen_schwartz.c
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
#include "rq_pricing_miltersen_schwartz.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* Miltersen Schwartz (1997) commodity option model */
RQ_EXPORT double 
rq_pricing_miltersen_swartz(
    short call, 
    double Pt, 
    double FT, 
    double X, 
    double t1,
    double T2, 
    double vS, 
    double vE, 
    double vf, 
    double rhoSe, 
    double rhoSf, 
    double rhoef, 
    double Kappae, 
    double Kappaf
    )
{
    double ret = 0.0;
                
    double vz = pow(vS, 2) * t1 + 2 * vS * 
        (vf * rhoSf * 1 / Kappaf * (t1 - 1 / Kappaf * exp(-Kappaf * T2) * (exp(Kappaf * t1) - 1)) 
         - vE * rhoSe * 1 / Kappae * (t1 - 1 / Kappae * exp(-Kappae * T2) * (exp(Kappae * t1) - 1))) 
        + pow(vE, 2) * 1 / pow(Kappae, 2) * 
        (t1 + 1 / (2 * Kappae) * exp(-2 * Kappae * T2) * 
         (exp(2 * Kappae * t1) - 1) - 2 * 1 / Kappae * exp(-Kappae * T2) * 
         (exp(Kappae * t1) - 1))
        + pow(vf, 2) * 1 / pow(Kappaf, 2) * 
        (t1 + 1 / (2 * Kappaf) * exp(-2 * Kappaf * T2) * 
         (exp(2 * Kappaf * t1) - 1) - 2 * 1 / Kappaf * 
         exp(-Kappaf * T2) * (exp(Kappaf * t1) - 1)) 
        - 2 * vE * vf * rhoef * 1 / Kappae * 1 / Kappaf * 
        (t1 - 1 / Kappae * exp(-Kappae * T2) * 
         (exp(Kappae * t1) - 1) - 1 / Kappaf * exp(-Kappaf * T2) * 
         (exp(Kappaf * t1) - 1) 
         + 1 / (Kappae + Kappaf) * exp(-(Kappae + Kappaf) * T2) * 
         (exp((Kappae + Kappaf) * t1) - 1));
    
    double vxz = vf * 1 / Kappaf * (vS * rhoSf * (t1 - 1 / Kappaf * 
                                                  (1 - exp(-Kappaf * t1))) 
                                    + vf * 1 / Kappaf * 
                                    (t1 - 1 / Kappaf * exp(-Kappaf * T2) * 
                                     (exp(Kappaf * t1) - 1) - 1 / Kappaf * 
                                     (1 - exp(-Kappaf * t1)) 
                                     + 1 / (2 * Kappaf) * exp(-Kappaf * T2) * 
                                     (exp(Kappaf * t1) - exp(-Kappaf * t1)))
                                    - vE * rhoef * 1 / Kappae * 
                                    (t1 - 1 / Kappae * exp(-Kappae * T2) * 
                                     (exp(Kappae * t1) - 1) - 1 / Kappaf * 
                                     (1 - exp(-Kappaf * t1)) 
                                     + 1 / (Kappae + Kappaf) * exp(-Kappae * T2) * 
                                     (exp(Kappae * t1) - exp(-Kappaf * t1))));
    
    double vz_sqrt = sqrt(vz);
                
    double d1 = (log(FT / X) - vxz + vz / 2.0) / vz_sqrt;
    double d2 = (log(FT / X) - vxz - vz / 2.0) / vz_sqrt;
                
    if (call)
        ret = Pt * (FT * exp(-vxz) * rq_pricing_cumul_norm_dist(d1) - X * rq_pricing_cumul_norm_dist(d2));
    else
        ret = Pt * (X * rq_pricing_cumul_norm_dist(-d2) - FT * exp(-vxz) * rq_pricing_cumul_norm_dist(-d1));
    return ret;
}
