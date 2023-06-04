#pragma once

#include "gtk/gtk.h"

G_BEGIN_DECLS

#define PEEK_TYPE_TREE_VIEW (peek_tree_view_get_type ())
G_DECLARE_FINAL_TYPE (PeekTreeView, peek_tree_view, PEEK, TREE_VIEW, GtkLabel)

PeekTreeView *peek_tree_view_new (void);

G_END_DECLS