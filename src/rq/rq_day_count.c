/*
** rq_day_count.c
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
#include "rq_day_count.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>

static rq_calendar_t rq_day_count_bus_252_calendar;

/* Set the calendar to be used for BUS_252 daycount calculation. */
RQ_EXPORT void
rq_day_count_set_bus_252_calendar(rq_calendar_t cal)
{
    rq_day_count_bus_252_calendar = cal;
}

static double rq_day_count_bus_252(rq_date start_date, rq_date end_date)
{
    double days_in_period;
    if(rq_day_count_bus_252_calendar)
        days_in_period = rq_calendar_businessday_count(rq_day_count_bus_252_calendar, start_date+1, end_date);
    else // If no calendar set then just calculate workdays difference.
        days_in_period = rq_date_weekday_diff(start_date, end_date);
    return days_in_period;
}

/* Note that some can be done in Excel using : */
/* 30_360        as YEARFRAC(d1,d2,0) */
/* ACTUAL_ACTUAL? as YEARFRAC(d1,d2,1) ? unknown*/
/* ACTUAL_360    as YEARFRAC(d1,d2,2) */
/* ACTUAL_365    as YEARFRAC(d1,d2,3) */
/* E30_360       as YEARFRAC(d1,d2,4) */

/* Tweaked for performance. */
RQ_EXPORT double
rq_day_count_get_year_fraction(
    enum rq_day_count_convention day_count_convention,
    rq_date start_date,
    rq_date end_date
    )
{
    double days_in_period = end_date - start_date;
    double days_in_year;

    switch (day_count_convention)
    {
        case RQ_DAY_COUNT_ACTUAL_360:
            days_in_year = 360.0;
            break;

        case RQ_DAY_COUNT_30_360:
        case RQ_DAY_COUNT_E30_360:
        {
			short start_day_of_month, start_month, start_year;
			short end_day_of_month, end_month, end_year;
            int num_months = rq_date_month_diff(start_date, end_date);

            rq_date_to_dmy(start_date, &start_day_of_month, &start_month, &start_year);
            rq_date_to_dmy(end_date, &end_day_of_month, &end_month, &end_year);

            /* All months are assumed to have 30 days in them. 
               16-jun-2000 - 2-mar-2001
               30-16 = 14
               2 = 2

               num_months = jjasondjfm
               1234567890
               = 10

               day_count = (9 * 30) + 2 + 14
            */

			/*! RQ_DAY_COUNT_30_360 implies ISDA 2006 definition */
            if (day_count_convention == RQ_DAY_COUNT_30_360)
            {
                if (start_day_of_month > 29)
                {
                    /* Only set end day of month to 30 if start day of month is 30 or 31.
                     */
                    if (end_day_of_month == 31)
                        end_day_of_month = 30;

                    start_day_of_month = 30;
                }
            }
            else
            {
                if (start_day_of_month == 31)
                    start_day_of_month = 30;

                if (end_day_of_month == 31)
                    end_day_of_month = 30;
            }

            days_in_period = num_months * 30 + end_day_of_month - start_day_of_month;
            days_in_year = 360.0;
        }
        break;

        case RQ_DAY_COUNT_ACTUAL_ACTUAL_ISMA:
        case RQ_DAY_COUNT_ACTUAL_ACTUAL:
        {
            if (days_in_period >= 28 && days_in_period <= 31)
                days_in_year = days_in_period * 12;
            else if (days_in_period >= (28*2) && days_in_period <= (31*2))
                days_in_year = days_in_period * 6;
            else if (days_in_period >= (29*3) && days_in_period <= (31*3))
                days_in_year = days_in_period * 4;
            else if (days_in_period >= (29*6) && days_in_period <= (31*6))
                days_in_year = days_in_period * 2;
            else if (days_in_period >= (29*12) && days_in_period <= (31*12))
                days_in_year = days_in_period;
            else
                days_in_year = 365.0;
        }
        break;

        case RQ_DAY_COUNT_ACTUAL_365_25:
            days_in_year = 365.25;
            break;

        case RQ_DAY_COUNT_ACTUAL_365:
        case RQ_DAY_COUNT_ACTUAL_365_FIXED:
            return days_in_period / 365.0;

        case RQ_DAY_COUNT_ACTUAL_ACTUAL_ISDA:
        {
            double start_year = rq_date_get_year(start_date);
            double end_year = rq_date_get_year(end_date);

            double num_years = end_year - start_year;

            if (num_years > 0)
            {
                double days_left_in_start = 
                    rq_date_from_dmy(1, 1, (short)(start_year+1)) - start_date;
                double days_left_in_end = 
                    end_date - rq_date_from_dmy(1, 1, (short)end_year);
                double y1 = (rq_date_is_leap_year((short)start_year) ? 366.0 : 365.0);
                double y2 = (rq_date_is_leap_year((short)end_year) ? 366.0 : 365.0);

                double year_frac1 = days_left_in_start / y1;
                double year_frac2 = days_left_in_end / y2;
                double day_count_frac = year_frac1 + year_frac2 + (num_years-1);

                /* NOTE! I'm returning here */
                return day_count_frac;
            }
            else
            {
                if (rq_date_is_leap_year(rq_date_get_year(start_date)))
                    days_in_year = 366.0;
                else
                    days_in_year = 365.0;
            }
        }
        break;

        case RQ_DAY_COUNT_ACTUAL_ACTUAL_AFB:
        {
            /* Count the number of years between start and end dates */
            double years = floor(((double)(end_date - start_date)) / 366.0);
            short start_year = rq_date_get_year(start_date);
            days_in_year = 365.0;
            if (years > 0)
            {
                short end_day_of_month, end_month, end_year;
                short new_year;
                rq_date new_date;

                rq_date_to_dmy(end_date, &end_day_of_month, &end_month, &end_year);
                new_year = end_year - (short)years;

                if (end_month == 2)
                {
                    if (end_day_of_month == 29)
                    {
                        if (!rq_date_is_leap_year(new_year))
                            end_day_of_month = 28;
                    }
                    else if (end_day_of_month == 28)
                    {
                        if (rq_date_is_leap_year(new_year))
                            end_day_of_month = 29;
                    }
                }

                new_date = rq_date_from_dmy(end_day_of_month, end_month, new_year);
                if (new_date > start_date)
                {
                    rq_date feb29 = 0;
                    if (rq_date_is_leap_year(new_year))
                        feb29 = rq_date_from_dmy(29, 2, new_year);
                    else if (rq_date_is_leap_year(start_year))
                        feb29 = rq_date_from_dmy(29, 2, start_year);

                    if (new_date > feb29 && start_date <= feb29)
                        days_in_year = 366.0;

                    days_in_period = new_date - start_date;
                    return years + (days_in_period / days_in_year);
                }
            }
            else if (rq_date_is_leap_year(start_year))
            {
                rq_date feb29 = rq_date_from_dmy(29, 2, start_year);

                if (end_date > feb29 && start_date <= feb29)
                    days_in_year = 366.0;
            }

        }
        break;

        case RQ_DAY_COUNT_ACTUAL_365_END:
            days_in_year = 365.0;
            if (rq_date_is_leap_year(rq_date_get_year(end_date)))
                days_in_year = 366.0;
            break;

        /* Brazilian Real bus days / 252. */
        case RQ_DAY_COUNT_BUS_252:
            days_in_year = 252.0;
            days_in_period = rq_day_count_bus_252(start_date, end_date);
            break;

        case RQ_DAY_COUNT_ACTUAL_365_ACTUAL:
        default:
            days_in_year = 365.0;
            if (rq_date_is_leap_year(rq_date_get_year(start_date)))
                days_in_year = 366.0;
            break;
    }

    return days_in_period / days_in_year;
}

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
    )
{
	double n, f, d;
	int months_per_period = 12/(int)periods_per_year;
	rq_date first_coupon_date;
	rq_date prev_date = end_date;
	n = 0;
	prev_date = rq_date_add_months(prev_date, -months_per_period, 0);
	while (prev_date >= start_date)
	{
		prev_date = rq_date_add_months(prev_date, -months_per_period, 0);
		n++;
	}
	first_coupon_date = rq_date_add_months(prev_date, months_per_period, 0);
	f = first_coupon_date - start_date;
	d = first_coupon_date - prev_date;

	*p_n = n;
	*p_f = f;
	*p_d = d;
	return (n + (f/d));
}

/* rq_day_count_get_year_fraction() with debugging. */
RQ_EXPORT double
rq_day_count_get_year_fraction_nfd(
    enum rq_day_count_convention day_count_convention,
    rq_date start_date,
    rq_date end_date,
	double* p_num_years,
	double* p_days_in_period,
	double* p_days_in_year
    )
{
    double years_addon = 0.0;
    double days_in_period = end_date - start_date;
    double days_in_year = 365.0;

    switch (day_count_convention)
    {
        case RQ_DAY_COUNT_ACTUAL_360:
            days_in_year = 360.0;
            break;

        case RQ_DAY_COUNT_30_360:
        case RQ_DAY_COUNT_E30_360:
        {
			short start_day_of_month, start_month, start_year;
			short end_day_of_month, end_month, end_year;
            int num_months = rq_date_month_diff(start_date, end_date);

            rq_date_to_dmy(start_date, &start_day_of_month, &start_month, &start_year);
            rq_date_to_dmy(end_date, &end_day_of_month, &end_month, &end_year);

            /* All months are assumed to have 30 days in them. 
               16-jun-2000 - 2-mar-2001
               30-16 = 14
               2 = 2

               num_months = jjasondjfm
               1234567890
               = 10

               day_count = (9 * 30) + 2 + 14
            */

			/*! RQ_DAY_COUNT_30_360 implies ISDA 2006 definition */
            if (day_count_convention == RQ_DAY_COUNT_30_360)
            {
                if (start_day_of_month > 29)
                {
                    /* Only set end day of month to 30 if start day of month is 30 or 31.
                     */
                    if (end_day_of_month == 31)
                        end_day_of_month = 30;

                    start_day_of_month = 30;
                }
            }
            else
            {
                if (start_day_of_month == 31)
                    start_day_of_month = 30;

                if (end_day_of_month == 31)
                    end_day_of_month = 30;
            }

            days_in_period = num_months * 30 + end_day_of_month - start_day_of_month;
            days_in_year = 360.0;
        }
        break;

        case RQ_DAY_COUNT_ACTUAL_ACTUAL_ISMA:
        case RQ_DAY_COUNT_ACTUAL_ACTUAL:
        {
            if (days_in_period >= 28 && days_in_period <= 31)
                days_in_year = days_in_period * 12;
            else if (days_in_period >= (28*2) && days_in_period <= (31*2))
                days_in_year = days_in_period * 6;
            else if (days_in_period >= (29*3) && days_in_period <= (31*3))
                days_in_year = days_in_period * 4;
            else if (days_in_period >= (29*6) && days_in_period <= (31*6))
                days_in_year = days_in_period * 2;
            else if (days_in_period >= (29*12) && days_in_period <= (31*12))
                days_in_year = days_in_period;
        }
        break;

        case RQ_DAY_COUNT_ACTUAL_365_25:
            days_in_year = 365.25;
            break;

        case RQ_DAY_COUNT_ACTUAL_365:
        case RQ_DAY_COUNT_ACTUAL_365_FIXED:
            days_in_year = 365.0;
            break;

        case RQ_DAY_COUNT_ACTUAL_ACTUAL_ISDA:
        {
            double start_year = rq_date_get_year(start_date);
            double end_year = rq_date_get_year(end_date);

            double num_years = end_year - start_year;

            if (num_years > 0)
            {
                double days_left_in_start = 
                    rq_date_from_dmy(1, 1, (short)(start_year+1)) - start_date;
                double days_left_in_end = 
                    end_date - rq_date_from_dmy(1, 1, (short)end_year);
                double y1 = (rq_date_is_leap_year((short)start_year) ? 366.0 : 365.0);
                double y2 = (rq_date_is_leap_year((short)end_year) ? 366.0 : 365.0);

                double year_frac1 = days_left_in_start / y1;
                double year_frac2 = days_left_in_end / y2;
                double day_count_frac = year_frac1 + year_frac2 + (num_years-1);

                /* NOTE! I'm returning here */
                return day_count_frac;
            }
            else
            {
                if (rq_date_is_leap_year(rq_date_get_year(start_date)))
                    days_in_year = 366.0;
                else
                    days_in_year = 365.0;
            }
        }
        break;

        case RQ_DAY_COUNT_ACTUAL_ACTUAL_AFB:
        {
            /* Count the number of years between start and end dates */
            double years = floor(((double)(end_date - start_date)) / 366.0);
            short start_year = rq_date_get_year(start_date);
            if (years > 0)
            {
                short end_day_of_month, end_month, end_year;
                short new_year;
                rq_date new_date;

                rq_date_to_dmy(end_date, &end_day_of_month, &end_month, &end_year);
                new_year = end_year - (short)years;

                if (end_month == 2)
                {
                    if (end_day_of_month == 29)
                    {
                        if (!rq_date_is_leap_year(new_year))
                            end_day_of_month = 28;
                    }
                    else if (end_day_of_month == 28)
                    {
                        if (rq_date_is_leap_year(new_year))
                            end_day_of_month = 29;
                    }
                }

                new_date = rq_date_from_dmy(end_day_of_month, end_month, new_year);
                if (new_date > start_date)
                {
                    rq_date feb29 = 0;
                    if (rq_date_is_leap_year(new_year))
                        feb29 = rq_date_from_dmy(29, 2, new_year);
                    else if (rq_date_is_leap_year(start_year))
                        feb29 = rq_date_from_dmy(29, 2, start_year);

                    if (new_date > feb29 && start_date <= feb29)
                        days_in_year = 366.0;

                    years_addon = years;
                    days_in_period = new_date - start_date;
                }
            }
            else if (rq_date_is_leap_year(start_year))
            {
                rq_date feb29 = rq_date_from_dmy(29, 2, start_year);

                if (end_date > feb29 && start_date <= feb29)
                    days_in_year = 366.0;
            }

        }
        break;

        case RQ_DAY_COUNT_ACTUAL_365_END:
            if (rq_date_is_leap_year(rq_date_get_year(end_date)))
                days_in_year = 366.0;
            break;

        /* Brazilian Real bus days / 252. */
        case RQ_DAY_COUNT_BUS_252:
            days_in_year = 252.0;
            days_in_period = rq_day_count_bus_252(start_date, end_date);
            break;

        case RQ_DAY_COUNT_ACTUAL_365_ACTUAL:
        default:
            if (rq_date_is_leap_year(rq_date_get_year(start_date)))
                days_in_year = 366.0;
            break;
    }

	if (p_num_years) *p_num_years = years_addon; 
	if (p_days_in_period) *p_days_in_period = days_in_period;
	if (p_days_in_year) *p_days_in_year = days_in_year;

    return years_addon + (days_in_period / days_in_year);
}
