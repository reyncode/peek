#include <gtk/gtk.h>

#include "peek-application.h"
#include "peek-window.h"
#include "peek-tree-model.h"

// static gboolean
// search_query_is_found (GtkTreeModel *model,
//                        GtkTreeIter  *iter,
//                        const gchar  *query)
// {
//   gchar *name;
//   gchar *user;
//   gchar *id;
//   pid_t  pid;

//   GRegex *regex;
//   GMatchInfo *match_info;
//   gchar **tokens;
//   gchar *pattern;

//   gboolean found;

//   gtk_tree_model_get (model, iter,
//                       COLUMN_NAME, &name,
//                       COLUMN_USER, &user,
//                       COLUMN_ID,   &pid,
//                       -1);

//   id = g_strdup_printf ("%d", pid);

//   tokens = g_strsplit_set (query, " |", -1);
//   pattern = g_strjoinv ("|", tokens);

//   regex = g_regex_new (pattern,
//                        G_REGEX_CASELESS, 
//                        G_REGEX_MATCH_PARTIAL,
//                        NULL);
  
//   g_regex_match (regex, query, G_REGEX_MATCH_PARTIAL, &match_info);

//   while (g_match_info_matches (match_info))
//   {
//     g_match_info_fetch_named (match_info, name);
//     found = (name && g_match_info_fetch_named (match_info, name));

//     if (found)
//       break;
//   }

//   g_strfreev (tokens);
//   g_clear_pointer (&pattern, g_free);
//   g_clear_pointer (&name, g_free);
//   g_clear_pointer (&user, g_free);
//   g_clear_pointer (&id, g_free);

//   g_regex_unref (regex);

//   return found;
// }

// static gboolean
// search_child_is_visible (GtkTreeModel *model,
//                          GtkTreeIter  *iter,
//                          gpointer      data)
// {
//   PeekApplication *app = PEEK_APPLICATION (data);
//   PeekWindow *window;

//   GtkWidget *entry;
//   GtkTreePath *path;

//   window = peek_window_get_instance (app);

//   entry = peek_window_get_search_entry (window);
//   path = gtk_tree_model_get_path (model, iter);

//   const gchar *query;
//   gboolean     found;

//   query = gtk_entry_get_text (GTK_ENTRY (entry));

//   if (g_strcmp0 (query, "") == 0)
//   {
//     gtk_tree_path_free (path);
//     return TRUE;
//   }

//   found = search_query_is_found (model, iter, query);

//   gtk_tree_path_free (path);

//   return found;
// }

static gboolean
mock_is_visible (GtkTreeModel *model,
                 GtkTreeIter  *iter,
                 gpointer      data)
{
  return TRUE;
}

GtkTreeModel *
peek_tree_model_new (PeekApplication *app)
{
  GtkTreeModel *model;
  GtkTreeModelFilter *filter;

  model = GTK_TREE_MODEL (gtk_list_store_new (NUM_COLUMNS,
                                              G_TYPE_STRING,    // Process Name
                                              G_TYPE_UINT,      // PID
                                              G_TYPE_STRING,    // User
                                              G_TYPE_ULONG,     // Memory
                                              G_TYPE_UINT,      // PPID
                                              G_TYPE_STRING));   // Status

  filter = GTK_TREE_MODEL_FILTER (gtk_tree_model_filter_new (model, NULL));
  gtk_tree_model_filter_set_visible_func (filter, mock_is_visible, app, NULL);

  return GTK_TREE_MODEL (filter);
}