/**
 * \file rq_random.h
 * \author Brett Hutley
 *
 * \brief The rq_random files provide functions to implement
 * psuedo-randomness.
 */
/*
** rq_random.h
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

#ifndef rq_random_h
#define rq_random_h

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

#include "rq_config.h"

/** Pick a random number from the normal distribution.
 */
RQ_EXPORT double rq_random_normal();


/** Pick a random number from the poisson distribution.
 *
 * This implementation was based on the book
 * "Numerical Recipes in C".
 */
RQ_EXPORT double rq_random_poisson(double xm);

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif


#endif
