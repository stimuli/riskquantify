/*
** rq_interpreter_error.c
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
#include "rq_interpreter_error.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_interpreter_error_t
rq_interpreter_error_alloc(const char *error_message)
{
    struct rq_interpreter_error *i = (struct rq_interpreter_error *)RQ_MALLOC(
        sizeof(struct rq_interpreter_error)
        );

    i->error_message = RQ_STRDUP(error_message);

    return i;
}

RQ_EXPORT void  
rq_interpreter_error_free(rq_interpreter_error_t i)
{
    RQ_FREE((char *)i->error_message);
    RQ_FREE(i);
}

RQ_EXPORT void 
_rq_interpreter_error_free(void *ie)
{
    rq_interpreter_error_free((rq_interpreter_error_t)ie);
}

RQ_EXPORT int
rq_interpreter_error_is_null(rq_interpreter_error_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT const char *
rq_interpreter_error_get_error_message(rq_interpreter_error_t i)
{
    return i->error_message;
}
