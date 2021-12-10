/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#ifndef SHELL_P3_H
#define SHELL_P3_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <pwd.h>
#include <sys/wait.h>

int cmd_priority(char **tr);
/*
 * If no value is given, show the priority of process pid. If neither pid nor value is given, the priority of the
 * process executing the shell is shown. If both arguments are specified, the priority of process pid will be changed
 * to value.
 */

int cmd_entorno(char **tr, char *envp[], char **environ);
void aux_showEnv(char **env, char *env_name);
/*
 * Show the environment of the shell process. Access will be through the third argument of main.
 * 	-environ		Access will be through the external variable environ.
 * 	   -addr		Show the value of environ and the third argument of main.
 */

int cmd_mostrarvar(char **tr, char *envp[], char **environ);
int aux_buscarvar(char *var_name, char *env[]);

int cmd_cambiarvar(char **tr, char *envp[], char **environ);
int aux_cambiarvar(char *var_name, char *valor, char *env[]);

int cmd_uid(char **tr);
void aux_uid_get();
void aux_uid_set(char *login);
char *aux_username(uid_t uid);
uid_t aux_useruid(char *login);

int cmd_fork();
/*
 * Create a child process and wait for it to end.
 */

int cmd_ejec(char **tr);
/*
 * Execute, replacing the shell, the desired program prog with arguments arg1, arg2, ...
 */

int cmd_ejecpri(char **tr);

int cmd_ejecas(char **tr);

int cmd_fg(char **tr);

int cmd_fgpri(char **tr);

int cmd_fgas(char **tr);

int cmd_back(char **tr);

int cmd_backpri(char **tr);

int cmd_bgas(char **tr);

#endif
