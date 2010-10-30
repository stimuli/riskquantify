/*
** rq_user.h
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
** Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
** USA 
*/
#include "rq_user.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT int 
rq_user_is_null(rq_user_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_user_t 
rq_user_alloc(const char *user_id)
{
    struct rq_user *u = (struct rq_user *)RQ_CALLOC(1, sizeof(struct rq_user));
    u->id = RQ_STRDUP(user_id);

    return u;
}

RQ_EXPORT void 
rq_user_free(rq_user_t u)
{
    if (u->id)
        RQ_FREE((char *)u->id);
    if (u->name)
        RQ_FREE((char *)u->name);
    RQ_FREE(u);
}

RQ_EXPORT const char *
rq_user_get_id(rq_user_t u)
{
    return u->id;
}

RQ_EXPORT void 
rq_user_set_name(rq_user_t u, const char *name)
{
    if (u->name)
        RQ_FREE((char *)u->name);
    u->name = RQ_STRDUP(name);
}

RQ_EXPORT const char *
rq_user_get_name(rq_user_t u)
{
    return u->name;
}
