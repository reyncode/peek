#include "peek-process.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <proc/readproc.h>

#define PROC_NAME_SIZE 50
#define EXE_PATH_SIZE 256
#define SYMBOLIC_PATH_SIZE 80

static char *
peek_process_parse_name_from_path (char *path)
{
  char *token;
  char  name[PROC_NAME_SIZE] = { 0 };

  token = strtok (path, "/");

  while (token != NULL)
  {
    strcpy (name, token);
    token = strtok (NULL, "/");
  }

  return strndup (name, PROC_NAME_SIZE);
}

static char *
peek_process_get_executable_name (char *proc_subdir)
{
  char    proc_symbolic_path[SYMBOLIC_PATH_SIZE] = { 0 };
  char    proc_exe_path[EXE_PATH_SIZE] = { 0 };
  ssize_t bytes;

  snprintf (proc_symbolic_path, SYMBOLIC_PATH_SIZE, "%s/exe", proc_subdir);

  bytes = readlink (proc_symbolic_path, proc_exe_path, EXE_PATH_SIZE);

  if (bytes != -1)
  {
    proc_exe_path[bytes] = '\0';

    return peek_process_parse_name_from_path (proc_exe_path);
  }
  else
  {
    return NULL;
  }
}

/* read directly from /proc/[pid] for process information */
static void
peek_process_create_proc_list (void)
{
  PROCTAB *proc;
  proc_t   proc_info;

  proc = openproc (PROC_FILLSTATUS | PROC_FILLUSR);

  memset (&proc_info, 0, sizeof(proc_info));

  // cycle through all procs
  while (readproc(proc, &proc_info) != NULL)
  {
    char *proc_name;

    // prefer the executable name
    proc_name = peek_process_get_executable_name (proc->path);
    if (!proc_name)
    {
      proc_name = strndup (proc_info.cmd, 64);
    }

    printf ("name: %s\nid: %d\nppid: %d\nuser: %s\nstate: %c\n\n",
            proc_name, proc_info.tid, proc_info.ppid, proc_info.ruser, proc_info.state);

    free (proc_name);
  }

  closeproc (proc);
}

void
peek_process_startup (void)
{
  peek_process_create_proc_list ();
}