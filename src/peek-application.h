#pragma once

#include "gtk/gtk.h"

G_BEGIN_DECLS

#define PEEK_TYPE_APPLICATION (peek_application_get_type ())
G_DECLARE_FINAL_TYPE (PeekApplication, peek_application, PEEK, APPLICATION, GtkApplication)

PeekApplication *peek_application_get_instance (void);

GtkTreeModel *peek_application_get_model (PeekApplication *self);

GHashTable *peek_application_get_proc_table (PeekApplication *self);

PeekApplication *peek_application_new (void);

G_END_DECLS