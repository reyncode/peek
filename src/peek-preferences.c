#include "peek-preferences.h"

#define RESOURCE_PATH "/com/github/reyncode/peek/ui/preferences.ui"

struct _PeekPreferences {
  GtkDialog parent;
};

G_DEFINE_TYPE (PeekPreferences, peek_preferences, GTK_TYPE_DIALOG)

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

  G_OBJECT_CLASS (klass)->finalize = peek_preferences_finalize;
}

PeekPreferences *
peek_preferences_new ()
{
  return g_object_new (PEEK_TYPE_PREFERENCES,
                       NULL);
}