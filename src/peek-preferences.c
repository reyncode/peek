#include "peek-preferences.h"
#include "peek-tree-view.h"
#include "peek-tree-model.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/ui/preferences.ui"

enum {
  PROP_TREE_VIEW = 1,
  N_PROPERTIES
};

static GParamSpec *preferences_props[N_PROPERTIES] = { NULL, };

struct _PeekPreferences {
  AdwPreferencesWindow parent;

  GtkWidget *tree_view;

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
peek_preferences_set_property (GObject      *object,
                               guint         prop_id,
                               const GValue *value,
                               GParamSpec   *pspec)
{
  PeekPreferences *prefs = PEEK_PREFERENCES (object);

  switch (prop_id)
  {
    case PROP_TREE_VIEW:
      prefs->tree_view = g_value_get_object (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
peek_preferences_get_property (GObject      *object,
                               guint         prop_id,
                               GValue       *value,
                               GParamSpec   *pspec)
{
  PeekPreferences *prefs = PEEK_PREFERENCES (object);

  switch (prop_id)
  {
    case PROP_TREE_VIEW:
      g_value_set_object (value, prefs->tree_view);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
  }
}

static void
peek_preferences_constructed (GObject *object)
{
  PeekPreferences *prefs = PEEK_PREFERENCES (object);
  GtkTreeViewColumn *column;

  for (gint i = 0; i < NUM_COLUMNS; i++)
  {
    column = gtk_tree_view_get_column (GTK_TREE_VIEW (prefs->tree_view), i);

    if (column)
    {
      switch (i)
      {
        case COLUMN_NAME:
          g_object_bind_property (prefs->name_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_USER:
          g_object_bind_property (prefs->user_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_MEMORY:
          g_object_bind_property (prefs->memory_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_CPU_P:
          g_object_bind_property (prefs->cpu_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_CPU_TIME:
          g_object_bind_property (prefs->cpu_time_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_PPID:
          g_object_bind_property (prefs->ppid_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_STATE:
          g_object_bind_property (prefs->state_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_NICE:
          g_object_bind_property (prefs->nice_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        case COLUMN_PRIORITY:
          g_object_bind_property (prefs->priority_check_button, "active",
                                  column, "visible",
                                  G_BINDING_DEFAULT);
          break;
        default:
          break;
      }
    }
  }
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
}

static void
peek_preferences_class_init (PeekPreferencesClass *klass)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               RESOURCE_PATH);

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
  
  G_OBJECT_CLASS (klass)->set_property = peek_preferences_set_property;
  G_OBJECT_CLASS (klass)->get_property = peek_preferences_get_property;

  preferences_props[PROP_TREE_VIEW] = g_param_spec_object ("tree-view",
                                                           "TreeView",
                                                           "The tree view holding the proc data rows.",
                                                           GTK_TYPE_WIDGET,
                                                           G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

  g_object_class_install_properties (G_OBJECT_CLASS (klass),
                                     N_PROPERTIES,
                                     preferences_props);

  G_OBJECT_CLASS (klass)->constructed = peek_preferences_constructed;
  G_OBJECT_CLASS (klass)->finalize = peek_preferences_finalize;
}

PeekPreferences *
peek_preferences_new (PeekWindow *window,
                      GtkWidget  *tree_view)
{
  return g_object_new (PEEK_TYPE_PREFERENCES,
                      //  "transient-for", window,
                       "tree-view", tree_view,
                       NULL);
}