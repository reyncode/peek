#include "peek-application.h"
#include "peek-window.h"
#include "peek-process.h"

#define APPLICATION_ID "com.github.reyncode.peek"

struct _PeekApplication {
  GtkApplication parent;

  // process_list
};

G_DEFINE_TYPE (PeekApplication, peek_application, GTK_TYPE_APPLICATION)

// 1
static void
peek_application_startup (GApplication *self)
{
  G_APPLICATION_CLASS (peek_application_parent_class)->startup (self);
  
  // setting accels here

  peek_process_startup ();
}

// 3
static void
peek_application_activate (GApplication *self)
{
  PeekWindow *window;

  window = peek_window_new (PEEK_APPLICATION (self));
  
  gtk_window_present (GTK_WINDOW (window));
}

static void
peek_application_init (PeekApplication *self)
{
  // call create GtkTreeModel here
}

static void
peek_application_class_init (PeekApplicationClass *klass)
{
  G_APPLICATION_CLASS (klass)->startup = peek_application_startup;
  G_APPLICATION_CLASS (klass)->activate = peek_application_activate;
}

PeekApplication *
peek_application_new (void)
{
  PeekApplication *application;

  application = g_object_new (PEEK_TYPE_APPLICATION,
                              "application-id", APPLICATION_ID,
                              "flags", G_APPLICATION_FLAGS_NONE,
                              NULL
  );

  return application;
}