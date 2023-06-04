#include "peek-tree-view.h"

/*
  list fields

  name,
  pid,
  memory,
  usr,
  priority
*/

struct _PeekTreeView {
  GtkBin        parent;

  GtkWidget    *tree_view;
  GtkTreeModel *model;
};

G_DEFINE_TYPE (PeekTreeView, peek_tree_view, GTK_TYPE_BIN)

static void
peek_tree_view_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_tree_view_parent_class)->finalize (object);
}

static void
peek_tree_view_init (PeekTreeView *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
peek_tree_view_class_init (PeekTreeViewClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = peek_tree_view_finalize;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               "/com/github/reyncode/peek/ui/tree-view.ui");

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekTreeView,
                                        tree_view);
}

PeekTreeView *
peek_tree_view_new (void)
{
  return g_object_new (
    PEEK_TYPE_TREE_VIEW, 
    NULL
  );
}