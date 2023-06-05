#pragma once

#include "gtk/gtk.h"

G_BEGIN_DECLS

#define PEEK_TYPE_TREE_VIEW (peek_tree_view_get_type ())
G_DECLARE_FINAL_TYPE (PeekTreeView, peek_tree_view, PEEK, TREE_VIEW, GtkBin)

PeekTreeView *peek_tree_view_new (void);

void peek_tree_view_set_search_entry (PeekTreeView *peek_tree_view,
                                      GtkEntry     *entry);

G_END_DECLS