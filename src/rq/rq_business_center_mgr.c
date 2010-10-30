/*
** rq_business_center_mgr.c
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2001-2008 Brett Hutley
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
/* -- includes ----------------------------------------------------- */
#include "rq_business_center_mgr.h"
#include <stdlib.h>
#include <string.h>

/* -- static code -------------------------------------------------- */
static void
rq_business_center_mgr_node_free(struct rq_business_center_mgr_node *n)
{
    if (n->left)
        rq_business_center_mgr_node_free(n->left);

    if (n->right)
        rq_business_center_mgr_node_free(n->right);

    rq_business_center_free(n->business_center);
}

static struct rq_business_center_mgr_node *
rq_business_center_mgr_node_alloc()
{
    return (struct rq_business_center_mgr_node *)
        RQ_CALLOC(1, sizeof(struct rq_business_center_mgr_node));
}

/* -- code --------------------------------------------------------- */

RQ_EXPORT rq_business_center_mgr_t 
rq_business_center_mgr_alloc()
{
    struct rq_business_center_mgr *bcm = 
        (struct rq_business_center_mgr *)RQ_CALLOC(1, sizeof(struct rq_business_center_mgr));

    return bcm;
}

RQ_EXPORT void 
rq_business_center_mgr_free(rq_business_center_mgr_t bcm)
{
    if (bcm->head)
        rq_business_center_mgr_node_free(bcm->head);
    RQ_FREE(bcm);
}

RQ_EXPORT void 
rq_business_center_mgr_add(rq_business_center_mgr_t bcm, rq_business_center_t business_center)
{
    if (!bcm->head)
    {
        bcm->head = rq_business_center_mgr_node_alloc();
        bcm->head->business_center = business_center;
    }
    else
    {
        struct rq_business_center_mgr_node *n = bcm->head;
        const char *business_center_id = rq_business_center_get_id(business_center);
        while (n)
        {
            int cmp = strcmp(
                business_center_id,
                rq_business_center_get_id(n->business_center)
                );
            if (cmp == 0)
            {
                if (n->business_center != business_center)
                {
                    rq_business_center_free(n->business_center);
                    n->business_center = business_center;
                    break;
                }
            }
            else if (cmp > 0)
            {
                if (n->right != NULL)
                    n = n->right;
                else
                {
                    n->right = rq_business_center_mgr_node_alloc();
                    n->right->business_center = business_center;
                    break;
                }
            }
            else /* cmp < 0 */
            {
                if (n->left != NULL)
                    n = n->left;
                else
                {
                    n->left = rq_business_center_mgr_node_alloc();
                    n->left->business_center = business_center;
                    break;
                }
            }
        }
    }
}

RQ_EXPORT int
rq_business_center_mgr_is_null(rq_business_center_mgr_t obj)
{
    return (obj == NULL);
}
