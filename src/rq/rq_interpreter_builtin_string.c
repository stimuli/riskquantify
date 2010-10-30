/*
** rq_interpreter_builtin_string.c
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
#include "rq_interpreter_builtin_string.h"
#include <stdlib.h>
#include <string.h>

rq_object_t
rq_func_string_equal(void *interpreter, rq_object_t args)
{
    rq_interpreter_t interp = (rq_interpreter_t)interpreter;
	rq_object_t str1;
    rq_object_t str2;

    str1 = rq_interpreter_eval(interp, rq_object_get_cons_car(args));
	str2 = rq_interpreter_eval(interp, rq_object_get_cons_car(rq_object_get_cons_cdr(args)));

    if (rq_object_get_object_type(str1) == rq_object_get_object_type(str2) == RQ_OBJECT_TYPE_STRING)
        if (!strcmp(rq_object_get_string(str1), rq_object_get_string(str2)))
            return rq_object_true;
    return rq_object_nil;
}

RQ_EXPORT void 
rq_interpreter_builtin_string_register(rq_interpreter_t interp)
{
    rq_interpreter_add_builtin(interp, "string-equal", rq_func_string_equal);
}
