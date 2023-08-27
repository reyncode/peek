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

#include <adwaita.h>

#include "peek-window.h"
#include "peek-preferences.h"
#include "peek-application.h"
#include "peek-tree-view.h"
#include "peek-tree-model.h"

#define WINDOW_RESOURCE_PATH "/com/github/reyncode/peek/data/ui/window.ui"
#define HEADER_MENU_RESOURCE_PATH "/com/github/reyncode/peek/data/ui/header-menu.ui"

struct _PeekWindow {
  AdwApplicationWindow parent;

  // header bar
  GtkWidget *search_entry;
  GtkWidget *header_menu_button;

  // main window
  GtkWidget *peek_tree_view;
};

G_DEFINE_TYPE (PeekWindow, peek_window, ADW_TYPE_APPLICATION_WINDOW)

static void preferences_activated (GSimpleAction *action,
                                   GVariant      *parameter,
                                   gpointer       data);

static void about_activated (GSimpleAction *action,
                             GVariant      *parameter,
                             gpointer       data);

static GActionEntry win_entries [] = 
{
  { "preferences", preferences_activated, NULL, NULL, NULL },
  { "about", about_activated, NULL, NULL, NULL },
};


GtkWidget *
peek_window_get_search_entry (PeekWindow *window)
{
  g_return_val_if_fail (PEEK_IS_WINDOW (window), NULL);

  return window->search_entry;
}

static void
search_entry_changed (GtkEditable *self,
                      gpointer     data)
{
  PeekApplication *app;
  GtkTreeModel *model;

  app = peek_application_get_instance ();
  model = peek_application_get_model (app);

  gtk_tree_model_filter_refilter (GTK_TREE_MODEL_FILTER (gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (model))));
}

static void
preferences_activated (GSimpleAction *action,
                       GVariant      *parameter,
                       gpointer       data)
{
  PeekWindow *window = PEEK_WINDOW (data);
  PeekPreferences *preferences;

  preferences = peek_preferences_new (window);

  gtk_window_present (GTK_WINDOW (preferences));
}

static void
about_activated (GSimpleAction *action,
                 GVariant      *parameter,
                 gpointer       data)
{
  const char *devs[] = {
    "Alex Reynolds",
    NULL
  };

  PeekApplication *app;
  
  app = peek_application_get_instance ();

  adw_show_about_window (gtk_application_get_active_window (GTK_APPLICATION (app)),
                         "application-name", "Peek",
                         "application-icon", "org.gnome.Adwaita1.Demo",
                         "version", "1.0.0",
                         "issue-url", "https://github.com/reyncode/peek/issues",
                         "license-type", GTK_LICENSE_GPL_3_0,
                         "developers", devs,
                         NULL);
}

static void
peek_window_finalize (GObject *object)
{
  G_OBJECT_CLASS (peek_window_parent_class)->finalize (object);
}

static void
peek_window_dispose (GObject *object)
{
  G_OBJECT_CLASS (peek_window_parent_class)->dispose (object);
}

static void
init_header_menu_button (PeekWindow *self)
{
	GtkBuilder *ui_builder;
	GMenuModel *header_menu;

	ui_builder = gtk_builder_new_from_resource (HEADER_MENU_RESOURCE_PATH);
	header_menu = G_MENU_MODEL (gtk_builder_get_object (ui_builder, "header-menu"));

	gtk_menu_button_set_menu_model (GTK_MENU_BUTTON (self->header_menu_button),
                                  header_menu);

  g_action_map_add_action_entries (G_ACTION_MAP (self),
                                   win_entries, G_N_ELEMENTS (win_entries),
                                   self);

  g_object_unref (ui_builder);
}

static void
peek_window_init (PeekWindow *self)
{
  g_type_ensure (PEEK_TYPE_TREE_VIEW);

  gtk_widget_init_template (GTK_WIDGET (self));

  init_header_menu_button (self);

  peek_tree_view_set_search_entry (PEEK_TREE_VIEW (self->peek_tree_view),
                                   GTK_EDITABLE (self->search_entry));

  gtk_search_entry_set_key_capture_widget (GTK_SEARCH_ENTRY (self->search_entry),
                                           GTK_WIDGET (self));
}

static void
peek_window_class_init (PeekWindowClass *klass)
{
  G_OBJECT_CLASS (klass)->finalize = peek_window_finalize;
  G_OBJECT_CLASS (klass)->dispose = peek_window_dispose;

  gtk_widget_class_set_template_from_resource (GTK_WIDGET_CLASS (klass),
                                               WINDOW_RESOURCE_PATH);

  // header bar
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekWindow,
                                        search_entry);
  
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekWindow,
                                        header_menu_button);

  // main window 
  gtk_widget_class_bind_template_child (GTK_WIDGET_CLASS (klass),
                                        PeekWindow,
                                        peek_tree_view);

  gtk_widget_class_bind_template_callback (GTK_WIDGET_CLASS (klass),
                                           search_entry_changed);
}

PeekWindow *
peek_window_new (PeekApplication *app)
{
  return g_object_new (PEEK_TYPE_WINDOW,
                       "application", app,
                        NULL);
}
