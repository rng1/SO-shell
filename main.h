#ifndef SHELL_MAIN_H
#define SHELL_MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLOR_RED   "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLUE  "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

#include "mem_list.h"
#include "job_list.h"

void loop(char *history[], tMemList *memList, tJobList *jobList, char *envp[], char **environ);
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

int process_cmd(char **tr, char *history[], tMemList *memList, tJobList *jobList, char *envp[], char **environ);
/*
 * Process the first word of the given array looking for a valid command.
 */

bool aux_isBg(char **tr);

#endif
