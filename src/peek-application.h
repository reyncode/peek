#pragma once

#include <adwaita.h>

G_BEGIN_DECLS

#define PEEK_TYPE_APPLICATION (peek_application_get_type ())
G_DECLARE_FINAL_TYPE (PeekApplication, peek_application, PEEK, APPLICATION, AdwApplication)

PeekApplication *peek_application_get_instance (void);

GtkTreeModel *peek_application_get_model (PeekApplication *self);

GHashTable *peek_application_get_proc_table (PeekApplication *self);

GtkWidget *peek_application_get_search_entry (PeekApplication *self);

guint peek_application_get_core_count (PeekApplication *self);

guint64 peek_application_get_cpu_time_total (PeekApplication *self);

void peek_application_set_cpu_time_total (PeekApplication *self, guint64 value);

guint64 peek_application_get_cpu_time_total_last (PeekApplication *self);

void peek_application_set_cpu_time_total_last (PeekApplication *self, guint64 value);

guint64 peek_application_get_cpu_frequency (PeekApplication *self);

void peek_application_set_cpu_frequency (PeekApplication *self, guint64 value);

PeekApplication *peek_application_new (void);

G_END_DECLS