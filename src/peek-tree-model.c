#include <gtk/gtk.h>

#include "peek-tree-model.h"

GtkTreeModel *
peek_tree_model_new (void)
{
  GtkListStore *store;

  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_STRING,    // Process Name
                              G_TYPE_UINT,      // PID
                              G_TYPE_STRING,    // User
                              G_TYPE_ULONG,     // Memory
                              G_TYPE_UINT,      // PPID
                              G_TYPE_STRING);   // Status

  // GtkTreeModelFilter
  // GtkTreeModelSort

  return GTK_TREE_MODEL (store);
}