#pragma once

#include "gtk/gtk.h"
#include "glib-object.h"

#include <stdio.h>

typedef struct ProcData {
  pid_t    pid;
  pid_t    ppid;
  guint32  uid;
  gchar   *name;
  guint    state;

  guint64  memory; 
  guint64  memory_vsize;
  guint64  memory_shared;
  guint64  memory_resident;

  GtkTreeIter iter;
} ProcData;

gboolean peek_process_updater (gpointer data);