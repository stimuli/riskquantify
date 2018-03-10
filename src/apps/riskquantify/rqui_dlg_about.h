/*
** rqui_dlg_about.h
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
#ifndef rqui_dlg_about_h
#define rqui_dlg_about_h

/* -- includes ---------------------------------------------------- */
#include <rq.h>

/* -- typedefs ---------------------------------------------------- */
RQ_DECLARE_OBJECT(rqui_dlg_about_t);

/* -- prototypes -------------------------------------------------- */
rqui_dlg_about_t rqui_dlg_about_alloc();
void rqui_dlg_about_free(rqui_dlg_about_t dlg);
void rqui_dlg_about_show_modal(rqui_dlg_about_t dlg);

#endif
