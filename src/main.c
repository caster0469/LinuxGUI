// main.c - Entry point for hybrid desktop shell-style GTK application
// Initializes GTK application, loads CSS, and constructs the desktop layout.

#include <gtk/gtk.h>
#include <stdlib.h>
#include <string.h>

#include "desktop.h"

// Helper to load CSS from possible install locations.
static void load_css(void) {
    const char *paths[] = {
        "data/theme.css",                                     // build tree
        "/usr/local/share/hybrid-desktop/theme.css",          // install path
        "/usr/share/hybrid-desktop/theme.css"                 // fallback
    };

    GtkCssProvider *provider = gtk_css_provider_new();
    GError *error = NULL;
    gboolean loaded = FALSE;

    for (gsize i = 0; i < G_N_ELEMENTS(paths); i++) {
        if (g_file_test(paths[i], G_FILE_TEST_EXISTS)) {
            gtk_css_provider_load_from_path(provider, paths[i], &error);
            if (!error) {
                loaded = TRUE;
                break;
            } else {
                g_warning("Failed to load CSS at %s: %s", paths[i], error->message);
                g_clear_error(&error);
            }
        }
    }

    if (!loaded) {
        g_warning("No CSS file found. UI will use default styling.");
    }

    gtk_style_context_add_provider_for_screen(gdk_screen_get_default(),
                                              GTK_STYLE_PROVIDER(provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    g_object_unref(provider);
}

static void on_activate(GtkApplication *app, gpointer user_data) {
    (void)user_data;
    load_css();
    GtkWidget *window = desktop_create(app);
    gtk_widget_show_all(window);
}

int main(int argc, char *argv[]) {
    GtkApplication *app = gtk_application_new("org.example.hybriddesktop", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_activate), NULL);

    int status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
