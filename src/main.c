#include <adwaita.h>

#include "peek-application.h"

int
main (int    argc,
      char **argv)
{
  return g_application_run (G_APPLICATION (peek_application_new ()), argc, argv);
}
