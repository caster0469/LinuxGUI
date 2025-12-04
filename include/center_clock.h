// center_clock.h - Interface for the large center clock widget.
#pragma once

#include <gtk/gtk.h>

GtkWidget *center_clock_create(void);
void center_clock_start(GtkWidget *clock_widget);
