#include <gtk/gtk.h>

void
on_button_clicked(GtkButton *button, gpointer data)
{
    GtkWidget *widget;
    widget = (GtkWidget *) data;
    if (widget == NULL)
        return;
    gtk_widget_show(widget);
    return;
}

gboolean
on_widget_deleted(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    gtk_widget_hide(widget);
    return TRUE;
}

int
main(int argc, char **argv)
{
    GtkWidget *window1;
    GtkWidget *window2;
    GtkWidget *button;
    gtk_init(&argc, &argv);

    window1 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    window2 = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    button = gtk_button_new_with_label("Show again...");

    g_signal_connect(G_OBJECT(window1),
        "destroy", gtk_main_quit, NULL);
    g_signal_connect(G_OBJECT(window2), 
        "delete-event", G_CALLBACK(on_widget_deleted), NULL);
    g_signal_connect(G_OBJECT(button), 
        "clicked", G_CALLBACK(on_button_clicked), window2);
    gtk_container_add(GTK_CONTAINER(window1), button);
    gtk_widget_set_size_request(window1, 300, 100);
    gtk_widget_set_size_request(window2, 300, 100);

    gtk_widget_show_all(window1);
    gtk_widget_show(window2);

    gtk_main();
    return 0;
}