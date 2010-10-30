/*
** rq_datehour.c
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
#include "rq_datehour.h"
#include <stdlib.h>
#include <string.h>
#include <time.h>

RQ_EXPORT rq_datehour 
rq_datehour_from_dmyh(short day, short month, short year, short hour)
{
    rq_datehour dt;

    dt.date = rq_date_from_dmy(day, month, year);
    dt.hour = hour % 24;

    return dt;
}

RQ_EXPORT short
rq_datehour_to_dmyh(
    rq_datehour dt, 
    short *day, 
    short *mon, 
    short *year,
    short *hour
    )
{
    short failed = rq_date_to_dmy(dt.date, day, mon, year);

    *hour = dt.hour;

    return failed;
}

rq_datehour 
rq_datehour_now()
{
    rq_datehour now;
    time_t t = time(0);
    struct tm *tm = localtime(&t);

    now.date = rq_date_from_dmy((short)tm->tm_mday, (short)(tm->tm_mon+1), (short)(tm->tm_year + 1900));
    now.hour = (short)tm->tm_hour;

    return now;
}

short 
rq_datehour_get_day(rq_datehour dh)
{
    return rq_date_get_day(dh.date);
}

short 
rq_datehour_get_month(rq_datehour dh)
{
    return rq_date_get_month(dh.date);
}

short 
rq_datehour_get_year(rq_datehour dh)
{
    return rq_date_get_year(dh.date);
}

short 
rq_datehour_get_hour(rq_datehour dh)
{
    return dh.hour;
}
