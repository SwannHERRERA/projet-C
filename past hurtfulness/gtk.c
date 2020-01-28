#include <gtk/gtk.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

static void
create_folder (GtkWidget *widget,
             gpointer   data)
{
    struct stat st;
    char* path_actual_dir;
    path_actual_dir = realpath(".", NULL);
    if (path_actual_dir != NULL) {
        if (stat(path_actual_dir, &st) == 0) {
            if (0 != mkdir(strcat(path_actual_dir, "/mon_dossier"),0777)) {
                printf("When executing: mkdir(\"%s\")\n", path_actual_dir);
                perror("mkdir");
                exit(1);
            }
        }
    }
}

static void
list_directory (char *path) {
    DIR *actual_directory;
    struct dirent *directory;
    actual_directory = opendir(path);
    if (actual_directory) {
        while ((directory = readdir(actual_directory)) != NULL) {
            printf("%s\n", directory->d_name);
        }
        closedir(actual_directory);
    }
}

static void
activate (GtkApplication *app,
          gpointer        user_data)
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *button2;
    GtkWidget *button_box;

    window = gtk_application_window_new (app);
    gtk_window_set_title (GTK_WINDOW (window), "Window");
    gtk_window_set_default_size (GTK_WINDOW (window), 500, 400);

    button_box = gtk_button_box_new (GTK_ORIENTATION_HORIZONTAL);
    gtk_container_add (GTK_CONTAINER (window), button_box);

    button = gtk_button_new_with_label ("crée un dossier");
    g_signal_connect (button, "clicked", G_CALLBACK (create_folder), NULL);
    gtk_container_add (GTK_CONTAINER (button_box), button);

    button2 = gtk_button_new_with_label ("lister les fichiers");
    g_signal_connect (button2, "clicked", G_CALLBACK (list_directory), NULL);
    gtk_container_add (GTK_CONTAINER (button_box), button2);

    gtk_widget_show_all (window);
}

int
main (int    argc,
      char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new ("app.id", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);

    return status;
}
