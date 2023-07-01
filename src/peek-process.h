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

gboolean peek_process_updater (gpointer data);