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

typedef struct {
  const gchar  *name;
  const guint   pid;
  const gdouble memory;
  const gchar  *user;
} Process;

enum {
  COLUMN_NAME,
  COLUMN_PID,
  COLUMN_MEMORY,
  COLUMN_USER,
  NUM_COLUMNS
};

static Process data[] = 
{
  { "firefox", 2817, 318.9, "alex" },
  { "code", 2476, 132.6, "alex" },
  { "dconf-service", 1931, 819.2, "root" },
};

static void
peek_tree_view_selection_changed (GtkTreeSelection *selection,
                                  gpointer          data)
{

}

static GtkTreeModel *
peek_tree_view_create_model ()
{
  GtkListStore *store;
  GtkTreeIter   iter;

  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_UINT,
                              G_TYPE_DOUBLE,
                              G_TYPE_STRING);
  
  for (guint i = 0; i < G_N_ELEMENTS (data); i++)
  {
    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,
                        COLUMN_NAME, data[i].name,
                        COLUMN_PID, data[i].pid,
                        COLUMN_MEMORY, data[i].memory,
                        COLUMN_USER, data[i].user,
                        -1);
  }

  return GTK_TREE_MODEL (store);
}

static void
peek_tree_view_add_columns (GtkTreeView *tree_view)
{
  GtkCellRenderer   *renderer;
  GtkTreeViewColumn *column;

  renderer = gtk_cell_renderer_text_new ();

  column = gtk_tree_view_column_new_with_attributes ("Process Name",
                                                     renderer,
                                                     "text", COLUMN_NAME,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);

  column = gtk_tree_view_column_new_with_attributes ("PID",
                                                     renderer,
                                                     "text", COLUMN_PID,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);

  column = gtk_tree_view_column_new_with_attributes ("Memory",
                                                     renderer,
                                                     "text", COLUMN_MEMORY,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);

  column = gtk_tree_view_column_new_with_attributes ("User",
                                                     renderer,
                                                     "text", COLUMN_USER,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);
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

  self->model = peek_tree_view_create_model ();

  gtk_tree_view_set_model (GTK_TREE_VIEW (self->tree_view), self->model);

  g_object_unref (self->model);

  peek_tree_view_add_columns (GTK_TREE_VIEW (self->tree_view));

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
  return g_object_new (
    PEEK_TYPE_TREE_VIEW, 
    NULL
  );
}