// topbar.c - Implements the translucent top bar with system labels and small clock.
// Provides construction and periodic time updates.

#include <gtk/gtk.h>
#include <time.h>

#include "topbar.h"

#define SMALL_TIME_FORMAT "%H:%M" // Switch to "%I:%M %p" for 12-hour format

typedef struct {
    GtkWidget *time_label;
} TopbarData;

static gboolean update_time(gpointer user_data) {
    TopbarData *data = user_data;
    if (!GTK_IS_WIDGET(data->time_label)) {
        return G_SOURCE_REMOVE;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char buffer[16];
    strftime(buffer, sizeof(buffer), SMALL_TIME_FORMAT, tm_info);
    gtk_label_set_text(GTK_LABEL(data->time_label), buffer);
    return G_SOURCE_CONTINUE;
}

GtkWidget *topbar_create(void) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 16);
    gtk_widget_set_hexpand(box, TRUE);
    gtk_widget_set_margin_start(box, 24);
    gtk_widget_set_margin_end(box, 24);
    gtk_widget_set_margin_top(box, 20);
    gtk_widget_add_css_class(box, "topbar");

    GtkWidget *left = gtk_label_new("Activities");
    GtkWidget *center = gtk_label_new("Linux");
    GtkWidget *wifi = gtk_label_new(""); // Using Wi-Fi icon from default fonts; fallback to text
    GtkWidget *time_label = gtk_label_new("00:00");

    gtk_widget_add_css_class(left, "topbar-label");
    gtk_widget_add_css_class(center, "topbar-center");
    gtk_widget_add_css_class(wifi, "topbar-icon");
    gtk_widget_add_css_class(time_label, "topbar-label");

    gtk_widget_set_halign(center, GTK_ALIGN_CENTER);
    gtk_widget_set_hexpand(center, TRUE);

    GtkWidget *right_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 8);
    gtk_widget_set_halign(right_box, GTK_ALIGN_END);
    gtk_box_pack_start(GTK_BOX(right_box), wifi, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(right_box), time_label, FALSE, FALSE, 0);

    gtk_box_pack_start(GTK_BOX(box), left, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), center, TRUE, TRUE, 0);
    gtk_box_pack_end(GTK_BOX(box), right_box, FALSE, FALSE, 0);

    TopbarData *data = g_new0(TopbarData, 1);
    data->time_label = time_label;
    g_object_set_data_full(G_OBJECT(box), "topbar-data", data, g_free);

    return box;
}

void topbar_start_clock(GtkWidget *topbar) {
    TopbarData *data = g_object_get_data(G_OBJECT(topbar), "topbar-data");
    if (!data) {
        return;
    }
    update_time(data);
    g_timeout_add_seconds(1, update_time, data);
}
