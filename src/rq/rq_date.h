/**
 * \file rq_date.h
 * \author Brett Hutley
 *
 * \brief Provides date helper functions and the rq_date type.
 */
/*
** rq_date.h
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
#ifndef rq_date_h
#define rq_date_h

#include "rq_config.h"
#include "rq_enum.h"
#include "rq_term.h"


#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- types ------------------------------------------------------- */
typedef long rq_date;

/* -- defines ----------------------------------------------------- */

#ifndef RQ_DATE_END_OF_DAYS
#define RQ_DATE_END_OF_DAYS 2488069  /* 31-12-2099 */
#endif

#ifndef RQ_DATE_INVALID
#define RQ_DATE_INVALID  0L
#endif

/* If RQ_DMY_CACHE_SIZE or RQ_DMY_CACHE_START is changed then rq_dmy_cache will need to be regenerated.  */
#define RQ_DMY_CACHE_SIZE 36525 /* 36525 = 100 years. */
#define RQ_DMY_CACHE_START 2451545 /* START is 1/1/2000 = Julian 2451545 */

/* -- prototypes -------------------------------------------------- */

/** Construct a julian date from the day, month and year parameters.
 */
RQ_EXPORT rq_date rq_date_from_dmy(short day, short month, short year);

/** Convert a julian date into a day, month and year.
 *
 * @return 0 on success, non-zero on failure.
 */
RQ_EXPORT short 
rq_date_to_dmy(
    rq_date julian, 
    short *day, 
    short *mon, 
    short *year
    );

/** Calculate the day of the month from the julian date.
 */
RQ_EXPORT short rq_date_get_day(rq_date date);

/** Calculate the month of the year of the julian date.
 */
RQ_EXPORT short rq_date_get_month(rq_date date);

/** Calculate the year of the julian date.
 */
RQ_EXPORT short rq_date_get_year(rq_date date);

/** Is the julian date a weekend (Saturday or Sunday).
 */
#define rq_date_is_weekend(date) ((date)%7 >= 5)

/** Is the julian date a leap year.
 */
RQ_EXPORT short rq_date_is_leap_year(short yr);

/** Get the number of days for the month parameter.
 */
RQ_EXPORT short rq_date_get_days_in_month(short month, short year);

/** Calculate the date reached by adding a number of months to another date.
 *
 * Return the date calculated by adding the number of months to the
 * julian date passed as a parameter. If sticky_last_date is true, if
 * we are currently on the last day of the month, stay on the last day
 * of the month when calculating the return date. For example, if we
 * are on the 31st of March and we move forward one month move to the
 * 30th April. 
 */
RQ_EXPORT rq_date
rq_date_add_months(
    rq_date from_date, 
    short months_to_add, 
    short sticky_last_day
    );

/** Add a number of years to the julian date passed in. Return the
 * calculated date.  
 */
RQ_EXPORT rq_date
rq_date_add_years(
    rq_date from_date, 
    short years_to_add
    );

/** Return the julian date today.
 */
RQ_EXPORT rq_date rq_date_today();

/** Get the (english) short name for the passed in month.
 */
RQ_EXPORT const char *rq_date_get_month_short_name(int mon);

/** Convert a date to a string using the passed in format and putting
 * the resultant string in the 'buf' parameter. The return value is a
 * pointer to the result buffer.  
 */
RQ_EXPORT char *rq_date_to_string(char *buf, const char *format, rq_date date);

/** Get the day of the week of the passed in julian date.
 */
/* RQ_EXPORT short rq_date_get_day_of_week(rq_date date); */
#define rq_date_get_day_of_week(date) (((date)+1)%7)

/** Get the (english) short name for the day of the week of the passed
 * in date.  
 */
RQ_EXPORT const char *rq_date_get_short_day_of_week_string(rq_date date);

/** Get the (english) long name for the day of the week of the passed
 * in date.  
 */
RQ_EXPORT const char *rq_date_get_long_day_of_week_string(rq_date date);

/** Construct a julian date from the passed in excel julian date.
 */
RQ_EXPORT rq_date rq_date_from_excel(long ed);

/** Construct an excel julian date from the passed in Risk Quantify
 * julian date.
 */
RQ_EXPORT long rq_date_to_excel(rq_date date);

/** Convert the passed in day string to a day number (1-7).
 * This function checks english short names and long names.
 */
RQ_EXPORT int rq_date_day_str_to_num(const char *str);

/** Convert the passed in month string to a month number (1-12).
 * This function checks english short names and long names.
 */
RQ_EXPORT int rq_date_month_str_to_num(const char *str);

/** Construct a julian date by parsing the passed in date string.
 */
RQ_EXPORT rq_date rq_date_parse(const char *s, enum rq_date_format date_format);

/** Add a term period to a julian date. sticky_last_day_for_months passes a flag through to the rq_date_add_months function
 *  e.g. when adding 1 month, will roll back the date to the last day of the month rather than going to far.
 *  i.e. 30th Feb becomes 28th Feb rather than 2nd Mar
 */
RQ_EXPORT rq_date rq_date_add_term_sticky_month(rq_date date, const struct rq_term *term, short sticky_last_day_for_months);

/** Add a term period to a julian date.
 */
RQ_EXPORT rq_date rq_date_add_term(rq_date date, const struct rq_term *term);

/** Subtract a term period from a julian date.
 */
RQ_EXPORT rq_date 
rq_date_subtract_term(rq_date date, const struct rq_term *term);

/** Calculate the number of days between two julian dates
 */
RQ_EXPORT int rq_date_diff(rq_date d1, rq_date d2);

/** Calculate the number of months between two julian dates
 */
RQ_EXPORT int rq_date_month_diff(rq_date d1, rq_date d2);

/** Calculate the number of weekdays (Mon-Fri) between two julian dates
 */
RQ_EXPORT int rq_date_weekday_diff(rq_date d1, rq_date d2);

/* Convert Julian day to number of days excluding Saturday and Sunday. */
RQ_EXPORT rq_date rq_date_weekdays(rq_date d);

/** Calculate the number of periods per year, given two julian dates.
 *
 * The function will try and find the closest match. If the dates
 * don't fit any of the possibilities defined by the enum
 * rq_periods_per_year, the function returns
 * RQ_PERIODS_PER_YEAR_INVALID.
 *
 * NOTE: This function isn't exact and takes allowance for possible
 * holidays. I am assuming that if you call it, you are fully aware of
 * it's limitations!
 */
RQ_EXPORT enum rq_periods_per_year 
rq_date_calc_periods_per_year(rq_date d1, rq_date d2);

#ifdef _DEBUG
/** Set a long result to represent the date in YYYYMMDD format.
 */
RQ_EXPORT long 
rq_date_debug(rq_date d);
#endif

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
