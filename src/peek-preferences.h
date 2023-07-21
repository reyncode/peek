#pragma once

#include <adwaita.h>

#include "peek-window.h"

G_BEGIN_DECLS

#define PEEK_TYPE_PREFERENCES (peek_preferences_get_type ())
G_DECLARE_FINAL_TYPE (PeekPreferences, peek_preferences, PEEK, PREFERENCES, AdwPreferencesWindow)

PeekPreferences *peek_preferences_new (PeekWindow *window);

G_END_DECLS