/**
 * \file rq_math.h
 * \author Brett Hutley
 *
 * \brief Some useful math routines
 */
/*
** rq_math.h
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
#ifndef rq_math_h
#define rq_math_h

#include <math.h>
#include "rq_config.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- macros ------------------------------------------------------ */
/* Sign - from Numerical Recipes in C. */
#ifndef RQ_SIGN
#define RQ_SIGN(a, b) ((b) >= 0.0 ? fabs(a) : -fabs(a))
#endif

/* -- constants --------------------------------------------------- */
#ifndef M_E
#define M_E        2.71828182845904523536028747135      /* e */
#endif

#ifndef M_LOG2E
#define M_LOG2E    1.44269504088896340735992468100      /* log_2 (e) */
#endif

#ifndef M_LOG10E
#define M_LOG10E   0.43429448190325182765112891892      /* log_10 (e) */
#endif

#ifndef M_SQRT2
#define M_SQRT2    1.41421356237309504880168872421      /* sqrt(2) */
#endif

#ifndef M_SQRT1_2
#define M_SQRT1_2  0.70710678118654752440084436210      /* sqrt(1/2) */
#endif


#ifndef M_SQRT3
#define M_SQRT3    1.73205080756887729352744634151      /* sqrt(3) */
#endif

#ifndef M_PI
#define M_PI       3.14159265358979323846264338328      /* pi */
#endif

#ifndef M_PI_2
#define M_PI_2     1.57079632679489661923132169164      /* pi/2 */
#endif

#ifndef M_PI_4
#define M_PI_4     0.78539816339744830966156608458      /* pi/4 */
#endif

#ifndef M_SQRTPI
#define M_SQRTPI   1.77245385090551602729816748334      /* sqrt(pi) */
#endif

#ifndef M_2_SQRTPI
#define M_2_SQRTPI 1.12837916709551257389615890312      /* 2/sqrt(pi) */
#endif

#ifndef M_1_PI
#define M_1_PI     0.31830988618379067153776752675      /* 1/pi */
#endif

#ifndef M_2_PI
#define M_2_PI     0.63661977236758134307553505349      /* 2/pi */
#endif

#ifndef M_LN10
#define M_LN10     2.30258509299404568401799145468      /* ln(10) */
#endif

#ifndef M_LN2
#define M_LN2      0.69314718055994530941723212146      /* ln(2) */
#endif

#ifndef M_LNPI
#define M_LNPI     1.14472988584940017414342735135      /* ln(pi) */
#endif

#ifndef M_EULER
#define M_EULER    0.57721566490153286060651209008      /* Euler constant */
#endif

#define RQ_DBL_EPSILON 2.2204460492503131e-16

/* -- macros ------------------------------------------------------ */
#ifndef MAX
#define MAX(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifndef MIN
#define MIN(x, y) ((x) < (y) ? (x) : (y))
#endif

/* extern RQ_EXPORT const double RQ_NAN; */

/* -- prototypes -------------------------------------------------- */

/** This function initializes the RQ_NAN constant.
 */
RQ_EXPORT short rq_math_init();

RQ_EXPORT double rq_math_get_nan();

RQ_EXPORT int fact(int x);

RQ_EXPORT double rq_math_round(double x, unsigned int p);

/** Return non-zero if the two numbers are equal.
 *
 * This function returns non-zero if the two numbers are
 * equal. Ignores small differences between the numbers caused by
 * floating-point errors.
 */
RQ_EXPORT short rq_math_equal(double x, double y);

/** Compute (a^2 + b^2)^{1/2} without destructive under/over flow.
 *
 * The implementation of this function was taken from 
 * "Numerical Recipes in C".
 */
RQ_EXPORT double rq_pythag(double a, double b);

#ifdef NO_EXPM1
RQ_EXPORT double expm1(const double x);
#endif

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
