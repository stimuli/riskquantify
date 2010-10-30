/*
** rq_user_mgr.c
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
#include "rq_user_mgr.h"
#include "rq_tree_rb.h"
#include <stdlib.h>
#include <string.h>


RQ_EXPORT int 
rq_user_mgr_is_null(rq_user_mgr_t obj)
{
    return (obj == NULL);
}

static void
_rq_user_mgr_node_free(void *p)
{
    rq_user_free((rq_user_t)p);
}

RQ_EXPORT rq_user_mgr_t 
rq_user_mgr_alloc()
{
    struct rq_user_mgr *um = (struct rq_user_mgr *)
        RQ_MALLOC(sizeof(struct rq_user_mgr));

    um->tree = rq_tree_rb_alloc(
        _rq_user_mgr_node_free,
        (int (*)(const void *, const void *))strcmp
        );

    return um;
}

RQ_EXPORT void 
rq_user_mgr_free(rq_user_mgr_t um)
{
    rq_tree_rb_free(um->tree);
    RQ_FREE(um);
}

RQ_EXPORT void 
rq_user_mgr_add(rq_user_mgr_t um, rq_user_t user)
{
    rq_tree_rb_add(um->tree, rq_user_get_id(user), user);
}
