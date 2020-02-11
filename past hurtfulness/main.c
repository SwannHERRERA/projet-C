/*
Author: Swann HERRERA
Date: 14/01/2020
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

enum {
    FILE_NAME,
    FILE_OFFSET,
    FILE_SIZE,
    FILE_DESCRIPTION,
    COLOR,
    N_COLUMNS
};

GtkWidget         *window, *entry, *grid, *go, *delete_button, *tree_view;
GtkListStore      *model;
GtkTreeViewColumn *column;

void get_model() {
    const char* path = gtk_entry_get_text(GTK_ENTRY(entry));
    u_int16_t nb_of_file = count_nb_file_in_dir(path);
    u_int8_t i;
    MY_FILE* file_names = list_directory(path);

    model = gtk_list_store_new(N_COLUMNS,
                            G_TYPE_STRING,   /* FILE_NAME */
                            G_TYPE_UINT,     /* FILE_OFFSET */
                            G_TYPE_UINT,     /* FILE_SIZE */
                            G_TYPE_STRING,   /* FILE_DESCRIPTION */
                            G_TYPE_STRING    /* COLOR */
                            );

    for (i = 0; i < nb_of_file; i++) {
        gtk_list_store_insert_with_values(model, NULL, -1,
                                        FILE_NAME, file_names[i].name,
                                        FILE_OFFSET, 0,
                                        FILE_SIZE, 10,
                                        -1);
    }
    /*****  FREE  ******/
    for (i = 0; i < nb_of_file; i++) {
        free((file_names + i)->name);
    }
    free(file_names);
}

void load_tree_view() {
    gtk_grid_attach(GTK_GRID(grid), tree_view, 0, 1, 8, 4);
    tree_view = gtk_tree_view_new();
    get_model();
    gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(model));
    g_object_unref(model);
    column = gtk_tree_view_column_new_with_attributes("Name",
                                                      gtk_cell_renderer_text_new(),
                                                      "text", FILE_NAME,
                                                      "background", COLOR,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("Offset",
                                                      gtk_cell_renderer_spin_new(),
                                                      "text", FILE_OFFSET,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

    column = gtk_tree_view_column_new_with_attributes("Size",
                                                      gtk_cell_renderer_text_new(),
                                                      "text", FILE_SIZE,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
    gtk_grid_attach(GTK_GRID(grid), tree_view, 0 , 1, 4, 4);
}

// void refresh_tree_view() {
//     gtk_container_remove(tree_view);
//     tree_view = gtk_tree_view_new();
//     get_model();
//     gtk_tree_view_set_model(GTK_TREE_VIEW(tree_view), GTK_TREE_MODEL(model));
//     g_object_unref(model);
//     column = gtk_tree_view_column_new_with_attributes("Name",
//                                                       gtk_cell_renderer_text_new(),
//                                                       "text", FILE_NAME,
//                                                       "background", COLOR,
//                                                       NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

//     column = gtk_tree_view_column_new_with_attributes("Offset",
//                                                       gtk_cell_renderer_spin_new(),
//                                                       "text", FILE_OFFSET,
//                                                       NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);

//     column = gtk_tree_view_column_new_with_attributes("Size",
//                                                       gtk_cell_renderer_text_new(),
//                                                       "text", FILE_SIZE,
//                                                       NULL);
//     gtk_tree_view_append_column(GTK_TREE_VIEW(tree_view), column);
// }

static void
activate (GtkApplication* app,
          gpointer        user_data) {

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "File Manager");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 500);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "/home/swann/Documents/ESGI/C");
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 4, 1);

    go = gtk_button_new_with_label("go");
    g_signal_connect (go, "clicked", G_CALLBACK(load_tree_view), NULL);
    gtk_grid_attach(GTK_GRID(grid), go, 5, 0, 2, 1);

    // load_tree_view();

    gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
  GtkApplication *app;
  int status;

  app = gtk_application_new("file.manager", G_APPLICATION_FLAGS_NONE);
  g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run (G_APPLICATION(app), argc, argv);
  g_object_unref (app);

  return status;
}
