#include <gtk/gtk.h>
#include <limits.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>

int main (int argc, char** argv) {
    GtkWidget *main_window = NULL;
    GtkBuilder *builder = NULL;
    GError *error = NULL;
    gchar *filename = NULL;
    /* Initialisation de la librairie Gtk. */
    gtk_init(&argc, &argv);

    /* Ouverture du fichier Glade de la fenêtre principale */
    builder = gtk_builder_new();

    /* Création du chemin complet pour accéder au fichier test.glade. */
    /* g_build_filename(); construit le chemin complet en fonction du système */
    /* d'exploitation. ( / pour Linux et \ pour Windows) */
    filename =  g_build_filename ("test.ui", NULL);

        /* Chargement du fichier test.glade. */
    gtk_builder_add_from_file (builder, filename, &error);
    g_free (filename);
    if (error)
    {
    gint code = error->code;
    g_printerr("%s\n", error->message);
    g_error_free (error);
    return code;
    }

    /* Récupération du pointeur de la fenêtre principale */
    main_window = GTK_WIDGET(gtk_builder_get_object (builder, "MainWindow"));

    /* Affectation du signal "destroy" à la fonction gtk_main_quit(); pour la */
    /* fermeture de la fenêtre. */
    g_signal_connect (G_OBJECT (main_window), "destroy", (GCallback)gtk_main_quit, NULL);

    /* Affichage de la fenêtre principale. */
    gtk_widget_show_all (main_window);

    gtk_main();

    return 0;
}