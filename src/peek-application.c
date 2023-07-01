#include "peek-application.h"
#include "peek-window.h"
#include "peek-process.h"
#include "peek-tree-model.h"

#define APPLICATION_ID "com.github.reyncode.peek"
#define REFRESH_INTERVAL 2

struct _PeekApplication {
  GtkApplication parent;

  GtkTreeModel *model;
  GHashTable   *proc_table;

  guint timeout;
};

G_DEFINE_TYPE (PeekApplication, peek_application, GTK_TYPE_APPLICATION)

static void
proc_table_value_destroy (gpointer data)
{
  g_return_if_fail (data != NULL);

  ProcData *proc_data = (ProcData *) data;

  g_free (proc_data->name);
  g_free (proc_data);
}

static void
peek_application_finalize (GObject *object)
{
  PeekApplication *app = PEEK_APPLICATION (object);

  if (app->model)
  {
    g_object_unref (app->model);
    app->model = NULL;
  }

  if (app->proc_table)
    g_hash_table_destroy (app->proc_table);
}

static void
peek_application_startup (GApplication *self)
{
  G_APPLICATION_CLASS (peek_application_parent_class)->startup (self);

  PeekApplication *app = PEEK_APPLICATION (self);

  // populate immediately
  peek_process_updater (app);

  app->timeout = g_timeout_add_seconds (REFRESH_INTERVAL, peek_process_updater, app);
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
  
  self->proc_table = g_hash_table_new_full (g_int_hash,
                                            g_int_equal,
                                            g_free,
                                            proc_table_value_destroy);
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

GHashTable *
peek_application_get_proc_table (PeekApplication *self)
{
  return self->proc_table;
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