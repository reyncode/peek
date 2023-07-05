#include "peek-tree-view.h"
#include "peek-application.h"
#include "peek-tree-model.h"
#include "peek-process.h"

struct _PeekTreeView {
  GtkBin        parent;

  GtkWidget    *tree_view;
};

G_DEFINE_TYPE (PeekTreeView, peek_tree_view, GTK_TYPE_BIN)

static void
memory_cell_data_func (GtkTreeViewColumn *column,
                       GtkCellRenderer   *cell,
                       GtkTreeModel      *model,
                       GtkTreeIter       *iter,
                       gpointer           data)
{
  const guint index = GPOINTER_TO_UINT (data);

  guint64 size;
  GValue value = { 0 };

  gtk_tree_model_get_value (model, iter, index, &value);

  switch (G_VALUE_TYPE (&value))
  {
    case G_TYPE_ULONG:
      size = g_value_get_ulong (&value);
      break;

    case G_TYPE_UINT64:
      size = g_value_get_uint64 (&value);
      break;

    default:
      g_assert_not_reached ();
  }

  g_value_unset (&value);

  if (size == 0)
  {
    g_object_set (cell,
                  "text", ("N/A"),
                  "style", PANGO_STYLE_ITALIC,
                  NULL);
  }
  else
  {
    char *str = g_format_size (size);
    g_object_set (cell,
                  "text", str,
                  "style", PANGO_STYLE_NORMAL,
                  NULL);
    g_free (str);
  }
}

static void
peek_tree_view_create_columns (GtkTreeView *tree_view)
{  
  GtkCellRenderer   *renderer;
  GtkTreeViewColumn *column;

  // Process Name
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Process Name",
                                                     renderer,
                                                     "text", COLUMN_NAME,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);

  // ID
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("ID",
                                                     renderer,
                                                     "text", COLUMN_ID,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);

  // User
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("User",
                                                     renderer,
                                                     "text", COLUMN_USER,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);

  // Memory
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Memory",
                                                     renderer,
                                                     "text", COLUMN_MEMORY,
                                                     NULL);

  gtk_tree_view_column_set_cell_data_func (column, renderer,
                                           memory_cell_data_func, 
                                           GUINT_TO_POINTER (COLUMN_MEMORY),
                                           NULL);
  // right align
  g_object_set (G_OBJECT (renderer), "xalign", 1.0f, NULL);
  gtk_tree_view_append_column (tree_view, column);


  // PPID
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("PPID",
                                                     renderer,
                                                     "text", COLUMN_PPID,
                                                     NULL);
  gtk_tree_view_append_column (tree_view, column);

  // State
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("State",
                                                     renderer,
                                                     "text", COLUMN_STATE,
                                                     NULL);
                                                    
  gtk_tree_view_append_column (tree_view, column);

  // callbacks
}

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
  model = peek_application_get_model (application);

  gtk_tree_view_set_model (GTK_TREE_VIEW (self->tree_view), model);

  peek_tree_view_create_columns (GTK_TREE_VIEW (self->tree_view));

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