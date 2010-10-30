/**
 * @file
 *
 * This file defines the functions for storing and loading the
 * Risk Quantify system from a directory on the local filesystem.
 * The underlying Risk Quantify objects are all contained within files
 */
/*
** rq_data_store_fs.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2009 Brett Hutley
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
#ifndef rq_data_store_fs_h
#define rq_data_store_fs_h

#include <stdio.h>

#include "rq_data_store.h"

#ifdef __cplusplus
extern "C" {
#if 0
} // purely to not screw up my indenting...
#endif
#endif

/* -- defines ----------------------------------------------------- */
/* Change the store version number, if the store format changes. */
#define RQ_DATA_STORE_FS_VERSION_NUM 1

/* -- structs ----------------------------------------------------- */
struct rq_data_store_fs_data {
    
};


/* -- prototypes -------------------------------------------------- */
RQ_EXPORT rq_data_store_t rq_data_store_fs_alloc();

#ifdef __cplusplus
#if 0
{ // purely to not screw up my indenting...
#endif
};
#endif

#endif
