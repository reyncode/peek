#pragma once

#include <adwaita.h>

#include "peek-window.h"

G_BEGIN_DECLS

#define PEEK_TYPE_PROCESS_VIEW (peek_process_view_get_type ())
G_DECLARE_FINAL_TYPE (PeekProcessView, peek_process_view, PEEK, PROCESS_VIEW, AdwPreferencesWindow)

PeekProcessView *peek_process_view_new (PeekWindow *window,
                                        const gchar *title);

G_END_DECLS