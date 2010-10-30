/*
** rq_string_list.c
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
#include "rq_string_list.h"
#include "rq_array.h"
#include <stdlib.h>
#include <string.h>

/**
 * The private string_list structure.
 */
static void
free_func(void *p)
{
    char *s = (char *)p;
   RQ_FREE(s);
}

RQ_EXPORT rq_string_list_t
rq_string_list_alloc()
{
    rq_string_list_t sl = (rq_string_list_t)RQ_MALLOC(sizeof(struct rq_string_list));
    sl->strings = rq_array_alloc(free_func);
    return sl;
}

RQ_EXPORT void  
rq_string_list_free(rq_string_list_t string_list)
{
    rq_array_free(string_list->strings);
    RQ_FREE(string_list);
}

RQ_EXPORT int
rq_string_list_is_null(rq_string_list_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT void 
rq_string_list_add(rq_string_list_t string_list, const char *string)
{
    rq_array_push_back(string_list->strings, RQ_STRDUP(string));
}

RQ_EXPORT unsigned long 
rq_string_list_size(rq_string_list_t string_list)
{
    return rq_array_size(string_list->strings);
}

RQ_EXPORT const char *
rq_string_list_get_at(rq_string_list_t string_list, unsigned long offset)
{
    return (const char *)rq_array_get_at(string_list->strings, offset);
}
