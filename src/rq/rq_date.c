/*
** rq_date.c
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
#include "rq_date.h"
#include "rq_tokenizer.h"
#include "rq_math.h"
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <math.h>
#include <string.h>
#include <ctype.h>

static long rq_date_excel_date_offset = 2415019L;

static unsigned char rq_date_days_in_month[12] = {
    31,	/* Jan */
    28,	/* Feb */
    31,	/* Mar */
    30,	/* Apr */
    31,	/* May */
    30,	/* Jun */
    31,	/* Jul */
    31,	/* Aug */
    30,	/* Sep */
    31,	/* Oct */
    30,	/* Nov */
    31	/* Dec */
};


static const char *rq_date_long_day_of_week[] = {
    "Sunday",
    "Monday",
    "Tuesday",
    "Wednesday",
    "Thursday",
    "Friday",
    "Saturday"
};

static const char *rq_date_short_day_of_week[] = {
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat"
};

static const char *rq_date_short_months[12] = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

static const char * rq_date_long_months[12] = {
    "January",
    "February",
    "March",
    "April",
    "May",
    "June",
    "July",
    "August",
    "September",
    "October",
    "November",
    "December"
};

RQ_EXPORT rq_date
rq_date_from_dmy(short day, short month, short year)
{
	rq_date julian = 367 * year - 7 * (year + (month + 9) / 12) / 4 + 275 * month / 9 + day + 1721014;
	if (100 * year + month <= 190002) julian += 1;
	return julian;
}

#ifdef RQ_USE_DATE_CACHE
struct rq_JulianDMY {
    short year;
    char month;
    char day;
};

static struct rq_JulianDMY rq_dmy_cache[RQ_DMY_CACHE_SIZE] = {
#include "rq_date_cache_initialisation.h"
};
#endif

RQ_EXPORT short
rq_date_to_dmy(
    rq_date julian, 
    short *day, 
    short *mon, 
    short *year
    )
{
#ifdef RQ_USE_DATE_CACHE
    int cache_pos = julian - RQ_DMY_CACHE_START;
    if(cache_pos >= 0 && cache_pos < RQ_DMY_CACHE_SIZE)
    {
        *year = rq_dmy_cache[cache_pos].year;
        *mon = rq_dmy_cache[cache_pos].month;
        *day = rq_dmy_cache[cache_pos].day;
        return 0;
    }
#endif

    /* Cache lookup failed, continue with slower code. */
    /* ... Normal rq_date_to_dmy() code ... */

    if (!julian)
        return 1;

    {
        int l = julian + 68569;
        int n = 4 * l / 146097;
        int it;
        l = l - (146097 * n + 3) / 4;
        it = 4000 * (l + 1) / 1461001;
        l = l - 1461 * it / 4 + 31;
        *mon = 80 * l / 2447;
        *day = l - 2447 * (*mon) / 80;
        l = (*mon) / 11;
        *mon = (*mon) + 2 - 12 * l;
        *year = 100 * (n - 49) + it + l;
    }

    return 0;
}


RQ_EXPORT short 
rq_date_get_day(rq_date date)
{
    short d, m, y;
    rq_date_to_dmy(date, &d, &m, &y);
    return d;
}

RQ_EXPORT short 
rq_date_get_month(rq_date date)
{
    short d, m, y;
    rq_date_to_dmy(date, &d, &m, &y);
    return m;
}

RQ_EXPORT short 
rq_date_get_year(rq_date date)
{
    short d, m, y;
    rq_date_to_dmy(date, &d, &m, &y);
    return y;
}

RQ_EXPORT short
rq_date_is_leap_year(short yr)
{
  return yr % 400 == 0 || (yr % 4 == 0 && yr % 100 != 0);
}

/*
RQ_EXPORT short
rq_date_is_weekend(rq_date date)
{
    short day_of_week = rq_date_get_day_of_week(date);
    return 
        day_of_week == RQ_DAY_OF_WEEK_SATURDAY || 
        day_of_week == RQ_DAY_OF_WEEK_SUNDAY;
}
*/

RQ_EXPORT short 
rq_date_get_days_in_month(short month, short year)
{
    assert(month >= 1 && month <= 12);
    if (month == 2 && rq_date_is_leap_year(year))
        return 29;
    return rq_date_days_in_month[month-1];
}

RQ_EXPORT rq_date
rq_date_add_months(
    rq_date from_date, 
    short months_to_add, 
    short sticky_last_day
    )
{
    if (months_to_add)
    {
        short months = abs(months_to_add);
        short day, mon, year;
        short new_month;
        short days_in_month;

        rq_date_to_dmy(from_date, &day, &mon, &year);
        if (months_to_add > 0)
        {
            new_month = ((mon + months - 1) % 12) + 1;
            year += (mon + months - 1) / 12;
        }
        else
        {
            new_month = months >= mon ? 12 + (mon - months) % 12 : (12 + mon - months) % 12;
            year += months >= mon ? (int)(((int)(mon - months)) / 12.0) - 1 : 0;
        }

        days_in_month = rq_date_get_days_in_month(new_month, year);
        if (day > days_in_month)
        {
            if (sticky_last_day)
                day = days_in_month;
            else
            {
                int days_to_add = day - days_in_month;
                from_date = rq_date_from_dmy(days_in_month, new_month, year);
                from_date += days_to_add;
				/* I hate putting more than one return
				   in a function, but this route is done...
				*/
                return from_date;
            }
        }

        from_date = rq_date_from_dmy(day, new_month, year);
    }

    return from_date;
}

RQ_EXPORT rq_date
rq_date_today()
{
    time_t t = time(0);
    struct tm *tm = localtime(&t);

    return rq_date_from_dmy((short)tm->tm_mday, (short)(tm->tm_mon+1), (short)(tm->tm_year + 1900));
}


RQ_EXPORT const char *
rq_date_get_month_short_name(int mon)
{
    if (mon >= 1 && mon <= 12)
        return rq_date_short_months[mon - 1];
    return NULL;
}

RQ_EXPORT char *
rq_date_to_string(char *buf, const char *format, rq_date date)
{
    unsigned int i;
    char *p = buf;
    short day, month, year;

    *buf = '\0';

    rq_date_to_dmy(date, &day, &month, &year);

    for (i = 0; i < strlen(format); i++)
    {
        if (tolower(format[i]) == 'd')
        {
            /* see how many d's there are... */
            if (tolower(format[i+1]) == 'd')
            {
                sprintf(p, "%02d", day);
                i++; /* eat up second 'd' */
            }
            else
                sprintf(p, "%d", day);
        }
        else if (tolower(format[i]) == 'm')
        {
            /* see how many 'm's there are... */
            if (!memcmp(format+i, "mmm", 3))
            {
                /* three 'm's... */
                strcpy(p, rq_date_get_month_short_name(month));
                i+=2; /* eat up 'm's */
            }
            else if (!memcmp(format+i, "mm", 2))
            {
                /* two m's */
                sprintf(p, "%02d", month);
                i++; /* eat up m's */
            }
            else
            {
                sprintf(p, "%d", month);
            }
        }
        else if (tolower(format[i]) == 'y')
        {
            if (!memcmp(format+i, "yyyy", 4))
            {
                sprintf(p, "%d", year);
                i+=3;
            }
            else if (!memcmp(format+i, "yy", 2))
            {
                short y = (year >= 2000 ? year - 2000 : year - 1900);
                sprintf(p, "%d", y);
                i++;
            }
            else
            {
                sprintf(p, "%d", year);
            }
        }
        else
        {
            *p = format[i];
            *(p + 1) = '\0';
        }

        p = p + strlen(p);
    }

    return buf;
}

/* This has been inlined.
RQ_EXPORT short
rq_date_get_day_of_week(rq_date date)
{
    return ((date + 1) % 7);
}
*/

RQ_EXPORT const char *
rq_date_get_short_day_of_week_string(rq_date date)
{
    return rq_date_short_day_of_week[rq_date_get_day_of_week(date)];
}

RQ_EXPORT const char *
rq_date_get_long_day_of_week_string(rq_date date)
{
    return rq_date_long_day_of_week[rq_date_get_day_of_week(date)];
}

RQ_EXPORT rq_date
rq_date_from_excel(long ed)
{
    return ed + rq_date_excel_date_offset;
}

RQ_EXPORT long
rq_date_to_excel(rq_date date)
{
    return date - rq_date_excel_date_offset;
}

RQ_EXPORT int
rq_date_day_str_to_num(const char *str)
{
    int retval = 0;
    int i;

    for (i = 0; i < 7; i++)
        if (!stricmp(rq_date_long_day_of_week[i], str))
        {
            retval = i + 1;
            break;
        }
    if (!retval)
    {
        for (i = 0; i < 7; i++)
            if (!stricmp(rq_date_long_day_of_week[i], str))
            {
                retval = i + 1;
                break;
            }
    }
    return retval;
}

RQ_EXPORT int
rq_date_month_str_to_num(const char *str)
{
    int retval = 0;
    int i;

    for (i = 0; i < 12; i++)
        if (!stricmp(rq_date_short_months[i], str))
        {
            retval = i + 1;
            break;
        }
    if (!retval)
    {
        for (i = 0; i < 12; i++)
            if (!stricmp(rq_date_long_months[i], str))
            {
                retval = i + 1;
                break;
            }
    }
    return retval;
}

RQ_EXPORT rq_date
rq_date_parse(const char *s, enum rq_date_format date_format)
{
    const char * const delims = " /-.\\";
    const char *ptr = s;
    unsigned short found = 0;
    unsigned int i;
    short day, month, year;

    for (i = 0; i < strlen(delims); i++)
        if (strchr(ptr, delims[i]))
        {
            found = 1;
            break;
        }

    if (found)
    {
        char tok[255];
        char fld1[255], fld2[255], fld3[255];
        int f1, f2, f3;
        int check;

        for (i = 0; 
             rq_tokenizer_get_token(&ptr, tok, 255, delims, " \t", ""); 
             i++)
        {
            switch (i)
            {
                case 0:
                    strcpy(fld1, tok);
                    break;
			
                case 1:
                    strcpy(fld2, tok);
                    break;
			
                case 2:
                    strcpy(fld3, tok);
                    break;
			
                default:
                    return 0;
            }
        }

        f1 = atoi(fld1);
        f2 = atoi(fld2);
        f3 = atoi(fld3);
        check = !f1 + !f2 + !f3;
        if (check > 1)
        {
            return 0;
        }
        else if (check == 1)
        {
            int which_is_month = 0;
            if (!f1)
            {
                f1 = rq_date_month_str_to_num(fld1);
                which_is_month = 1;
            }
            else if (!f2)
            {
                f2 = rq_date_month_str_to_num(fld2);
                which_is_month = 2;
            }
            else if (!f3)
            {
                return 0;
				/* f3 = monthStrToNum(fld3); */
				/* which_is_month = 3; */
            }
            if (which_is_month == 1)
            {
                day = f2;
                month = f1;
            }
            else /*if (which_is_month == 2) */
            {
                day = f1;
                month = f2;
            }
            year = f3;
        }
        else
        {
            switch (date_format)
            {
                case RQ_DATE_FORMAT_DMY:
                    day = f1;
                    month = f2;
                    year = f3;
                    break;

                case RQ_DATE_FORMAT_MDY:
                    month = f1;
                    day = f2;
                    year = f3;
                    break;

                case RQ_DATE_FORMAT_YMD:
                    year = f1;
                    month = f2;
                    day = f3;
                    break;

                default:
                    assert(0);
                    break;
            }
        }
    }
    else /* no delimiters found... */
    {
        char buf[4];
        day = atoi(ptr);
        memcpy(buf, ptr+2, 3);
        buf[3] = '\0';
        month = rq_date_month_str_to_num(buf);
        year = atoi(ptr+5);
    }

    if (year < 70)
        year += 2000;
    else if (year < 100)
        year += 1900;

    return rq_date_from_dmy(day, month, year);
}

RQ_EXPORT rq_date 
rq_date_add_term(rq_date date, const struct rq_term *term)
{
	return rq_date_add_term_sticky_month( date, term, 0 );
}

RQ_EXPORT rq_date rq_date_add_term_sticky_month(rq_date date, const struct rq_term *term, short sticky_last_day_for_months)
{
	date += term->days + (term->weeks * 7);
    date = rq_date_add_months(date, term->months, sticky_last_day_for_months);
    date = rq_date_add_years(date, term->years);

    return date;
}

RQ_EXPORT rq_date 
rq_date_subtract_term(rq_date date, const struct rq_term *term)
{
    date -= term->days - (term->weeks * 7);
    date = rq_date_add_months(date, -term->months, 0);
    date = rq_date_add_years(date, -term->years);

    return date;
}


RQ_EXPORT rq_date
rq_date_add_years(
    rq_date from_date, 
    short years_to_add
    )
{
    short day, month, year;

    rq_date_to_dmy(from_date, &day, &month, &year);
    year += years_to_add;
    return rq_date_from_dmy(day, month, year);
}

RQ_EXPORT int 
rq_date_diff(rq_date d1, rq_date d2)
{
    return d1 - d2;
}

/* Convert Julian day to number of days excluding Saturday and Sunday. */
RQ_EXPORT rq_date
rq_date_weekdays(rq_date d)
{
    return d/7*5 + MIN(d%7, 4);
}

RQ_EXPORT int
rq_date_weekday_diff(rq_date d1, rq_date d2)
{
    return rq_date_weekdays(d2) - rq_date_weekdays(d1);
}


RQ_EXPORT int 
rq_date_month_diff(rq_date d1, rq_date d2)
{
    rq_date tmp;
    short day1, month1, year1;
    short day2, month2, year2;
    int num_months;
    int num_years;
    short swapped = 0;

    if (d2 < d1)
    {
        tmp = d1;
        d1 = d2;
        d2 = tmp;
        swapped = 1;
    }

    rq_date_to_dmy(d1, &day1, &month1, &year1);
    rq_date_to_dmy(d2, &day2, &month2, &year2);
    
    num_years = year2 - year1;
    if (month2 < month1)
    {
        /* m1 = 5, m2 = 3 (12-5)+3 */
        num_years--;
        num_months = (12 - month1) + month2;
    }
    else
        num_months = month2 - month1;

    if (num_years > 0)
        num_months += (12 * num_years);

    return (swapped ? -num_months : num_months);
}

RQ_EXPORT enum rq_periods_per_year 
rq_date_calc_periods_per_year(rq_date d1, rq_date d2)
{
    long diff = d2 - d1;
    enum rq_periods_per_year ret = RQ_PERIODS_PER_YEAR_INVALID;

    if (diff < 0)
        diff = -diff;

    if (diff >= 358 && diff <= 369)
        ret = RQ_PERIODS_PER_YEAR_YEARLY;
    else if (diff >= 172 && diff <= 188)
        ret = RQ_PERIODS_PER_YEAR_SEMI;
    else if (diff >= 82 && diff <= 95)
        ret = RQ_PERIODS_PER_YEAR_QUARTERLY;
    else if (diff >= 26 && diff <= 33)
        ret = RQ_PERIODS_PER_YEAR_MONTHLY;
    else if (diff >= 12 && diff <= 16)
        ret = RQ_PERIODS_PER_YEAR_FORTNIGHTLY;
    else if (diff >= 5 && diff <= 9)
        ret = RQ_PERIODS_PER_YEAR_WEEKLY;

    return ret;
}

#ifdef _DEBUG

RQ_EXPORT long 
rq_date_debug(rq_date d)
{
	short day, mon, year;
	long res;

	rq_date_to_dmy(d, &day, &mon, &year);

	res = year * 10000 + mon * 100 + day;

	return res;
}

#endif
