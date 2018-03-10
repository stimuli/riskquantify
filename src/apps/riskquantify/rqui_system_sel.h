/*
** rqui_system_sel.h
**
** Written by Brett Hutley - brett@hutley.net
**
** Copyright (C) 2002 Brett Hutley
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
#ifndef rqui_system_sel_h
#define rqui_system_sel_h

#include <rq.h>

RQ_DECLARE_OBJECT(rqui_system_sel_t);

rqui_system_sel_t rqui_system_sel_alloc();
rqui_system_sel_t rqui_system_sel_free(rqui_system_sel_t sel);

short rqui_system_sel_choose_file(rqui_system_sel_t sel);

#endif
