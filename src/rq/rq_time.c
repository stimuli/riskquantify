/*
** rq_time.c
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
#include "rq_time.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

RQ_EXPORT void
rq_time_init(rq_time *tm)
{
    tm->t[0] = tm->t[1] = tm->t[2] = tm->t[3] = 0;
}

RQ_EXPORT short 
rq_time_is_valid(rq_time tm)
{
    /* Should I put more tests here? */
    return 
        tm.t[3] == 0x01;
}

RQ_EXPORT rq_time 
rq_time_from_hms(rq_hour hour, rq_minute minute, rq_second second)
{
    rq_time tm;

    /* The condition that these variables are greater than zero is
       enforced by the fact that the type is unsigned.
    */
    if (hour < 24 && minute < 60 && second < 60)
    {
        unsigned char hh = (unsigned char)(hour / 10);
        unsigned char h = (unsigned char)(hour % 10);
        unsigned char mm = (unsigned char)(minute / 10);
        unsigned char m = (unsigned char)(minute % 10);
        unsigned char ss = (unsigned char)(second / 10);
        unsigned char s = (unsigned char)(second % 10);

        tm.t[0] = (hh << 4) | h;
        tm.t[1] = (mm << 4) | m;
        tm.t[2] = (ss << 4) | s;
        tm.t[3] = 0x01;
    }
    else
    {
        tm.t[0] = tm.t[1] = tm.t[2] = tm.t[3] = 0;
    }

    return tm;
}

RQ_EXPORT short 
rq_time_to_hms(
    rq_time time, 
    rq_hour *hour,
    rq_minute *minute,
    rq_second *second
    )
{
    short failed = 1;

    if (time.t[3] == 0x01)
    {
        *hour = ((time.t[0] >> 4) * 10) + (time.t[0] & 0x0f);
        *minute = ((time.t[1] >> 4) * 10) + (time.t[1] & 0x0f);
        *second = ((time.t[2] >> 4) * 10) + (time.t[2] & 0x0f);
        failed = 0;
    }

    return failed;
}

RQ_EXPORT rq_time
rq_time_parse(const char *s)
{
    rq_time tm;
    const char *first_delim;

    /* mark this structure as invalid */
    tm.t[3] = 0;

    first_delim = strchr(s, ':');
    if (first_delim)
    {
        const char *second_delim = strchr(first_delim+1, ':');
        if (second_delim && 
            second_delim == first_delim + 3 &&
            second_delim[1] != '\0')
        {
            char buf[3];
            rq_hour hh;
            rq_minute mm;
            rq_second ss;

            if (s < first_delim - 2)
                s = first_delim - 2;

            buf[0] = s[0];
            buf[1] = s[1];
            buf[2] = '\0';
            hh = (rq_hour) atoi(buf);

            buf[0] = first_delim[1];
            buf[1] = first_delim[2];
            /* no need to terminate we did it already */

            mm = (rq_minute) atoi(buf);
            
            buf[0] = second_delim[1];
            buf[1] = second_delim[2];

            ss = (rq_second) atoi(buf);

            tm = rq_time_from_hms(hh, mm, ss);
        }
    }

    return tm;
}

RQ_EXPORT char *
rq_time_to_string(char *buf, unsigned int buflen, const char *format, rq_time tm)
{
    char *p = buf;
    rq_hour hh;
    rq_minute mm;
    rq_second ss;

    if (buflen == 0)
        return NULL;

    buf[0] = '\0';
    if (tm.t[3] != 0x01)
        return buf;

    if (rq_time_to_hms(tm, &hh, &mm, &ss))
        return buf;

    while (*format && buflen > 1)
    {
        if (*format == '%')
        {
            char bb[3];
            char *b = bb;
            switch (*(format+1))
            {
                case 'H':
                    format += 2;
                    sprintf(b, "%02d", hh);
                    while (*b && buflen > 1)
                    {
                        *p++ = *b++;
                        buflen--;
                    }
                    break;

                case 'M':
                    format += 2;
                    sprintf(b, "%02d", mm);
                    while (*b && buflen > 1)
                    {
                        *p++ = *b++;
                        buflen--;
                    }
                    break;

                case 'S':
                    format += 2;
                    sprintf(b, "%02d", ss);
                    while (*b && buflen > 1)
                    {
                        *p++ = *b++;
                        buflen--;
                    }
                    break;

                default:
                    *p++ = *format++;
                    buflen--;
                    break;
            }
        }
        else
        {
            *p++ = *format++;
            buflen--;
        }
    }

    *p = '\0';

    return buf;
}

RQ_EXPORT rq_time 
rq_time_now()
{
    time_t t = time(0);
    struct tm *tm = localtime(&t);

    return rq_time_from_hms((rq_hour)tm->tm_hour, (rq_minute)tm->tm_min, (rq_second)tm->tm_sec);
}

RQ_EXPORT unsigned 
rq_time_get_hour(rq_time tm)
{
    unsigned hour = 0;

    if (tm.t[3] == 0x01)
        hour = ((tm.t[0] >> 4) * 10) + (tm.t[0] & 0x0f);

    return hour;
}

RQ_EXPORT unsigned 
rq_time_get_minute(rq_time tm)
{
    unsigned minute = 0;

    if (tm.t[3] == 0x01)
        minute = ((tm.t[1] >> 4) * 10) + (tm.t[1] & 0x0f);

    return minute;
}

RQ_EXPORT unsigned 
rq_time_get_second(rq_time tm)
{
    unsigned second = 0;

    if (tm.t[3] == 0x01)
        second = ((tm.t[2] >> 4) * 10) + (tm.t[2] & 0x0f);

    return second;
}

RQ_EXPORT int 
rq_time_compare(rq_time tm1, rq_time tm2)
{
    if (tm1.t[0] != tm2.t[0])
        return tm1.t[0] - tm2.t[0];
    if (tm1.t[1] != tm2.t[1])
        return tm1.t[1] - tm2.t[1];
    return tm1.t[2] - tm2.t[2];
}

#ifdef DEBUG_RQ_TIME
int
main(int argc, char **argv)
{
    char buf[1024];

    while (1)
    {
        printf("Enter time string: ");
        if (fgets(buf, 1024, stdin))
        {
            rq_time tm;

            char *c = strchr(buf, '\r');
            if (c)
                *c = '\0';
            c = strchr(buf, '\n');
            if (c)
                *c = '\0';

            tm = rq_time_parse(buf);
            if (rq_time_is_valid(tm))
            {
                printf("Valid time. Please input a format string: ");
                if (fgets(buf, 1024, stdin))
                {
                    char bufsz[1024];

                    c = strchr(buf, '\r');
                    if (c)
                        *c = '\0';
                    c = strchr(buf, '\n');
                    if (c)
                        *c = '\0';

                    printf("How big an output buffer: ");
                    if (fgets(bufsz, 1024, stdin))
                    {
                        unsigned int sz = (unsigned int)atoi(bufsz);
                        char *ob = NULL;
                        if (sz > 0)
                            ob = (char *)malloc(sz);

                        rq_time_to_string(ob, sz, buf, tm);
                        printf("Time: '%s'\n", ob);
                        if (ob)
                            free(ob);
                    }
                    else
                        break;
                }
                else
                    break;
            }
        }
        else
            break;
    }

    return 0;
}
#endif
