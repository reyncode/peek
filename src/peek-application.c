#include "peek-application.h"
#include "peek-window.h"
#include "peek-process.h"
#include "peek-tree-model.h"

#define APPLICATION_ID "com.github.reyncode.peek"

struct _PeekApplication {
  GtkApplication parent;

  GtkTreeModel *model;
};

G_DEFINE_TYPE (PeekApplication, peek_application, GTK_TYPE_APPLICATION)

static void
peek_application_finalize (GObject *object)
{
  PeekApplication *app = PEEK_APPLICATION (object);

  if (app->model)
  {
    g_object_unref (app->model);
    app->model = NULL;
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
  PeekApplication *app = PEEK_APPLICATION (self);
  PeekWindow      *window;

  window = peek_window_new (app);
  
  gtk_window_present (GTK_WINDOW (window));
}

static void
peek_application_init (PeekApplication *self)
{
  self->model = peek_tree_model_new ();
  peek_process_populate_model (GTK_LIST_STORE (self->model));
}

static void
peek_application_class_init (PeekApplicationClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = peek_application_finalize;

  G_APPLICATION_CLASS (klass)->startup = peek_application_startup;
  G_APPLICATION_CLASS (klass)->activate = peek_application_activate;
}

GtkTreeModel *
peek_application_get_model (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_APPLICATION (self), NULL);

  return self->model;
}

PeekApplication *
peek_application_get_instance (void)
{
  static PeekApplication *app = NULL;

  if (!app)
    app = g_object_new (PEEK_TYPE_APPLICATION,
                        "application-id", APPLICATION_ID,
                        "flags", G_APPLICATION_FLAGS_NONE,
                        NULL);

  return app;
}

PeekApplication *
peek_application_new (void)
{
  return peek_application_get_instance ();
}