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

#ifndef PEEK_WINDOW_H
#define PEEK_WINDOW_H

#include <adwaita.h>

#include "peek-application.h"

G_BEGIN_DECLS

#define PEEK_TYPE_WINDOW (peek_window_get_type ())
G_DECLARE_FINAL_TYPE (PeekWindow, peek_window, PEEK, WINDOW, AdwApplicationWindow)

PeekWindow *peek_window_new (PeekApplication *app);

GtkWidget *peek_window_get_search_entry (PeekWindow *window);

G_END_DECLS

#endif
