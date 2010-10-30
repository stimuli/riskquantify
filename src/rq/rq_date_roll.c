/*
** rq_date_roll.c
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
#include "rq_date_roll.h"
#include <stdlib.h>

RQ_EXPORT short
rq_date_roll_is_good_date(
    rq_date date,
    enum rq_date_roll_convention date_roll_convention, 
    const rq_calendar_t *cals,
    unsigned short num_cals
    )
{
    short good_date = 1;
    if (num_cals > 0)
    {
        unsigned short i;
        for (i = 0; i < num_cals && good_date; i++)
        {
            const rq_calendar_t cal = cals[i];
            good_date = !rq_calendar_is_weekend(cal, date) && 
                (date_roll_convention == RQ_DATE_ROLL_NO_ADJUSTMENT ||
                 !rq_calendar_is_holiday(cal, date));
        }
    }
    else
        good_date = !rq_date_is_weekend(date);

    return good_date;
}

/** Get the offset date in the date roll. Days field only is business days with date_roll_convention.
 */
RQ_EXPORT rq_date 
rq_date_roll_offset_term(
    rq_date start_date, 
    enum rq_day_type day_type, /* day type used to adjust initial offset before adding term,
                                  currently only supports WEEK and BUSINESS(default) */
    const struct rq_term *term,
    const rq_calendar_t *cals,
    unsigned short num_cals,
    enum rq_date_roll_convention date_roll_convention
    )
{
    struct rq_term t;
	int bus_days;
	/* Special case: if the offset is zero then do not apply the bus day logic. */
	if(term->days == 0 && term->weeks == 0 && term->months == 0 && term->years == 0)
		return start_date;
    /* Split off the business days as rq_term does not support yet. */
    bus_days = rq_term_get_days(term);
    t = *term;
    t.days = 0;

    /* Now add the days. */
    switch (day_type)
    {
        case RQ_DAY_TYPE_WEEK:
            /* can use RQ_DATE_ROLL_NO_ADJUSTMENT but faster by passing empty calendar so only check for weekend */
            /* start_date = rq_date_roll_days_offset(start_date, bus_days, cals, num_cals, RQ_DATE_ROLL_NO_ADJUSTMENT); */
            start_date = rq_date_roll_days_offset(start_date, bus_days, NULL, 0, date_roll_convention);
            break;
        /* add any new day type that needs to be supported */
        case RQ_DAY_TYPE_BUSINESS:
        default:
            start_date = rq_date_roll_days_offset(start_date, bus_days, cals, num_cals, date_roll_convention);
    }

    /* Add the weeks, months, years. */
    start_date = rq_date_add_term(start_date, &t);

    /* Now apply bus day convention. */
    start_date = rq_date_roll_adjust_date(start_date, date_roll_convention, cals, num_cals);
    return start_date;
}

/** Get the offset date in the date roll.
 * Get the offset date in the date roll, passing in the current date,
 * the offset, and current calendar(s).  
 */
RQ_EXPORT rq_date 
rq_date_roll_get_offset_date(
    rq_date start_date, 
    int offset,
    const rq_calendar_t *cals,
    unsigned short num_cals
    )
{
    return rq_date_roll_days_offset(start_date, offset, cals, num_cals, RQ_DATE_ROLL_FOLLOWING);
}

RQ_EXPORT rq_date 
rq_date_roll_days_offset(
    rq_date start_date, 
    int offset,
    const rq_calendar_t *cals,
    unsigned short num_cals,
    enum rq_date_roll_convention date_roll_convention
    )
{
    int count = 0;
    int step = offset > 0 ? 1 : -1;
    offset = abs(offset);
    while(offset > 0 && (count++ < 368)) //use 368 as sanity check for infinite loop
    {
        start_date += step;
        if(rq_date_roll_is_good_date(start_date, date_roll_convention, cals, num_cals))
        {
            offset--;
            //count = 0; maybe reset so can handle large offset that's greater than 365
        }
    }

    return start_date;
}

/**
 * Adjust the date according to the date roll convention until it is "Good".
 */
/*
DS (IT&E) - made this an export to be available for start and end dates in generate cashflows from params.
*/
RQ_EXPORT rq_date
rq_date_roll_adjust_date(
    rq_date date, 
    enum rq_date_roll_convention date_roll_convention, 
    const rq_calendar_t *cals,
    unsigned short num_cals
    )
{
    if (date_roll_convention != RQ_DATE_ROLL_NO_ADJUSTMENT)
    {
        if (!rq_date_roll_is_good_date(date, date_roll_convention, cals, num_cals))
        {
            int rolling_forward = 
                (date_roll_convention != RQ_DATE_ROLL_MOD_PRECEDING &&
                 date_roll_convention != RQ_DATE_ROLL_PRECEDING);
            int modified = 
                (date_roll_convention == RQ_DATE_ROLL_MOD_PRECEDING ||
                 date_roll_convention == RQ_DATE_ROLL_MOD_FOLLOWING);

            short day, month, year;

			rq_date in_date;
			unsigned short need_reset = 0; /* initialize to false */

            if (modified)
			{
                rq_date_to_dmy(date, &day, &month, &year);
				in_date = date; /* remember input date */
			}

            while (!rq_date_roll_is_good_date(date, date_roll_convention, cals, num_cals))
            {
                int next_date = date;
                if (rolling_forward)
                    next_date++;
                else
                    next_date--;
                if (modified)
                {
                    unsigned short next_day, next_month, next_year;
                    rq_date_to_dmy(next_date, &next_day, &next_month, &next_year);
                    if (month != next_month)
					{
                        rolling_forward = !rolling_forward; /* reverse rolling direction */
						need_reset = 1; /* signal reset date */
					}
                }

				if (need_reset == 0)
					date = next_date;
				else
				{
					date = in_date; /* reset date */
					need_reset = 0; /* turn off signal */
				}
            }
        }
    }

    return date;
}

RQ_EXPORT rq_date 
rq_date_roll_get_date(
    rq_date start_date, 
    const struct rq_term *term,
    enum rq_roll_convention roll_convention,
    enum rq_date_roll_convention date_roll_convention,
    const rq_calendar_t *cals,
    unsigned short num_cals
    )
{
	short modified = ( date_roll_convention == RQ_DATE_ROLL_MOD_PRECEDING 
					|| date_roll_convention == RQ_DATE_ROLL_MOD_FOLLOWING );
    rq_date d = rq_date_add_term_sticky_month(start_date, term, modified);

    if (roll_convention != RQ_ROLL_CONVENTION_NONE &&
        roll_convention != RQ_ROLL_CONVENTION_INVALID)
    {
        if (roll_convention >= 1 && roll_convention <= 30)
        {
            /* These conventions mean that the date rolls on the
               specific day of the month.
            */
            short day, month, year;
            rq_date_to_dmy(d, &day, &month, &year);
            if (day != roll_convention)
            {
                d = rq_date_from_dmy((short)roll_convention, month, year);
                while (d < start_date)
                    d = rq_date_add_months(d, 1, 0);
            }
        }
        else if (roll_convention >= RQ_ROLL_CONVENTION_MON &&
                 roll_convention <= RQ_ROLL_CONVENTION_SUN)
        {
            short dow = rq_date_get_day_of_week(d);
            short target_dow = (short)(roll_convention - RQ_ROLL_CONVENTION_MON);
            if (target_dow >= dow)
                d += (target_dow - dow);
            else
            {
                d -= (dow - target_dow);
                if (d <= start_date)
                    d += 7;
            }
            d++;
        }
        else if (roll_convention == RQ_ROLL_CONVENTION_EOM)
        {
            short day, month, year;
            rq_date_to_dmy(d, &day, &month, &year);
            day = rq_date_get_days_in_month(month, year);
            d = rq_date_from_dmy(day, month, year);
        }
        else if (roll_convention == RQ_ROLL_CONVENTION_IMM)
        {
            /* The 3rd Wednesday of the month */
            short day, month, year;
            short dow;

            rq_date_to_dmy(d, &day, &month, &year);
            d = rq_date_from_dmy(1, month, year);
            dow = rq_date_get_day_of_week(d);
            if (dow <= RQ_DAY_OF_WEEK_WEDNESDAY)
            {
                // 2 weeks + Days to get to the next wednesday 
                d += 14 + (RQ_DAY_OF_WEEK_WEDNESDAY - dow);
            }
            else
            {
				// 3 weeks, then subtract how far past wednesday we are.
                d += 21 - ( dow - RQ_DAY_OF_WEEK_WEDNESDAY );
            }
        }
        else if (roll_convention == RQ_ROLL_CONVENTION_CADIMM)
        {
            /* The 3rd Wednesday of the month - 1 good day */
            short day, month, year;
            short dow;

            rq_date_to_dmy(d, &day, &month, &year);
            d = rq_date_from_dmy(1, month, year);
            dow = rq_date_get_day_of_week(d);
            if (dow <= RQ_DAY_OF_WEEK_WEDNESDAY)
            {
                /* (3 * 7) + num_days_to_get_to_1st_wed */
                d += (21 + (RQ_DAY_OF_WEEK_WEDNESDAY - dow));
            }
            else
            {
                d += (21 + (7 - (dow - RQ_DAY_OF_WEEK_WEDNESDAY)));
            }
        }
        else if (roll_convention == RQ_ROLL_CONVENTION_SFE)
        {
            /* The 2nd Friday of the month */
            short day, month, year;
            short dow;

            rq_date_to_dmy(d, &day, &month, &year);
            d = rq_date_from_dmy(1, month, year);
            dow = rq_date_get_day_of_week(d);
            if (dow <= RQ_DAY_OF_WEEK_FRIDAY)
            {
                /* (2 * 7) + num_days_to_get_to_1st_wed */
                d += (14 + (RQ_DAY_OF_WEEK_FRIDAY - dow));
            }
            else
            {
                d += (14 + (7 - (dow - RQ_DAY_OF_WEEK_FRIDAY)));
            }
            d++;
        }
        else if (roll_convention == RQ_ROLL_CONVENTION_TBILL)
        {
            /* Each Monday */
            short dow = rq_date_get_day_of_week(d);
            short target_dow = (short)RQ_DAY_OF_WEEK_MONDAY;
            if (target_dow > dow)
                d += target_dow - dow;
            else
            {
                d -= (dow - target_dow);
                if (d <= start_date)
                    d += 7;
            }
            d++;
        }

    }

    d = rq_date_roll_adjust_date(
        d, 
        date_roll_convention, 
        cals,
        num_cals
        );

    return d;
}

RQ_EXPORT int
rq_date_roll_generate_dates(
    rq_date *dates,
    unsigned int max_dates,
    rq_date start_date,
    rq_date end_date,
    const struct rq_term *term,
    enum rq_roll_convention roll_convention,
    enum rq_date_roll_convention date_roll_convention,
    const rq_calendar_t *cals,
    unsigned short num_cals,
    enum rq_date_roll_stub_position stub_position,
    unsigned allowable_days_matching_error,
	int *startStubIndex
    )
{
    unsigned int i = 0;

	if (startStubIndex)
		(*startStubIndex) = -1;	/* no stub */

	/* don't generate dates if the term is 0/NULL (added due to UI hangs
       TODO find the underlying bug.
    */
	if (term)
	{
		struct rq_term next_term;
		rq_date date;

		rq_term_copy(&next_term, term);

		if (stub_position != RQ_DATE_ROLL_STUB_POSITION_START)
		{
			/* DS(IT&E) - Need to adjust in case start_date or end_date falls on a weekend. */
			dates[0] = rq_date_roll_adjust_date(
				start_date, 
				date_roll_convention, 
				cals,
				num_cals
				);

			end_date = rq_date_roll_adjust_date(
						end_date, 
						date_roll_convention, 
						cals,
						num_cals
						);

			for (i = 1; i < max_dates - 1; i++)
			{
				date = rq_date_roll_get_date(
					start_date,
					&next_term,
					roll_convention,
					date_roll_convention,
					cals,
					num_cals
					);
				/* DS was possible a fudge to make sure weekends were accounted for.
				if (date > (rq_date)(end_date + (long)allowable_days_matching_error))
                */

				if (date > end_date)
					break;
				if (date == dates[i-1])
                {
					i -= 1;		//DS go back an index if duplicate dates.   BUG BUG BUG
                    rq_term_add_term(&next_term, term);
                    // Guard against infinite loop if term is zero.
                    if(rq_term_is_equal(&next_term, term))
                        break;
                }
				else
                {
					dates[i] = date;
                    rq_term_add_term(&next_term, term);
                }
			}

			if (stub_position == RQ_DATE_ROLL_STUB_POSITION_END &&
				dates[i-1] < end_date)
				/* DS was possible a fudge to make sure weekends were accounted for.
				dates[i-1] < (end_date - (long)allowable_days_matching_error))
                */
			{
                if (startStubIndex)
                    (*startStubIndex) = i - 1;
				dates[i++] = end_date;
			}
		}
		else
		{
			struct rq_term term_decr;
			unsigned j;
			int num_dates;

			/* DS(IT&E) - Need to adjust in case end_date falls on a weekend. */

			dates[0] = rq_date_roll_adjust_date(
				end_date, 
				date_roll_convention, 
				cals,
				num_cals
				);

			start_date = rq_date_roll_adjust_date(
					start_date, 
					date_roll_convention, 
					cals,
					num_cals
					);

			rq_term_invert(&next_term);
			rq_term_copy(&term_decr, &next_term);

			for (i = 1; i < max_dates - 1; i++)
			{
				date = rq_date_roll_get_date(
					end_date,
					&next_term,
					roll_convention,
					date_roll_convention,
					cals,
					num_cals
					);
				/* DS was possible a fudge to make sure weekends were accounted for.
				if (date < (rq_date)(start_date - (long)allowable_days_matching_error))
                */
				if (date < start_date)
					break;
				dates[i] = date;
				rq_term_add_term(&next_term, &term_decr);
			}
			/* DS was possible a fudge to make sure weekends were accounted for.
			if (dates[i-1] > (start_date + (long)allowable_days_matching_error))
            */
			if (dates[i-1] > start_date)
			{
                if (startStubIndex)
                    (*startStubIndex) = 0;		//upin reversing 0 will point to start of stub
				dates[i++] = start_date;
			}

			num_dates = i;
			i--;
			for (j = 0; j < i; j++, i--)
			{
				rq_date tmp = dates[j];
				dates[j] = dates[i];
				dates[i] = tmp;
			}

			i = num_dates;
		}
	}
	else
	{
		dates[0] = start_date;
		dates[1] = end_date;
	}

    return i;
}

