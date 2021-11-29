/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#include "main.h"
#include "p0.h"
#include "p1.h"
#include "p2.h"
#include "p3.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define COLOR_RED   "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_BLUE  "\x1b[34m"
#define COLOR_RESET "\x1b[0m"

#define DELIM " \t\n"
#define ARRAY 4096
#define BUFFERSIZE 1024

int main(int argc, char *argv[], char *envp[])
{
	extern char **environ;

    // Initialize the history array which is going to be used all along the program.
    char *history[ARRAY];
    tList memList;

    createEmptyList(&memList);
    history[0] = NULL;
    loop(history, &memList, envp, environ);

    return 1;
}

void loop(char *history[], tList *memList, char *envp[], char **environ)
{
    char *cmd = NULL;
    char **args = NULL;
    int status;

    // Infinite loop; all functions return 1 except "cmd_exit", which returns 0 and effectively ends the shell.
    do
    {
        printf("$ ");
        cmd = read_cmd();
        save_cmd(cmd, history);
        args = split_cmd(cmd);
        status = process_cmd(args, history, memList, envp, environ);

        free(cmd);
        free(args);
    }
    while (status);
}


void save_cmd(char *cmd, char *history[])
{
    int i;
    char* line = strtok(cmd,"\n");
    if (line != NULL) // Check if only input is ENTER.
    {
        // Find the end of the history and insert the inputted command in it
        // and erase the earlier commands if the end is reached.
        for (i = 0; (history[i] != NULL) && (i < ARRAY); i++);
        if(i == ARRAY)
        {
            for (i = 0; i < ARRAY - 1; i++)
                history[i] = history[i+1];
            i++;
            history[i] = strdup(line);
        }
        else
        {
            history[i] = strdup(line);
            history[i + 1] = NULL;
        }
    }
}

char *read_cmd()
{
    char *cmd = NULL;
    size_t len = 0;

    if (getline(&cmd, &len, stdin) == -1)
    {
        if (feof(stdin))
            exit(EXIT_SUCCESS);
        else
            printf(COLOR_RED "read: getline: %s" COLOR_RESET "\n", strerror(errno));
    }

    return cmd;
}

char **split_cmd(char *cmd)
{
    int max_len = 50, pos = 0;
    char *piece = strtok(cmd, DELIM);
    char **tokens = malloc(sizeof(char*) * max_len);

    if (!tokens)
        printf(COLOR_RED "split: tokens_1: %s" COLOR_RESET "\n", strerror(errno));
    else
    {
        while (piece != NULL)
        {
            tokens[pos++] = piece;
            piece = strtok(NULL, DELIM);

            // If limit is exceded, allocate more space.
            if (pos >= max_len)
            {
                max_len += max_len;
                tokens = realloc(tokens, sizeof(char *) * max_len);

                if (!tokens)
                    printf(COLOR_RED "split: tokens_2: %s" COLOR_RESET "\n", strerror(errno));
            }
        }
    }

    tokens[pos] = NULL;
    return tokens;
}

int process_cmd(char **tr, char *history[], tList *memList, char *envp[], char **environ)
{
    if (tr[0] != NULL)
    {
        if (strcmp(tr[0], "autores") == 0)
            return cmd_autores(tr);
        else if (strcmp(tr[0], "pid") == 0)
            return cmd_pid(tr);
        else if (strcmp(tr[0], "carpeta") == 0)
            return cmd_carpeta(tr);
        else if (strcmp(tr[0], "fecha") == 0)
            return cmd_fecha(tr);
        else if (strcmp(tr[0], "hist") == 0)
            return cmd_hist(tr, history);
        else if (strcmp(tr[0], "comando") == 0)
            return cmd_comando(tr, history, memList, envp, environ);
        else if (strcmp(tr[0], "infosis") == 0)
            return cmd_infosis();
        else if (strcmp(tr[0], "ayuda") == 0)
            return cmd_ayuda(tr);
        else if (strcmp(tr[0], "fin") == 0 || strcmp(tr[0], "salir") == 0 ||
                 strcmp(tr[0], "bye") == 0 || strcmp(tr[0], "exit") == 0)
            return cmd_exit(history, memList);
        else if (strcmp(tr[0], "crear") == 0)
            return cmd_crear(tr);
        else if (strcmp(tr[0], "borrar") == 0)
            return cmd_borrar(tr);
        else if (strcmp(tr[0], "borrarrec") == 0)
            return cmd_borrarRec(tr);
        else if (strcmp(tr[0], "listfich") == 0)
            return cmd_listFich(tr);
        else if (strcmp(tr[0], "listdir") == 0)
            return cmd_listDir(tr);
        else if (strcmp(tr[0], "malloc") == 0)
            return cmd_malloc(tr, memList);
        else if (strcmp(tr[0], "mmap") == 0)
            return cmd_mmap(tr, memList);
        else if (strcmp(tr[0], "shared") == 0)
            return cmd_shared(tr, memList);
        else if (strcmp(tr[0], "dealloc") == 0)
            return cmd_dealloc(tr, memList);
        else if (strcmp(tr[0], "memoria") == 0)
            return cmd_memoria(tr, memList);
        else if (strcmp(tr[0], "volcarmem") == 0)
            return cmd_volcarmem(tr);
        else if (strcmp(tr[0], "llenarmem") == 0)
            return cmd_llenarmem(tr);
        else if (strcmp(tr[0], "recursiva") == 0)
            return cmd_recursiva(tr);
        else if (strcmp(tr[0], "e-s") == 0)
            return cmd_es(tr);
        else if (strcmp(tr[0], "priority") == 0)
            return cmd_priority(tr);
		else if (strcmp(tr[0], "entorno") == 0)
			return cmd_entorno(tr, envp, environ);
        else
            printf(COLOR_RED "%s: command not found" COLOR_RESET "\n", tr[0]);
    }

    return 1;
}
