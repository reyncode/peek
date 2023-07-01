#include "peek-application.h"
#include "peek-process.h"
#include "peek-tree-model.h"

#include <glib.h>
#include <glibtop.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

/* read from /proc/[pid] for process information */

static ProcData *
proc_data_new (pid_t pid)
{
  ProcData *proc_data; 
  glibtop_proc_state pstate;
  glibtop_proc_uid puid;

  proc_data = g_malloc0 (sizeof (ProcData));
  
  g_assert (proc_data != NULL);

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

  g_assert (key != NULL);

  *key = pid;

  return key;
}

static gboolean
pid_is_still_valid (pid_t    key,
                    pid_t   *pids,
                    guint64  pid_count)
{
  // for the keys we hold, check the array of pids to verify its existence

  for (int i = 0; i < pid_count; i++)
  {
    if (key == pids[i])
      return TRUE;
  }

  return FALSE;
}

static void
update_proc_list (PeekApplication *app,
                  pid_t           *pids,
                  guint64          pid_count)
{
  GHashTable   *proc_table;
  GtkTreeModel *model;
  GtkListStore *store;
  ProcData     *proc_data;
  
  proc_table = peek_application_get_proc_table (app);
  model = peek_application_get_model (app);
  store = GTK_LIST_STORE (model);

  // Inserting & Updating
  for (int i = 0; i < pid_count; i++)
  {
    proc_data = g_hash_table_lookup (proc_table,  &pids[i]);

    if (!proc_data) // INSERT
    {
      pid_t *key = key_new (pids[i]);
      ProcData *proc_data = proc_data_new (pids[i]);

      g_hash_table_insert (proc_table, key, proc_data);

      // tree insertion
      gtk_list_store_append (store, &proc_data->iter);
      gtk_list_store_set (store, &proc_data->iter,
                          COLUMN_NAME,   proc_data->name,
                          COLUMN_ID,     proc_data->pid,
                          COLUMN_USER,   "User",
                          COLUMN_MEMORY, 0,
                          COLUMN_PPID,   proc_data->ppid,
                          COLUMN_STATE,  "X",
                          -1);
    }
    else // UPDATE
    {
      // do the update
    }
  }

  // Removal
  GList *keys = g_hash_table_get_keys (proc_table);
  GList *tmp = g_list_first (keys);

  // some sort algo for keys

  while (tmp->next != NULL)
  {
    pid_t key = *(pid_t *) tmp->data;

    if (!pid_is_still_valid (key, pids, pid_count))
    {
      ProcData *proc_data = g_hash_table_lookup (proc_table, &key);

      // tree removal
      gtk_list_store_remove (store, &proc_data->iter);

      g_hash_table_remove (proc_table, &key);
    }

    tmp = tmp->next;
  }

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
  guint64 which = GLIBTOP_KERN_PROC_ALL;
  guint64 arg = 0;

  pids = glibtop_get_proclist (&proclist, which, arg);

  g_assert (pids != NULL);

  update_proc_list (app, pids, proclist.number);

  return G_SOURCE_CONTINUE;
}