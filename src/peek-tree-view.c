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
size_cell_data_func (GtkTreeViewColumn *column,
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
percentage_cell_data_func (GtkTreeViewColumn *column,
                           GtkCellRenderer   *cell,
                           GtkTreeModel      *model,
                           GtkTreeIter       *iter,
                           gpointer           data)
{
  const guint index = GPOINTER_TO_UINT (data);

  gdouble size;
  GValue value = { 0 };

  gtk_tree_model_get_value (model, iter, index, &value);

  size = g_value_get_double (&value);
  g_value_unset (&value);

  char *str = g_strdup_printf ("%.2f", size);

  g_object_set (cell, "text", str, NULL);
  g_free (str);
}

static gchar *
parse_priority_from_nice (gint nice)
{
  if (nice < -7)
    return "Very High";
  else if (nice < -2)
    return "High";
  else if (nice < 3)
    return "Normal";
  else if (nice < 7)
    return "Low";
  else
    return "Very Low";
}

static inline guint
divide (guint *q,
        guint *r,
        guint  d)
{
  *q = *r / d;
  *r = *r % d;
  return *q != 0;
}

static gchar *
parse_duration_centiseconds (guint centiseconds)
{
  guint weeks = 0, days = 0, hours = 0, minutes = 0, seconds = 0;

  (void)(divide (&seconds, &centiseconds, 100)
      && divide (&minutes, &seconds, 60)
      && divide (&hours, &minutes, 60)
      && divide (&days, &hours, 24)
      && divide (&weeks, &days, 7));

  if (weeks)
    return g_strdup_printf ("%uw%ud", weeks, days);

  if (days)
    return g_strdup_printf ("%ud%02uh", days, hours);

  if (hours)
    return g_strdup_printf ("%uh%02u:%02u", hours, minutes, seconds);

  return g_strdup_printf ("%u:%02u.%02u", minutes, seconds, centiseconds);
}

static void
priority_cell_data_func (GtkTreeViewColumn *column,
                         GtkCellRenderer   *cell,
                         GtkTreeModel      *model,
                         GtkTreeIter       *iter,
                         gpointer           data)
{
  const guint index = GPOINTER_TO_UINT (data);

  GValue value = { 0 };

  gtk_tree_model_get_value (model, iter, index, &value);

  gint priority = g_value_get_int (&value);

  g_value_unset (&value);

  g_object_set (cell, "text", parse_priority_from_nice (priority), NULL);
}

gint
priority_cell_sort_func (GtkTreeModel*model,
                         GtkTreeIter *a,
                         GtkTreeIter *b,
                         gpointer     data)
{
  const guint index = GPOINTER_TO_UINT (data);

  gint   result;
  GValue value1 = { 0 };
  GValue value2 = { 0 };
  
  gtk_tree_model_get_value (model, a, index, &value1);
  gtk_tree_model_get_value (model, b, index, &value2);

  result = g_value_get_int (&value1) - g_value_get_int (&value2);

  g_value_unset (&value1);
  g_value_unset (&value2);

  return result;
}

static void
duration_cell_data_func (GtkTreeViewColumn *column,
                         GtkCellRenderer   *cell,
                         GtkTreeModel      *model,
                         GtkTreeIter       *iter,
                         gpointer           data)
{
  const guint index = GPOINTER_TO_UINT (data);

  guint time;
  GValue value = { 0 };

  gtk_tree_model_get_value (model, iter, index, &value);

  switch (G_VALUE_TYPE (&value))
  {
    case G_TYPE_ULONG:
      time = g_value_get_ulong (&value);
      break;

    case G_TYPE_UINT64:
      time = g_value_get_uint64 (&value);
      break;

    default:
      g_assert_not_reached ();
  }

  g_value_unset (&value);

  PeekApplication *app;
  guint64          frequency;

  app = peek_application_get_instance ();
  frequency = peek_application_get_cpu_frequency (app);

  time = 100 * time / frequency;
  char *str = parse_duration_centiseconds (time);

  g_object_set (cell, "text", str, NULL);

  g_free (str);
}

static void
peek_tree_view_create_columns (GtkTreeView *tree_view)
{  
  GtkTreeModel      *model;
  GtkCellRenderer   *renderer;
  GtkTreeViewColumn *column;

  model = gtk_tree_view_get_model (tree_view);

  // Process Name
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Process Name",
                                                     renderer,
                                                     "text", COLUMN_NAME,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_NAME);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // ID
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("ID",
                                                     renderer,
                                                     "text", COLUMN_ID,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_ID);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // User
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("User",
                                                     renderer,
                                                     "text", COLUMN_USER,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_USER);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // Memory
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Memory",
                                                     renderer,
                                                     "text", COLUMN_MEMORY,
                                                     NULL);

  gtk_tree_view_column_set_cell_data_func (column, renderer,
                                           size_cell_data_func, 
                                           GUINT_TO_POINTER (COLUMN_MEMORY),
                                           NULL);

  g_object_set (G_OBJECT (renderer), "xalign", 1.0f, NULL); // right alignment
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_MEMORY);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);
  
  // CPU %
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("CPU",
                                                     renderer,
                                                     "text", COLUMN_CPU_P,
                                                     NULL);

  gtk_tree_view_column_set_cell_data_func (column, renderer,
                                           percentage_cell_data_func, 
                                           GUINT_TO_POINTER (COLUMN_CPU_P),
                                           NULL);

  g_object_set (G_OBJECT (renderer), "xalign", 1.0f, NULL); // right alignment
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_CPU_P);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // CPU Time
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("CPU Time",
                                                     renderer,
                                                     "text", COLUMN_CPU_TIME,
                                                     NULL);

  gtk_tree_view_column_set_cell_data_func (column, renderer,
                                           duration_cell_data_func, 
                                           GUINT_TO_POINTER (COLUMN_CPU_TIME),
                                           NULL);

  g_object_set (G_OBJECT (renderer), "xalign", 1.0f, NULL); // right alignment
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_CPU_TIME);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // PPID
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("PPID",
                                                     renderer,
                                                     "text", COLUMN_PPID,
                                                     NULL);
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_PPID);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // State
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("State",
                                                     renderer,
                                                     "text", COLUMN_STATE,
                                                     NULL);
                                                    
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_STATE);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // Nice
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Nice",
                                                     renderer,
                                                     "text", COLUMN_NICE,
                                                     NULL);
                                                    
  gtk_tree_view_column_set_sort_column_id (column, COLUMN_NICE);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);

  // Priority
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Priority",
                                                     renderer,
                                                     "text", COLUMN_PRIORITY,
                                                     NULL);
                                                    
  gtk_tree_view_column_set_cell_data_func (column, renderer,
                                           priority_cell_data_func, 
                                           GUINT_TO_POINTER (COLUMN_NICE),
                                           NULL);

  gtk_tree_sortable_set_sort_func (GTK_TREE_SORTABLE (model), COLUMN_PRIORITY,
                                   priority_cell_sort_func, GUINT_TO_POINTER (COLUMN_NICE),
                                   NULL);

  gtk_tree_view_column_set_sort_column_id (column, COLUMN_PRIORITY);
  gtk_tree_view_column_set_reorderable (column, TRUE);
  gtk_tree_view_append_column (tree_view, column);
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