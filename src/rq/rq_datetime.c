/*
** rq_datetime.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2004-2008 Brett Hutley
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
#include "rq_datetime.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

RQ_EXPORT rq_datetime 
rq_datetime_from_dmyhms(short day, short month, short year, rq_hour hour, rq_minute minute, rq_second second)
{
    rq_datetime dt;

    dt.date = rq_date_from_dmy(day, month, year);
    dt.time = rq_time_from_hms(hour, minute, second);

    return dt;
}

RQ_EXPORT short
rq_datetime_to_dmyhms(
    rq_datetime dt, 
    short *day, 
    short *mon, 
    short *year,
    rq_hour *hour,
    rq_minute *minute,
    rq_second *second
    )
{
    short failed = rq_date_to_dmy(dt.date, day, mon, year);

    failed = (rq_time_to_hms(dt.time, hour, minute, second) != 0 ||
              failed != 0);

    return failed;
}

rq_datetime 
rq_datetime_now()
{
    rq_datetime now;
    time_t t = time(0);
    struct tm *tm = localtime(&t);

    now.date = rq_date_from_dmy((short)tm->tm_mday, (short)(tm->tm_mon+1), (short)(tm->tm_year + 1900));
    now.time = rq_time_from_hms((short)tm->tm_hour, (short)tm->tm_min, (short)tm->tm_sec);

    return now;
}

rq_datetime 
rq_datetime_next_with_accuracy(rq_datetime from, enum rq_datetime_accuracy accuracy)
{
    rq_datetime ret;
    unsigned hours = rq_time_get_hour(from.time);
    unsigned minutes = rq_time_get_minute(from.time);
    unsigned num_intervals = (unsigned) ((double)minutes / (double)accuracy);
    num_intervals++;
    ret.date = from.date;
    minutes = num_intervals * (unsigned)accuracy;
    if (minutes >= 60)
    {
        hours++;
        minutes -= 60;

        if (hours >= 24)
        {
            hours -= 24;
            ret.date++;
        }
    }

    ret.time = rq_time_from_hms(hours, minutes, 0);

    return ret;
}


