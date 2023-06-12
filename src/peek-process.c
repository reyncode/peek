#include "peek-process.h"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <proc/readproc.h>

#define PROC_NAME_SIZE 50
#define EXE_PATH_SIZE 256
#define SYMBOLIC_PATH_SIZE 80

enum {
  COLUMN_NAME,
  COLUMN_ID,
  COLUMN_USER,
  COLUMN_MEMORY,
  COLUMN_PPID,
  COLUMN_STATE,
  NUM_COLUMNS
};

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
GtkTreeModel *
peek_process_create_proc_model (void)
{
  GtkListStore *store;
  GtkTreeIter   iter;

  store = gtk_list_store_new (NUM_COLUMNS,
                              G_TYPE_STRING,
                              G_TYPE_UINT,
                              G_TYPE_STRING,
                              G_TYPE_ULONG,
                              G_TYPE_UINT,
                              G_TYPE_STRING);

  PROCTAB *proc;
  proc_t   proc_info;

  proc = openproc (PROC_FILLSTATUS | PROC_FILLUSR);

  memset (&proc_info, 0, sizeof(proc_info));

  // cycle through all pids in /proc
  while (readproc(proc, &proc_info) != NULL)
  {
    gchar *proc_name;
    gchar  proc_state[2] = { 0 };
    gulong proc_mem = 0;

    // prefer the executable name
    proc_name = peek_process_get_executable_name (proc->path);
    if (!proc_name)
    {
      proc_name = strndup (proc_info.cmd, 64);
    }

    snprintf (proc_state, 2, "%c", proc_info.state);

    gtk_list_store_append (store, &iter);
    gtk_list_store_set (store, &iter,
                        COLUMN_NAME,   proc_name,
                        COLUMN_ID,     proc_info.tid,
                        COLUMN_USER,   proc_info.ruser,
                        COLUMN_MEMORY, proc_mem,
                        COLUMN_PPID,   proc_info.ppid,
                        COLUMN_STATE,  proc_state,
                        -1);

    free (proc_name);
  }

  closeproc (proc);

  return GTK_TREE_MODEL (store);
}