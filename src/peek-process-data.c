#include "peek-process-data.h"

#include <stdlib.h>
#include <string.h>

#define PROCESS_NAME_SIZE 35
#define PROCESS_USER_SIZE 33
#define LINE_SIZE 120
#define POPEN_CALL 0

typedef struct {
  char     *name;
  unsigned  pid;
  double    memory;
  char     *user;
} Process;

static char *raw = { "firefox   2817  318.9   alex" };

static void peek_process_data_tokenize_stream (void);

void
peek_process_data_test_call (void)
{

  if (POPEN_CALL)
  {
    FILE *fp;
    char buffer[128] = { 0 };

    fp = popen ("/usr/bin/ps -eo comm,pid,%mem,user --sort comm", "r");

    if (!fp)
      printf ("command failed\n");

    while (fgets (buffer, 128, fp) != NULL)
      printf ("%s", buffer);
      // tokenize call here?

    pclose (fp);
  }

  peek_process_data_tokenize_stream ();
}

static void
peek_process_data_tokenize_stream (void)
{
  Process *p = malloc (sizeof (Process));

  p->name = malloc (sizeof (char) * PROCESS_NAME_SIZE);
  p->user = malloc (sizeof (char) * PROCESS_USER_SIZE);

  char string[LINE_SIZE] = { 0 };
  char *token;
  unsigned col = 0;

  strcpy (string, raw);
  token = strtok (string, " ");

  // for every row
  while (token)
  {
    switch (col)
    {
      case 0:
        strcpy (p->name, token);
        break;

      case 1:
        p->pid = strtol (token, NULL, 10);
        break;
      
      case 2:
        p->memory = strtod (token, NULL);
        break;
      
      case 3:
        strcpy (p->user, token);
        break;
    }
    token = strtok (NULL, " ");

    col++;
  }

  printf ("PROCESS NAME\tPID\tMEM\tUSER\n");
  printf ("%s\t\t%u\t%.2f\t%s\n",
          p->name, p->pid, p->memory, p->user);

  free (p->user);
  free (p->name);
  free (p);
}