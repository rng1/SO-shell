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

int cmd_mostrarVar(char **tr, char *envp[], char **environ);
void aux_mostrarVar(char **env, char *env_name, char *var_name);

#endif
