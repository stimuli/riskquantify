/*
** rq_statistics.c
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA
*/
#include "rq_statistics.h"
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdarg.h>

short
rq_statistics_histogram(double *inputs, unsigned long num_inputs, double *buckets, unsigned long num_buckets, unsigned long *counts, unsigned long *underflow)
{
    unsigned int c;
    unsigned last_bucket_offset = num_buckets - 1;

    if (num_buckets == 0)
        return 1; /* This is an error. The last bucket is an overflow bucket. */

    for (c = 0; c < num_inputs; c++)
    {
        double val = inputs[c];
        unsigned int b;

        if (val < buckets[0])
            (*underflow)++;
        else
        {
            for (b = 0; b < num_buckets; b++)
            {
                if (b == last_bucket_offset)
                    counts[b]++;
                else if (val >= buckets[b] && val < buckets[b+1])
                {
                    counts[b]++;
                    break;
                }
            }
        }
    }

    return 0;
}

RQ_EXPORT double
rq_statistics_mean(double *inputs, unsigned long num_inputs)
{
    unsigned long i;
    double sum = 0.0;

    for (i = 0; i < num_inputs; i++)
        sum += inputs[i];

    return sum / (double)num_inputs;
}

RQ_EXPORT double
rq_statistics_max(double *inputs, unsigned long num_inputs)
{
    unsigned long i;
    double m = inputs[0];

    for (i = 1; i < num_inputs; i++)
        if (inputs[i] > m)
            m = inputs[i];

    return m;
}

RQ_EXPORT double
rq_statistics_min(double *inputs, unsigned long num_inputs)
{
    unsigned long i;
    double m = inputs[0];

    for (i = 1; i < num_inputs; i++)
        if (inputs[i] < m)
            m = inputs[i];

    return m;
}

RQ_EXPORT short
rq_statistics_mean_max_min(double *inputs, unsigned long num_inputs, double *mean, double *max_val, double *min_val)
{
    unsigned long i;
    double sum = 0.0;
    double mn = inputs[0];
    double mx = inputs[0];

    for (i = 0; i < num_inputs; i++)
    {
        sum += inputs[i];
        if (inputs[i] < mn)
            mn = inputs[i];
        if (inputs[i] > mx)
            mx = inputs[i];
    }

    *mean = sum / (double)num_inputs;
    *max_val = mx;
    *min_val = mn;

    return 0;
}

RQ_EXPORT rq_matrix_t
rq_statistics_get_covariance_matrix(unsigned long num_inputs, unsigned num_series, ...)
{
    rq_matrix_t mat = rq_matrix_build(num_series, num_series);
    int i;
    int j;
    double *means = (double *)calloc(num_series, sizeof(double));
    double *factors = (double *)calloc(num_series, sizeof(double));
    double **series = (double **)calloc(num_series, sizeof(double *));
    va_list arglist;

    va_start(arglist, num_series);

    for (i = 0; i < num_series; i++)
        series[i] = va_arg(arglist, double *);

    /* calculate the means */
    for (i = 0; i < num_series; i++)
    {
        double *s = series[i];

        for (j = 0; j < num_inputs; j++)
            means[i] += s[j];
    }

    for (i = 0; i < num_series; i++)
        means[i] /= (double)(num_inputs - 1);

    for (j = 0; j < num_inputs; j++)
    {
        for (i = 0; i < num_series; i++)
        {
            double *s = series[i];
            factors[i] = (s[j] - means[i]);
        }

        for (i = 0; i < num_series; i++)
        {
            int k;

            for (k = 0; k < num_series; k++)
            {
                double f = factors[i] * factors[k];

                rq_matrix_set(
                    mat, 
                    i, k, 
                    rq_matrix_get(mat, i, k) + f
                    );
            }
        }
    }


    for (i = 0; i < num_series; i++)
    {
        int k;

        for (k = 0; k < num_series; k++)
        {
            rq_matrix_set(
                mat, 
                i, k, 
                rq_matrix_get(mat, i, k) / (double)(num_inputs - 1)
                );
        }
    }


    free(means);
    free(factors);
    free(series);

    va_end(arglist);

    return mat;
}
