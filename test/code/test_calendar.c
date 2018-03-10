#include <rq.h>
#include <stdlib.h>
#include <unistd.h>

short
create_temp_file(const char *filename, unsigned int *num_holidays)
{
    short ok = 1;
    rq_stream_t stream = rq_stream_file_alloc();

    *num_holidays = 0;

    rq_stream_file_set_filename(stream, filename);
    rq_stream_file_set_openmode(stream, RQ_STREAM_FILE_OPENMODE_WRITING);

    if (rq_stream_open(stream) == RQ_OK)
    {
        rq_date today = rq_date_today();
        unsigned int i;

        for (i = 0; i < 10000; i++)
        {
            char buf[20];
            char fmt_buf[20];
            char is_holiday = ((rand() % 10 == 0) ? 'N' : 'Y');
            rq_date date;

            if (i % 2 == 0)
                date = today + (i * 2);
            else
                date = today + i;

            sprintf(fmt_buf, "yyyy-mm-dd\t \n", is_holiday);
            rq_date_to_string(buf, fmt_buf, date);
            buf[11] = is_holiday;
            rq_stream_write_string(
                stream,
                buf
                );
            if (is_holiday == 'Y')
                (*num_holidays)++;
            date++;
        }

        rq_stream_close(stream);
    }
    else
        ok = 0;

    rq_stream_free(stream);
    return 1;
}

short
load_calendar(const char *filename, rq_calendar_t cal)
{
    short ok = 1;
    unsigned int num_dates_read = 0;
    rq_stream_t stream = rq_stream_file_alloc();

    rq_stream_file_set_filename(stream, filename);
    rq_stream_file_set_openmode(stream, RQ_STREAM_FILE_OPENMODE_READING);

    if (rq_stream_open(stream) == RQ_OK)
    {
        while (1)
        {
            char buf[1024];
            int ret = rq_stream_read_line(stream, buf, 1024);
            if (ret > 0)
            {
                rq_date date = rq_date_parse(
                    buf,
                    RQ_DATE_FORMAT_YMD
                    );
                char *p;
                if (date != 0 && (p = strchr(buf, '\t')) != NULL)
                {
                    num_dates_read++;
                    if (*(p+1) == 'Y')
                    {
                        rq_calendar_add_event(
                            cal,
                            date,
                            RQ_DATE_EVENT_GEN_HOLIDAY
                            );
                    }
                }
            }
            else
                break;
        }

        rq_stream_close(stream);
    }
    else
    {
        printf("There was an error opening the file '%s'\n", filename);
        ok = 0;
    }

    rq_stream_free(stream);
}

short
check_calendar(const char *filename, rq_calendar_t cal, short *check_successful)
{
    short ok = 1;
    rq_stream_t stream = rq_stream_file_alloc();
    unsigned dates_processed = 0;
    unsigned dates_ok = 0;

    *check_successful = 1;

    rq_stream_file_set_filename(stream, filename);
    rq_stream_file_set_openmode(stream, RQ_STREAM_FILE_OPENMODE_READING);

    if (rq_stream_open(stream) == RQ_OK)
    {
        while (1)
        {
            char buf[1024];
            int ret = rq_stream_read_line(stream, buf, 1024);
            if (ret > 0)
            {
                rq_date date = rq_date_parse(
                    buf,
                    RQ_DATE_FORMAT_YMD
                    );
                char *p;
                if (date != 0 && (p = strchr(buf, '\t')) != NULL)
                {
                    int is_holiday = (*(p+1) == 'Y');
                    dates_processed++;

                    if (rq_calendar_is_holiday(cal, date) != is_holiday)
                        *check_successful = 0;
                    else
                        dates_ok++;
                }
            }
            else
                break;
        }

        printf("There were %d dates processed, %d validated\n", dates_processed, dates_ok);

        rq_stream_close(stream);
    }
    else
    {
        printf("There was an error opening the file '%s'\n", filename);
        ok = 0;
    }

    rq_stream_free(stream);

    return ok;
}

int
main(int argc, char **argv)
{
    rq_calendar_t cal = rq_calendar_alloc("AUD");
    rq_stream_t stream;
    char path[4096];
    int failed = 0;
    short checks_ok;
    int num_holidays = 0;

    path[0] = '\0';
    tmpnam(path);

    if (!create_temp_file(path, &num_holidays))
    {
        printf("Couldn't create the calendar file!\n");
        failed = 1;
    }
    else if (!load_calendar(path, cal))
    {
        printf("Couldn't load the calendar file!\n");
        failed = 1;
    }
    else if (num_holidays != rq_calendar_size(cal))
    {
        printf("The # of holidays %d doesn't match the # of dates in the calendar %d!\n", num_holidays, rq_calendar_size(cal));
        failed = 1;
    }
    else if (!check_calendar(path, cal, &checks_ok))
    {
        printf("Couldn't check the calendar file!\n");
        failed = 1;
    }
    else
    {
        if (checks_ok)
            printf("Calendar test successful\n");
        else
        {
            printf("Calendar test failed\n");
            failed = 1;
        }
    }
    unlink(path);

    rq_calendar_free(cal);

    return failed;
}
