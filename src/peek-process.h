#pragma once

#include "gtk/gtk.h"
#include "glib-object.h"

#include <stdio.h>

void peek_process_populate_model (GtkListStore *store);

gboolean peek_process_updater (gpointer data);