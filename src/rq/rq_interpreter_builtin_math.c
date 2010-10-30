/*
** rq_interpreter_builtin_math.c
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
#include "rq_interpreter_builtin_math.h"
#include <stdlib.h>
#include <string.h>

/* add together the list of arguments */
rq_object_t
rq_func_add(void *interpreter, rq_object_t args)
{
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;
    int treat_as_double = 0;
    rq_object_t res;

    res = rq_object_alloc_integer(
        rq_interpreter_get_object_mgr(interp), 
        0
        );
    while (!rq_object_is_nil(args))
    {
        rq_object_t car = rq_interpreter_eval(interp, rq_object_get_cons_car(args));
        if (!treat_as_double && 
            rq_object_get_object_type(car) == RQ_OBJECT_TYPE_DOUBLE)
        {
            treat_as_double = 1;
            rq_object_coerce_double(res);
        }
        if (treat_as_double)
            rq_object_set_double(res, rq_object_get_double(res) + rq_object_coerce_double(car));
        else
            rq_object_set_integer(res, rq_object_get_integer(res) + rq_object_coerce_integer(car));
        args = rq_object_get_cons_cdr(args);
    }
    return res;
}

rq_object_t
rq_func_substract(void *interpreter, rq_object_t args)
{
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;
    int treat_as_double = 0;
    int got_first = 0;
    rq_object_t res;

    res = rq_object_alloc_integer(
        rq_interpreter_get_object_mgr(interp), 
        0
        );
    while (!rq_object_is_nil(args))
    {
        rq_object_t car = rq_interpreter_eval(interp, rq_object_get_cons_car(args));
        if (!treat_as_double && 
            rq_object_get_object_type(car) == RQ_OBJECT_TYPE_DOUBLE)
        {
            treat_as_double = 1;
            rq_object_coerce_double(res);
        }
        if (!got_first)
        {
            got_first = 1;
            if (treat_as_double)
                rq_object_set_double(res, rq_object_coerce_double(car));
            else
                rq_object_set_integer(res, rq_object_coerce_integer(car));
        }
        else
        {
            if (treat_as_double)
                rq_object_set_double(res, rq_object_get_double(res) - rq_object_coerce_double(car));
            else
                rq_object_set_integer(res, rq_object_get_integer(res) - rq_object_coerce_integer(car));
        }
        args = rq_object_get_cons_cdr(args);
    }
    return res;
}

rq_object_t
rq_func_multiply(void *interpreter, rq_object_t args)
{
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;
    int treat_as_double = 0;
    rq_object_t res;

    res = rq_object_alloc_integer(
        rq_interpreter_get_object_mgr(interp), 
        0
        );
    while (!rq_object_is_nil(args))
    {
        rq_object_t car = rq_interpreter_eval(interp, rq_object_get_cons_car(args));
        if (!treat_as_double && 
            rq_object_get_object_type(car) == RQ_OBJECT_TYPE_DOUBLE)
        {
            treat_as_double = 1;
            rq_object_coerce_double(res);
        }
        if (treat_as_double)
            rq_object_set_double(res, rq_object_get_double(res) * rq_object_coerce_double(car));
        else
            rq_object_set_integer(res, rq_object_get_integer(res) * rq_object_coerce_integer(car));
        args = rq_object_get_cons_cdr(args);
    }
    return res;
}

rq_object_t
rq_func_divide(void *interpreter, rq_object_t args)
{
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;
    int treat_as_double = 0;
    int got_first = 0;
    rq_object_t res;

    res = rq_object_alloc_integer(
        rq_interpreter_get_object_mgr(interp), 
        0
        );
    while (!rq_object_is_nil(args))
    {
        rq_object_t car = rq_interpreter_eval(interp, rq_object_get_cons_car(args));
        if (!treat_as_double && 
            rq_object_get_object_type(car) == RQ_OBJECT_TYPE_DOUBLE)
        {
            treat_as_double = 1;
            rq_object_coerce_double(res);
        }
        if (!got_first)
        {
            got_first = 1;
            if (treat_as_double)
                rq_object_set_double(res, rq_object_coerce_double(car));
            else
                rq_object_set_integer(res, rq_object_coerce_integer(car));
        }
        else
        {
            if (treat_as_double)
                rq_object_set_double(res, rq_object_get_double(res) / rq_object_coerce_double(car));
            else
                rq_object_set_integer(res, rq_object_get_integer(res) / rq_object_coerce_integer(car));
        }
        args = rq_object_get_cons_cdr(args);
    }
    return res;
}

RQ_EXPORT void 
rq_interpreter_builtin_math_register(rq_interpreter_t interp)
{
    rq_interpreter_add_builtin(interp, "+", rq_func_add);
    rq_interpreter_add_builtin(interp, "-", rq_func_substract);
    rq_interpreter_add_builtin(interp, "*", rq_func_multiply);
    rq_interpreter_add_builtin(interp, "/", rq_func_multiply);
}
