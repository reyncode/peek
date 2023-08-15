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
