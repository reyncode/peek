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

#ifndef PEEK_TREE_VIEW_H
#define PEEK_TREE_VIEW_H

#include <adwaita.h>

G_BEGIN_DECLS

#define PEEK_TYPE_TREE_VIEW (peek_tree_view_get_type ())
G_DECLARE_FINAL_TYPE (PeekTreeView, peek_tree_view, PEEK, TREE_VIEW, GtkBox)

PeekTreeView *peek_tree_view_new (void);

void peek_tree_view_set_search_entry (PeekTreeView *peek_tree_view,
                                      GtkEditable  *editable);

const gchar *parse_priority_from_nice (gint nice);

gchar *parse_duration_centiseconds (guint centiseconds);

G_END_DECLS

#endif
