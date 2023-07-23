#include "peek-process-view.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/ui/process-view.ui"

struct _PeekProcessView {
  AdwPreferencesWindow parent;
};

G_DEFINE_TYPE (PeekProcessView, peek_process_view, ADW_TYPE_PREFERENCES_WINDOW)

static void
peek_process_view_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_process_view_parent_class)->finalize (object);
}

static void
peek_process_view_init (PeekProcessView *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));
}

static void
peek_process_view_class_init (PeekProcessViewClass *klass)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               RESOURCE_PATH);

  G_OBJECT_CLASS (klass)->finalize = peek_process_view_finalize;
}

PeekProcessView *
peek_process_view_new (PeekWindow *window)
{
  return g_object_new (PEEK_TYPE_PROCESS_VIEW,
                       "transient-for", window,
                       NULL);
}