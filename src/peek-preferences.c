#include <adwaita.h>

#include "peek-preferences.h"
#include "peek-tree-view.h"
#include "peek-tree-model.h"
#include "peek-application.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/data/ui/preferences.ui"

struct _PeekPreferences {
  AdwPreferencesWindow parent;

  GtkWidget *tree_view;

  GtkWidget *interval_spin_button;

  GtkWidget *name_check_button;
  GtkWidget *user_check_button;
  GtkWidget *memory_check_button;
  GtkWidget *cpu_check_button;
  GtkWidget *cpu_time_check_button;
  GtkWidget *ppid_check_button;
  GtkWidget *state_check_button;
  GtkWidget *nice_check_button;
  GtkWidget *priority_check_button;
};

G_DEFINE_TYPE (PeekPreferences, peek_preferences, ADW_TYPE_PREFERENCES_WINDOW)

static void
interval_value_changed (GtkSpinButton *self,
                            gpointer       data)
{
  PeekApplication *app;

  app = peek_application_get_instance ();
  peek_application_set_interval (app, gtk_spin_button_get_value_as_int (self));
}

static void
bind_check_buttons (PeekPreferences *self)
{
  PeekApplication *app;
  GSettings *settings;

  app = peek_application_get_instance ();
  settings = peek_application_get_settings (app);

  g_settings_bind (settings, "show-name",
                   self->name_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-user",
                   self->user_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-memory",
                   self->memory_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-cpu",
                   self->cpu_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-cpu-time",
                   self->cpu_time_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-ppid",
                   self->ppid_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-state",
                   self->state_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-nice",
                   self->nice_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);

  g_settings_bind (settings, "show-priority",
                   self->priority_check_button, "active",
                   G_SETTINGS_BIND_DEFAULT);
}

static void
peek_preferences_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_preferences_parent_class)->finalize (object);
}

static void
peek_preferences_init (PeekPreferences *self)
{
  gtk_widget_init_template (GTK_WIDGET (self));

  bind_check_buttons (self);

  PeekApplication *app;
  guint interval;

  app = peek_application_get_instance ();
  interval = peek_application_get_interval (app);

  gtk_spin_button_set_value (GTK_SPIN_BUTTON (self->interval_spin_button),
                             interval);
}

static void
peek_preferences_class_init (PeekPreferencesClass *klass)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               RESOURCE_PATH);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        interval_spin_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        name_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        user_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        memory_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        cpu_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        cpu_time_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        ppid_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        state_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        nice_check_button);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        priority_check_button);

  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (klass),
                                           interval_value_changed);
  
  G_OBJECT_CLASS (klass)->finalize = peek_preferences_finalize;
}

PeekPreferences *
peek_preferences_new (PeekWindow *window)
{
  return g_object_new (PEEK_TYPE_PREFERENCES,
                       "transient-for", window,
                       NULL);
}
