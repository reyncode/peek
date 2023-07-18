#pragma once

#include <adwaita.h>

#include "peek-application.h"

G_BEGIN_DECLS

#define PEEK_TYPE_WINDOW (peek_window_get_type ())
G_DECLARE_FINAL_TYPE (PeekWindow, peek_window, PEEK, WINDOW, GtkApplicationWindow)

PeekWindow *peek_window_new (PeekApplication *app);

GtkWidget *peek_window_get_search_entry (PeekWindow *window);

G_END_DECLS