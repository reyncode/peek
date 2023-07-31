#include "peek-process-view.h"
#include "peek-application.h"
#include "peek-tree-model.h"
#include "peek-tree-view.h"
#include "peek-process.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/data/ui/process-view.ui"

enum {
  PROP_PID = 1,
  N_PROPERTIES
};

static GParamSpec *process_view_props[N_PROPERTIES] = { NULL, };

struct _PeekProcessView {
  AdwWindow parent;

  pid_t pid;

  GtkWidget *name_label;
  GtkWidget *pid_label;
  GtkWidget *ppid_label;
  GtkWidget *user_label;
  GtkWidget *status_label;
  GtkWidget *memory_label;
  GtkWidget *cpu_usage_label;
  GtkWidget *cpu_time_label;
  GtkWidget *priority_label;
  GtkWidget *nice_label;
};

G_DEFINE_TYPE (PeekProcessView, peek_process_view, ADW_TYPE_WINDOW)

static void
proc_update_cb (PeekApplication *app,
                gpointer         data)
{
  PeekProcessView *view = PEEK_PROCESS_VIEW (data);
  guint64          frequency;
  GHashTable      *table;
  ProcData        *proc;

  table = peek_application_get_proc_table (app);
  proc = g_hash_table_lookup (table, &view->pid);

  if (!proc)
  {
    gchar *title;
    title = g_strdup_printf ("Dead - %s", gtk_window_get_title (GTK_WINDOW (view)));
    gtk_window_set_title (GTK_WINDOW (view), title);

    gtk_label_set_label (GTK_LABEL (view->status_label), "Dead");

    g_signal_handlers_disconnect_by_func (app,
                                          proc_update_cb,
                                          view);

    return;
  }
  
  gchar *nice_label = g_strdup_printf ("%d", proc->nice);
  gchar *memory_formatted = g_format_size (proc->memory);
  gchar *cpu_usage_formatted = g_strdup_printf ("%.2f", proc->cpu_usage);

  frequency = peek_application_get_cpu_frequency (app);

  guint time = proc->cpu_time;
  time = 100 * time / frequency;
  gchar *cpu_time_formatted = parse_duration_centiseconds (time);

  gtk_label_set_label (GTK_LABEL (view->nice_label), nice_label);
  gtk_label_set_label (GTK_LABEL (view->priority_label), parse_priority_from_nice (proc->nice));
  gtk_label_set_label (GTK_LABEL (view->user_label), parse_user_from_uid (proc->uid));
  gtk_label_set_label (GTK_LABEL (view->status_label), parse_proc_state (proc->state));
  gtk_label_set_label (GTK_LABEL (view->memory_label), memory_formatted);
  gtk_label_set_label (GTK_LABEL (view->cpu_usage_label), cpu_usage_formatted);
  gtk_label_set_label (GTK_LABEL (view->cpu_time_label), cpu_time_formatted);

  g_clear_pointer (&nice_label, g_free);
  g_clear_pointer (&memory_formatted, g_free);
  g_clear_pointer (&cpu_usage_formatted, g_free);
  g_clear_pointer (&cpu_time_formatted, g_free);
}

static void
peek_process_view_set_property (GObject      *object,
                                guint         prop_id,
                                const GValue *value,
                                GParamSpec   *pspec)
{
  PeekProcessView *view = PEEK_PROCESS_VIEW (object);

  switch (prop_id)
  {
    case PROP_PID:
      view->pid = g_value_get_int (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
peek_process_view_get_property (GObject      *object,
                                guint         prop_id,
                                GValue       *value,
                                GParamSpec   *pspec)
{
  PeekProcessView *view = PEEK_PROCESS_VIEW (object);

  switch (prop_id)
  {
    case PROP_PID:
      g_value_set_int (value, view->pid);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
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
peek_process_view_constructed (GObject *object)
{
  PeekProcessView *self = PEEK_PROCESS_VIEW (object);
  PeekApplication *app;
  guint64          frequency;
  GHashTable      *table;
  ProcData        *proc;

  app = peek_application_get_instance ();
  frequency = peek_application_get_cpu_frequency (app);
  table = peek_application_get_proc_table (app);
  proc = g_hash_table_lookup (table, &self->pid);

  gchar *title = g_strdup_printf ("%s (%d)", proc->name, proc->pid);
  gchar *pid_label = g_strdup_printf ("%d", proc->pid);
  gchar *ppid_label = g_strdup_printf ("%d", proc->ppid);
  gchar *nice_label = g_strdup_printf ("%d", proc->nice);
  gchar *memory_formatted = g_format_size (proc->memory);
  gchar *cpu_usage_formatted = g_strdup_printf ("%.2f", proc->cpu_usage);

  guint time = proc->cpu_time;
  time = 100 * time / frequency;
  gchar *cpu_time_formatted = parse_duration_centiseconds (time);

  gtk_window_set_title (GTK_WINDOW (self), title);

  // static
  gtk_label_set_label (GTK_LABEL (self->name_label), proc->name);
  gtk_label_set_label (GTK_LABEL (self->pid_label), pid_label);
  gtk_label_set_label (GTK_LABEL (self->ppid_label), ppid_label);

  // dynamic
  gtk_label_set_label (GTK_LABEL (self->nice_label), nice_label);
  gtk_label_set_label (GTK_LABEL (self->priority_label), parse_priority_from_nice (proc->nice));
  gtk_label_set_label (GTK_LABEL (self->user_label), parse_user_from_uid (proc->uid));
  gtk_label_set_label (GTK_LABEL (self->status_label), parse_proc_state (proc->state));
  gtk_label_set_label (GTK_LABEL (self->memory_label), memory_formatted);
  gtk_label_set_label (GTK_LABEL (self->cpu_usage_label), cpu_usage_formatted);
  gtk_label_set_label (GTK_LABEL (self->cpu_time_label), cpu_time_formatted);

  g_clear_pointer (&title, g_free);
  g_clear_pointer (&pid_label, g_free);
  g_clear_pointer (&ppid_label, g_free);
  g_clear_pointer (&nice_label, g_free);
  g_clear_pointer (&memory_formatted, g_free);
  g_clear_pointer (&cpu_usage_formatted, g_free);
  g_clear_pointer (&cpu_time_formatted, g_free);

  G_OBJECT_CLASS (peek_process_view_parent_class)->constructed (object);
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

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        name_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        pid_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        ppid_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        user_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        status_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        memory_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        cpu_usage_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        cpu_time_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        nice_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        priority_label);
  
  G_OBJECT_CLASS (klass)->set_property = peek_process_view_set_property;
  G_OBJECT_CLASS (klass)->get_property = peek_process_view_get_property;

  process_view_props[PROP_PID] = g_param_spec_int ("pid",
                                                   "PID",
                                                   "Associated process ID",
                                                   -G_MAXINT,
                                                   G_MAXINT,
                                                   -1,
                                                   G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (G_OBJECT_CLASS (klass),
                                     N_PROPERTIES,
                                     process_view_props);

  G_OBJECT_CLASS (klass)->constructed = peek_process_view_constructed;
  G_OBJECT_CLASS (klass)->finalize = peek_process_view_finalize;
}

PeekProcessView *
peek_process_view_new (const pid_t pid)
{
  return g_object_new (PEEK_TYPE_PROCESS_VIEW,
                       "pid", pid,
                       NULL);
}