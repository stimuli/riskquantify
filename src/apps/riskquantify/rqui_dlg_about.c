/*
** rqui_dlg_about.c
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
#include "rqui_dlg_about.h"
#include <gtk/gtk.h>

struct rqui_dlg_about {
    GtkWidget *dlg;
    GtkWidget *lbl;
    GtkWidget *bt_ok;
};

static void
rqui_dlg_about_ok(GtkWidget *bt, void *dlg)
{
    struct rqui_dlg_about *d = (struct rqui_dlg_about *)dlg;

    gtk_widget_hide(d->dlg);
    gtk_main_quit();
}

rqui_dlg_about_t 
rqui_dlg_about_alloc()
{
    struct rqui_dlg_about *d = (struct rqui_dlg_about *)calloc(1, sizeof(struct rqui_dlg_about));
    d->dlg = gtk_dialog_new();
    d->lbl = gtk_label_new("About Risk Quantify");
    d->bt_ok = gtk_button_new_with_label("OK");

    gtk_window_set_modal(GTK_WINDOW(d->dlg), TRUE);
   
    gtk_signal_connect(
        GTK_OBJECT(d->bt_ok), 
        "clicked",
        rqui_dlg_about_ok,
        d
        );
    gtk_container_add(
        GTK_CONTAINER(GTK_DIALOG(d->dlg)->action_area),
        d->bt_ok
        );

    gtk_container_add(
        GTK_CONTAINER(GTK_DIALOG(d->dlg)->vbox),
        d->lbl
        );

    RQ_RETURN(rqui_dlg_about_t, d);
}

void 
rqui_dlg_about_free(rqui_dlg_about_t dlg)
{
    struct rqui_dlg_about *d = (struct rqui_dlg_about *)dlg.p;
    gtk_widget_destroy(d->dlg);
    free(d);
}

void 
rqui_dlg_about_show_modal(rqui_dlg_about_t dlg)
{
    struct rqui_dlg_about *d = (struct rqui_dlg_about *)dlg.p;
    gtk_widget_show_all(d->dlg);
    gtk_main();
}
