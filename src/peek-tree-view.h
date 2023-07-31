#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define PEEK_TYPE_TREE_VIEW (peek_tree_view_get_type ())
G_DECLARE_FINAL_TYPE (PeekTreeView, peek_tree_view, PEEK, TREE_VIEW, GtkBox)

PeekTreeView *peek_tree_view_new (void);

void peek_tree_view_set_search_entry (PeekTreeView *peek_tree_view,
                                      GtkEditable  *editable);

GtkWidget *peek_tree_view_get_tree_view (PeekTreeView *peek_tree_view);

const gchar *parse_priority_from_nice (gint nice);

gchar *parse_duration_centiseconds (guint centiseconds);

G_END_DECLS