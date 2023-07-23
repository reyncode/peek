#include "peek-process-view.h"
#include "peek-application.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/ui/process-view.ui"

struct _PeekProcessView {
  AdwPreferencesWindow parent;
};

G_DEFINE_TYPE (PeekProcessView, peek_process_view, ADW_TYPE_PREFERENCES_WINDOW)

static void
proc_update_cb (PeekApplication *app,
                gpointer         data)
{
  PeekProcessView *view = PEEK_PROCESS_VIEW (data);

  g_print ("update!\n");
}

static void
peek_process_view_finalize (GObject *object)
{
  PeekApplication *app;
  app = peek_application_get_instance ();

  g_signal_handlers_disconnect_by_func (app,
                                        proc_update_cb,
                                        PEEK_PROCESS_VIEW (object));

  G_OBJECT_CLASS (peek_process_view_parent_class)->finalize (object);
}

static void
peek_process_view_init (PeekProcessView *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  PeekApplication *app;
  app = peek_application_get_instance ();

  g_signal_connect (app, "proc-update", G_CALLBACK (proc_update_cb), self);
}

static void
peek_process_view_class_init (PeekProcessViewClass *klass)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               RESOURCE_PATH);

  G_OBJECT_CLASS (klass)->finalize = peek_process_view_finalize;
}

PeekProcessView *
peek_process_view_new (PeekWindow *window,
                       const gchar *title)
{
  return g_object_new (PEEK_TYPE_PROCESS_VIEW,
                       "transient-for", window,
                       "title", title,
                       NULL);
}