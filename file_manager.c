/*
Author: Swann HERRERA
Date: 29/01/2020
Objectif: List all file in a folder
*/
#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_util.h"

GtkWidget* window;
GtkWidget* container;
GtkWidget* button;
GtkWidget* entry;
GtkWidget* label;
GtkBuilder *builder;

void activate(int argc,char** argv);
void load_widget();
void on_btn_click();

void on_quit() {
    const char* txt = gtk_entry_get_text(GTK_ENTRY(entry));
    printf("%s\n", txt);
    gtk_main_quit();
}

int main(int argc, char** argv) {
    activate(argc, argv);

    return EXIT_SUCCESS;
}
void activate(int argc,char** argv) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("./test.ui");

    load_widget();

    g_signal_connect(window, "destroy", G_CALLBACK(on_quit), NULL);

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show_all(window);
    gtk_main();
}

void load_widget() {
    window      = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    container   = GTK_WIDGET(gtk_builder_get_object(builder, "container"));
    button      = GTK_WIDGET(gtk_builder_get_object(builder, "btn_search"));
    entry       = GTK_WIDGET(gtk_builder_get_object(builder, "entry_search"));
    label       = GTK_WIDGET(gtk_builder_get_object(builder, "test_label"));
}

void on_btn_click(GtkButton *b) {
    const char* txt = gtk_entry_get_text(GTK_ENTRY(entry));
    gtk_label_set_text(GTK_LABEL(label), (const gchar*) txt);
}