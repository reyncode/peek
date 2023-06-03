#pragma once

#include "gtk/gtk.h"

G_BEGIN_DECLS

#define PEEK_TYPE_APP (peek_app_get_type ())
G_DECLARE_FINAL_TYPE (PeekApp, peek_app, PEEK, APP, GtkApplication)

PeekApp *peek_app_new (void);

G_END_DECLS