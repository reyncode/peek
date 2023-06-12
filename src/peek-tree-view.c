#include "peek-tree-view.h"
#include "peek-application.h"

struct _PeekTreeView {
  GtkBin        parent;

  GtkWidget    *tree_view;
};

G_DEFINE_TYPE (PeekTreeView, peek_tree_view, GTK_TYPE_BIN)

void
peek_tree_view_set_search_entry (PeekTreeView *peek_tree_view,
                                 GtkEntry *entry)
{
  gtk_tree_view_set_search_entry (GTK_TREE_VIEW (peek_tree_view->tree_view),
                                  entry);
}

static void
peek_tree_view_selection_changed (GtkTreeSelection *selection,
                                  gpointer          data)
{

}

static void
peek_tree_view_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_tree_view_parent_class)->finalize (object);
}

static void
peek_tree_view_init (PeekTreeView *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  PeekApplication *application;
  application = peek_application_get_instance ();

  GtkTreeModel *model;
  model = peek_application_get_proc_model (application);

  gtk_tree_view_set_model (GTK_TREE_VIEW (self->tree_view), model);

  gtk_widget_show_all (GTK_WIDGET (self));
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
  
  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (klass),
                                           peek_tree_view_selection_changed);
}

PeekTreeView *
peek_tree_view_new (void)
{
  PeekTreeView *tree_view;

  tree_view = g_object_new (PEEK_TYPE_TREE_VIEW, NULL);

  return tree_view;
}