#pragma once

#include "gtk/gtk.h"
#include "glib-object.h"

#include <stdio.h>

typedef struct ProcData {
  pid_t  pid;
  pid_t  ppid;
  char  *name;

  GtkTreeIter iter;
} ProcData;

void peek_process_populate_model (GtkListStore *store);

gboolean peek_process_updater (gpointer data);