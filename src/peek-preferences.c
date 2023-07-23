#include "peek-preferences.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/ui/preferences.ui"

struct _PeekPreferences {
  AdwPreferencesWindow parent;

  GtkWidget *name_check_button;
  GBinding  *name_binding;
};

G_DEFINE_TYPE (PeekPreferences, peek_preferences, ADW_TYPE_PREFERENCES_WINDOW)

static void
create_tree_bindings (PeekPreferences *self)
{
  PeekApplication *app;
  GtkTreeModel *model;
  GtkTreeIter   iter;
  GtkTreePath  *path;
  GtkTreeViewColumn *column;

  app = peek_application_get_instance ();
  model = peek_application_get_model (app);

  gtk_tree_model_get_iter (model, &iter, path);

  gtk_tree_model_get (model, &iter, 2, &column, -1);

  self->name_binding = g_object_bind_property (self->name_check_button, "active",
                                               column, "visible",
                                               G_BINDING_DEFAULT);
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

  // create_tree_bindings (self);
}

static void
peek_preferences_class_init (PeekPreferencesClass *klass)
{
  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               RESOURCE_PATH);

  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekPreferences,
                                        name_check_button);

  G_OBJECT_CLASS (klass)->finalize = peek_preferences_finalize;
}

PeekPreferences *
peek_preferences_new (PeekWindow *window)
{
  return g_object_new (PEEK_TYPE_PREFERENCES,
                       "transient-for", window,
                       NULL);
}