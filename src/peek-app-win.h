#pragma once

#include "gtk/gtk.h"

#include "peek-app.h"

G_BEGIN_DECLS

#define PEEK_TYPE_APP_WIN (peek_app_win_get_type ())
G_DECLARE_FINAL_TYPE (PeekAppWin, peek_app_win, PEEK, APP_WIN, GtkApplicationWindow)

PeekAppWin *peek_app_win_new (PeekApp *app);

G_END_DECLS