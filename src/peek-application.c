#include <adwaita.h>

#include "peek-application.h"
#include "peek-window.h"
#include "peek-process.h"
#include "peek-tree-model.h"

#include <glibtop.h>
#include <glibtop/sysinfo.h>

#define APPLICATION_ID "com.github.reyncode.peek"
#define REFRESH_INTERVAL 2

struct _PeekApplication {
  AdwApplication parent;

  GtkTreeModel *model;
  GHashTable   *proc_table;
  GtkWidget    *search_entry;

  guint         cores;
  guint64       cpu_time_total;
  guint64       cpu_time_total_last;
  guint64       cpu_frequency;

  guint timeout;
};

// consider making private and public members

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

  PeekWindow *window;

  window = peek_window_new (app);

  app->search_entry = peek_window_get_search_entry (window);
  
  gtk_window_present (GTK_WINDOW (window));
}

static guint
get_system_core_count ()
{
  const glibtop_sysinfo *info;

  info = glibtop_get_sysinfo ();

  g_assert (info != NULL);

  return info->ncpu;
}

static void
peek_application_init (PeekApplication *self)
{
  self->model = peek_tree_model_new (self);
  
  self->proc_table = g_hash_table_new_full (g_int_hash,
                                            g_int_equal,
                                            g_free,
                                            proc_table_value_destroy);

  self->search_entry = NULL;

  self->cores = get_system_core_count ();

  self->cpu_time_total = 0;
  self->cpu_time_total_last = 0;
  self->cpu_frequency = 0;
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
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), NULL);

  return self->proc_table;
}

guint
peek_application_get_core_count (PeekApplication *self)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (self), 0);

  return self->cores;
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

GtkWidget *
peek_application_get_search_entry (PeekApplication *self)
{
  // to speed up filtering and refiltering a search over the tree model,
  // we are providing a reference directly to application so that we
  // can speed up entry text retrieval in the filter func

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
                        "flags", G_APPLICATION_FLAGS_NONE,
                        NULL);

  return app;
}

PeekApplication *
peek_application_new (void)
{
  return peek_application_get_instance ();
}