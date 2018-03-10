#include <rq.h>
#include <stdlib.h>
#include <math.h>

double
payoff_option(void *user_defined, double log_S, double prev_value)
{
    double X = *((double *)user_defined);
    double S = exp(log_S);
    double v = S - X;
    if (v < 0)
        return 0;
    return v;
}

int
compare(const void *d1, const void *d2)
{
    return (int)*((double *)d1) - *((double *)d2);
}

int
main(int argc, char **argv)
{
    double X = 100.0;
    double S = 100.0;
    double r_dom = 0.08;
    double r_for = 0.01;
    double sigma = 0.15;
    double tau_d = 1.0;
    int num_iters = (argc == 1 ? 1000 : atoi(argv[1]));
    double *terminal_distribution = (double *)malloc(num_iters * sizeof(double));
    int i;

    double value_bs = rq_pricing_blackscholes(
        1,
        S, /* assume this rate is passed in domestic over foreign terms */
        X, /* assume this rate is passed in domestic over foreign terms */
        r_dom, /* aka the numerator rate (continuously compounded) */
        r_for, /* aka the denominator rate (continuously compounded) */
        sigma,
        tau_d,
        tau_d
        );

    double value_mc = rq_pricing_monte_carlo(
        S,
        r_dom,
        r_for,
        sigma,
        tau_d,
        num_iters,
        terminal_distribution,
        365,
        NULL,
        &X,
        NULL,
        NULL,
        payoff_option,
        NULL,
        NULL
        );

    /*
    qsort(terminal_distribution, num_iters, sizeof(double), compare);

    for (i = 0; i < num_iters; i++)
        printf("%.8f\n", terminal_distribution[i]);
    */

    printf("Black Scholes = %.8f\nMonte Carlo = %.8f\nDifference = %.8f\n", value_bs, value_mc, value_bs - value_mc);

    free(terminal_distribution);

    if (value_bs - value_mc > 0.5)
        return -1;

    return 0;
}
