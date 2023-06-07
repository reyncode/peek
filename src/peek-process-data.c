#include "peek-process-data.h"

void
peek_process_data_test_call (void)
{
  FILE *fp;
  char buffer[128] = { 0 };

  fp = popen ("/usr/bin/ps -eo comm,pid,%mem,user --sort comm", "r");

  if (!fp)
    printf ("command failed\n");

  while (fgets (buffer, 128, fp) != NULL)
    printf ("%s", buffer);

  pclose (fp);
}