/*
 *  Peek - A process monitoring tool
 *  Copyright (C) 2023 Alex Reynolds <hellport@protonmail.com>
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef PEEK_APPLICATION_H
#define PEEK_APPLICATION_H

#include <adwaita.h>

G_BEGIN_DECLS

#define PEEK_TYPE_APPLICATION (peek_application_get_type ())
G_DECLARE_FINAL_TYPE (PeekApplication, peek_application, PEEK, APPLICATION, AdwApplication)

PeekApplication *peek_application_get_instance (void);

GtkTreeModel *peek_application_get_model (PeekApplication *self);

GHashTable *peek_application_get_proc_table (PeekApplication *self);

GtkWidget *peek_application_get_search_entry (PeekApplication *self);

guint64 peek_application_get_cpu_time_total (PeekApplication *self);

void peek_application_set_cpu_time_total (PeekApplication *self, guint64 value);

guint64 peek_application_get_cpu_time_total_last (PeekApplication *self);

void peek_application_set_cpu_time_total_last (PeekApplication *self, guint64 value);

guint64 peek_application_get_cpu_frequency (PeekApplication *self);

void peek_application_set_cpu_frequency (PeekApplication *self, guint64 value);

void peek_application_set_interval (PeekApplication *self, guint value);

guint peek_application_get_interval (PeekApplication *self);

GSettings *peek_application_get_settings (PeekApplication *self);

PeekApplication *peek_application_new (void);

G_END_DECLS

#endif
