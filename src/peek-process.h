/*
 *  Peek - A process monitoring tool
 *  Copyright (C) 2023 Alex Reynolds <hellport@protonmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PEEK_PROCESS_H
#define PEEK_PROCESS_H

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
const gchar *parse_proc_state (guint state);

#endif
