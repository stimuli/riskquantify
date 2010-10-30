/**
 * \file rq_interpolate.h
 * \author Brett Hutley
 *
 * \brief This file provides interpolation routines.
 */
/*
** rq_interpolate.h
**
** Interpolation functions.
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
#ifndef rq_interpolate_h
#define rq_interpolate_h

#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

RQ_EXPORT double rq_interpolate_linear(double x, double x1, double y1, double x2, double y2);

/** The log linear interpolation is alternatively known as the geometric interpolation
 * This method linearly interpolates the logarithm of a point at (x,y), given the two points (x1,y1) and (x2,y2)
 */
RQ_EXPORT double rq_interpolate_log_linear(double x, double x1, double y1, double x2, double y2);

/** The Cubic Spline algorithm is based on Numerical Recipes p115-116.
 *
 * This func is the first step of the two step cubic spline.  This
 * func calculates the time consuming calculations of retrieving the
 * second derivates.  Hence this func should only be called once due
 * to performance.
 *
 * ARGS
 * natural  - for a natural spline set to 1 otherwise 0
 * x[]  -  ordered array of x co-ordinate points
 * y[]  -  ordered array of y co-ordinate points
 * n    -  number of input points
 * yp1  -  first derivative at point 1
 * ypn  -  first derivative at point n
 *
 * RETURN
 * y2[]  -  ordered array of the second derivatives of points 1 to n
 */
RQ_EXPORT void rq_interpolate_spline(unsigned int natural, double x[], double y[], int n, double yp1, double ypn, double y2[]);

// This func is the second step of the two step cubic spline.  It can calculate any interpolated value
// for x given the second derivative.
// ARGS
// xa[]  -  ordered array of x co-ordinate points
// ya[]  -  oredered array of y co-ordinate points
// y2a[]  -  ordered array of the second derivatives of points 1 to n
// n    -  number of input points
// x  -  x co-ordinate of an interpolated point
// y  -  interpolated y co-ordinate
RQ_EXPORT double rq_interpolate_splint(double x, double xa[], double ya[], double y2a[], int n);


#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
