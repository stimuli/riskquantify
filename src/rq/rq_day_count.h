/**
 * \file rq_day_count.h
 * \author Brett Hutley
 *
 * \brief This file contains functions for calculating the day count
 * fraction given a day count convention.
 */
/*
** rq_day_count.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#ifndef rq_day_count_h
#define rq_day_count_h

#include "rq_config.h"
#include "rq_date.h"
#include "rq_enum.h"
#include "rq_calendar.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* Set the calendar used for bus 252 day count calculation. */
RQ_EXPORT void
rq_day_count_set_bus_252_calendar(rq_calendar_t cal);

RQ_EXPORT double
rq_day_count_get_year_fraction(
    enum rq_day_count_convention day_count_convention,
    rq_date start_date,
    rq_date end_date
    );

// returns n+f/d
RQ_EXPORT double
rq_day_count_get_nfd(
    enum rq_day_count_convention day_count_convention,
    rq_date start_date,
    rq_date end_date,
	double periods_per_year,
	double* p_n,
	double* p_f,
	double* p_d
    );

RQ_EXPORT double
rq_day_count_get_year_fraction_nfd(
    enum rq_day_count_convention day_count_convention,
    rq_date start_date,
    rq_date end_date,
        double* num_years,
        double* days_in_period,
        double* days_in_year
    );

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
