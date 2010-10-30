/*
** rq_iterator.c
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
#include "rq_iterator.h"
#include <stdlib.h>
#include <string.h>

RQ_EXPORT rq_iterator_t 
_rq_iterator_alloc(
    void (*freefunc)(void *iterdata),
    void (*incrfunc)(void *iterdata),
    void (*decrfunc)(void *iterdata),
    short (*atendfunc)(void *iterdata),
    short (*atbeginfunc)(void *iterdata),
    short (*seekfunc)(void *iterdata, enum rq_iterator_seek_position seek_pos, long offset),
    struct rq_variant (*getvaluefunc)(void *iterdata),
    void *iterdata
    )
{
    struct rq_iterator *it = (struct rq_iterator *)RQ_MALLOC(sizeof(struct rq_iterator));
    it->freefunc = freefunc;
    it->incrfunc = incrfunc;
    it->decrfunc = decrfunc;
    it->atendfunc = atendfunc;
    it->atbeginfunc = atbeginfunc;
    it->seekfunc = seekfunc;
    it->getvaluefunc = getvaluefunc;
    it->iterdata = iterdata;

    return it;
}


RQ_EXPORT void  
rq_iterator_free(rq_iterator_t it)
{
    if (it->iterdata && it->freefunc)
        (*it->freefunc)(it->iterdata);
    RQ_FREE(it);
}

RQ_EXPORT int
rq_iterator_is_null(rq_iterator_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT short 
rq_iterator_is_bidirectional(rq_iterator_t it)
{
    return it->decrfunc && it->atbeginfunc;
}

RQ_EXPORT void 
rq_iterator_incr(rq_iterator_t it)
{
    (*it->incrfunc)(it->iterdata);
}

RQ_EXPORT void 
rq_iterator_decr(rq_iterator_t it)
{
    (*it->decrfunc)(it->iterdata);
}

RQ_EXPORT short 
rq_iterator_at_end(rq_iterator_t it)
{
    return (*it->atendfunc)(it->iterdata);
}

RQ_EXPORT short 
rq_iterator_at_beginning(rq_iterator_t it)
{
    return (*it->atbeginfunc)(it->iterdata);
}

RQ_EXPORT struct rq_variant
rq_iterator_get_value(rq_iterator_t it)
{
    return (*it->getvaluefunc)(it->iterdata);
}

RQ_EXPORT short
rq_iterator_seek(rq_iterator_t it, enum rq_iterator_seek_position seek_pos, long offset)
{
    if (it->seekfunc)
        return (*it->seekfunc)(it->iterdata, seek_pos, offset);
    return -1;
}
