// desktop.c - Creates the fullscreen desktop window and assembles UI regions.
// Responsible for background window hints and placing top bar, center clock, and dock.

#include <gtk/gtk.h>

#include "desktop.h"
#include "topbar.h"
#include "center_clock.h"
#include "dock.h"

GtkWidget *desktop_create(GtkApplication *app) {
    GtkWidget *window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "Hybrid Desktop");
    gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_fullscreen(GTK_WINDOW(window));
    gtk_window_set_type_hint(GTK_WINDOW(window), GDK_WINDOW_TYPE_HINT_DESKTOP);
    // Some window managers may ignore the desktop type hint; a real shell would
    // use compositor APIs. Here we rely on fullscreen + borderless to achieve the effect.

    GtkWidget *overlay = gtk_overlay_new();
    gtk_widget_add_css_class(overlay, "desktop-bg");
    gtk_container_add(GTK_CONTAINER(window), overlay);

    GtkWidget *topbar = topbar_create();
    GtkWidget *center = center_clock_create();
    GtkWidget *dock = dock_create();

    // Overlay positioning
    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), center);
    gtk_widget_set_halign(center, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(center, GTK_ALIGN_CENTER);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), topbar);
    gtk_widget_set_halign(topbar, GTK_ALIGN_FILL);
    gtk_widget_set_valign(topbar, GTK_ALIGN_START);

    gtk_overlay_add_overlay(GTK_OVERLAY(overlay), dock);
    gtk_widget_set_halign(dock, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(dock, GTK_ALIGN_END);

    topbar_start_clock(topbar);
    center_clock_start(center);

    return window;
}
