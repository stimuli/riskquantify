/*
** rq_math.c
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
#include "rq_math.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <float.h>
#include <assert.h>

#define RQ_MATH_FLOATING_POINT_ERROR 0.00000001

/*
 * The following code is adapted from gnan.c which is part of
 * libguppi. The code to libguppi is developed under the GPL.
 *
 * Copyright (C) 2000 EMC Capital Management, Inc.
 *
 * Developed by Jon Trowbridge <trow@gnu.org>.
 *
 */

union ieee754_big_endian_double {
  
    double d;

    struct {
        unsigned int signbit:1;
        unsigned int exp:11;
        unsigned int man0:20;
        unsigned int man1:32;
    
    } ieee;
};

union ieee754_little_endian_double {
  
    double d;

    struct {
        unsigned int man1:32;
        unsigned int man0:20;
        unsigned int exp:11;
        unsigned int signbit:1;
    } ieee;
};

/* little endian, but with big endian float word order */
union ieee754_mixed_endian_double {
  
    double d;

    struct {
        unsigned int man0:20;
        unsigned int exp:11;
        unsigned int signbit:1;
        unsigned int man1:32;
    } ieee;
};

static double RQ_NAN;

#define SIGNBIT 0
#define MAN0 0
#define MAN1 0
#define EXP 1023


RQ_EXPORT short
rq_math_init()
{
#ifdef NAN
    double d = NAN;

    /* If we have NAN, we just use it. */
    assert(isnan(NAN));
    memcpy((double *)&RQ_NAN, &d, sizeof(double));

#else
    union ieee754_big_endian_double be;
    union ieee754_little_endian_double le;
    union ieee754_mixed_endian_double me;

    /* Note: we declared NAN const so we have to be a bit careful about
       changing it.  Otherwise we outsmart the compiler and thus
       ourselves.  Not good.  
    */

    be.d = 1.0;
    if (sizeof (double) == 8
        && be.ieee.signbit == SIGNBIT
        && be.ieee.man0 == MAN0
        && be.ieee.man1 == MAN1
        && be.ieee.exp == EXP) 
    {
        be.ieee.signbit = 0;
        be.ieee.man0 = 1;
        be.ieee.man1 = 1;
        be.ieee.exp = 2047;

        memcpy((double *)&RQ_NAN, &be.d, sizeof(double));
        if (isnan(RQ_NAN)) 
            return 0;
    }

    le.d = 1.0;
    if (sizeof(double) == 8
        && le.ieee.signbit == SIGNBIT
        && le.ieee.man0 == MAN0
        && le.ieee.man1 == MAN1
        && le.ieee.exp == EXP) 
    {
        le.ieee.signbit = 0;
        le.ieee.man0 = 1;
        le.ieee.man1 = 1;
        le.ieee.exp = 2047;

        memcpy((double *)&RQ_NAN, &le.d, sizeof (double));
        if (isnan (RQ_NAN)) 
            return 0;
    }

    me.d = 1.0;
    if (sizeof(double) == 8
        && me.ieee.signbit == SIGNBIT
        && me.ieee.man0 == MAN0
        && me.ieee.man1 == MAN1
        && me.ieee.exp == EXP) 
    {
        me.ieee.signbit = 0;
        me.ieee.man0 = 1;
        me.ieee.man1 = 1;
        me.ieee.exp = 2047;

        memcpy ((double *)&RQ_NAN, &me.d, sizeof(double));
        if (isnan(RQ_NAN)) 
            return 0;
    }


#endif /* NAN */

    return !isnan(RQ_NAN);
}

RQ_EXPORT double
rq_math_get_nan()
{
    return RQ_NAN;
}

RQ_EXPORT int
fact(int x)
{
    int i = 2;
    int r = 1;
    while (i <= x)
    {
        r *= i;
        i++;
    }
    return r;
}

RQ_EXPORT double
rq_math_round(double x, unsigned int p)
{
	/* Round x to p decimal places */

	double f;
	double fx;

	f = (p > 0) ? pow(10, p) : 1.0;
	x *= f;
	fx = floor(x);
	x = (x - fx >= 0.5) ? fx + 1.0 : fx;
	x /= f;

	return x;
}

RQ_EXPORT short 
rq_math_equal(double x, double y)
{
    return 
        x + RQ_MATH_FLOATING_POINT_ERROR >= y && 
        x - RQ_MATH_FLOATING_POINT_ERROR <= y;
}

RQ_EXPORT double
rq_pythag(double a, double b)
{
    double absa = fabs(a);
    double absb = fabs(b);

    if (absa > absb)
    {
        double sq = absb / absa;
        sq *= sq;
        return absa * sqrt(1.0 + sq);
    }
    else
    {
        double sq = absa / absb;
        sq *= sq;
        return (absb == 0.0 ? 0.0 : absb * sqrt(1.0 + sq));
    }
}

#ifdef NO_EXPM1
/**
 * This is taken from the GSL library
 */
RQ_EXPORT double 
expm1 (const double x)
{
    if (fabs(x) < M_LN2)
    {
        /* Compute the taylor series S = x + (1/2!) x^2 + (1/3!) x^3 + ... */

        double i = 1.0;
        double sum = x;
        double term = x / 1.0;

        do
        {
            term *= x / ++i;
            sum += term;
        }
        while (fabs(term) > fabs(sum) * RQ_DBL_EPSILON);
      
        return sum;
    }

    return exp(x) - 1.0;
}
#endif

#ifdef DEBUG_RQ_MATH
int
main(int argc, char **argv)
{
    double val;

    rq_math_init();

    val = RQ_NAN;

    if (isnan(val))
        printf("Val *is* NAN!\n");
    else
        printf("Val *isn't* NAN!\n");

    return 0;
}
#endif
