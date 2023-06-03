#include "peek-app.h"
#include "peek-app-win.h"

#define APPLICATION_ID "com.github.reyncode.peek"

struct _PeekApp {
  GtkApplication parent;
};

G_DEFINE_TYPE (PeekApp, peek_app, GTK_TYPE_APPLICATION)

static void
peek_app_startup (GApplication *self)
{
  G_APPLICATION_CLASS (peek_app_parent_class)->startup (self);
  
  // css applied here
}

static void
peek_app_activate (GApplication *self)
{
  PeekAppWin *window;

  window = peek_app_win_new (PEEK_APP (self));
  
  gtk_window_present (GTK_WINDOW (window));
}

static void
peek_app_init (PeekApp *self)
{

}

static void
peek_app_class_init (PeekAppClass *klass)
{
  G_APPLICATION_CLASS (klass)->startup = peek_app_startup;
  G_APPLICATION_CLASS (klass)->activate = peek_app_activate;
}

PeekApp *
peek_app_new (void)
{
  return g_object_new (
    PEEK_TYPE_APP,
    "application-id", APPLICATION_ID,
    "flags", G_APPLICATION_FLAGS_NONE,
    NULL
  );
}