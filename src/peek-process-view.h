#ifndef PEEK_PROCESS_VIEW_H
#define PEEK_PROCESS_VIEW_H

#include <adwaita.h>

#include "peek-window.h"

G_BEGIN_DECLS

#define PEEK_TYPE_PROCESS_VIEW (peek_process_view_get_type ())
G_DECLARE_FINAL_TYPE (PeekProcessView, peek_process_view, PEEK, PROCESS_VIEW, AdwWindow)

PeekProcessView *peek_process_view_new (const pid_t pid);

G_END_DECLS

#endif
