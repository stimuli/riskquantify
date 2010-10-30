/*
** rq_floating_flow_list.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2005-2008 Brett Hutley
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
#include <stdlib.h>
#include "rq_floating_flow_list.h"

RQ_EXPORT int 
rq_floating_flow_list_is_null(rq_floating_flow_list_t obj)
{
    return obj == NULL;
}

RQ_EXPORT rq_floating_flow_list_t 
rq_floating_flow_list_alloc()
{
    rq_floating_flow_list_t ffl = (rq_floating_flow_list_t)RQ_MALLOC(sizeof(struct rq_floating_flow_list));
    ffl->floating_flow_list = rq_array_alloc(
        (void (*)(void *))rq_floating_flow_free
        );
    return ffl;
}

RQ_EXPORT rq_floating_flow_list_t 
rq_floating_flow_list_alloc_unmanaged()
{
    rq_floating_flow_list_t ffl = (rq_floating_flow_list_t)RQ_MALLOC(sizeof(struct rq_floating_flow_list));
    ffl->floating_flow_list = rq_array_alloc(NULL);
    return ffl;
}

RQ_EXPORT void 
rq_floating_flow_list_free(rq_floating_flow_list_t ffl)
{
    rq_array_free(ffl->floating_flow_list);
    RQ_FREE(ffl);
}

RQ_EXPORT rq_floating_flow_list_t 
rq_floating_flow_list_clone(rq_floating_flow_list_t ffl)
{
    unsigned i;
    unsigned sz = rq_array_size(ffl->floating_flow_list);
    rq_floating_flow_list_t ffl2 = rq_floating_flow_list_alloc();

    for (i = 0; i < sz; i++)
    {
        rq_floating_flow_t ff = (rq_floating_flow_t)
            rq_array_get_at(
                ffl->floating_flow_list,
                i
                );
        rq_floating_flow_t ff2 = rq_floating_flow_clone(ff);
        rq_array_push_back(ffl2->floating_flow_list, ff2);
    }

    return ffl2;
}

RQ_EXPORT rq_floating_flow_list_t 
rq_floating_flow_list_clone_from(rq_floating_flow_list_t ffl, rq_date from_date)
{
    unsigned i;
    unsigned sz = rq_array_size(ffl->floating_flow_list);
    rq_floating_flow_list_t ffl2 = rq_floating_flow_list_alloc();

    for (i = 0; i < sz; i++)
    {
        rq_floating_flow_t ff = (rq_floating_flow_t)
            rq_array_get_at(
                ffl->floating_flow_list,
                i
                );
        if (ff->payment_date >= from_date)
        {
            rq_floating_flow_t ff2 = rq_floating_flow_clone(ff);
            rq_array_push_back(ffl2->floating_flow_list, ff2);
        }
    }

    return ffl2;
}

RQ_EXPORT void 
rq_floating_flow_list_clear(rq_floating_flow_list_t ffl)
{
    rq_array_clear(ffl->floating_flow_list);
}

RQ_EXPORT void 
rq_floating_flow_list_revert(rq_floating_flow_list_t ffl)
{
    unsigned i;
    unsigned sz = rq_array_size(ffl->floating_flow_list);

    for (i = 0; i < sz; i++)
    {
        rq_floating_flow_t ff = (rq_floating_flow_t)
            rq_array_get_at(
                ffl->floating_flow_list,
                i
                );

		// turn off the rate set flag
		if (ff->flow_status & RQ_FLOATING_FLOW_STATUS_FLOATING_RATE_SET)
			ff->flow_status ^= RQ_FLOATING_FLOW_STATUS_FLOATING_RATE_SET;
    }
}

RQ_EXPORT void 
rq_floating_flow_list_add(
    rq_floating_flow_list_t ffl, 
    rq_floating_flow_t f
    )
{
    rq_array_push_back(ffl->floating_flow_list, f);
}

RQ_EXPORT unsigned long
rq_floating_flow_list_size(
    rq_floating_flow_list_t ffl
    )
{
    return rq_array_size(ffl->floating_flow_list);
}

RQ_EXPORT rq_floating_flow_t
rq_floating_flow_list_get_at(
    rq_floating_flow_list_t ffl,
    unsigned long offset
    )
{
    return (rq_floating_flow_t)
        rq_array_get_at(
            ffl->floating_flow_list,
            offset
            );
}

RQ_EXPORT void
rq_floating_flow_list_invert_pay_receive(
    rq_floating_flow_list_t ffl
    )
{
    unsigned i;
    unsigned sz = rq_array_size(ffl->floating_flow_list);

    for (i = 0; i < sz; i++)
    {
        rq_floating_flow_t ff = (rq_floating_flow_t)
            rq_array_get_at(
                ffl->floating_flow_list,
                i
                );
        ff->pay_receive = 
            (ff->pay_receive == RQ_PAY_RECEIVE_PAY ? 
             RQ_PAY_RECEIVE_RECEIVE :
             RQ_PAY_RECEIVE_PAY);
    }
}

RQ_EXPORT rq_floating_flow_list_iterator
rq_floating_flow_list_begin(rq_floating_flow_list_t ffl)
{
	return (rq_floating_flow_list_iterator)(ffl->floating_flow_list->array);
}

RQ_EXPORT rq_floating_flow_list_iterator
rq_floating_flow_list_end(rq_floating_flow_list_t ffl)
{
	return (rq_floating_flow_list_iterator)(ffl->floating_flow_list->array + ffl->floating_flow_list->size);
}
