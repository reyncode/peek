#include "peek-process-view.h"
#include "peek-application.h"
#include "peek-tree-model.h"
#include "peek-process.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/ui/process-view.ui"

struct _PeekProcessView {
  AdwPreferencesWindow parent;

  pid_t pid;

  GtkWidget *memory_label;
  GtkWidget *cpu_usage_label;
};

G_DEFINE_TYPE (PeekProcessView, peek_process_view, ADW_TYPE_PREFERENCES_WINDOW)

static void
proc_update_cb (PeekApplication *app,
                gpointer         data) // param of pid
{

  PeekProcessView *view = PEEK_PROCESS_VIEW (data);
  GHashTable *table;
  ProcData *proc;

  table = peek_application_get_proc_table (app);
  proc = g_hash_table_lookup (table, &view->pid);
  
  g_print ("%s | %lu\n", proc->name, proc->memory);

  /*
    this callback is where we need to get the ProcData

    1. create members for each of the label values 
    and update them with setters

    all fields would need to be allocated prior
    
    // if guint
    if (procA->memory == procB->memory)
      return
    else
      procA->memory = procB->memory

    // if gchar
    ...
    else
      
      ... operation to convert # to gchar

      g_snprintf (procA->status, "%s", formatted_status)

    2. look up the app's hash table and go through the value
      how to do you get the pid to look up the value

      one tree view corresponds to each call of "proc-update"
      when we create the window, we could also pass in/use
      setter for setting a pid

      currently segfaulting
      handle edge case where a valid proc is removed

  */
  
  gchar *memory_formatted = g_format_size (proc->memory);
  gchar *cpu_usage_formatted = g_strdup_printf ("%.2f", proc->cpu_usage);

  gtk_label_set_label (GTK_LABEL (view->memory_label), memory_formatted);
  gtk_label_set_label (GTK_LABEL (view->cpu_usage_label), cpu_usage_formatted);

  g_clear_pointer (&memory_formatted, g_free);
  g_clear_pointer (&cpu_usage_formatted, g_free);
}

void peek_process_view_set_pid (PeekProcessView *view, 
                                const pid_t      pid)
{
  view->pid = pid;
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

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        memory_label);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekProcessView,
                                        cpu_usage_label);

  G_OBJECT_CLASS (klass)->finalize = peek_process_view_finalize;
}

PeekProcessView *
peek_process_view_new (PeekWindow *window,
                       const gchar *title)
{
  return g_object_new (PEEK_TYPE_PROCESS_VIEW,
                      //  "transient-for", window,
                       "title", title,
                       NULL);
}