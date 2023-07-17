#pragma once

// TODO convert to libadwaita
#include "gtk/gtk.h"

G_BEGIN_DECLS

#define PEEK_TYPE_PREFERENCES (peek_preferences_get_type ())
G_DECLARE_FINAL_TYPE (PeekPreferences, peek_preferences, PEEK, PREFERENCES, GtkDialog)

PeekPreferences *peek_preferences_new ();

G_END_DECLS