#include <gtk/gtk.h>

#include "peek-application.h"
#include "peek-window.h"
#include "peek-tree-model.h"

static gboolean
search_query_is_found (GtkTreeModel *model,
                       GtkTreeIter  *iter,
                       const gchar  *query)
{
  gchar *name;
  gchar *user;
  gchar *id;
  pid_t  pid;

  GRegex *regex;
  gchar **tokens;
  gchar *pattern;

  gboolean found = TRUE;

  gtk_tree_model_get (model, iter,
                      COLUMN_NAME, &name,
                      COLUMN_USER, &user,
                      COLUMN_ID,   &pid,
                      -1);

  id = g_strdup_printf ("%d", pid);

  tokens = g_strsplit_set (query, " |", -1);
  pattern = g_strjoinv ("|", tokens);

  regex = g_regex_new (pattern,
                       G_REGEX_CASELESS, 
                       G_REGEX_MATCH_PARTIAL,
                       NULL);
  
  g_regex_match (regex, query, G_REGEX_MATCH_PARTIAL, NULL);

  // cycle through found

  g_strfreev (tokens);
  g_free (pattern);
  g_free (name);
  g_free (user);
  g_free (id);

  g_regex_unref (regex);

  return found;
}

static gboolean
search_child_is_visible (GtkTreeModel *model,
                         GtkTreeIter  *iter,
                         gpointer      data)
{
  PeekApplication *app = PEEK_APPLICATION (data);
  PeekWindow *window;

  GtkWidget *entry;
  GtkTreePath *path;

  window = peek_window_get_instance (app);

  entry = peek_window_get_search_entry (window);
  path = gtk_tree_model_get_path (model, iter);

  const gchar *query;
  gboolean     found;

  query = gtk_entry_get_text (GTK_ENTRY (entry));

  if (g_strcmp0 (query, "") == 0)
  {
    gtk_tree_path_free (path);
    return TRUE;
  }

  found = search_query_is_found (model, iter, query);

  gtk_tree_path_free (path);

  return found;
}

GtkTreeModel *
peek_tree_model_new (PeekApplication *app)
{
  GtkListStore *store;
  GtkTreeModelFilter *filter;

  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_STRING,    // Process Name
                              G_TYPE_UINT,      // PID
                              G_TYPE_STRING,    // User
                              G_TYPE_ULONG,     // Memory
                              G_TYPE_UINT,      // PPID
                              G_TYPE_STRING);   // Status

  // caught in infinite loop

  // filter = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (GTK_TREE_MODEL (store), NULL));
  // gtk_tree_model_filter_set_visible_func (filter, search_child_is_visible, app, NULL);

  // GtkTreeModelSort

  return GTK_TREE_MODEL (store);
}