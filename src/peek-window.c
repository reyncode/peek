#include "peek-window.h"
#include "peek-application.h"
#include "peek-tree-view.h"

struct _PeekWindow {
  GtkApplicationWindow parent;

  // header bar
  GtkWidget *search_entry;
  GtkWidget *header_menu_button;

  // main window
  GtkWidget *peek_tree_view;
};

G_DEFINE_TYPE (PeekWindow, peek_window, GTK_TYPE_APPLICATION_WINDOW)

GtkWidget *
peek_window_get_search_entry (PeekWindow *window)
{
  g_return_val_if_fail (PEEK_IS_WINDOW (window), NULL);

  return window->search_entry;
}

static gboolean
on_key_press_event (GtkWidget *widget,
                    GdkEvent  *event,
                    gpointer   data)
{
  PeekWindow *window = PEEK_WINDOW (data);

  return gtk_search_entry_handle_event (GTK_SEARCH_ENTRY (window->search_entry), event);
}

static void
search_entry_changed (GtkEditable *self,
                      gpointer     data)
{
  // do the refiltering
  // gtk_tree_model_filter_refilter
}

static void
peek_window_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_window_parent_class)->finalize (object);
}

static void
peek_window_dispose (GObject *object)
{
  G_OBJECT_CLASS (peek_window_parent_class)->dispose (object);
}

static void
peek_window_init (PeekWindow *self)
{
  g_type_ensure (PEEK_TYPE_TREE_VIEW);

  gtk_widget_init_template (GTK_WIDGET (self));

  peek_tree_view_set_search_entry (PEEK_TREE_VIEW (self->peek_tree_view),
                                   GTK_ENTRY (self->search_entry));
}

static void
peek_window_class_init (PeekWindowClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = peek_window_finalize;
  G_OBJECT_CLASS (klass)->dispose = peek_window_dispose;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               "/com/github/reyncode/peek/ui/window.ui");

  // header bar
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekWindow,
                                        search_entry);
  
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekWindow,
                                        header_menu_button);

  // main window 
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekWindow,
                                        peek_tree_view);

  // callbacks
  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (klass),
                                           on_key_press_event);

  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (klass),
                                           search_entry_changed);
}

PeekWindow *
peek_window_get_instance (PeekApplication *app)
{
  static PeekWindow *window = NULL;

  window = g_object_new (PEEK_TYPE_WINDOW,
                         "application", app,
                         NULL);

  return window;
}

PeekWindow *
peek_window_new (PeekApplication *app)
{
  return peek_window_get_instance (app);
}