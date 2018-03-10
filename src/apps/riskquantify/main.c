/*
** main.c
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
#include <gtk/gtk.h>
#include "rqui.h"
#include "rqui_system_sel.h"

static void
wnd_main_system_exit(void *x)
{
    gtk_exit(0);
}

static void
wnd_main_system_new(void *x)
{
    /* 
       free the rq_system object (if it has been allocated)
       and then reallocate it.
    */
}

static void
wnd_main_system_open(void *x)
{
    /* create the file selection dialog */
    rqui_system_sel_t system_sel = rqui_system_sel_alloc();
    if (rqui_system_sel_choose_file(system_sel))
    {
    }
    rqui_system_sel_free(system_sel);
}

static void
wnd_main_destroy()
{
    gtk_exit(0);
}

void
wnd_main_menu_create(GtkWidget *parent)
{
    GtkWidget *mnubar_main = gtk_menu_bar_new();
    GtkWidget *mnu_system = gtk_menu_new();
    GtkWidget *mnu_help = gtk_menu_new();
    GtkWidget *mnit_system = gtk_menu_item_new_with_label("System");
    GtkWidget *mnit_system_new = gtk_menu_item_new_with_label("New");
    GtkWidget *mnit_system_open = gtk_menu_item_new_with_label("Open...");
    GtkWidget *mnit_system_sep1 = gtk_menu_item_new();
    GtkWidget *mnit_system_exit = gtk_menu_item_new_with_label("Exit");
    GtkWidget *mnit_help = gtk_menu_item_new_with_label("Help");
    GtkWidget *mnit_help_about = gtk_menu_item_new_with_label("About...");

    /* add the handlers to the menu items */
    gtk_signal_connect(
        GTK_OBJECT(mnit_system_new),
        "activate",
        wnd_main_system_new,
        NULL
        );
    gtk_signal_connect(
        GTK_OBJECT(mnit_system_open),
        "activate",
        wnd_main_system_open,
        NULL
        );
    gtk_signal_connect(
        GTK_OBJECT(mnit_system_exit),
        "activate",
        wnd_main_system_exit,
        NULL
        );

    /* append the menu items to the system menu */
    gtk_menu_shell_append(
        GTK_MENU_SHELL(mnu_system),
        mnit_system_new
        );
    gtk_widget_show(mnit_system_new);
    gtk_menu_shell_append(
        GTK_MENU_SHELL(mnu_system),
        mnit_system_open
        );
    gtk_widget_show(mnit_system_open);
    gtk_menu_shell_append(
        GTK_MENU_SHELL(mnu_system),
        mnit_system_sep1
        );
    gtk_widget_show(mnit_system_sep1);
    gtk_menu_shell_append(
        GTK_MENU_SHELL(mnu_system),
        mnit_system_exit
        );
    gtk_widget_show(mnit_system_exit);

    /* append the menu items to the help menu */
    gtk_menu_shell_append(
        GTK_MENU_SHELL(mnu_help),
        mnit_help_about
        );
    gtk_widget_show(mnit_help_about);

    /* show the top level menu items */
    gtk_widget_show(mnit_system);
    gtk_widget_show(mnit_help);

    /* add the submenus */
    gtk_menu_item_set_submenu(
        GTK_MENU_ITEM(mnit_system),
        mnu_system
        );
    gtk_menu_item_set_submenu(
        GTK_MENU_ITEM(mnit_help),
        mnu_help
        );

    /* pack the menu bar into the box */
    gtk_box_pack_start(
        GTK_BOX(parent),
        mnubar_main,
        FALSE,
        FALSE,
        2
        );
    gtk_widget_show(mnubar_main);

    /* append the top level menu items to the menu bar */
    gtk_menu_shell_append(
        GTK_MENU_SHELL(mnubar_main),
        mnit_system
        );
    gtk_menu_shell_append(
        GTK_MENU_SHELL(mnubar_main),
        mnit_help
        );
    gtk_menu_item_right_justify(GTK_MENU_ITEM(mnit_help));
}

void
wnd_main_workarea_create(GtkWidget *vbox_main)
{
    GtkWidget *hpaned_main = gtk_hpaned_new();
    GtkWidget *tree_portfolio = gtk_tree_new();
    GtkWidget *sw_deals = gtk_scrolled_window_new(NULL, NULL);
    GtkWidget *clist_deals = gtk_clist_new(2);
    GtkWidget *colhdr_deals_dealid = gtk_label_new("Deal ID");
    GtkWidget *colhdr_deals_desc = gtk_label_new("Description");

    /* show all the widgets */
    gtk_widget_show(hpaned_main);
    gtk_widget_show(tree_portfolio);
    gtk_widget_show(sw_deals);
    gtk_widget_show(clist_deals);
    gtk_widget_show(colhdr_deals_dealid);
    gtk_widget_show(colhdr_deals_desc);

    /* pack the widgets */
    gtk_box_pack_start(GTK_BOX(vbox_main), hpaned_main, TRUE, TRUE, 0);

    gtk_paned_pack1(GTK_PANED(hpaned_main), tree_portfolio, FALSE, TRUE);
    gtk_widget_set_usize(tree_portfolio, 100, 200);

    gtk_paned_pack2(GTK_PANED(hpaned_main), sw_deals, TRUE, TRUE);

    gtk_container_add(GTK_CONTAINER(sw_deals), clist_deals);

    /* customize the deals listing */
    gtk_clist_set_column_width(GTK_CLIST(clist_deals), 0, 80);
    gtk_clist_set_column_width(GTK_CLIST(clist_deals), 1, 200);
    gtk_clist_column_titles_show(GTK_CLIST(clist_deals));

    gtk_clist_set_column_widget(GTK_CLIST(clist_deals), 0, colhdr_deals_dealid);
    gtk_clist_set_column_widget(GTK_CLIST(clist_deals), 1, colhdr_deals_desc);
}

GtkWidget *
wnd_main_create()
{
    GtkWidget *vbox_main = NULL;
    GtkWidget *wnd_main = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GtkWidget *status_main = gtk_statusbar_new();

    /* -- main window -- */
    gtk_window_set_title(
        GTK_WINDOW(wnd_main),
        "Risk Quantify"
        );
    gtk_widget_set_usize(
        wnd_main,
        400,
        400
        );

    /* hook signals up to the main window */
    gtk_signal_connect(
        GTK_OBJECT(wnd_main), 
        "destroy",
        GTK_SIGNAL_FUNC(wnd_main_destroy), 
        NULL
        );

    /*
      divide the main window into 3 sections:
      <menubar>
      <main work area>
      <status bar>
    */
    vbox_main = gtk_vbox_new(0, 0);
    gtk_container_add(
        GTK_CONTAINER(wnd_main),
        vbox_main
        );
    gtk_widget_show(vbox_main);

    /* create the menu bar */
    wnd_main_menu_create(vbox_main);

    /* create the main work area */
    wnd_main_workarea_create(vbox_main);

    /* create a status bar */
    gtk_widget_show(status_main);
    gtk_box_pack_start(GTK_BOX(vbox_main), status_main, FALSE, FALSE, 0);

    return wnd_main;
}

int
main (int argc, char *argv[])
{
    GtkWidget *wnd_main;
    
    gtk_init(&argc, &argv);

    /* -- initialise the riskquantify objects -- */
    rqui_init();

  
    wnd_main = wnd_main_create();
    gtk_widget_show(wnd_main);
    
    gtk_main();

    rqui_term();
    
    return 0;
}

