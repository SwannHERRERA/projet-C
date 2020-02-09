/*
Author: Swann HERRERA
Date: 29/01/2020
Objectif: List all file in a folder
*/
#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_util.h"

GtkBuilder* builder;

/* ---- Main windows ---- */
GtkWidget* window;
GtkWidget* container;
GtkWidget* btn_search;
GtkWidget* entry_search;
GtkWidget* btn_new;
GtkWidget* btn_new_folder;

/* ---- Modal create file ---- */
GtkWidget* modal_create_file;
GtkWidget* modal_create_file_fixed;
GtkWidget* create_file_entry;
GtkWidget* create_file_btn;
GtkWidget* error_create_file_label;

/*---- Modal file ----*/
GtkWidget* modal_file;
GtkWidget* modal_file_fixed;
GtkWidget* rename_entry;
GtkWidget* rename_btn;
GtkWidget* delete_btn;
GtkWidget* parent_folder_label;
GtkWidget* error_rename;
char* old_name;

/* ---- Modal creation of folder ---- */
GtkWidget* modal_create_folder;
GtkWidget* modal_create_folder_fixed;
GtkWidget* create_folder_entry;
GtkWidget* create_folder_btn;
GtkWidget* error_create_folder_label;

GtkWidget*          list_of_file;
GtkListStore*       model;
GtkTreeViewColumn*  column;

enum {
    FILE_NAME,
    FILE_SIZE,
    FILE_DESCRIPTION,
    COLOR,
    N_COLUMNS
};

void on_quit();
void quit_modal_create_file();
void quit_modal_create_folder();
void quit_modal_file();
void activate(int argc,char** argv);
void load_widget();
void on_btn_search_clicked();
void on_btn_new_clicked();
void on_click_new_folder();
void on_create_file_btn_clicked();
void on_create_folder_btn_clicked();
void row_click();
void init_tree_view();
char* get_name_row_activated(GtkTreeView *tree_view, GtkTreePath *path);
void actualize_tree_view();
void on_delete_btn_clicked();
void on_rename_btn_clicked();

int main(int argc, char** argv) {
    activate(argc, argv);

    return EXIT_SUCCESS;
}
void activate(int argc,char** argv) {
    gtk_init(&argc, &argv);

    builder = gtk_builder_new_from_file("./test.ui");

    load_widget();

    gtk_builder_connect_signals(builder, NULL);

    gtk_widget_show(window);
    gtk_main();
}

void load_widget() {
    /*----  Main window  ----*/
    window          = GTK_WIDGET(gtk_builder_get_object(builder, "window"));
    gtk_window_set_title(GTK_WINDOW(window), "File manager");
    g_signal_connect(window, "destroy", G_CALLBACK(on_quit), NULL);
    container       = GTK_WIDGET(gtk_builder_get_object(builder, "container"));
    btn_search      = GTK_WIDGET(gtk_builder_get_object(builder, "btn_search"));
    entry_search    = GTK_WIDGET(gtk_builder_get_object(builder, "entry_search"));
    init_tree_view();
    btn_new         = GTK_WIDGET(gtk_builder_get_object(builder, "btn_new"));
    btn_new_folder  = GTK_WIDGET(gtk_builder_get_object(builder, "btn_new_folder"));

    /*----  modal create file  ----*/
    modal_create_file       = GTK_WIDGET(gtk_builder_get_object(builder, "modal_create_file"));
    gtk_window_set_title(GTK_WINDOW(modal_create_file), "create file");
    g_signal_connect(modal_create_file, "delete-event", G_CALLBACK(quit_modal_create_file), NULL);
    modal_create_file_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "modal_create_file_fixed"));
    create_file_entry       = GTK_WIDGET(gtk_builder_get_object(builder, "create_file_entry"));
    create_file_btn         = GTK_WIDGET(gtk_builder_get_object(builder, "create_file_btn"));
    error_create_file_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_create_file_label"));
    
    /*----  modal file  ----*/
    modal_file          = GTK_WIDGET(gtk_builder_get_object(builder, "modal_file"));
    gtk_window_set_title(GTK_WINDOW(modal_file), "Propriété");
    g_signal_connect(modal_file, "delete-event", G_CALLBACK(quit_modal_file), NULL);
    modal_file_fixed    = GTK_WIDGET(gtk_builder_get_object(builder, "modal_file_fixed"));
    rename_entry        = GTK_WIDGET(gtk_builder_get_object(builder, "rename_entry"));
    rename_btn          = GTK_WIDGET(gtk_builder_get_object(builder, "rename_btn"));
    delete_btn          = GTK_WIDGET(gtk_builder_get_object(builder, "delete_btn"));
    parent_folder_label = GTK_WIDGET(gtk_builder_get_object(builder, "parent_folder"));
    error_rename        = GTK_WIDGET(gtk_builder_get_object(builder, "error_rename"));


    /*----  modal create folder  ----*/
    modal_create_folder       = GTK_WIDGET(gtk_builder_get_object(builder, "modal_create_folder"));
    gtk_window_set_title(GTK_WINDOW(modal_create_folder), "create folder");
    g_signal_connect(modal_create_folder, "delete-event", G_CALLBACK(quit_modal_create_folder), NULL);
    modal_create_folder_fixed = GTK_WIDGET(gtk_builder_get_object(builder, "modal_create_folder_fixed"));
    create_folder_entry       = GTK_WIDGET(gtk_builder_get_object(builder, "create_folder_entry"));
    create_folder_btn         = GTK_WIDGET(gtk_builder_get_object(builder, "create_folder_btn"));    
    error_create_folder_label = GTK_WIDGET(gtk_builder_get_object(builder, "error_create_folder_label"));
}

void on_quit() {
    gtk_main_quit();
}

void quit_modal_create_file() {
    gtk_widget_hide(modal_create_file);
}

void quit_modal_create_folder() {
    gtk_widget_hide(modal_create_folder);
}

void quit_modal_file() {
    actualize_tree_view();
    free(old_name);
    gtk_widget_hide(modal_file);
}

void on_btn_search_clicked() {
    actualize_tree_view();
}

void actualize_tree_view() {
    const char* path = gtk_entry_get_text(GTK_ENTRY(entry_search));
    u_int16_t nb_of_file = count_nb_file_in_dir(path);
    u_int8_t i;
    MY_FILE* file_names = list_directory(path);
    gtk_list_store_clear(model);

    for (i = 0; i < nb_of_file; i++) {
        if (file_names[i].is_dir == true) {
            char* path = malloc(sizeof(char) * (strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) + strlen(file_names[i].name) + 2));
            strcpy(path, (char*)gtk_entry_get_text(GTK_ENTRY(entry_search)));
            strcat(path, "/");
            strcat(path, file_names[i].name);

            gtk_list_store_insert_with_values(model, NULL, -1,
                                        FILE_NAME, file_names[i].name,
                                        FILE_SIZE, count_nb_file_in_dir(path),
                                        COLOR, "#154871",
                                        -1);
            free(path);
        } else {
            gtk_list_store_insert_with_values(model, NULL, -1,
                                        FILE_NAME, file_names[i].name,
                                        FILE_SIZE, 10,
                                        -1);
        }
    }
    free(file_names);
    gtk_tree_view_set_model(GTK_TREE_VIEW(list_of_file), GTK_TREE_MODEL(model));
}

void on_btn_new_clicked() {
    gtk_widget_show(modal_create_file);
}

void on_click_new_folder() {
    gtk_widget_show(modal_create_folder);
}

void on_create_file_btn_clicked() {
    bool result;
    char* path = malloc(sizeof(char) * (strlen(gtk_entry_get_text(GTK_ENTRY(create_file_entry))) + strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) + 2));
    strcpy(path, (char*)gtk_entry_get_text(GTK_ENTRY(entry_search)));
    strcat(path, "/");
    strcat(path, (char*)gtk_entry_get_text(GTK_ENTRY(create_file_entry)));
    result = create_file((char*)path);
    if (result == true) {
        gtk_label_set_text(GTK_LABEL(error_create_file_label), "Error creation of file");
    }
    free(path);
    gtk_widget_hide(modal_create_file);
    actualize_tree_view();
}

void on_create_folder_btn_clicked() {
    char* path = (char*)gtk_entry_get_text(GTK_ENTRY(entry_search));
    char* name = (char*)gtk_entry_get_text(GTK_ENTRY(create_folder_entry));
    bool result;
    result = create_folder(path, name);
    if (result == true) {
        gtk_label_set_text(GTK_LABEL(error_create_folder_label), "Error creation of folder");
    }
    gtk_widget_hide(modal_create_folder);
    actualize_tree_view();
}

void row_click(GtkTreeView *tree_view, GtkTreePath *path) {
    char* name = get_name_row_activated(tree_view, path);
    char* parent_folder;
    parent_folder = (char*)malloc(sizeof(char) * (strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) + strlen("Parent folder : ") + 1));
    strcpy(parent_folder, "Parent folder : ");
    strcat(parent_folder, (char*)gtk_entry_get_text(GTK_ENTRY(entry_search)));
    
    /*----  make old name in globale ----*/
    old_name = (char*)malloc(sizeof(char) * strlen(name));
    old_name = name;

    gtk_label_set_text(GTK_LABEL(parent_folder_label), parent_folder);
    free(parent_folder);
    gtk_entry_set_text(GTK_ENTRY(rename_entry), (const gchar*)name);
    gtk_widget_show(modal_file);

}

void init_tree_view() {
    char* path = (char*)malloc(sizeof(char) * (strlen("/home/") + strlen(getenv("USER")) + 1));
    strcpy(path, "/home/");
    strcat(path, getenv("USER"));

    gtk_entry_set_text(GTK_ENTRY(entry_search), (const gchar*) path);

    u_int16_t nb_of_file = count_nb_file_in_dir(path);
    u_int8_t i;
    MY_FILE* file_names = list_directory(path);
    list_of_file = GTK_WIDGET(gtk_builder_get_object(builder, "list_of_file"));

    model = gtk_list_store_new(N_COLUMNS,
                            G_TYPE_STRING,   /* FILE_NAME */
                            G_TYPE_STRING,     /* FILE_SIZE */
                            G_TYPE_STRING    /* COLOR */
                            );

    for (i = 0; i < nb_of_file; i++) {
        if (file_names[i].is_dir == true) {
            char* path = malloc(sizeof(char) * (strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) + strlen(file_names[i].name) + 2));
            strcpy(path, (char*)gtk_entry_get_text(GTK_ENTRY(entry_search)));
            strcat(path, "/");
            strcat(path, file_names[i].name);

            gtk_list_store_insert_with_values(model, NULL, -1,
                                        FILE_NAME, file_names[i].name,
                                        FILE_SIZE, count_nb_file_in_dir(path),
                                        COLOR, "#154871",
                                        -1);
            free(path);
        } else {
            gtk_list_store_insert_with_values(model, NULL, -1,
                                        FILE_NAME, file_names[i].name,
                                        FILE_SIZE, humanFileSize(file_names[i].size),
                                        -1);
        }
    }
    /*****  FREE  *****/
    for (i = 0; i < nb_of_file; i++) {
        free((file_names + i)->name);
    }
    free(file_names);
    gtk_tree_view_set_model(GTK_TREE_VIEW(list_of_file), GTK_TREE_MODEL(model));
    column = gtk_tree_view_column_new_with_attributes("Name",
                                                        gtk_cell_renderer_text_new(),
                                                        "text", FILE_NAME,
                                                        "background", COLOR,
                                                        NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_of_file), column);

    column = gtk_tree_view_column_new_with_attributes("Size",
                                                        gtk_cell_renderer_text_new(),
                                                        "text", FILE_SIZE,
                                                        NULL);
    gtk_tree_view_append_column(GTK_TREE_VIEW(list_of_file), column);

}

char* get_name_row_activated(GtkTreeView *tree_view, GtkTreePath *path) {
    char* name;

    GtkTreeIter iter;
    GtkTreeModel *model = gtk_tree_view_get_model(tree_view);
    if (gtk_tree_model_get_iter(model, &iter, path)) {
        gtk_tree_model_get(model, &iter, 0, &name, -1);
        return name;
    } else {
        fprintf(stderr, "Error! selected column not found!\n");
        exit(EXIT_FAILURE);
    }
}

void on_rename_btn_clicked() {
    char* new_path;
    char* old_path;
    char* error = "Error rename wrong new name";

    old_path = (char*)malloc(sizeof(char) * (strlen(old_name) + strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) + 2));
    new_path = (char*)malloc(sizeof(char) * (strlen(gtk_entry_get_text(GTK_ENTRY(rename_entry))) + strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) + 2));

    strcpy(old_path, (char*)gtk_entry_get_text(GTK_ENTRY(entry_search)));
    strcpy(new_path, (char*)gtk_entry_get_text(GTK_ENTRY(entry_search)));

    strcat(old_path, "/");
    strcat(new_path, "/");

    strcat(old_path, old_name);
    strcat(new_path, (char*)gtk_entry_get_text(GTK_ENTRY(rename_entry)));

    if (rename_file(old_path, new_path) != 0) {
        gtk_label_set_text(GTK_LABEL(error_rename), error);
    }
    free(new_path);
}


void on_delete_btn_clicked() {
    char* path;
    struct stat statbuf;

    path = (char*)malloc(sizeof(char) * (strlen(old_name) + strlen(gtk_entry_get_text(GTK_ENTRY(entry_search))) + 2));
    strcpy(path, (char*)gtk_entry_get_text(GTK_ENTRY(entry_search)));
    strcat(path, "/");
    strcat(path, old_name);

    if (stat(path, &statbuf) == -1) {
        perror("stat");
        exit(EXIT_FAILURE);
    }
    if (S_ISDIR(statbuf.st_mode)) {
        remove_directory(path);
    } else {
        delete_file(path);
    }
    free(path);
    actualize_tree_view();
    gtk_widget_hide(modal_file);
}