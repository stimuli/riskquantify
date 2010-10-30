/*
** rq_random.c
** 
** Written by Brett Hutley - brett@hutley.net
** 
** Copyright (C) 2001-2008 Brett Hutley
** 
** This file is part of the Risk Quantify Library
** 
** Risk Quantify is free software; you can redistribute it and/or
** modify it under the terms of the GNU Library General Public License
** as published by the Free Software Foundation; either version 2 of
** the License, or (at your option) any later version.
** 
** Risk Quantify is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Library General Public License for more details.
** 
** You should have received a copy of the GNU Library General Public
** License along with Risk Quantify; if not, write to the Free
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA
*/ 
#include "rq_config.h"
#include "rq_random.h"
#include "rq_math.h"
#include "rq_pricing_normdist.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*
  This code was taken from the implementation available from 
  Makoto Matsumoto. I downloaded it from 
  http://random.mat.sbg.ac.at/generators/ - see statement below.
*/

/* A C-program for TT800 : July 8th 1996 Version */
/* by M. Matsumoto, email: matumoto@math.keio.ac.jp */
/* genrand() generate one pseudorandom number with double precision */
/* which is uniformly distributed on [0,1]-interval */
/* for each call.  One may choose any initial 25 seeds */
/* except all zeros. */

/* See: ACM Transactions on Modelling and Computer Simulation, */
/* Vol. 4, No. 3, 1994, pages 254-266. */

#define N 25
#define M 7

double
tt800()
{
    unsigned long y;
    static int k = 0;
    static unsigned long x[N]={ /* initial 25 seeds, change as you wish */
        0x95f24dab, 0x0b685215, 0xe76ccae7, 0xaf3ec239, 0x715fad23,
        0x24a590ad, 0x69e4b5ef, 0xbf456141, 0x96bc1b7b, 0xa7bdf825,
        0xc1de75b7, 0x8858a9c9, 0x2da87693, 0xb657f9dd, 0xffdc8a9f,
        0x8121da71, 0x8b823ecb, 0x885d05f5, 0x4e20cd47, 0x5a9ad5d9,
        0x512c0c03, 0xea857ccd, 0x4cc1d30f, 0x8891a8a1, 0xa6b7aadb
    };
    static unsigned long mag01[2]={ 
        0x0, 0x8ebfd028 /* this is magic vector `a', don't change */
    };
    if (k==N) { /* generate N words at one time */
        int kk;
        for (kk=0;kk<N-M;kk++) {
            x[kk] = x[kk+M] ^ (x[kk] >> 1) ^ mag01[x[kk] % 2];
        }
        for (; kk<N;kk++) {
            x[kk] = x[kk+(M-N)] ^ (x[kk] >> 1) ^ mag01[x[kk] % 2];
        }
        k=0;
    }
    y = x[k];
    y ^= (y << 7) & 0x2b5b2500; /* s and b, magic vectors */
    y ^= (y << 15) & 0xdb8b0000; /* t and c, magic vectors */
    y &= 0xffffffff; /* you may delete this line if word size = 32 */
/* 
   the following line was added by Makoto Matsumoto in the 1996 version
   to improve lower bit's corellation.
   Delete this line to o use the code published in 1994.
*/
    y ^= (y >> 16); /* added to the 1994 version */
    k++;
    return( (double) y / (unsigned long) 0xffffffff);
}

/* 
   The Box-Muller method of generating a normal random variable with
   mean 0 and standard deviation of 1. To adjust to some other
   distribution, multiply by the standard deviation and add the mean.
*/

RQ_EXPORT double 
rq_random_normal() 
{
    static double v2;
    static double fac;
    static int flipflop = 0;
    double r;

    if (flipflop)
        r = v2 * fac;
    else
    {
        double s;
        double v1;

        do 
        {
            double u1 = (double)rand() / (double)RAND_MAX;
            double u2 = (double)rand() / (double)RAND_MAX;

            v1 = 2.0 * u1 - 1.0;
            v2 = 2.0 * u2 - 1.0;
            s = (v1 * v1) + (v2 * v2);
        } 
        while(s >= 1.0);

        fac = sqrt(-2.0 * log(s) / s);
        r = v1 * fac;
    }

    flipflop = !flipflop;

    return r;
}

RQ_EXPORT double
rq_random_poisson(double xm)
{
    static double sq;
    static double alxm;
    static double g;
    static double oldm;

    double em;

    if (xm < 12.0)
    {
        /* use direct method */

        double t = 1.0;

        if (xm != oldm)
        {
            oldm = xm;
            g = exp(-xm);
        }

        em = -1;

        do
        {
            ++em;
            t *= rq_random_normal();
        }
        while (t > g);
    }
    else
    {
        double t;

        if (xm != oldm)
        {
            oldm = xm;
            sq = sqrt(2.0 * xm);
            alxm = log(xm);
            g = xm * alxm - rq_log_gamma(xm + 1.0);
        }

        do
        {
            double y;

            do
            {
                y = tan(M_PI * rq_random_normal());
                em = sq * y + xm;
            }
            while (em < 0.0);

            em = floor(em);

            t = 0.9 * (1.0 + y * y) * exp(em * alxm - rq_log_gamma(em + 1.0) - g);
        }
        while (rq_random_normal() > t);
    }

    return em;
}

