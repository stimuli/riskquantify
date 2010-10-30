/*
** rq_string_set.c
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
#include "rq_string_set.h"
#include "rq_set_rb.h"

#include <stdlib.h>
#include <string.h>

static void
_rq_string_free(void *p)
{
    char *s = (char *)p;
    RQ_FREE(s);
}

rq_string_set_t
rq_string_set_alloc()
{
    rq_string_set_t string_set = (rq_string_set_t)RQ_MALLOC(
        sizeof(struct rq_string_set)
        );
    string_set->strings = rq_set_rb_alloc(
        _rq_string_free,
        (int (*)(const void *, const void *))strcmp
        );

    return string_set;
}

void  
rq_string_set_free(rq_string_set_t string_set)
{
    rq_set_rb_free(string_set->strings);
    RQ_FREE(string_set);
}

int
rq_string_set_is_null(rq_string_set_t obj)
{
    return (obj == NULL);
}

void
rq_string_set_add(rq_string_set_t ss, const char *s)
{
    rq_set_rb_add(ss->strings, strdup(s));
}

int
rq_string_set_exists(rq_string_set_t ss, const char *s)
{
    return rq_set_rb_find(ss->strings, s) != NULL;
}
