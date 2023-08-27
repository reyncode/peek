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

#ifndef PEEK_PREFERENCES_H
#define PEEK_PREFERENCES_H

#include <adwaita.h>

#include "peek-window.h"
#include "peek-tree-view.h"

G_BEGIN_DECLS

#define PEEK_TYPE_PREFERENCES (peek_preferences_get_type ())
G_DECLARE_FINAL_TYPE (PeekPreferences, peek_preferences, PEEK, PREFERENCES, AdwPreferencesWindow)

PeekPreferences *peek_preferences_new (PeekWindow *window);

G_END_DECLS

#endif
