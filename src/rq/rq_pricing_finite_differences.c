/*
** rq_pricing_finite_differences.c
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
#include "rq_config.h"
#include "rq_pricing_finite_differences.h"
#include "rq_pricing_blackscholes.h"
#include "rq_error.h"
#include "rq_math.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_X_MULTIPLIER      2.50    

#define SOR_OMEGA               1.03E0
#define SOR_DOMEGA              1.0E-2
#define SOR_EPS                 1.0E-5
#define SOR_MAXITS              1000		

RQ_EXPORT double
rq_pricing_finite_differences_equity_american(
    short call,
    double S,
    double X,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d,
    unsigned num_timesteps,
    unsigned num_values,
    double *mesh, /* num_values + 1 */
    double *value,  /* num_values + 1 */
    double *vold,  /* num_values + 1 */
    double *ivalue,  /* num_values + 1 */
    double *a,  /* num_values + 1 */
    double *b,  /* num_values + 1 */
    double *c,  /* num_values + 1 */
    double *tvalue,  /* num_timesteps + 1 */
    int *err
    )
{
    double result = 0.0;
    int s_steps = num_values;
    int t_steps = num_timesteps;
    double hibarrier = MAX_X_MULTIPLIER * X;
    double lobarrier = 0.0;
    double ds = (hibarrier - lobarrier) / (num_values - 1);
    double dt = tau_e / (double)t_steps;
    int i;
    double vol2 = sigma * sigma;
    double ss;
    double se;
    double rate1 = r_dom;
    double cost_carry = r_dom - r_for;
    double del;
    int time_index;
    int t_i;
    int s_i;
    double omega = SOR_OMEGA;
    double domega = SOR_DOMEGA;
    double eps = SOR_EPS;
	int its;
	int oldits;
    double elasticity = 1.0;

    *err = RQ_OK;

    for (i = 0; i < s_steps; ++i)
        mesh[i] = lobarrier + (ds * (double) i);

    if (call)
    {
        for (i = 0; i < s_steps; ++i)
        {
            value[i] = MAX(0.0, mesh[i] - X);
            ivalue[i] = vold[i] = value[i];
        }
    }
    else
    {
        for (i = 0; i < s_steps; ++i)
        {
            value[i] = MAX(0.0, X - mesh[i]);
            ivalue[i] = vold[i] = value[i];
        }
    }

    ss = mesh[0] / ds;
    se = pow(mesh[0], elasticity) / ds;

    a[0] = 0;
    b[0] = 1.0 + (vol2 * se * se + rate1) * dt;
    c[0] = -(vol2 * se * se + cost_carry * ss) * dt / 2.0;

    for (i = 1; i < s_steps; ++i)
    {
        ss = mesh[i] / ds;
        se = pow(mesh[i], elasticity) / ds;
        a[i] = -(vol2 * se * se - cost_carry * ss) * dt / 2.0;
        b[i] = 1.0 + (vol2 * se * se + rate1) * dt;
        c[i] = -(vol2 * se * se + cost_carry * ss) * dt / 2.0;
    }

    /* -- get the time index -- */
    del = fabs(S - mesh[0]);
    time_index = 0;

    for (i = 1; i < s_steps; i++)
    {
        double new_del = fabs(S - mesh[i]);
        if (new_del <= del)
        {
            del = new_del;
            time_index = i;
        }
    }

    /* time_index has been set */
    oldits = SOR_MAXITS + 1000;

    for (t_i = t_steps; t_i > 0; t_i--)
    {
        int can_exercise = 0;
        int count; /* for converging */
        double tmp;
        double l2norm;
        double l2err;

        double time = (double) t_i * dt;

        /* do discrete dividend stuff here */

        /* */

        count = 0;

        for (s_i = 0; s_i < s_steps; ++s_i)
            value[s_i] = vold[s_i];

        tmp = vold[0] / b[0];
        if (can_exercise)
            tmp = MAX(tmp, ivalue[0]);

        value[0] = tmp;

        s_i = s_steps;
        value[s_i] = (2.0 * value[s_i - 1] - value[s_i - 2]);

        do
        {
            l2norm = 0.0;
            l2err = 0.0;

            s_i = s_steps;
            tmp = (2 * value[s_i - 1] - value[s_i - 2]);
            tmp = value[s_i] + omega * (tmp - value[s_i]);
            tmp = MAX(tmp, 0.0);

            value[s_i] = tmp;

            for (s_i = 1; s_i < s_steps; ++s_i)
            {
                tmp = (vold[s_i] - value[s_i - 1] * a[s_i] -
                       value[s_i + 1] * c[s_i]) / b[s_i];
                tmp = value[s_i] + omega * (tmp - value[s_i]);

                if (can_exercise)
                    tmp = MAX(tmp, ivalue[s_i]);
                else
                    tmp = MAX(tmp, 0.0);

                l2norm += tmp * tmp;
                l2err += (value[s_i] - tmp) * (value[s_i] - tmp);
                value[s_i] = tmp;
            }

            ++count;
            l2err = sqrt(l2err);
            l2norm = sqrt(l2norm);

            if (l2norm < 0.1)
                l2norm = 0.1;
        }
        while (l2err > eps * l2norm && count < SOR_MAXITS);

        its = count;

        if (l2err > eps * l2norm)
        {
            *err = RQ_PRICING_FINITE_DIFFERENCES_SOR_DID_NOT_CONVERGE;
            return 0.0;
        }
        /* */

        if (oldits < its)
            domega *= -0.9;
        omega += domega;

        oldits = its;

        tvalue[t_i-1] = value[time_index];

        if (t_i > 1)
        {
            int j;
            for (j = 0; j < s_steps; ++j)
                vold[j] = value[j];
        }
    }
    /* end of time stepping loop */

    result = tvalue[0];

    return result;
}

RQ_EXPORT double
rq_pricing_finite_differences_equity_bermudan(
    short call,
    double S,
    double X,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d,
    double tau_ex_start,
    double tau_ex_end,
    unsigned num_timesteps,
    unsigned num_values,
    double *mesh, /* num_values + 1 */
    double *value,  /* num_values + 1 */
    double *vold,  /* num_values + 1 */
    double *ivalue,  /* num_values + 1 */
    double *a,  /* num_values + 1 */
    double *b,  /* num_values + 1 */
    double *c,  /* num_values + 1 */
    double *tvalue,  /* num_timesteps + 1 */
    int *err
    )
{
    double result = 0.0;
    int s_steps = num_values;
    int t_steps = num_timesteps;
    double hibarrier = MAX_X_MULTIPLIER * X;
    double lobarrier = 0.0;
    double ds = (hibarrier - lobarrier) / (num_values - 1);
    double dt = tau_e / (double)t_steps;
    int i;
    double vol2 = sigma * sigma;
    double ss;
    double se;
    double rate1 = r_dom;
    double cost_carry = r_dom - r_for;
    double del;
    int time_index;
    int t_i;
    int s_i;
    double omega = SOR_OMEGA;
    double domega = SOR_DOMEGA;
    double eps = SOR_EPS;
	int its;
	int oldits;
    double elasticity = 1.0;

    *err = RQ_OK;

    for (i = 0; i < s_steps; ++i)
        mesh[i] = lobarrier + (ds * (double) i);

    if (call)
    {
        for (i = 0; i < s_steps; ++i)
        {
            value[i] = MAX(0.0, mesh[i] - X);
            ivalue[i] = vold[i] = value[i];
        }
    }
    else
    {
        for (i = 0; i < s_steps; ++i)
        {
            value[i] = MAX(0.0, X - mesh[i]);
            ivalue[i] = vold[i] = value[i];
        }
    }

    ss = mesh[0] / ds;
    se = pow(mesh[0], elasticity) / ds;

    a[0] = 0;
    b[0] = 1.0 + (vol2 * se * se + rate1) * dt;
    c[0] = -(vol2 * se * se + cost_carry * ss) * dt / 2.0;

    for (i = 1; i < s_steps; ++i)
    {
        ss = mesh[i] / ds;
        se = pow(mesh[i], elasticity) / ds;
        a[i] = -(vol2 * se * se - cost_carry * ss) * dt / 2.0;
        b[i] = 1.0 + (vol2 * se * se + rate1) * dt;
        c[i] = -(vol2 * se * se + cost_carry * ss) * dt / 2.0;
    }

    /* -- get the time index -- */
    del = fabs(S - mesh[0]);
    time_index = 0;

    for (i = 1; i < s_steps; i++)
    {
        double new_del = fabs(S - mesh[i]);
        if (new_del <= del)
        {
            del = new_del;
            time_index = i;
        }
    }

    /* time_index has been set */
    oldits = SOR_MAXITS + 1000;

    for (t_i = t_steps; t_i > 0; t_i--)
    {
        int can_exercise = 0;
        int count; /* for converging */
        double tmp;
        double l2norm;
        double l2err;

        double time = (double) t_i * dt;

        if (time >= tau_ex_start && time <= tau_ex_end)
            can_exercise = 1;

        /* do discrete dividend stuff here */

        /* */

        count = 0;

        for (s_i = 0; s_i < s_steps; ++s_i)
            value[s_i] = vold[s_i];

        tmp = vold[0] / b[0];
        if (can_exercise)
            tmp = MAX(tmp, ivalue[0]);

        value[0] = tmp;

        s_i = s_steps;
        value[s_i] = (2.0 * value[s_i - 1] - value[s_i - 2]);

        do
        {
            l2norm = 0.0;
            l2err = 0.0;

            s_i = s_steps;
            tmp = (2 * value[s_i - 1] - value[s_i - 2]);
            tmp = value[s_i] + omega * (tmp - value[s_i]);
            tmp = MAX(tmp, 0.0);

            value[s_i] = tmp;

            for (s_i = 1; s_i < s_steps; ++s_i)
            {
                tmp = (vold[s_i] - value[s_i - 1] * a[s_i] -
                       value[s_i + 1] * c[s_i]) / b[s_i];
                tmp = value[s_i] + omega * (tmp - value[s_i]);

                if (can_exercise)
                    tmp = MAX(tmp, ivalue[s_i]);
                else
                    tmp = MAX(tmp, 0.0);

                l2norm += tmp * tmp;
                l2err += (value[s_i] - tmp) * (value[s_i] - tmp);
                value[s_i] = tmp;
            }

            ++count;
            l2err = sqrt(l2err);
            l2norm = sqrt(l2norm);

            if (l2norm < 0.1)
                l2norm = 0.1;
        }
        while (l2err > eps * l2norm && count < SOR_MAXITS);

        its = count;

        if (l2err > eps * l2norm)
        {
            *err = RQ_PRICING_FINITE_DIFFERENCES_SOR_DID_NOT_CONVERGE;
            return 0.0;
        }
        /* */

        if (oldits < its)
            domega *= -0.9;
        omega += domega;

        oldits = its;

        tvalue[t_i-1] = value[time_index];

        if (t_i > 1)
        {
            int j;
            for (j = 0; j < s_steps; ++j)
                vold[j] = value[j];
        }
    }
    /* end of time stepping loop */

    result = tvalue[0];

    return result;
}

RQ_EXPORT double
rq_pricing_finite_differences_fxo_barrier(
    short in,
    short call,
    double S,
    double X,
    double H,
    double r_dom,
    double r_for,
    double sigma,
    double tau_e,
    double tau_d,
    double *bar_defs, /* (4 * num_bar_defs) - hi / lo / start_tau / end_tau */
    unsigned short num_bar_defs,
    unsigned num_timesteps,
    unsigned num_values,
    double *mesh, /* num_values + 1 */
    double *value,  /* num_values + 1 */
    double *vold,  /* num_values + 1 */
    double *ivalue,  /* num_values + 1 */
    double *a,  /* num_values + 1 */
    double *b,  /* num_values + 1 */
    double *c,  /* num_values + 1 */
    double *tvalue,  /* num_timesteps + 1 */
    int *err
    )
{
    double result = 0.0;
    int s_steps = num_values;
    int t_steps = num_timesteps;
    double hibarrier = MAX_X_MULTIPLIER * X;
    double lobarrier = 0.0;
    double ds = (hibarrier - lobarrier) / (num_values - 1);
    double dt = tau_e / (double)t_steps;
    int i;
    double vol2 = sigma * sigma;
    double ss;
    double se;
    double rate1 = r_dom;
    double cost_carry = r_dom - r_for;
    double del;
    int time_index;
    int t_i;
    int s_i;
    double omega = SOR_OMEGA;
    double domega = SOR_DOMEGA;
    double eps = SOR_EPS;
	int its;
	int oldits;
    double elasticity = 1.0;

    *err = RQ_OK;

    for (i = 0; i < s_steps; ++i)
        mesh[i] = lobarrier + (ds * (double) i);

    if (call)
    {
        for (i = 0; i < s_steps; ++i)
        {
            value[i] = MAX(0.0, mesh[i] - X);
            ivalue[i] = vold[i] = value[i];
        }
    }
    else
    {
        for (i = 0; i < s_steps; ++i)
        {
            value[i] = MAX(0.0, X - mesh[i]);
            ivalue[i] = vold[i] = value[i];
        }
    }

    ss = mesh[0] / ds;
    se = pow(mesh[0], elasticity) / ds;

    a[0] = 0;
    b[0] = 1.0 + (vol2 * se * se + rate1) * dt;
    c[0] = -(vol2 * se * se + cost_carry * ss) * dt / 2.0;

    for (i = 1; i < s_steps; ++i)
    {
        ss = mesh[i] / ds;
        se = pow(mesh[i], elasticity) / ds;
        a[i] = -(vol2 * se * se - cost_carry * ss) * dt / 2.0;
        b[i] = 1.0 + (vol2 * se * se + rate1) * dt;
        c[i] = -(vol2 * se * se + cost_carry * ss) * dt / 2.0;
    }

    /* -- get the time index -- */
    del = fabs(S - mesh[0]);
    time_index = 0;

    for (i = 1; i < s_steps; i++)
    {
        double new_del = fabs(S - mesh[i]);
        if (new_del <= del)
        {
            del = new_del;
            time_index = i;
        }
    }

    /* time_index has been set */
    oldits = SOR_MAXITS + 1000;

    for (t_i = t_steps; t_i > 0; t_i--)
    {
        int can_exercise = 0;
        int count; /* for converging */
        double tmp;
        double l2norm;
        double l2err;

        double time = (double) t_i * dt;

        /*
        if (time >= tau_ex_start && time <= tau_ex_end)
            can_exercise = 1;
        */

        /* do discrete dividend stuff here */

        /* */

        count = 0;

        for (s_i = 0; s_i < s_steps; ++s_i)
            value[s_i] = vold[s_i];

        tmp = vold[0] / b[0];
        if (can_exercise)
            tmp = MAX(tmp, ivalue[0]);

        value[0] = tmp;

        s_i = s_steps;
        value[s_i] = (2.0 * value[s_i - 1] - value[s_i - 2]);

        do
        {
            l2norm = 0.0;
            l2err = 0.0;

            s_i = s_steps;
            tmp = (2 * value[s_i - 1] - value[s_i - 2]);
            tmp = value[s_i] + omega * (tmp - value[s_i]);
            tmp = MAX(tmp, 0.0);

            value[s_i] = tmp;

            for (s_i = 1; s_i < s_steps; ++s_i)
            {
                tmp = (vold[s_i] - value[s_i - 1] * a[s_i] -
                       value[s_i + 1] * c[s_i]) / b[s_i];
                tmp = value[s_i] + omega * (tmp - value[s_i]);

                if (can_exercise)
                    tmp = MAX(tmp, ivalue[s_i]);
                else
                    tmp = MAX(tmp, 0.0);

                l2norm += tmp * tmp;
                l2err += (value[s_i] - tmp) * (value[s_i] - tmp);
                value[s_i] = tmp;
            }

            ++count;
            l2err = sqrt(l2err);
            l2norm = sqrt(l2norm);

            if (l2norm < 0.1)
                l2norm = 0.1;
        }
        while (l2err > eps * l2norm && count < SOR_MAXITS);

        its = count;

        if (l2err > eps * l2norm)
        {
            *err = RQ_PRICING_FINITE_DIFFERENCES_SOR_DID_NOT_CONVERGE;
            return 0.0;
        }
        /* */

        if (oldits < its)
            domega *= -0.9;
        omega += domega;

        oldits = its;

        tvalue[t_i-1] = value[time_index];

        if (t_i > 1)
        {
            int j;
            for (j = 0; j < s_steps; ++j)
                vold[j] = value[j];
        }
    }
    /* end of time stepping loop */

    result = tvalue[0];

    return result;
}
