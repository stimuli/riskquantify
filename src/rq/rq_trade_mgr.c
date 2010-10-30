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
#include "rq_trade_mgr.h"
#include "rq_trade.h"
#include <stdlib.h>

static int
rq_trade_mgr_cmp(void *x, void *y)
{
    return ((int)x) != ((int)y);
}

static unsigned int
rq_trade_mgr_hash(void *x)
{
    return ((unsigned int)x);
}

rq_trade_mgr_t
rq_trade_mgr_alloc()
{
    rq_trade_mgr_t trade_mgr = (rq_trade_mgr_t)RQ_CALLOC(1, sizeof(struct rq_trade_mgr));
    trade_mgr->ht = rq_hashtable_init(rq_trade_mgr_cmp, rq_trade_mgr_hash);
    return trade_mgr;
}

void
rq_trade_mgr_free(rq_trade_mgr_t trade_mgr)
{
    if (trade_mgr)
    {
        if (trade_mgr->ht)
            rq_hashtable_free(trade_mgr->ht, rq_trade_free);
        RQ_FREE(trade_mgr);
    }
}

void
rq_trade_mgr_add(rq_trade_mgr_t trade_mgr, rq_trade_t trade)
{
    if (trade_mgr && trade)
    {
        if (trade_mgr->ht)
            rq_hashtable_insert(trade_mgr->ht, (void *)trade->trd_id, trade);
    }
}

rq_trade_t
rq_trade_mgr_get(rq_trade_mgr_t trade_mgr, long id)
{
    rq_trade_t *f = NULL;

    if (trade_mgr && id)
    {
        if (trade_mgr->ht)
            f = (rq_trade_t)rq_hashtable_find(trade_mgr->ht, (void *)id);
    }

    return f;
}

long
rq_trade_mgr_load(rq_trade_mgr_t trade_mgr, sqlite3 *db)
{
    sqlite3_stmt *stmt;
    const char *sql = "SELECT trd_id,trd_key,trd_insert_time,trd_usr_id,trd_tst_code,trd_tty_code,trd_bys_code,trd_cpt_key,trd_bok_code,trd_data FROM trade";
    long row_count;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL))
    {
        return -1;
    }

    for (row_count = 0; sqlite3_step(stmt) == SQLITE_ROW; row_count++)
    {
        int trd_id = sqlite3_column_int(stmt, 0);
        const char *trd_key = (const char *)sqlite3_column_text(stmt, 1);
        const char *trd_insert_time = (const char *)sqlite3_column_text(stmt, 2);
        const char *trd_usr_id = (const char *)sqlite3_column_text(stmt, 3);
        const char *trd_tst_code = (const char *)sqlite3_column_text(stmt, 4);
        const char *trd_tty_code = (const char *)sqlite3_column_text(stmt, 5);
        const char *trd_bys_code = (const char *)sqlite3_column_text(stmt, 6);
        const char *trd_cpt_key = (const char *)sqlite3_column_text(stmt, 7);
        const char *trd_bok_code = (const char *)sqlite3_column_text(stmt, 8);
        const char *trd_data = (const char *)sqlite3_column_text(stmt, 9);

        rq_trade_t *trade = rq_trade_alloc(
            trd_id,
            trd_key,
            trd_insert_time,
            trd_usr_id,
            trd_tst_code,
            trd_tty_code,
            trd_bys_code,
            trd_cpt_key,
            trd_bok_code,
            trd_data
            );
        trade_mgr_add(trade);

    }

    sqlite3_finalize(stmt);

    return row_count;
}

