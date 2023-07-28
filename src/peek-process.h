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
  gint     nice;

  guint64  memory; 
  guint64  memory_vsize;
  guint64  memory_shared;
  guint64  memory_resident;

  guint64  cpu_time;
  gdouble  cpu_usage;

  GtkTreeIter iter;
} ProcData;

gboolean peek_process_updater (gpointer data);

const gchar *parse_user_from_uid (guint32 uid);