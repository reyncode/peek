#include <adwaita.h>

#include "peek-application.h"
#include "peek-window.h"
#include "peek-process.h"
#include "peek-tree-model.h"

#include <glibtop.h>
#include <glibtop/sysinfo.h>

#define APPLICATION_ID "com.github.reyncode.peek"
#define REFRESH_INTERVAL 2

static guint proc_update_signal;
static guint proc_interval_signal;

struct _PeekApplication {
  AdwApplication parent;

  GSettings    *settings;

  guint         timeout_id;
  guint         interval;

  GtkTreeModel *model;
  GHashTable   *proc_table;
  GtkWidget    *search_entry;

  guint64       cpu_time_total;
  guint64       cpu_time_total_last;
  guint64       cpu_frequency;
};

G_DEFINE_TYPE (PeekApplication, peek_application, ADW_TYPE_APPLICATION)

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

  g_clear_object (&app->settings);
}

static void
peek_application_startup (GApplication *self)
{
  G_APPLICATION_CLASS (peek_application_parent_class)->startup (self);

  PeekApplication *app = PEEK_APPLICATION (self);

  // populate immediately
  peek_process_updater (app);

  app->timeout_id = g_timeout_add_seconds (app->interval, peek_process_updater, app);
}

static void
peek_application_activate (GApplication *self)
{
  PeekApplication *app = PEEK_APPLICATION (self);

  PeekWindow *window;

  window = peek_window_new (app);

  app->search_entry = peek_window_get_search_entry (window);
  
  gtk_window_present (GTK_WINDOW (window));
}

static void
restart_timeout_cb (gpointer data)
{
  PeekApplication *self = PEEK_APPLICATION (data);

  // cancel the old
  g_source_remove (self->timeout_id);

  // start the new
  self->timeout_id = g_timeout_add_seconds (self->interval, peek_process_updater, self);
}

static void
peek_application_init (PeekApplication *self)
{
  
  self->settings = g_settings_new (APPLICATION_ID);
  self->interval = g_settings_get_int (self->settings, "interval");

  self->model = peek_tree_model_new (self);
  
  self->proc_table = g_hash_table_new_full (g_int_hash,
                                            g_int_equal,
                                            g_free,
                                            proc_table_value_destroy);

  self->search_entry = NULL;

  self->cpu_time_total = 0;
  self->cpu_time_total_last = 0;
  self->cpu_frequency = 0;

  g_signal_connect (self, "interval-update", G_CALLBACK (restart_timeout_cb), self);
}

static void
peek_application_class_init (PeekApplicationClass *klass)
{
  proc_update_signal = g_signal_new ("proc-update",
                                     G_TYPE_FROM_CLASS (klass),
                                     G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                     0,
                                     NULL,
                                     NULL,
                                     NULL,
                                     G_TYPE_NONE,
                                     0);

  proc_interval_signal = g_signal_new ("interval-update",
                                       G_TYPE_FROM_CLASS (klass),
                                       G_SIGNAL_RUN_LAST | G_SIGNAL_NO_RECURSE | G_SIGNAL_NO_HOOKS,
                                       0,
                                       NULL,
                                       NULL,
                                       NULL,
                                       G_TYPE_NONE,
                                       0);


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
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), NULL);

  return self->proc_table;
}

guint64
peek_application_get_cpu_time_total (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), 0);

  return self->cpu_time_total;
}

void
peek_application_set_cpu_time_total (PeekApplication *self,
                                     guint64          value)
{
  g_return_if_fail (PEEK_IS_APPLICATION (self));

  self->cpu_time_total = value;
}

guint64
peek_application_get_cpu_time_total_last (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), 0);

  return self->cpu_time_total_last;
}

void
peek_application_set_cpu_time_total_last (PeekApplication *self,
                                          guint64          value)
{
  g_return_if_fail (PEEK_IS_APPLICATION (self));

  self->cpu_time_total_last = value;
}

guint64
peek_application_get_cpu_frequency (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), 0);

  return self->cpu_frequency;
}

void
peek_application_set_cpu_frequency (PeekApplication *self,
                                    guint64          value)
{
  g_return_if_fail (PEEK_IS_APPLICATION (self));

  self->cpu_frequency = value;
}

guint
peek_application_get_interval (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), 0);

  return self->interval;
}

void
peek_application_set_interval (PeekApplication *self,
                               guint            value)
{
  g_return_if_fail (PEEK_IS_APPLICATION (self));

  self->interval = value;

  g_settings_set_int (self->settings, "interval", self->interval);

  g_signal_emit_by_name (self, "interval-update");
}

GSettings *
peek_application_get_settings (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), NULL);

  return self->settings;
}

GtkWidget *
peek_application_get_search_entry (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), NULL);

  return self->search_entry;
}

PeekApplication *
peek_application_get_instance (void)
{
  static PeekApplication *app = NULL;

  if (!app)
    app = g_object_new (PEEK_TYPE_APPLICATION,
                        "application-id", APPLICATION_ID,
                        "flags", G_APPLICATION_DEFAULT_FLAGS,
                        NULL);

  return app;
}

PeekApplication *
peek_application_new (void)
{
  return peek_application_get_instance ();
}
