#include <gtk/gtk.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
// #include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "file_util.h"
#include "param_file.h"

enum {
    FILE_NAME,
    FILE_SIZE,
    COLOR,
    N_COLUMNS
};

void on_quit();
void quit_modal_create_file();
void quit_modal_create_folder();
void quit_modal_settings();
void quit_modal_file();
void activate(int argc,char** argv);
void load_widgets();
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
void on_setting_btn_clicked();