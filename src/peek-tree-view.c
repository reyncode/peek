#include "peek-tree-view.h"

struct _PeekTreeView {
    GtkLabel parent;
};

G_DEFINE_TYPE (PeekTreeView, peek_tree_view, GTK_TYPE_LABEL)

static void
peek_tree_view_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_tree_view_parent_class)->finalize (object);
}

static void
peek_tree_view_dispose (GObject *object)
{
  G_OBJECT_CLASS (peek_tree_view_parent_class)->dispose (object);
}

static void
peek_tree_view_init (PeekTreeView *self)
{
    // widget specific stuff
}

static void
peek_tree_view_class_init (PeekTreeViewClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = peek_tree_view_finalize;
  G_OBJECT_CLASS (klass)->dispose = peek_tree_view_dispose;
}

PeekTreeView *
peek_tree_view_new (void)
{
    return g_object_new (
        PEEK_TYPE_TREE_VIEW, 
        NULL
    );
}