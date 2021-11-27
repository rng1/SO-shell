/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#ifndef SHELL_P0_H
#define SHELL_P0_H

#include "mem_list.h"

int cmd_autores(char **tr);
/*
 * Print the names and logins of the program authors.
 *  -l      print only the logins.
 *  -n      print only the names.
*/

int cmd_pid(char **tr);
/*
 * Prints the pid of the process running the shell.
 *  -p      prints the pid of the shell's parent process.
 */

int cmd_carpeta(char **tr);
/*
 * Prints the current working directory.
 *  DIRECT      changes the current working directory to DIRECT.
 */

int cmd_fecha(char **tr);
/*
 * Print the current date and time.
 *  -d  print the current date in format dd/mm/yyyy.
 *  -h  print the current time in format hh:mm:ss.
 */

int cmd_hist(char **tr, char* history[]);
/*
 * Shows the history of commands executed by the shell.
 *  -c  clears the history.
 *  -N  prints the first N commands.
 */

int cmd_comando(char **tr, char *history[], tList *memList);
/*
 * Repeats command number N.
 */

int cmd_infosis();
/*
 * Prints the info on the machine running the shell using uname library.
 */

int cmd_ayuda(char **tr);
/*
 * Displays a list of available commands.
 *  ayuda [CMD]   gives a brief help on the usage of the given CMD.
 */

int cmd_exit(char* history[], tList *memList);
/*
 * End the shell after freeing all the allocated pointers.
 */

#endif
