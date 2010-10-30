/*
** rq_trade.c
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
#include "rq_trade.h"
#include "rq_array.h"
#include <stdlib.h>

RQ_EXPORT int
rq_trade_is_null(rq_trade_t obj)
{
    return (obj == NULL);
}

RQ_EXPORT rq_trade_t
rq_trade_alloc(
    int trd_id,
    const char *trd_key,
    const char *trd_insert_time,
    const char *trd_usr_id,
    const char *trd_tst_code,
    const char *trd_tty_code,
    const char *trd_bys_code,
    const char *trd_cpt_key,
    const char *trd_bok_code,
    const char *trd_data
    )
{
    rq_trade_t trade = (rq_trade_t)RQ_CALLOC(1, sizeof(struct rq_trade));
    trade->trd_id = trd_id;
    if (trd_key)
        trade->trd_key = (const char *)RQ_STRDUP(trd_key);
    else
        trade->trd_key = NULL;
    if (trd_insert_time)
        trade->trd_insert_time = (const char *)RQ_STRDUP(trd_insert_time);
    else
        trade->trd_insert_time = NULL;
    if (trd_usr_id)
        trade->trd_usr_id = (const char *)RQ_STRDUP(trd_usr_id);
    else
        trade->trd_usr_id = NULL;
    if (trd_tst_code)
        trade->trd_tst_code = (const char *)RQ_STRDUP(trd_tst_code);
    else
        trade->trd_tst_code = NULL;
    if (trd_tty_code)
        trade->trd_tty_code = (const char *)RQ_STRDUP(trd_tty_code);
    else
        trade->trd_tty_code = NULL;
    if (trd_bys_code)
        trade->trd_bys_code = (const char *)RQ_STRDUP(trd_bys_code);
    else
        trade->trd_bys_code = NULL;
    if (trd_cpt_key)
        trade->trd_cpt_key = (const char *)RQ_STRDUP(trd_cpt_key);
    else
        trade->trd_cpt_key = NULL;
    if (trd_bok_code)
        trade->trd_bok_code = (const char *)RQ_STRDUP(trd_bok_code);
    else
        trade->trd_bok_code = NULL;
    if (trd_data)
        trade->trd_data = (const char *)RQ_STRDUP(trd_data);
    else
        trade->trd_data = NULL;

    return trade;
}

RQ_EXPORT void
rq_trade_free(rq_trade_t trade)
{
    if (trade)
    {
        if (trade->trd_key)
            RQ_FREE(trade->trd_key);
        if (trade->trd_insert_time)
            RQ_FREE(trade->trd_insert_time);
        if (trade->trd_usr_id)
            RQ_FREE(trade->trd_usr_id);
        if (trade->trd_tst_code)
            RQ_FREE(trade->trd_tst_code);
        if (trade->trd_tty_code)
            RQ_FREE(trade->trd_tty_code);
        if (trade->trd_bys_code)
            RQ_FREE(trade->trd_bys_code);
        if (trade->trd_cpt_key)
            RQ_FREE(trade->trd_cpt_key);
        if (trade->trd_bok_code)
            RQ_FREE(trade->trd_bok_code);
        if (trade->trd_data)
            RQ_FREE(trade->trd_data);
        RQ_FREE(trade);
    }
}


