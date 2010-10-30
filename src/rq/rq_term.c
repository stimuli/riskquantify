/*
** rq_term.c
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
#include "rq_term.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_term_t
rq_term_alloc()
{
    return (rq_term_t)RQ_CALLOC(1, sizeof(struct rq_term));
}

RQ_EXPORT rq_term_t
rq_term_build(short days, short weeks, short months, short years)
{
    rq_term_t term = rq_term_alloc();
    rq_term_fill(term, days, weeks, months, years);
    return term;
}

RQ_EXPORT rq_term_t rq_term_clone(rq_term_t term)
{
    rq_term_t clone = (rq_term_t)RQ_MALLOC(sizeof(struct rq_term));
	rq_term_copy(clone, term);

    return clone;
}

RQ_EXPORT void 
rq_term_init(struct rq_term *term)
{
    term->days = term->weeks = term->months = term->years = 0;
}

RQ_EXPORT void 
rq_term_fill(
    struct rq_term *term, 
    short days, 
    short weeks,
    short months,
    short years
    )
{
    term->days = days;
    term->weeks = weeks;
    term->months = months;
    term->years = years;
}

RQ_EXPORT void
rq_term_copy(
    struct rq_term *term_dst,
    const struct rq_term *term_src
    )
{
    memcpy(term_dst, term_src, sizeof(struct rq_term));
}

RQ_EXPORT void
rq_term_add_term(
    struct rq_term *term,
    const struct rq_term *term_to_add
    )
{
    term->days += term_to_add->days;
    term->weeks += term_to_add->weeks;
    term->months += term_to_add->months;
    term->years += term_to_add->years;
}

RQ_EXPORT void
rq_term_subtract_term(
    struct rq_term *term,
    const struct rq_term *term_to_subtract
    )
{
    term->days -= term_to_subtract->days;
    term->weeks -= term_to_subtract->weeks;
    term->months -= term_to_subtract->months;
    term->years -= term_to_subtract->years;
}

RQ_EXPORT int 
rq_term_write_xml(struct rq_term *term, rq_stream_t stream)
{
    int bytes_written = rq_stream_write_string(stream, "<term>");
    bytes_written += rq_stream_write_string(stream, "<days>");
    bytes_written += rq_stream_write_integer_as_string(stream, term->days);
    bytes_written += rq_stream_write_string(stream, "</days>");
    bytes_written += rq_stream_write_string(stream, "<weeks>");
    bytes_written += rq_stream_write_integer_as_string(stream, term->weeks);
    bytes_written += rq_stream_write_string(stream, "</weeks>");
    bytes_written += rq_stream_write_string(stream, "<months>");
    bytes_written += rq_stream_write_integer_as_string(stream, term->months);
    bytes_written += rq_stream_write_string(stream, "</months>");
    bytes_written += rq_stream_write_string(stream, "<years>");
    bytes_written += rq_stream_write_integer_as_string(stream, term->years);
    bytes_written += rq_stream_write_string(stream, "</years>");
    bytes_written += rq_stream_write_string(stream, "</term>");
    return bytes_written;
}

RQ_EXPORT short 
rq_term_get_days(const struct rq_term *term)
{
    return term->days;
}

RQ_EXPORT short 
rq_term_get_weeks(const struct rq_term *term)
{
    return term->weeks;
}

RQ_EXPORT short 
rq_term_get_months(const struct rq_term *term)
{
    return term->months;
}

RQ_EXPORT short 
rq_term_get_years(const struct rq_term *term)
{
    return term->years;
}

RQ_EXPORT short 
rq_term_is_equal(const struct rq_term *term1, const struct rq_term *term2)
{
    if ( (term1->days == term2->days)
			&& (term1->weeks == term2->weeks)
			&& (term1->months == term2->months)
			&& (term1->years == term2->years)
			)
		return 1;
	else
		return 0;
}

RQ_EXPORT void 
rq_term_free(rq_term_t term)
{
   RQ_FREE(term);
}

RQ_EXPORT void 
rq_term_invert(struct rq_term *term)
{
    if (term->days)
        term->days = -term->days;
    if (term->weeks)
        term->weeks = -term->weeks;
    if (term->months)
        term->months = -term->months;
    if (term->years)
        term->years = -term->years;
}

RQ_EXPORT double
rq_term_get_annualized_period(const struct rq_term *term)
{
    if (term->days)
    {
        double days = term->days;
        days += (term->weeks * 7);
        days += (term->months * 30);
        days += (term->years * 365);
        return days / 365.0;
    }
    else if (term->weeks)
    {
        double weeks = term->weeks;
        weeks += (term->months * 4.33);
        weeks += (term->years * 52);
        return weeks / 52.0;
    }
    else if (term->months)
    {
        double months = term->months;
        months += (term->years * 12);
        return months / 12.0;
    }
    else
        return term->years;
}

RQ_EXPORT int 
rq_term_get_days_period(const struct rq_term *term)
{
    int days = term->days;
    days += (term->weeks * 7);
    days += (term->months * 30);
    days += (term->years * 365);
    return days;
}

RQ_EXPORT short 
rq_term_compare(struct rq_term *term1, struct rq_term *term2)
{
	return ( rq_term_get_days_period(term1) - rq_term_get_days_period(term2) );
}

RQ_EXPORT void 
rq_term_parse(
    struct rq_term *term, /**< The term to get filled from the parse. */
    const char *str, /**< The string to parse. Must be in the form 1M/1D/1Y, etc */
    const char *delims /**< delimiters to use when parsing. */
    )
{
    int done = 0;
    short days = 0;
    short weeks = 0;
    short months = 0;
    short years = 0;

    while (str && str[0] != '\0')
    {
        const char *nxt_ptr = NULL;
        unsigned int clen;
        unsigned int ci;

        int value = atoi(str);

        if (delims)
        {
            unsigned int di;
            unsigned int dlen = (unsigned int)strlen(delims);

            for (di = 0; di < dlen; di++)
            {
                if ((nxt_ptr = strchr(str, delims[di])) != NULL)
                    break;
            }

            /* OK, if we got to here then either nxt_ptr is pointing to
               the next delimiter, or it is NULL.
            */
        }

        if (nxt_ptr != NULL)
            clen = (unsigned int)(nxt_ptr - str); /* Cast down from 64 bit int to 32 bit. */
        else
            clen = (unsigned int)strlen(str);

        /* Now we look for a d/D, w/W, m/M, yY */
        for (ci = 0; ci < clen; ci++)
        {
            if (str[ci] == 'd' || str[ci] == 'D')
            {
                /* got days. */
                days = value;
            }
            else if (str[ci] == 'w' || str[ci] == 'W')
            {
                /* got weeks. */
                weeks = value;
            }
            else if (str[ci] == 'm' || str[ci] == 'M')
            {
                /* got months. */
                months = value;
            }
            else if (str[ci] == 'y' || str[ci] == 'Y')
            {
                /* got years */
                years = value;
            }
        }


        if (nxt_ptr)
            str = nxt_ptr + 1; /* skip past delimiter */
        else
            str = nxt_ptr; /* assign to NULL to leave loop. */
    }

    term->days = days;
    term->weeks = weeks;
    term->months = months;
    term->years = years;
}

RQ_EXPORT char *
rq_term_to_string(char *buf, unsigned int max_len, const char *format, struct rq_term *term)
{
    unsigned int len = (unsigned int)strlen(format);
    unsigned int i;
    unsigned int buf_pos = 0;

    for (i = 0; i < len && buf_pos < max_len; i++)
    {
        if (format[i] == '%')
        {
            /* peek ahead */
            switch (format[i+1])
            {
                case 'd':
                    snprintf(
                        buf+buf_pos, 
                        max_len - buf_pos,
                        "%d",
                        term->days
                        );
                    buf_pos = (unsigned int)strlen(buf);
                    i++; /* skip past next char in format */
                    break;

                case 'w':
                    snprintf(
                        buf+buf_pos, 
                        max_len - buf_pos,
                        "%d",
                        term->weeks
                        );
                    buf_pos = (unsigned int)strlen(buf);
                    i++; /* skip past next char in format */
                    break;

                case 'm':
                    snprintf(
                        buf+buf_pos, 
                        max_len - buf_pos,
                        "%d",
                        term->months
                        );
                    buf_pos = (unsigned int)strlen(buf);
                    i++; /* skip past next char in format */
                    break;

                case 'y':
                    snprintf(
                        buf+buf_pos, 
                        max_len - buf_pos,
                        "%d",
                        term->years
                        );
                    buf_pos = (unsigned int)strlen(buf);
                    i++; /* skip past next char in format */
                    break;

                case '%':
                    buf[buf_pos] = '%';
                    buf_pos++;
                    i++;
                    break;

                default:
                    buf[buf_pos] = format[i];
                    buf_pos++;
                    break;
            }
        }
        else
        {
            buf[buf_pos] = format[i];
            buf_pos++;
        }
    }

    if (buf_pos < max_len)
        buf[buf_pos] = '\0';
    else if (max_len > 0)
        buf[max_len-1] = '\0';

    return buf;
}

RQ_EXPORT void 
rq_term_set(struct rq_term* term, const char* period, const int multi)
{
    rq_term_init(term);

	if (period && period[0] != '\0')
	{
		if (period[0] == 'm' || period[0] == 'M')
			term->months = multi;
		else if (period[0] == 'y' || period[0] == 'Y')
			term->years = multi;
		else if (period[0] == 'd' || period[0] == 'D')
			term->days = multi;
		else if (period[0] == 'w' || period[0] == 'W')
			term->weeks = multi;
	}
}

RQ_EXPORT double
rq_term_diff(struct rq_term* termBase, struct rq_term* termDiff)
{
	double diff = rq_term_get_annualized_period(termBase) - rq_term_get_annualized_period(termDiff);
	return diff;
}

