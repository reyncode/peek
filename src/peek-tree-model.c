#include <gtk/gtk.h>

#include "peek-application.h"
#include "peek-window.h"
#include "peek-tree-model.h"

static gboolean
filter_for_query (GtkTreeModel *model,
                  GtkTreeIter  *iter,
                  const gchar  *query)
{
  gchar   *name;
  gchar   *user;
  gchar   *id;
  pid_t    pid;
  GRegex  *regex;
  GError  *error = NULL;
  gboolean found = FALSE;

  gtk_tree_model_get (model, iter,
                      COLUMN_NAME, &name,
                      COLUMN_USER, &user,
                      COLUMN_ID,   &pid,
                      -1);

  id = g_strdup_printf ("%d", pid);

  regex = g_regex_new (query,
                       G_REGEX_CASELESS, 
                       G_REGEX_MATCH_PARTIAL,
                       &error);

  if (error) // TODO better debugging
    g_print ("Error: %s\n", error->message);

  found = (name && g_regex_match (regex, name, G_REGEX_MATCH_PARTIAL, NULL)) ||
          (user && g_regex_match (regex, user, G_REGEX_MATCH_PARTIAL, NULL)) ||
          (id && g_regex_match (regex, id, G_REGEX_MATCH_PARTIAL, NULL));

  g_clear_pointer (&name, g_free);
  g_clear_pointer (&user, g_free);
  g_clear_pointer (&id, g_free);
  g_clear_error (&error);
  g_regex_unref (regex);

  return found;
}

static gboolean
search_child_is_visible (GtkTreeModel *model,
                         GtkTreeIter  *iter,
                         gpointer      data)
{
  PeekApplication *app = PEEK_APPLICATION (data);
  GtkEntry        *entry;
  const gchar     *query;
  gboolean         found = FALSE;

  entry = GTK_ENTRY (peek_application_get_search_entry (app));
  if (!entry)
    return TRUE;

  query = gtk_entry_get_text (entry);

  if (g_strcmp0 (query, "") == 0)
    return TRUE;

  found = filter_for_query (model, iter, query);

  return found;
}

GtkTreeModel *
peek_tree_model_new (PeekApplication *app)
{
  GtkTreeModel *model;
  GtkTreeModelSort *sort;
  GtkTreeModelFilter *filter;

  model = GTK_TREE_MODEL (gtk_list_store_new (NUM_COLUMNS,
                                              G_TYPE_STRING,    // Process Name
                                              G_TYPE_UINT,      // PID
                                              G_TYPE_STRING,    // User
                                              G_TYPE_ULONG,     // Memory
                                              G_TYPE_DOUBLE,    // CPU %
                                              G_TYPE_UINT,      // PPID
                                              G_TYPE_STRING,    // Status
                                              G_TYPE_INT));     // Nice

  filter = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (model, NULL));
  gtk_tree_model_filter_set_visible_func (filter, search_child_is_visible, app, NULL);

  sort = GTK_TREE_MODEL_SORT (gtk_tree_model_sort_new_with_model (GTK_TREE_MODEL (filter)));

  return GTK_TREE_MODEL (sort);
}