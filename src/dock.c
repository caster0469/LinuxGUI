// dock.c - Implements the bottom dock with launch buttons and hover effects.
// Buttons spawn external applications using g_spawn_command_line_async().

#include <gtk/gtk.h>

#include "dock.h"

typedef struct {
    const char *icon;
    const char *label;
    const char *command;
} DockItem;

static void on_launch(GtkWidget *widget, gpointer user_data) {
    (void)widget;
    const char *cmd = (const char *)user_data;
    if (cmd && *cmd) {
        // Commands can be replaced to match the local environment.
        g_spawn_command_line_async(cmd, NULL);
    }
}

static GtkWidget *create_dock_button(const DockItem *item) {
    GtkWidget *button = gtk_button_new();
    gtk_widget_add_css_class(button, "dock-button");

    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 6);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);

    GtkWidget *icon = gtk_label_new(item->icon);
    gtk_widget_add_css_class(icon, "dock-icon");

    GtkWidget *label = gtk_label_new(item->label);
    gtk_widget_add_css_class(label, "dock-label");

    gtk_box_pack_start(GTK_BOX(box), icon, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), label, FALSE, FALSE, 0);
    gtk_container_add(GTK_CONTAINER(button), box);

    g_signal_connect(button, "clicked", G_CALLBACK(on_launch), (gpointer)item->command);
    return button;
}

GtkWidget *dock_create(void) {
    GtkWidget *frame = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_add_css_class(frame, "dock");
    gtk_widget_set_margin_bottom(frame, 28);
    gtk_widget_set_margin_top(frame, 12);
    gtk_widget_set_margin_start(frame, 16);
    gtk_widget_set_margin_end(frame, 16);

    const DockItem items[] = {
        {"", "Applications", "rofi -show drun"},      // Replace with xfce4-appfinder if preferred
        {"", "Files", "thunar"},                      // Or nautilus
        {"", "Browser", "firefox"},
        {"", "Terminal", "xfce4-terminal"},
        {"", "Notes", "xed"},
        {"", "Trash", "thunar trash:///"}
    };

    for (guint i = 0; i < G_N_ELEMENTS(items); i++) {
        GtkWidget *btn = create_dock_button(&items[i]);
        gtk_box_pack_start(GTK_BOX(frame), btn, TRUE, TRUE, 0);
    }

    return frame;
}
