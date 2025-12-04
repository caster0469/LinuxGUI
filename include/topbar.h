// topbar.h - Interface for creating and updating the translucent top bar.
#pragma once

#include <gtk/gtk.h>

GtkWidget *topbar_create(void);
void topbar_start_clock(GtkWidget *topbar);
