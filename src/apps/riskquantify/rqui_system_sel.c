/*
** rqui_system_sel.cpp
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
#include "rqui_system_sel.h"
#include "rqui_dialog.h"
#include <gtk/gtk.h>
#include <stdio.h>

/* -- structs ----------------------------------------------------- */
struct rqui_system_sel {
    GtkWidget *filesel_system;
    GtkWidget *bt_ok;
    GtkWidget *bt_cancel;
    int ok_pressed;
};

/* -- code -------------------------------------------------------- */
static void
rqui_system_sel_ok(GtkWidget *bt, void *s)
{
    struct rqui_system_sel *sel = (struct rqui_system_sel *)s;
    const char *filename = gtk_file_selection_get_filename(GTK_FILE_SELECTION(sel->filesel_system));

    if (strlen(filename) == 0 || filename[strlen(filename)-1] == '/')
    {
        rqui_dialog_t dlg = rqui_dialog_alloc("Error", "Must specify a file name");
        rqui_dialog_show_modal(dlg);
        rqui_dialog_free(dlg);

        return;
    }


    gtk_main_quit();
}

static void
rqui_system_sel_cancel(GtkWidget *bt, void *s)
{
    struct rqui_system_sel *sel = (struct rqui_system_sel *)s;

    gtk_main_quit();
}

rqui_system_sel_t 
rqui_system_sel_alloc()
{
    struct rqui_system_sel *sel = (struct rqui_system_sel *)calloc(
        1,
        sizeof(struct rqui_system_sel)
        );

    GtkWidget *filesel_system;

    sel->filesel_system = gtk_file_selection_new("Select the system XML file");
    gtk_container_set_border_width(GTK_CONTAINER(sel->filesel_system), 10);
    GTK_WINDOW(sel->filesel_system)->type = GTK_WINDOW_DIALOG;

    gtk_window_set_modal(GTK_WINDOW(sel->filesel_system), TRUE);

    sel->bt_ok = GTK_FILE_SELECTION(sel->filesel_system)->ok_button;
    gtk_widget_show(sel->bt_ok);
    GTK_WIDGET_SET_FLAGS(sel->bt_ok, GTK_CAN_DEFAULT);
    /* gtk_widget_set_sensitive(sel->bt_ok, FALSE); */

    /* add a handler to the OK button */
    gtk_signal_connect(
        GTK_OBJECT(sel->bt_ok),
        "clicked",
        rqui_system_sel_ok,
        sel
        );

    sel->bt_cancel = GTK_FILE_SELECTION(sel->filesel_system)->cancel_button;
    gtk_widget_show(sel->bt_cancel);
    GTK_WIDGET_SET_FLAGS(sel->bt_cancel, GTK_CAN_DEFAULT);

    /* add a handler to the Cancel button */
    gtk_signal_connect(
        GTK_OBJECT(sel->bt_cancel),
        "clicked",
        rqui_system_sel_cancel,
        sel
        );

    RQ_RETURN(rqui_system_sel_t, sel);
}

rqui_system_sel_t 
rqui_system_sel_free(rqui_system_sel_t s)
{
    struct rqui_system_sel *sel = (struct rqui_system_sel *)s.p;

    gtk_widget_destroy(sel->filesel_system);
    
    free(sel);
}

short 
rqui_system_sel_choose_file(rqui_system_sel_t s)
{
    struct rqui_system_sel *sel = (struct rqui_system_sel *)s.p;

    gtk_widget_show_all(sel->filesel_system);
    gtk_main();

    return 1;
}
