#pragma once

#include <gtk/gtk.h>

enum {
  COLUMN_NAME,
  COLUMN_ID,
  COLUMN_USER,
  COLUMN_MEMORY,
  COLUMN_PPID,
  COLUMN_STATE,
  NUM_COLUMNS
};

GtkTreeModel *peek_tree_model_new (void);