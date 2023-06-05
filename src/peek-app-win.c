#include "peek-app-win.h"
#include "peek-tree-view.h"

struct _PeekAppWin {
  GtkApplicationWindow parent;

  // header bar
  GtkWidget *search_entry;
  GtkWidget *header_menu_button;

  // main window
  GtkWidget *peek_tree_view;
};

G_DEFINE_TYPE (PeekAppWin, peek_app_win, GTK_TYPE_APPLICATION_WINDOW)

static void
peek_app_win_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_app_win_parent_class)->finalize (object);
}

static void
peek_app_win_dispose (GObject *object)
{
  G_OBJECT_CLASS (peek_app_win_parent_class)->dispose (object);
}

static void
peek_app_win_init (PeekAppWin *self)
{
  g_type_ensure (PEEK_TYPE_TREE_VIEW);

  gtk_widget_init_template (GTK_WIDGET (self));
  
  peek_tree_view_set_search_entry (PEEK_TREE_VIEW (self->peek_tree_view),
                                   GTK_ENTRY (self->search_entry));
}

static void
peek_app_win_class_init (PeekAppWinClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = peek_app_win_finalize;
  G_OBJECT_CLASS (klass)->dispose = peek_app_win_dispose;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               "/com/github/reyncode/peek/ui/window.ui");

  // header bar
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekAppWin,
                                        search_entry);
  
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekAppWin,
                                        header_menu_button);

  // main window 
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekAppWin,
                                        peek_tree_view);
}

PeekAppWin *
peek_app_win_new (PeekApp *app)
{
  return g_object_new (
    PEEK_TYPE_APP_WIN, 
    "application",
    app,
    NULL
  );
}