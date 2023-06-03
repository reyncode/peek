/*

what should the app display?
-active processes
-number of threads
-memory usage
-cpu usage

*/

#include <gtk/gtk.h>

#include "peek-app.h"

int
main (int    argc,
      char **argv)
{
  return g_application_run (G_APPLICATION (peek_app_new ()), argc, argv);
}
