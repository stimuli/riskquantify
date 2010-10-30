/*
** rq_interpreter_builtin_core.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2003-2008 Brett Hutley
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
#include "rq_interpreter_builtin_core.h"
#include "rq_date.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* add together the list of arguments */
static rq_object_t
rq_func_setq(void *interpreter, rq_object_t args)
{
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;
    rq_object_t res;

	rq_object_t p = args;

    do 
    {
		rq_object_t id = rq_object_get_cons_car(p);
		res = rq_interpreter_eval(interp, rq_object_get_cons_car(rq_object_get_cons_cdr(p)));
		rq_interpreter_symbol_set(
            interp, 
            rq_object_coerce_identifier(id), 
            rq_object_clone(rq_interpreter_get_object_mgr(interp), res)
            );
		p = rq_object_get_cons_cdr(rq_object_get_cons_cdr(p));
	} 
    while (p != rq_object_nil);

	return res;
}

static rq_object_t
rq_func_progn(void *interpreter, rq_object_t args)
{
	rq_object_t p = args;
    rq_object_t res;
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;

    while (!rq_object_is_nil(p))
    {
		res = rq_interpreter_eval(interp, rq_object_get_cons_car(p));
		p = rq_object_get_cons_cdr(p);
	} 

	return res;
}

static rq_object_t
rq_func_if(void *interpreter, rq_object_t args)
{
	rq_object_t if_test = rq_object_get_cons_car(args);
	rq_object_t if_body = rq_object_get_cons_car(rq_object_get_cons_cdr(args));
	rq_object_t else_body = rq_object_get_cons_cdr(rq_object_get_cons_cdr(args));
	rq_object_t res;
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;

	res = rq_interpreter_eval(interp, if_test);
    if (rq_object_is_nil(res))
		return rq_interpreter_eval(interp, if_body);

	return rq_func_progn(interpreter, else_body);
}

static rq_object_t
rq_func_defun(void *interpreter, rq_object_t args)
{
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;

    if (!rq_object_is_nil(args))
    {
        rq_object_t func_name = rq_object_get_cons_car(args);
        rq_object_t func_defn = rq_object_get_cons_cdr(args);

        if (rq_object_get_object_type(func_name) != RQ_OBJECT_TYPE_IDENTIFIER)
        {
            rq_interpreter_signal_error(
                interp,
                RQ_ERROR_SEVERITY_CRITICAL,
                "defun: Expected identifier as first argument"
                );
            return rq_object_nil;
        }

        if (rq_object_get_object_type(func_defn) == RQ_OBJECT_TYPE_CONS)
        {
            rq_object_t arg_list = rq_object_get_cons_car(func_defn);
            rq_object_t body_list = rq_object_get_cons_cdr(func_defn);

            rq_object_t obj_id = rq_object_alloc_identifier(
                rq_interpreter_get_object_mgr(interp), 
                "lambda"
                );

            rq_object_t obj_body = rq_object_alloc_cons(
                rq_interpreter_get_object_mgr(interp), 
                arg_list,
                body_list
                );

            rq_object_t func = rq_object_alloc_cons(
                rq_interpreter_get_object_mgr(interp), 
                obj_id,
                obj_body
                );

            rq_interpreter_symbol_set(
                interp,
                rq_object_get_identifier(func_name),
                func
                );

            return func;
        }
    }

	return rq_object_nil;
}

rq_object_t
rq_func_princ(void *interpreter, rq_object_t args)
{
	rq_object_t p = args;
    rq_object_t p1 = rq_object_nil;
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;

    while (!rq_object_is_nil(p) && rq_object_get_object_type(p) == RQ_OBJECT_TYPE_CONS)
    {
        rq_object_t val = rq_object_get_cons_car(p);
		p1 = rq_interpreter_eval(interp, val);

        switch (rq_object_get_object_type(p1)) 
        {
            case RQ_OBJECT_TYPE_NIL:
                printf("*nil*");
                break;

            case RQ_OBJECT_TYPE_TRUE:
                printf("*t*");
                break;

            case RQ_OBJECT_TYPE_IDENTIFIER:
                printf("%s", rq_object_get_identifier(p1));
                break;

            case RQ_OBJECT_TYPE_STRING:
                printf("%s", rq_object_get_string(p1));
                break;

            case RQ_OBJECT_TYPE_INTEGER:
                printf("%d", rq_object_get_integer(p1));
                break;

            case RQ_OBJECT_TYPE_DOUBLE:
                printf("%f", rq_object_get_double(p1));
                break;

            case RQ_OBJECT_TYPE_DATE:
            {
                char buf[125];
                printf("%s", rq_date_to_string(buf, "yyyy-mm-dd", rq_object_get_date(p1)));
            }
            break;

            case RQ_OBJECT_TYPE_CONS:
            {
                rq_object_t curr_obj = p1;

                printf("(");
                while (!rq_object_is_nil(rq_object_get_cons_car(curr_obj)) &&
                       rq_object_get_object_type(curr_obj) == RQ_OBJECT_TYPE_CONS)
                {
                    rq_func_princ(interpreter, rq_object_get_cons_car(curr_obj));
                    curr_obj = rq_object_get_cons_cdr(curr_obj);
                    if (!rq_object_is_nil(curr_obj)) 
                    {
                        printf(" ");
                        if (rq_object_get_object_type(curr_obj) != RQ_OBJECT_TYPE_CONS) 
                        {
                            printf(". ");
                            rq_func_princ(interpreter, curr_obj);
                        }
                    }
                } 

                printf(")");
            }
            break;
        }

		p = rq_object_get_cons_cdr(p);
	} 

	return p1;
}

RQ_EXPORT void 
rq_interpreter_builtin_core_register(rq_interpreter_t interp)
{
    rq_interpreter_add_builtin(interp, "setq", rq_func_setq);
    rq_interpreter_add_builtin(interp, "if", rq_func_if);
    rq_interpreter_add_builtin(interp, "progn", rq_func_progn);
    rq_interpreter_add_builtin(interp, "princ", rq_func_princ);
    rq_interpreter_add_builtin(interp, "defun", rq_func_defun);
}
