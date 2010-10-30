/*
** rq_matrix.c
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
#include "rq_matrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>

#include "rq_math.h"

RQ_EXPORT rq_matrix_t
rq_matrix_alloc()
{
    struct rq_matrix *m = (struct rq_matrix *)RQ_MALLOC(sizeof(struct rq_matrix));
    m->rows = m->cols = 0;
    m->vals = NULL;

    return m;
}

RQ_EXPORT rq_matrix_t 
rq_matrix_build(unsigned long rows, unsigned long cols)
{
    struct rq_matrix *m = (struct rq_matrix *)RQ_CALLOC(1, sizeof(struct rq_matrix));
    m->rows = rows;
    m->cols = cols;
    m->vals = (double *)RQ_CALLOC(rows * cols, sizeof(double));

    return m;
}

RQ_EXPORT rq_matrix_t 
rq_matrix_build_with_values(unsigned long rows, unsigned long cols, double first_value, ...)
{
    struct rq_matrix *m = (struct rq_matrix *)RQ_CALLOC(1, sizeof(struct rq_matrix));
    unsigned long max_offset = rows * cols;
    unsigned long i;
    va_list arglist;

    va_start(arglist, first_value);

    m->rows = rows;
    m->cols = cols;
    m->vals = (double *)RQ_MALLOC(rows * cols * sizeof(double));

    m->vals[0] = first_value;

    for (i = 1; i < max_offset; i++)
        m->vals[i] = va_arg(arglist, double);

    va_end(arglist);

    return m;
}

RQ_EXPORT rq_matrix_t 
rq_matrix_clone(rq_matrix_t mat)
{
    struct rq_matrix *m = (struct rq_matrix *)RQ_CALLOC(1, sizeof(struct rq_matrix));
    unsigned long max_offset = mat->rows * mat->cols;
    unsigned long i;

    m->rows = mat->rows;
    m->cols = mat->cols;
    m->vals = (double *)RQ_MALLOC(max_offset * sizeof(double));

    for (i = 0; i < max_offset; i++)
        m->vals[i] = mat->vals[i];

    return m;
}

RQ_EXPORT void 
rq_matrix_dim(rq_matrix_t m, unsigned long rows, unsigned long cols)
{
    unsigned long dim = rows * cols;
    if (m->rows * m->cols < dim)
    {
        if (m->vals)
            RQ_FREE(m->vals);

        m->vals = (double *)RQ_CALLOC(rows * cols, sizeof(double));
    }

    m->rows = rows;
    m->cols = cols;
}

RQ_EXPORT void  
rq_matrix_free(rq_matrix_t m)
{
    if (m->vals)
        RQ_FREE(m->vals);
    RQ_FREE(m);
}

RQ_EXPORT int
rq_matrix_is_null(rq_matrix_t obj)
{
    return (obj == NULL);
}

/* 1 2
   3 4

   1 0 3 -> offset 2
         -> row * m->cols + col
*/
RQ_EXPORT void 
rq_matrix_set(rq_matrix_t m, unsigned row, unsigned col, double val)
{
    unsigned long i = row * m->cols + col;
    m->vals[i] = val;
}

RQ_EXPORT void 
rq_matrix_set_values(rq_matrix_t matrix, unsigned long num_values, double first_value, ...)
{
    unsigned long max_offset = matrix->rows * matrix->cols;
    unsigned i;
    va_list arglist;

    va_start(arglist, first_value);

    if (num_values < max_offset)
        num_values = max_offset;

    matrix->vals[0] = first_value;

    for (i = 1; i < max_offset; i++)
        matrix->vals[i] = va_arg(arglist, double);

    va_end(arglist);
}

RQ_EXPORT double 
rq_matrix_get(const rq_matrix_t m, unsigned row, unsigned col)
{
    unsigned long i = row * m->cols + col;
    return m->vals[i];
}

RQ_EXPORT void 
rq_matrix_zero(rq_matrix_t m)
{
    unsigned long dim = m->rows * m->cols;
    unsigned long i;

    for (i = 0; i < dim; i++)
        m->vals[i] = 0.0;
}

RQ_EXPORT void
rq_matrix_print(const rq_matrix_t m)
{
    unsigned r;

    for (r = 0; r < m->rows; r++)
    {
        unsigned c;
        for (c = 0; c < m->cols; c++)
        {
            double v = rq_matrix_get(m, r, c);
            if (c > 0)
                printf("\t");
            printf("%.6f", v);
        }
        printf("\n");
    }
}

RQ_EXPORT short 
rq_matrix_cholesky(const rq_matrix_t in, rq_matrix_t out)
{
    short failed = 1;

    if (in->rows == out->rows && in->cols == out->cols)
    {
        unsigned r;
        for (r = 0; r < in->rows; r++)
        {
            unsigned c;
            for (c = r; c < in->cols; c++)
            {
                double sum = rq_matrix_get(in, r, c);

                if (r > 0)
                {
                    int k;
                    for (k = r - 1; k >= 0; k--)
                    {
                        sum -= 
                            (rq_matrix_get(out, r, (unsigned)k) * 
                             rq_matrix_get(out, c, (unsigned)k));
                    }
                }

                if (r == c)
                {
                    if (sum < 0.0) /* changed from <= 0 */
                    {
                        /* Not positive definite. Return failure. */
                        printf("Not positive definite! sum = %f\n", sum);
                        return 2;
                    }
                    rq_matrix_set(out, r, c, sqrt(sum));
                }
                else
                {
                    double v = sum / rq_matrix_get(out, r, r);

                    /* rq_matrix_set(in, c, r, v); */
                    rq_matrix_set(out, c, r, v);
                }
            }
        }

        failed = 0; /* yippee */
    }

    return failed;
}

RQ_EXPORT void
rq_matrix_transpose(const rq_matrix_t in, rq_matrix_t out)
{
    unsigned long r;
    for (r = 0; r < in->rows; r++)
    {
        unsigned long c;
        for (c = 0; c < in->cols; c++)
            rq_matrix_set(out, c, r, rq_matrix_get(in, r, c));
    }
}

RQ_EXPORT unsigned long 
rq_matrix_get_rows(const rq_matrix_t matrix)
{
    return matrix->rows;
}

RQ_EXPORT unsigned long 
rq_matrix_get_columns(const rq_matrix_t matrix)
{
    return matrix->cols;
}

RQ_EXPORT short
rq_matrix_is_equal(const rq_matrix_t m, const rq_matrix_t m2)
{
    unsigned long r;

    if (m->rows != m2->rows || m->cols !=  m2->cols)
        return 0;

    for (r = 0; r < m->rows; r++)
    {
        unsigned long c;
        for (c = 0; c < m->cols; c++)
            if (!rq_math_equal(rq_matrix_get(m, r, c), rq_matrix_get(m2, r, c)))
                return 0;
    }

    return 1;
}

RQ_EXPORT short
rq_matrix_subtract(const rq_matrix_t m1, const rq_matrix_t m2, rq_matrix_t out)
{
    unsigned long max_offset = m1->rows * m1->cols;
    unsigned long i;

    if (m1->rows != m2->rows || m1->cols != m2->cols ||
        m1->rows != out->rows || m1->cols != out->cols)
        return 1;

    for (i = 0; i < max_offset; i++)
        out->vals[i] = m1->vals[i] - m2->vals[i];

    return 0;
}

RQ_EXPORT short
rq_matrix_add(const rq_matrix_t m1, const rq_matrix_t m2, rq_matrix_t out)
{
    unsigned long max_offset = m1->rows * m1->cols;
    unsigned long i;

    if (m1->rows != m2->rows || m1->cols != m2->cols ||
        m1->rows != out->rows || m1->cols != out->cols)
        return 1;

    for (i = 0; i < max_offset; i++)
        out->vals[i] = m1->vals[i] + m2->vals[i];

    return 0;
}

RQ_EXPORT short
rq_matrix_multiply_scalar(const rq_matrix_t m1, double scalar, rq_matrix_t out)
{
    unsigned long max_offset = m1->rows * m1->cols;
    unsigned long i;

    if (m1->rows != out->rows || m1->cols != out->cols)
        return 1;

    for (i = 0; i < max_offset; i++)
        out->vals[i] = scalar * m1->vals[i];

    return 0;
}

RQ_EXPORT void
rq_matrix_householder(rq_matrix_t m, double *d, double *e)
{
    int i;

    for (i = m->rows-1; i >= 1; i--)
    {
        int l = i - 1;
        double h = 0.0;

        if (l > 0)
        {
            int k;
            double scale = 0.0;

            for (k = 0; k <= l; k++)
                scale += fabs(rq_matrix_get(m, i, k));

            if (scale == 0.0)
            {
                e[i] = rq_matrix_get(m, i, l);
            }
            else
            {
                double f;
                double g;
                double hh;
                int j;
                int k;

                for (k = 0; k <= l; k++)
                {
                    double v = rq_matrix_get(m, i, k);
                    v /= scale;
                    rq_matrix_set(m, i, k, v);
                    h += (v * v);
                }

                f = rq_matrix_get(m, i, l);
                g = (f >= 0.0 ? -sqrt(h) : sqrt(h));

                e[i] = scale * g;

                h -= f * g;

                rq_matrix_set(m, i, l, f - g);

                f = 0.0;

                for (j = 0; j <= l; j++)
                {
                    /* next statement can be ommitted if eigenvalues
                     * not wanted.
                     */
                    rq_matrix_set(m, j, i, rq_matrix_get(m, i, j) / h);

                    g = 0.0;

                    for (k = 0; k <= j; k++)
                        g += rq_matrix_get(m, j, k) * rq_matrix_get(m, i, k);

                    for (k = j + 1; k <= l; k++)
                        g += rq_matrix_get(m, k, j) * rq_matrix_get(m, i, k);

                    e[j] = g / h;

                    f += e[j] * rq_matrix_get(m, i, j);
                }

                hh = f / (h + h);

                for (j = 0; j <= l; j++)
                {
                    f = rq_matrix_get(m, i, j);

                    g = e[j] - hh * f;
                    e[j] = g;

                    for (k = 0; k <= j; k++)
                        rq_matrix_set(m, j, k, rq_matrix_get(m, j, k) - (f * e[k] + g * rq_matrix_get(m, i, k)));
                }
            }
        }
        else
        {
            e[i] = rq_matrix_get(m, i, l);
        }

        d[i] = h;
    }

    d[0] = 0.0;
    e[0] = 0.0;

    for (i = 0; i < m->rows; i++)
    {
        int l = i - 1;
        int j;

        if (d[i])
        {
            for (j = 0; j <= l; j++)
            {
                double g = 0.0;
                int k;

                for (k = 0; k <= l; k++)
                    g += rq_matrix_get(m, i, k) * rq_matrix_get(m, k, j);

                for (k = 0; k <= l; k++)
                    rq_matrix_set(m, k, j, rq_matrix_get(m, k, j) - (g * rq_matrix_get(m, k, i)));
            }
        }

        d[i] = rq_matrix_get(m, i, i);
        rq_matrix_set(m, i, i, 1.0);

        for (j = 0; j <= l; j++)
        {
            rq_matrix_set(m, j, i, 0.0);
            rq_matrix_set(m, i, j, 0.0);
        }
    }
}

short
rq_matrix_ql(rq_matrix_t z, double *d, double *e)
{
    int i;
    int l;
    unsigned long n = rq_matrix_get_columns(z);

    for (i = 1; i < n; i++)
        e[i-1] = e[i];
    e[n-1] = 0.0;

    for (l = 0; l < n; l++)
    {
        int iter = 0;
        int m;

        do
        {

            /* look for a single small subdiagonal element to split
               the matrix.
            */
            for (m = l; m < n-1; m++)
            {
                double dd = fabs(d[m]) + fabs(d[m+1]);
                if ((double)(fabs(e[m]) + dd) == dd)
                    break;
            }

            if (m != l)
            {
                double g;
                double r;
                double s;
                double p;
                double c;

                if (iter++ == 30)
                    return 1;

                g = (d[l+1] - d[l]) / (2.0 * e[l]);
                r = rq_pythag(g, 1.0);
                g = d[m] - d[l] + e[l] / (g + RQ_SIGN(r, g));
                s = c = 1.0;
                p = 0.0;

                for (i = m - 1; i >= l; i--)
                {
                    int k;
                    double f = s * e[i];
                    double b = c * e[i];
                    e[i + 1] = (r = rq_pythag(f, g));

                    if (r == 0.0)
                    {
                        d[i+1] -= p;
                        e[m] = 0.0;
                        break;
                    }

                    s = f / r;
                    c = g / r;
                    g = d[i+1] - p;
                    r = (d[i] - g) * s + 2.0 * c * b;
                    d[i+1] = g + (p = s * r);
                    g = c * r - b;

                    for (k = 0; k < n; k++)
                    {
                        f = rq_matrix_get(z, k, i + 1);
                        rq_matrix_set(
                            z, k, i + 1, 
                            s * rq_matrix_get(z, k, i) + c * f
                            );
                        rq_matrix_set(
                            z, k, i, 
                            c * rq_matrix_get(z, k, i) - s * f
                            );
                    }
                }

                if (r == 0.0 && i >= l)
                    continue;

                d[l] -= p;
                e[l] = g;
                e[m] = 0.0;
            }
        }
        while (m != l);
    }

    return 0;
}

RQ_EXPORT short
rq_matrix_multiply(const rq_matrix_t m1, rq_matrix_t m2, rq_matrix_t out)
{
    if (m1->cols == m2->rows)
    {
        if (out->rows == m1->rows && out->cols == m2->cols)
        {
            unsigned long c;
            for (c = 0; c < m2->cols; c++)
            {
                unsigned long r;
                for (r = 0; r < m1->rows; r++)
                {
                    double v = 0.0;
                    unsigned long i;

                    for (i = 0; i < m1->cols; i++)
                        v += rq_matrix_get(m1, r, i) * rq_matrix_get(m2, i, c);

                    rq_matrix_set(out, r, c, v);
                }
            }

            return 0;
        }

        return 2;
    }

    return 1;
}
