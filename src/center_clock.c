// center_clock.c - Implements the large center clock with date information.
// Uses GLib timeouts to refresh every second.

#include <gtk/gtk.h>
#include <time.h>

#include "center_clock.h"

#define MAIN_TIME_FORMAT "%H:%M"            // Change to "%I:%M %p" for 12-hour format
#define MAIN_DATE_FORMAT "%A, %B %d"       // Displays e.g., Tuesday, April 23

typedef struct {
    GtkWidget *time_label;
    GtkWidget *date_label;
} ClockData;

static gboolean update_clock(gpointer user_data) {
    ClockData *data = user_data;
    if (!GTK_IS_WIDGET(data->time_label) || !GTK_IS_WIDGET(data->date_label)) {
        return G_SOURCE_REMOVE;
    }

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    char time_buf[32];
    char date_buf[64];
    strftime(time_buf, sizeof(time_buf), MAIN_TIME_FORMAT, tm_info);
    strftime(date_buf, sizeof(date_buf), MAIN_DATE_FORMAT, tm_info);

    gtk_label_set_text(GTK_LABEL(data->time_label), time_buf);
    gtk_label_set_text(GTK_LABEL(data->date_label), date_buf);
    return G_SOURCE_CONTINUE;
}

GtkWidget *center_clock_create(void) {
    GtkWidget *box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 4);
    gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
    gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
    gtk_widget_add_css_class(box, "center-clock");

    GtkWidget *time_label = gtk_label_new("00:00");
    GtkWidget *date_label = gtk_label_new("Weekday, Month 01");

    gtk_widget_add_css_class(time_label, "center-time");
    gtk_widget_add_css_class(date_label, "center-date");

    gtk_box_pack_start(GTK_BOX(box), time_label, FALSE, FALSE, 0);
    gtk_box_pack_start(GTK_BOX(box), date_label, FALSE, FALSE, 0);

    ClockData *data = g_new0(ClockData, 1);
    data->time_label = time_label;
    data->date_label = date_label;
    g_object_set_data_full(G_OBJECT(box), "clock-data", data, g_free);

    return box;
}

void center_clock_start(GtkWidget *clock_widget) {
    ClockData *data = g_object_get_data(G_OBJECT(clock_widget), "clock-data");
    if (!data) {
        return;
    }
    update_clock(data);
    g_timeout_add_seconds(1, update_clock, data);
}
