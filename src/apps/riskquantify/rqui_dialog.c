#include "rqui_dialog.h"
#include <gtk/gtk.h>

struct rqui_dialog {
    GtkWidget *window;
    GtkWidget *message;
};

rqui_dialog_t 
rqui_dialog_alloc(const char *title, const char *message)
{
    struct rqui_dialog *d = (struct rqui_dialog *)malloc(sizeof(struct rqui_dialog));

    d->window = gtk_window_new(GTK_WINDOW_DIALOG);
    gtk_window_set_title(GTK_WINDOW(d->window), title);

    gtk_container_border_width(GTK_CONTAINER(d->window), 10);
     
    d->message = gtk_label_new(message);
    gtk_container_add(
        GTK_CONTAINER(d->window), 
        d->message
        );
    gtk_widget_show(d->message);

    RQ_RETURN(rqui_dialog_t, d);
}

void
rqui_dialog_show_modal(rqui_dialog_t dlg)
{
    struct rqui_dialog *d = (struct rqui_dialog *)dlg.p;

    gtk_window_set_modal(GTK_WINDOW(d->window), TRUE);

    gtk_widget_show(d->window);
}

void
rqui_dialog_free(rqui_dialog_t dlg)
{
    struct rqui_dialog *d = (struct rqui_dialog *)dlg.p;

    gtk_widget_destroy(d->window);
    free(d);
}
