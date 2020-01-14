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

enum {
    FILE_NAME,
    FILE_OFFSET,
    FILE_SIZE,
    FILE_DESCRIPTION,
    COLOR,
    N_COLUMNS
};

char**
list_directory(const char* path) {
    char** file_names = (char**)malloc(sizeof(char *) * 50);
    u_int8_t i = 0;
    DIR *actual_directory;
    struct dirent *directory;
    actual_directory = opendir(path);
    if (actual_directory && file_names) {
        while ((directory = readdir(actual_directory)) != NULL || i < 50) {
            file_names[i] = (char*)malloc(sizeof(char) * strlen(directory->d_name));
            strcpy(file_names[i], directory->d_name);
            i += 1;
        }
        closedir(actual_directory);
    } else exit(1);
    return file_names;
}

static void
activate (GtkApplication* app,
          gpointer        user_data)
{
    GtkWidget         *window, *entry, *grid, *button, *array;
    GtkListStore      *model;
    GtkTreeViewColumn *column;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "File Manager");
    gtk_window_set_default_size (GTK_WINDOW (window), 700, 500);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);

    entry = gtk_entry_new();
    gtk_entry_set_text(GTK_ENTRY(entry), "/");
    gtk_grid_attach(GTK_GRID(grid), entry, 0, 0, 2, 1);

    button = gtk_button_new_with_label("go");
    gtk_grid_attach(GTK_GRID(grid), button, 3, 0, 1, 1);

    const char* pwd = gtk_entry_get_text(GTK_ENTRY(entry));
    // char** file_names = list_directory(pwd);
    model = gtk_list_store_new(N_COLUMNS,
                            G_TYPE_STRING,   /* FILE_NAME */
                            G_TYPE_UINT,     /* FILE_OFFSET */
                            G_TYPE_UINT,     /* FILE_SIZE */
                            G_TYPE_STRING,   /* FILE_DESCRIPTION */
                            G_TYPE_STRING    /* COLOR */
                            );

    gtk_list_store_insert_with_values(model, NULL, -1,
                                      FILE_NAME, "file_names[1]",
                                      FILE_OFFSET, 0,
                                      FILE_SIZE, 10,
                                      -1);
    // /* MODEL */
    // model = gtk_list_store_new(N_COLUMNS,
    //                            G_TYPE_STRING,   /* FILE_NAME */
    //                            G_TYPE_UINT,     /* FILE_OFFSET */
    //                            G_TYPE_UINT,     /* FILE_SIZE */
    //                            G_TYPE_STRING,   /* FILE_DESCRIPTION */
    //                            G_TYPE_STRING    /* COLOR */
    //                           );
    // gtk_list_store_insert_with_values(model, NULL, -1,
    //                                   FILE_NAME, "test name",
    //                                   FILE_OFFSET, 0,
    //                                   FILE_SIZE, 10,
    //                                   -1);
    // gtk_list_store_insert_with_values(model, NULL, -1,
    //                                   FILE_NAME, "Dummy",
    //                                   FILE_OFFSET, 123,
    //                                   COLOR, "black",
    //                                   -1);
    // gtk_list_store_insert_with_values(model, NULL, -1,
    //                                   COLOR, "blue",
    //                                   -1);

    array = gtk_tree_view_new_with_model(GTK_TREE_MODEL(model));
    g_object_unref(model);

    column = gtk_tree_view_column_new_with_attributes("Name",
                                                      gtk_cell_renderer_text_new(),
                                                      "text", FILE_NAME,
                                                      "background", COLOR,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(array), column);

    column = gtk_tree_view_column_new_with_attributes("Offset",
                                                      gtk_cell_renderer_spin_new(),
                                                      "text", FILE_OFFSET,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(array), column);

    column = gtk_tree_view_column_new_with_attributes("Size",
                                                      gtk_cell_renderer_text_new(),
                                                      "text", FILE_SIZE,
                                                      NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(array), column);

    gtk_grid_attach(GTK_GRID(grid), array, 0 , 1, 4, 4);

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
