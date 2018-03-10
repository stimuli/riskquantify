#include <rq.h>
#include <stdio.h>

struct test {
    rq_date date;
    double rate;
    int ok;
} testlist[40];

void
test_forward_rate(const struct rq_forward_rate *forward_rate)
{
    int i;

    for (i = 0; i < 40; i++)
    {
        if (testlist[i].date == forward_rate->date)
        {
            testlist[i].ok = (testlist[i].rate == forward_rate->rate);
            break;
        }
    }
}

int
all_ok()
{
    int i;
    for (i = 0; i < 40; i++)
        if (!testlist[i].ok)
            return 0;

    printf("All OK!\n");
    return 1;
}

int
main(int argc, char **argv)
{
    rq_forward_curve_t fwdcurve = rq_forward_curve_build("AUD/USD", "AUD/USD");
    rq_date today = rq_date_today();
    int i;

    for (i = 0; i < 40; i++)
    {
        rq_date date;
        double rate = 20 + i;

        if (i % 2 == 0)
            date = today + (i * 2);
        else
            date = today + i;

        rq_forward_curve_set_rate(fwdcurve, date, rate);

        testlist[i].date = date;
        testlist[i].rate = rate;
        testlist[i].ok = 0;
    }

    for (i = 0; i < rq_forward_curve_size(fwdcurve); i++)
    {
        const struct rq_forward_rate *forward_rate = rq_forward_curve_element_at(fwdcurve, i);
        test_forward_rate(forward_rate);
    }

    rq_forward_curve_free(fwdcurve);

    return all_ok() == 0;
}
