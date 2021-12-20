/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#include <fcntl.h>
#include "p3.h"
#include "p0.h"
#include "color.h"

char *rederr_dir = NULL;

int cmd_priority(char **tr)
{
    int which = PRIO_PROCESS;
    int ret, val;
    pid_t pid;

    if (tr[1] == NULL)
    {
        if ((pid = getpid()) < 0)
            printf(COLOR_RED "priority: getpid: %s" COLOR_RESET "\n", strerror(errno));
        else if ((ret = getpriority(which, pid)) == -1)
            printf(COLOR_RED "priority: getpriority: %s" COLOR_RESET "\n", strerror(errno));
        else
        {
            printf("%5s %3s\n", "PID", "NI");
            printf("%5d %3d\n", pid, ret);
        }
    }
    else if (tr[2] == NULL)
    {
        pid = (pid_t) strtol(tr[1], NULL, 10);
        //errno = 0;

        if ((ret = getpriority(which, pid)) == -1)
            printf(COLOR_RED "priority: getpriority: %s" COLOR_RESET "\n", strerror(errno));
        else
        {
            printf("  PID\tNI\n");
            printf("%5d\t%2d\n", pid, ret);
        }
    }
    else if (tr[2] != NULL)
    {
        pid = (pid_t) strtol(tr[1], NULL, 10);
        val = (int) strtol(tr[2], NULL, 10);

        if (setpriority(which, pid, val))
            printf(COLOR_RED "priority: setpriority: %s" COLOR_RESET "\n", strerror(errno));
    }
    else
    {
        printf(COLOR_RED "priority: invalid option" COLOR_RESET "\n");
    }

    return 1;
}

int cmd_rederr(char **tr)
{
    int fd;
    int flags = O_WRONLY | O_CREAT;

    if (tr[1] == NULL)
    {
        if (rederr_dir == NULL)
            printf("Original configuration\n");
        else
            printf("%s", rederr_dir);
    }
    else if (strcmp(tr[1], "-reset") == 0)
        dup2(STDOUT_FILENO, STDERR_FILENO);
    else
    {
        if ((fd = open(tr[1], flags, 0777)) == -1)
            printf(COLOR_RED "rederr: open: %s" COLOR_RESET "\n", strerror(errno));
        else
        {
            dup2(fd, STDERR_FILENO);
            close(fd);
            strcpy(rederr_dir, tr[1]);
        }
    }

    return 1;
}

int cmd_entorno(char **tr, char *envp[], char **environ)
{
    if (tr[1] == NULL)
    {
        aux_showEnv(envp, "main arg3");
    }
    else if (strcmp(tr[1], "-environ") == 0)
    {
        aux_showEnv(environ, "environ");
    }
    else
    {
        printf(COLOR_RED "entorno: invalid input" COLOR_RESET "\n");
    }

    return 1;
}

void aux_showEnv(char **env, char *env_name)
{
    int i = 0;

    while (env[i] != NULL)
    {
        printf("%p->%s[%d]=(%p) %s\n", &env[i], env_name, i, env[i], env[i]);
        ++i;
    }
}

int cmd_mostrarvar(char **tr, char *envp[], char **environ)
{
    int pos;

    if (tr[1] == NULL)
    {
        aux_showEnv(envp, "main arg3");
    }
    else
    {
        // If the first variable is found, it doesn't need further checking.
        if ((pos = aux_buscarvar(tr[1], envp)) == -1)
        {
            printf(COLOR_RED "mostrarvar: %s" COLOR_RESET "\n", strerror(errno));
        }
        else
        {
            printf("main arg3 %s=(%p) @%p\n", envp[pos], envp[pos], &envp[pos]);
            // environ
            pos = aux_buscarvar(tr[1], environ);
            printf("environ %s=(%p) @%p\n", environ[pos], environ[pos], &environ[pos]);
            // getenv()
            printf("getenv %s(%p)\n", getenv(tr[1]), getenv(tr[1]));
        }
    }

    return 1;
}

int aux_buscarvar(char *var_name, char *env[])
{
    int pos = 0;
    char aux[4092];

    strcpy(aux, var_name);
    strcat(aux, "=");

    while (env[pos] != NULL)
    {
        if (strncmp(env[pos], aux, strlen(aux)) == 0)
            return pos;
        else
            ++pos;
    }

    errno = ENOENT;
    return -1;
}

int cmd_cambiarvar(char **tr, char *envp[], char **environ)
{
    int aux = 0;
    char buff[1024];

    if (tr[1] == NULL || tr[2] == NULL || tr[3] == NULL)
    {
        printf(COLOR_RED "cambiarvar: invalid option" COLOR_RESET "\n");
    }
    else
    {
        if (strcmp(tr[1], "-a") == 0)
            aux = aux_cambiarvar(tr[2], tr[3], envp);
        else if (strcmp(tr[1], "-e") == 0)
            aux = aux_cambiarvar(tr[2], tr[3], environ);
        else if (strcmp(tr[1], "-p") == 0)
        {
            sprintf(buff,"%s=%s",tr[2],tr[3]);
            putenv(buff);
        }

        if (aux == -1)
            printf("cambiarvar:");
        else
            printf(COLOR_RED "cambiarvar: invalid option" COLOR_RESET "\n");

    }

    return 1;
}

int aux_cambiarvar(char *var_name, char *valor, char *env[])
{
    int pos;
    char *aux;

    if ((pos = aux_buscarvar(var_name, env)) == -1)
        return -1;

    if ((aux = (char *)malloc(strlen(var_name) + strlen(valor) + 2)) == NULL)
        return -1;

    strcpy(aux, var_name);
    strcat(aux, "=");
    strcat(aux, valor);
    env[pos] = aux;

    return pos;
}

int cmd_uid(char **tr)
{
    uid_t uid;

    if (tr[1] == NULL || strcmp(tr[1], "-get") == 0)
    {
        aux_uid_get();
    }
    else if (strcmp(tr[1], "-set") == 0)
    {
        if (tr[2] == NULL)
            aux_uid_get();
        else if (strcmp(tr[2], "-l") == 0)
            aux_uid_set(tr[3]);
        else
        {
            uid = (uid_t) strtol(tr[3], NULL, 10);
            setuid(uid);
        }
    }
    else
        printf(COLOR_RED "uid: invalid option" COLOR_RESET "\n");

    return 1;
}

void aux_uid_get()
{
    uid_t real = getuid();
    uid_t effective = geteuid();

    printf("real user ID: %d (%s)\n", real, aux_username(real));
    printf("effective user ID: %d (%s)\n", effective, aux_username(effective));
}

void aux_uid_set(char *login)
{
    uid_t uid;

    if ((uid = aux_useruid(login)) == (uid_t) -1)
    {
        printf(COLOR_RED "uid set: invalid credentials" COLOR_RESET "\n");
        return;
    }

    if (setuid(uid) == -1)
        printf(COLOR_RED "uid set: %s" COLOR_RESET "\n", strerror(errno));
}

char *aux_username(uid_t uid)
{
    struct passwd *p;

    if ((p = getpwuid(uid)) == NULL)
        return " ??????";

    return p->pw_name;
}

uid_t aux_useruid(char *login)
{
    struct passwd *p;

    if ((p = getpwnam(login)) == NULL)
        return (uid_t) -1;

    return p->pw_uid;
}

int cmd_fork()
{
    pid_t pid, wpid;

    if ((pid = fork()) == 0)
    {
        if ((wpid = getpid()) < 0)
            printf(COLOR_RED "fork: getpid: %s" COLOR_RESET "\n", strerror(errno));
        else
            printf("forking process %d\n", wpid);
    }
    else if (pid < 0)
        printf(COLOR_RED "fork: %s" COLOR_RESET "\n", strerror(errno));
    else
        waitpid(pid, NULL, 0);

    return 1;
}

int cmd_ejec(char **tr)
{
    if (execvp(tr[1], tr + 1) == -1)
            printf(COLOR_RED "ejec: %s" COLOR_RESET "\n", strerror(errno));

    return 1;
}

int cmd_ejecpri(char **tr)
{
    pid_t pid = getpid();
    int prio;
    int which = PRIO_PROCESS;

    if (tr[1] == NULL || tr[2] == NULL)
    {
        printf(COLOR_RED "fgpri: missing arguments" COLOR_RESET "\n");
    }
    else if ((prio = (int) strtol(tr[1], NULL, 10)) == 0)
    {
        printf(COLOR_RED "fgpri: invalid priority" COLOR_RESET "\n");
    }
    else
    {
        if (setpriority(which, pid, prio))
            printf(COLOR_RED "ejecpri: setpriority: %s" COLOR_RESET "\n", strerror(errno));
        else if (execvp(tr[2], tr + 2) == -1)
            printf(COLOR_RED "ejecpri: execvp: %s" COLOR_RESET "\n", strerror(errno));
    }

    return 1;
}

int cmd_ejecas(char **tr)
{
    aux_uid_set(tr[1]);

    if (execvp(tr[1], tr + 1) == -1)
        printf(COLOR_RED "ejec: %s" COLOR_RESET "\n", strerror(errno));

    return 1;
}

int cmd_fg(char **tr)
{
    pid_t pid;
    int start;

    if (strcmp(tr[0], "fg") == 0)
        start = 1;
    else
        start = 0;

    if ((pid = fork()) == 0)
    {
        if (execvp(tr[start], tr + start) == -1)
            printf(COLOR_RED "fg: execvp: %s" COLOR_RESET "\n", strerror(errno));
        exit(0);
    }
    else if (pid < 0)
        printf(COLOR_RED "fg: %s" COLOR_RESET "\n", strerror(errno));
    else
        waitpid(pid, NULL, 0);

    return 1;
}

int cmd_fgpri(char **tr)
{
    pid_t pid;
    int prio;
    int which = PRIO_PROCESS;

    if (tr[1] == NULL || tr[2] == NULL)
    {
        printf(COLOR_RED "fgpri: missing arguments" COLOR_RESET "\n");
    }
    else if ((prio = (int) strtol(tr[1], NULL, 10)) == 0)
    {
        printf(COLOR_RED "fgpri: invalid priority" COLOR_RESET "\n");
    }
    else if ((pid = fork()) == 0)
    {
        if (setpriority(which, pid, prio))
            printf(COLOR_RED "fgpri: setpriority: %s" COLOR_RESET "\n", strerror(errno));
        else
            if (execvp(tr[2], tr + 2) == -1)
                printf(COLOR_RED "fgpri: execvp: %s" COLOR_RESET "\n", strerror(errno));
        exit(0);
    }
    else if (pid < 0)
    {
        printf(COLOR_RED "fgpri: %s" COLOR_RESET "\n", strerror(errno));
    }
    else
    {
        waitpid(pid, NULL, 0);
    }
    return 1;
}

int cmd_fgas(char **tr)
{
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        aux_uid_set(tr[1]);

        if (execvp(tr[1], tr + 1) == -1)
            printf(COLOR_RED "fg: execvp: %s" COLOR_RESET "\n", strerror(errno));
        exit(0);
    }
    else if (pid < 0)
    {
        printf(COLOR_RED "fg: %s" COLOR_RESET "\n", strerror(errno));
    }
    else
    {
        waitpid(pid, NULL, 0);
    }

    return 1;
}

int cmd_back(char **tr, tJobList *jobList)
{
    pid_t pid;
    int start;

    if (strcmp(tr[0], "back") == 0)
        start = 1;
    else
        start = 0;

    if ((pid = fork()) == 0)
    {
        // Placing the child in a new process group to keep it away from foreground processes.
        setpgid(0, 0);

        if (execvp(tr[start], tr + start) == -1)
            printf(COLOR_RED "back: execvp: %s" COLOR_RESET "\n", strerror(errno));
        exit(0);
    }
    else if (pid < 0)
        printf(COLOR_RED "back: %s" COLOR_RESET "\n", strerror(errno));
    else
        aux_addJobList(pid, tr + start, jobList);

    return 1;
}

int cmd_backpri(char **tr, tJobList *jobList)
{
    pid_t pid;
    int prio;
    int which = PRIO_PROCESS;

    if (tr[1] == NULL || tr[2] == NULL)
        printf(COLOR_RED "backpri: missing arguments" COLOR_RESET "\n");
    else if ((prio = (int) strtol(tr[1], NULL, 10)) == 0)
        printf(COLOR_RED "backpri: invalid priority" COLOR_RESET "\n");
    else if ((pid = fork()) == 0)
    {
        setpgid(0, 0);
        if (setpriority(which, pid, prio))
            printf(COLOR_RED "backpri: setpriority: %s" COLOR_RESET "\n", strerror(errno));
        else if (execvp(tr[2], tr + 2) == -1)
            printf(COLOR_RED "backpri: execvp: %s" COLOR_RESET "\n", strerror(errno));
        exit(0);
    }
    else if (pid < 0)
        printf(COLOR_RED "backpri: %s" COLOR_RESET "\n", strerror(errno));
    else
        aux_addJobList(pid, tr + 2, jobList);

    return 1;
}

int cmd_bgas(char **tr, tJobList *jobList)
{
    pid_t pid;

    if ((pid = fork()) == 0)
    {
        // Placing the child in a new process group to keep it away from foreground processes.
        setpgid(0, 0);
        aux_uid_set(tr[1]);

        if (execvp(tr[1], tr + 1) == -1)
            printf(COLOR_RED "back: execvp: %s" COLOR_RESET "\n", strerror(errno));
        exit(0);
    }
    else if (pid < 0)
        printf(COLOR_RED "back: %s" COLOR_RESET "\n", strerror(errno));
    else
        aux_addJobList(pid, tr + 1, jobList);

    return 1;
}

int cmd_listjobs(tJobList *jobList)
{
    tJobPosL pos;
    tJobItemL aux;

    if (isEmptyJobList(*jobList) == true)
        printf(COLOR_RED "listjobs: empty list" COLOR_RESET "\n");
    else
    {
        printf("%5s %-12s %3s %3s %-19s %9s %-9s %s\n",
               "PID", "USER", "PRI", "NI", "TIME", "STATUS", "SIGNAL", "Command");
        pos = firstJob(*jobList);

        do
        {
            aux = getJobItem(pos);
            aux_jobListPrint(aux);
            pos = nextJob(pos);
        }
        while (pos != NULL);
    }

    return 1;
}

int cmd_job(char **tr, tJobList *jobList)
{
    pid_t pid;
    tJobPosL pos;

    if (tr[1] == NULL || tr[2] == NULL)
        cmd_listjobs(jobList);
    else if (strcmp(tr[1], "-fg") == 0)
        printf("job -fg\n");
    else
    {
        pid = (pid_t) strtol(tr[2], NULL, 10);
        if ((pos = findJobItem(pid, *jobList)) == NULL)
            printf(COLOR_RED "job: %s" COLOR_RESET "\n", strerror(errno));
        else
            aux_jobListPrint(getJobItem(pos));
    }

    return 1;
}

int cmd_borrarjobs(char **tr, tJobList *jobList)
{
    char *sig  = "SIGNALED";
    char *term = "EXITED";

    if (tr[1] == NULL)
        printf(COLOR_RED "borrarjobs: missing arguments" COLOR_RESET "\n");
    else if (strcmp(tr[1], "-clear") == 0)
        clearJobList(jobList);
    else if (strcmp(tr[1], "-sig") == 0)
        aux_deleteJobList(sig, jobList);
    else if (strcmp(tr[1], "-term") == 0)
        aux_deleteJobList(term, jobList);
    else if (strcmp(tr[1], "-all") == 0)
    {
        aux_deleteJobList(sig, jobList);
        aux_deleteJobList(term, jobList);
    }
    else
        printf(COLOR_RED "borrarjobs: invalid input" COLOR_RESET "\n");

    return 1;
}

void aux_deleteJobList(char *status, tJobList *jobList)
{
    tJobPosL pos;
    tJobItemL aux;

    pos = firstJob(*jobList);

    do
    {
        aux = getJobItem(pos);
        printf("eliminando %d status %s", aux.pid, aux.status);
        if (strcmp(status, aux.status) == 0) {
            deleteJobAtPosition(pos, jobList);
        }
        pos = nextJob(pos);
    }
    while (pos != NULL);
}

void aux_jobListPrint(tJobItemL item)
{
    struct tm *tm_item;
    int which = PRIO_PROCESS;
    int ni, wstatus;

    if ((ni = getpriority(which, item.pid)) == -1)
        printf(COLOR_RED "priority: getpriority: %s" COLOR_RESET "\n", strerror(errno));

    if (waitpid(item.pid, &wstatus, WNOHANG | WUNTRACED | WCONTINUED) == item.pid)
    {
        if (WIFSIGNALED(wstatus))
        {
            updateJobItem(&item, "SIGNALED");
            item.val = WTERMSIG(wstatus);
        }
        else if (WIFEXITED(wstatus))
        {
            updateJobItem(&item, "EXITED");
            item.val = WEXITSTATUS(wstatus);
        }
        else if (WIFCONTINUED(wstatus))
        {
            updateJobItem(&item, "CONTINUED");
            item.val = 0;
        }
        else if (WIFSTOPPED(wstatus))
        {
            updateJobItem(&item, "STOPPED");
            item.val = WSTOPSIG(wstatus);
        }
    }

    time(&item.time);
    tm_item = localtime(&item.time);

    printf("%5d %-12s %3d %3d %d/%02d/%02d %02d:%02d:%02d %9s ",
           item.pid, item.user, 20 + ni, ni, tm_item->tm_year + 1900, tm_item->tm_mon + 1, tm_item->tm_mday,
           tm_item->tm_hour, tm_item->tm_min, tm_item->tm_sec, item.status);

    if (strcmp(item.status, "SIGNALED") == 0)
        printf("(%-6s) ", aux_sigName(item.val));
    else
        printf("(%-6d) ", item.val);

    printf(" %-s\n", item.command);
}

void aux_addJobList(pid_t pid, char **tr, tJobList *jobList)
{
    tJobItemL item;
    int i;

    item.pid = pid;
    item.time = time(NULL);
    strcpy(item.user, getpwuid(getuid())->pw_name);
    strcpy(item.command, tr[0]);
    strcpy(item.status, "ACTIVATED");
    item.val = 0;

    for (i = 1;  tr[i] != NULL; ++i)
    {
        strcat(item.command, " ");
        strcat(item.command, tr[i]);
    }

    insertJobItem(item, jobList);
}

char *aux_sigName(int sig)
{
    int i;

    for (i = 0; sigstrnum[i].name != NULL; ++i)
        if (sig == sigstrnum[i].sig)
            return sigstrnum[i].name;

    return "SIGUNKNOWN";
}