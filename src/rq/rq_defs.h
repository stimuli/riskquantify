/**
 * @file
 *
 * This header file provides some helper defines
 */
/*
** rq_defs.h
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
#ifndef rq_defs_h
#define rq_defs_h

/** The following macro is for testing whether an object is NULL, and
 * is DEPRECATED.
 */
#define RQ_IS_NULL(x) ( (x == NULL) )

/** The following macro initializes an object pointer to NULL, and is
 * DEPRECATED. 
 */
#define RQ_INIT(x) (x) = NULL

/** A boolean typedef.
 */
typedef enum {
    RQ_FALSE = 0,
    RQ_TRUE = 1
} rq_boolean;


/** Macros for double comparison.
 */
#define RQ_TINY_DOUBLE 0.000000001
#define RQ_DOUBLE_EQUAL(d, t) (d < t + RQ_TINY_DOUBLE && d > t - RQ_TINY_DOUBLE)
#define RQ_DOUBLE_GREATER_THAN(d, t) (d > t + RQ_TINY_DOUBLE)
#define RQ_DOUBLE_LESS_THAN(d, t) (d < t - RQ_TINY_DOUBLE)

/** Risk Quantify's identifiers.
  In Risk Quantify ID's are unsigned long, and an ID of zero is
  treated as *unassigned*.
*/
typedef unsigned long rq_id;
#define RQ_ID_UNASSIGNED    0

/** rq_error.
 */
typedef long rq_error_code;

/* Lengths */
#define RQ_LENGTH_ASSET_ID                  50
#define RQ_LENGTH_TERMSTRUCT_GROUP_ID       50
#define RQ_LENGTH_TERMSTRUCT_ID				100

#define RQ_MAX_TERMSTRUCT_SPECIFICATIONS    256

#define	RQ_MAX_ROLL_DATES		3072		/**< weekly rolls for 50 years */
#define	RQ_MAX_SWAP_RATES		RQ_MAX_TERMSTRUCT_SPECIFICATIONS		/**< swap rates in the termstructure */
#define	RQ_MAX_SPLINE_POINTS	(50 + RQ_MAX_SWAP_RATES)	/**< 50 short end points + swaps */


#define RQ_FACTOR_CACHE_SIZE                365*3

#define RQ_YIELD_CURVE_MAX_FACTORS 250
#define RQ_DIVIDEND_YIELD_FUNCTION_MAX_SIZE 250
#define RQ_CDS_CURVE_MAX_SPREADS 250
#define RQ_SPREAD_CURVE_MAX_SPREADS 250

#endif
