#include "peek-application.h"
#include "peek-process.h"
#include "peek-tree-model.h"

#include <glib.h>
#include <glibtop.h>
#include <glibtop/proclist.h>
#include <glibtop/procstate.h>
#include <glibtop/proctime.h>
#include <glibtop/procuid.h>
#include <glibtop/procmem.h>

#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>

#include <pwd.h>

static const gchar *parse_user_from_uid (guint32 uid);
static const gchar *parse_proc_state (guint state);

/* read from /proc/[pid] for process information */

static void
proc_data_update (ProcData *proc_data)
{
  glibtop_proc_state pstate;
  glibtop_proc_uid puid;
  glibtop_proc_mem pmem;
  
  glibtop_get_proc_state (&pstate, proc_data->pid);
  glibtop_get_proc_uid (&puid, proc_data->pid);
  glibtop_get_proc_mem (&pmem, proc_data->pid);

  proc_data->uid = puid.uid;
  proc_data->state = pstate.state;

  proc_data->memory_vsize = pmem.vsize;
  proc_data->memory_shared = pmem.share;
  proc_data->memory_resident = pmem.resident;
  proc_data->memory = pmem.resident - pmem.share;
}

static ProcData *
proc_data_new (pid_t pid)
{
  ProcData *proc_data; 
  glibtop_proc_state pstate;
  glibtop_proc_uid puid;
  glibtop_proc_mem pmem;

  proc_data = g_malloc0 (sizeof (ProcData));
  
  g_assert (proc_data != NULL);

  glibtop_get_proc_state (&pstate, pid);
  glibtop_get_proc_uid (&puid, pid);
  glibtop_get_proc_mem (&pmem, pid);

  proc_data->pid = pid;
  proc_data->ppid = puid.ppid;
  proc_data->uid = puid.uid;

  proc_data->name = g_strdup (pstate.cmd);
  proc_data->state = pstate.state;

  proc_data->memory_vsize = pmem.vsize;
  proc_data->memory_shared = pmem.share;
  proc_data->memory_resident = pmem.resident;
  proc_data->memory = pmem.resident - pmem.share;

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

static const gchar *
parse_proc_state (guint state)
{
  const gchar *status;

  switch (state)
  {
    case GLIBTOP_PROCESS_RUNNING:
      status = "Running";
      break;
    case GLIBTOP_PROCESS_UNINTERRUPTIBLE:
      status = "Uninterruptible";
      break;
    case GLIBTOP_PROCESS_ZOMBIE:
      status = "Zombie";
      break;
    case GLIBTOP_PROCESS_STOPPED:
      status = "Stopped";
      break;
    default:
      status = "Sleeping";
      break;
  }

  return status;
}

static const gchar *
parse_user_from_uid (guint32 uid)
{
  const gchar *user;
  struct passwd *pwd;

  pwd = getpwuid (uid);

  if (pwd && pwd->pw_name)
  {
    user = pwd->pw_name;
  }
  else
  {
    gchar buf[5];
    g_snprintf (buf, 5, "%u", uid);
    user = buf;
  }

  return user;
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
  GtkTreeModel *parent; // GtkTreeModelSort
  GtkTreeModel *child;  // GtkListStore
  ProcData     *proc_data;
  
  proc_table = peek_application_get_proc_table (app);
  parent = peek_application_get_model (app);

  // make a call to get child inside filter layer wrapper
  child = gtk_tree_model_filter_get_model (GTK_TREE_MODEL_FILTER (gtk_tree_model_sort_get_model (GTK_TREE_MODEL_SORT (parent))));

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
      gtk_list_store_append (GTK_LIST_STORE (child), &proc_data->iter);
      gtk_list_store_set (GTK_LIST_STORE (child), &proc_data->iter,
                          COLUMN_NAME,   proc_data->name,
                          COLUMN_ID,     proc_data->pid,
                          COLUMN_USER,   parse_user_from_uid (proc_data->uid),
                          COLUMN_MEMORY, proc_data->memory,
                          COLUMN_PPID,   proc_data->ppid,
                          COLUMN_STATE,  parse_proc_state (proc_data->state),
                          -1);
    }
    else // UPDATE
    {
      proc_data_update (proc_data);

      gtk_list_store_set (GTK_LIST_STORE (child), &proc_data->iter,
                          COLUMN_USER,   parse_user_from_uid (proc_data->uid),
                          COLUMN_MEMORY, proc_data->memory,
                          COLUMN_STATE,  parse_proc_state (proc_data->state),
                          -1);
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
      gtk_list_store_remove (GTK_LIST_STORE (child), &proc_data->iter);

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