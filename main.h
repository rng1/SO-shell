#ifndef SHELL_MAIN_H
#define SHELL_MAIN_H

#include "mem_list.h"

void loop(char *history[], tList *memList, char *envp[], char **environ);
/*
 * Main loop.
 */

void save_cmd(char *cmd, char *history[]);
/*
 * Save command to history.
 */

char *read_cmd();
/*
 * Read command.
 */

char **split_cmd(char *cmd);
/*
 * Split string into pieces.
 */

int process_cmd(char **tr, char *history[], tList *memList, char *envp[], char **environ);
/*
 * Process the first word of the given array looking for a valid command.
 */

#endif
