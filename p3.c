/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#include "p3.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdlib.h>

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

int cmd_rederr(char **tr)
{
    // TODO(roi) buscar info
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

int cmd_mostrarVar(char **tr, char *envp[], char **environ)
{
    if (tr[1] == NULL)
    {
        aux_showEnv(envp, "main arg3");
    }
    else
    {
        aux_mostrarVar(envp, "main arg3", tr[1]);
        aux_mostrarVar(environ, "environ", tr[1]);
        //aux_mostrarVar(envp, tr[1]);
    }

    return 1;
}

void aux_mostrarVar(char **env, char *env_name, char *var_name)
{
    int i = 0;
    char *token;
    char *aux;
    char *var;

    //while (env[i] != NULL)
    //{
        aux = var = env[i];
        printf("%s\n", aux);
        token = strtok(aux,"=");
        printf("t %s  v %s  e %s\n", token, aux, var);
        if (strcmp(var_name, token) == 0)
            token = strtok(NULL, "=");
            printf("%s -> %s\n", env_name, aux);

    //    ++i;
    //}
}
