#include "peek-application.h"
#include "peek-window.h"
#include "peek-process.h"

#define APPLICATION_ID "com.github.reyncode.peek"

struct _PeekApplication {
  GtkApplication parent;

  GtkTreeModel *proc_model;
};

G_DEFINE_TYPE (PeekApplication, peek_application, GTK_TYPE_APPLICATION)

static void
peek_application_finalize (GObject *object)
{
  
  PeekApplication *application = PEEK_APPLICATION (object);

  if (application->proc_model)
  {
    g_object_unref (application->proc_model);
    application->proc_model = NULL;
  }
}

static void
peek_application_startup (GApplication *self)
{
  G_APPLICATION_CLASS (peek_application_parent_class)->startup (self);
  
  // setting accels here
}

static void
peek_application_activate (GApplication *self)
{
  PeekWindow *window;

  window = peek_window_new (PEEK_APPLICATION (self));
  
  gtk_window_present (GTK_WINDOW (window));
}

static void
peek_application_init (PeekApplication *self)
{
  self->proc_model = peek_process_create_proc_model ();
}

static void
peek_application_class_init (PeekApplicationClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = peek_application_finalize;

  G_APPLICATION_CLASS (klass)->startup = peek_application_startup;
  G_APPLICATION_CLASS (klass)->activate = peek_application_activate;
}

GtkTreeModel *
peek_application_get_proc_model (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_APPLICATION (self), NULL);

  return self->proc_model;
}

PeekApplication *
peek_application_get_instance (void)
{
  static PeekApplication *application = NULL;

  if (!application)
    application = g_object_new (PEEK_TYPE_APPLICATION,
                                "application-id", APPLICATION_ID,
                                "flags", G_APPLICATION_FLAGS_NONE,
                                NULL
    );

  return application;
}

PeekApplication *
peek_application_new (void)
{
  return peek_application_get_instance ();
}