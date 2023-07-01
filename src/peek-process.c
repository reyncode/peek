#include "peek-application.h"
#include "peek-process.h"
#include "peek-tree-model.h"

// to be phased out
#include <stdlib.h>
#include <string.h>
#include <proc/readproc.h>

#include <glib.h>
#include <glibtop.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

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

void
peek_process_populate_model (GtkListStore *store)
{
  g_return_if_fail (GTK_IS_LIST_STORE (store));

  GtkTreeIter iter;

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

    // append the proc data to a new node in the linked list

    // append the data row by row
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
}


static ProcData *
proc_data_new (pid_t pid)
{
  ProcData *proc_data; 
  glibtop_proc_state pstate;
  glibtop_proc_uid puid;

  proc_data = g_malloc0 (sizeof (ProcData));
  g_return_val_if_fail (proc_data != NULL, NULL);

  glibtop_get_proc_state (&pstate, pid);
  glibtop_get_proc_uid (&puid, pid);

  proc_data->pid = pid;
  proc_data->ppid = puid.ppid;
  proc_data->name = g_strdup (pstate.cmd);

  return proc_data;
}

static pid_t *
key_new (pid_t pid)
{
  pid_t *key = g_new0 (pid_t, 1);
  *key = pid;

  return key;
}

static void
for_each (gpointer key,
          gpointer value,
          gpointer data)
{
  ProcData *proc_data = (ProcData *) value;

  g_print ("key: %d\n", *(pid_t *) key);
  g_print ("value:\nProcData-PID: %d\nProcData-PPID: %d\nProcData-Name: %s\n\n",
           proc_data->pid, proc_data->ppid, proc_data->name);
}

static void
update_proc_list (PeekApplication *app,
                  pid_t           *pids,
                  guint64          pid_count)
{
  GHashTable *proc_table;
  ProcData   *proc_data;
  
  proc_table = peek_application_get_proc_table (app);

  for (int i = 0; i < pid_count; i++)
  {
    proc_data = g_hash_table_lookup (proc_table,  &pids[i]);

    if (!proc_data) // INSERT
    {
      pid_t *key = key_new (pids[i]);
      ProcData *proc_data = proc_data_new (pids[i]);

      g_hash_table_insert (proc_table, key, proc_data);

      // insert into tree

    }
    else // UPDATE
    {
      // do the update
    }
  }

  g_hash_table_foreach (proc_table, for_each, NULL);

  g_free (pids);
}

gboolean
peek_process_updater (gpointer data)
{
  g_return_val_if_fail (PEEK_IS_APPLICATION (data), G_SOURCE_REMOVE);

  PeekApplication *app;
  app = PEEK_APPLICATION (data);

  pid_t *pids;
  glibtop_proclist proclist;
  guint64 which = GLIBTOP_KERN_PROC_UID;
  guint64 arg = 0;

  pids = glibtop_get_proclist (&proclist, which, arg);
  g_return_val_if_fail (pids != NULL, G_SOURCE_REMOVE);

  update_proc_list (app, pids, proclist.number);

  // return G_SOURCE_CONTINUE;
  return G_SOURCE_REMOVE;
}

/*
  *GET*

  get a list of all the pids [yes]
  
  *FIND*

  compare that list against what the app table has
  g_hash_table_lookup (...) / g_hash_table_find (...)

  *ACTION*

  ADD: if pid is not in app table, insert into app table, insert into tree model
  proc_data_new (...)
  g_hash_table_insert (...)

  REMOVE: if pid is in app table but not in pid list, remove from app table, remove from tree model
  g_hash_table_remove (...)

  NOTHING: if pid found in pid list and pid found in app table

*/