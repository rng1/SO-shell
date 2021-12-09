/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#include "p3.h"

#define COLOR_RED   "\x1b[31m"
#define COLOR_RESET "\x1b[0m"

int cmd_priority(char **tr)
{
    int which = PRIO_PROCESS;
    int ret, val;
    pid_t pid;

    if (tr[1] == NULL)
    {
        if ((pid = getpid()) < 0)
            printf(COLOR_RED "priority: getpid: %s" COLOR_RESET "\n", strerror(errno));
        else
            printf("%d\n", pid);
    }
    else if (tr[2] == NULL)
    {
        pid = (pid_t) strtol(tr[1], NULL, 10);
        //errno = 0;

        if ((ret = getpriority(which, pid)) == -1)
            printf(COLOR_RED "priority: getpriority: %s" COLOR_RESET "\n", strerror(errno));
        else
        {
            printf(" PID\tNI\n");
            printf("%4d\t%2d\n", pid, ret);
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

/*int cmd_rederr(char **tr)
{
    // TODO(roi) buscar info
}*/

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

    if (tr[1] == NULL || tr[2] == NULL || tr[3] == NULL)
    {
        printf(COLOR_RED "cambiarvar: invalid option" COLOR_RESET "\n");
    }
    else
    {
        if (strcmp(tr[1], "-a") == 0)
        {
            aux = aux_cambiarvar(tr[2], tr[3], envp);
        }
        else if (strcmp(tr[1], "-e") == 0)
        {
            aux = aux_cambiarvar(tr[2], tr[3], environ);
        }
        else if (strcmp(tr[1], "-p") == 0)
        {
            printf("putenv\n");
        }

        if (aux == -1)
        {
            printf("cambiarvar:");
        }
        else
        {
            printf(COLOR_RED "cambiarvar: invalid option" COLOR_RESET "\n");
        }

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

    if (setuid(uid) == 1) // TODO(roi) comprobar si es 1 o .1
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

int cmd_fork(char **tr)
{

}