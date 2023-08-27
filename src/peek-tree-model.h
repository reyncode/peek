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

#ifndef PEEK_TREE_MODEL_H
#define PEEK_TREE_MODEL_H

#include <gtk/gtk.h>

#include "peek-application.h"

enum {
  COLUMN_NAME,
  COLUMN_ID,
  COLUMN_USER,
  COLUMN_MEMORY,
  COLUMN_CPU_P,
  COLUMN_CPU_TIME,
  COLUMN_PPID,
  COLUMN_STATE,
  COLUMN_NICE,
  COLUMN_PRIORITY,
  NUM_COLUMNS
};

GtkTreeModel *peek_tree_model_new (PeekApplication *app);

#endif
