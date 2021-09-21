#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <sys/time.h>
#include <sys/utsname.h>

#define DELIM " \t\n"
#define ARRAY 4096

int cmd_authors(char **tr);
int cmd_pid(char **tr);
int cmd_carpeta(char **tr);
int cmd_date(char **tr);
int cmd_infosis();
int cmd_ayuda(char **tr);
int cmd_exit();
int cmd_hist(char **tr, char* list[]);


int process(char **tr, char* list[])
{
    if (tr[0] != NULL)
    {
        if (strcmp(tr[0], "autores") == 0)
            return cmd_authors(tr);
        else if (strcmp(tr[0], "pid") == 0)
            return cmd_pid(tr);
        else if (strcmp(tr[0], "carpeta") == 0)
            return cmd_carpeta(tr);
        else if (strcmp(tr[0], "fecha") == 0)
            return cmd_date(tr);
        else if (strcmp(tr[0], "hist") == 0)
            return cmd_hist(tr,list);/*
        else if (strcmp(tr[0], "comando") == 0)
            return cmd_comando();*/
        else if (strcmp(tr[0], "infosis") == 0)
            return cmd_infosis();
        else if (strcmp(tr[0], "ayuda") == 0)
            return cmd_ayuda(tr);
        else if (strcmp(tr[0], "fin") == 0 || strcmp(tr[0], "salir") == 0 ||
                 strcmp(tr[0], "bye") == 0 || strcmp(tr[0], "exit") == 0)
            return cmd_exit();
        else
        {
            printf("%s: command not found\n", tr[0]);
            return (EXIT_FAILURE);
        }
    }
    else
    {
        return (EXIT_FAILURE);
    }
}

char **split_cmd(char *cmd)
{
    int max_len = 50, pos = 0;
    char *piece = strtok(cmd, DELIM);
    char **tokens = malloc(sizeof(char*) * max_len);

    if (!tokens) {
        perror("Error: allocation error\n");
        exit(EXIT_FAILURE);
    }

    while (piece != NULL)
    {
        tokens[pos++] = piece;
        piece = strtok(NULL, DELIM);

        if (pos >= max_len)
        {
            max_len += max_len;
            tokens = realloc(tokens, sizeof(char*) * max_len);

            if (!tokens) {
                perror("Error: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    tokens[pos] = NULL;
    return tokens;
}

/*char *read_cmd()
{
    char *cmd = NULL;
    size_t len = 0;

    if (getline(&cmd, &len, stdin) == -1)
    {
        if (feof(stdin))
            exit(EXIT_SUCCESS);
        else {
            perror("Error read");
            exit(EXIT_FAILURE);
        }
    }

    return cmd;
}*/

char *read_cmd()
{
    char *cmd = NULL;
    size_t len = 0;

    if (getline(&cmd, &len, stdin) == -1)
    {
        if (feof(stdin))
            exit(EXIT_SUCCESS);
        else {
            perror("Error read_cmd");
            exit(EXIT_FAILURE);
        }
    }

    return cmd;
}

void save_cmd(char *cmd, char *list[])
{
    char* line = strtok(cmd,"\n");
    int i;
    for(i = 0; list[i] != NULL; i++);
    //list[i] = strdup(cmd);
    list[i] = strdup(line);

}

void loop()
{
    char *cmd;
    char **args;
    int status;

    int i = 0;
    char *list[ARRAY];

    do
    {
        printf("$ ");
        cmd = read_cmd();
        save_cmd(cmd, list);


        args = split_cmd(cmd);
        status = process(args, list);

        free(cmd);
        free(args);

        i++;
    } while (status);
}

int main()
{
    loop();
    return EXIT_SUCCESS;
}

/**COMANDOS**/


int cmd_authors(char **tr)
/*
 * Print the names and logins of the program authors
 *  -l      print only the logins
 *  -n      print only the names
*/
{
    char aut1[] = "Rodrigo Naranjo González";
    char log1[] = "r.naranjo";
    char aut2[] = "Martin do Rio Rico";
    char log2[] = "martin.dorio";

    if (tr[1] == NULL)
    {
        printf("%s:%s\n", aut1, log1);
        printf("%s:%s\n", aut2, log2);
    }
    else if (strcmp(tr[1],"-l") == 0)
    {
        printf("%s\n", log1);
        printf("%s\n", log2);
    }
    else if (strcmp(tr[1],"-n") == 0)
    {
        printf("%s\n", aut1);
        printf("%s\n", aut2);
    }
    else
        printf("autores: invalid option");
    return 1;
}

int cmd_pid(char **tr)
/*
 * Prints the pid of the process running the shell
 *  -p      prints the pid of the shell's parent process
 */
{
    pid_t pid, ppid;

    if (tr[1] == NULL)
    {
        if ((pid = getpid()) < 0)
            perror("unable to get pid");
        else
            printf("%d\n", pid);
    }
    else if (strcmp(tr[1], "-p") == 0)
    {
        if ((ppid = getppid()) < 0)
            perror("unable to get the ppid");
        else
            printf("%d\n", ppid);
    }
    else
        printf("pid: invalid option\n");

    return 1;
}

int cmd_carpeta(char **tr)
/*
 * Prints the current working directory
 *  DIRECT      changes the current working directory to DIRECT
 */
{
    char s[100];

    if (tr[1] == NULL)
        printf("%s\n",getcwd(s,100));
    else if (chdir(tr[1]) != 0)
        printf("carpeta: %s: No such file or directory\n", tr[1]);

    return 1;
}

int cmd_date(char **tr)
/*
 * Print the current date and time
 *  -d  print the current date in format dd/mm/yyyy
 *  -h  print the current time in format hh:mm:ss
 */
{
    time_t t;
    struct tm * current;

    time(&t);
    current = localtime(&t);

    if (tr[1] == NULL)
        printf("%s", asctime(current));
    else if (strcmp(tr[1], "-d") == 0)
        // tm_mon counts from 0-11, add 1 to show real month number
        // tm_year counts the years since 1900, add that to the output to show the current year
        printf("%02d/%02d/%d\n", current->tm_mday, current->tm_mon + 1, current->tm_year + 1900);
    else if (strcmp(tr[1], "-h") == 0)
        printf("%02d:%02d:%02d\n", current->tm_hour, current->tm_min, current->tm_sec);
    else
        printf("fecha: invalid option\n");

    return 1;
}

int cmd_infosis()
/*
 * Prints the info on the machine running the shell using uname library
 */
{
    struct utsname infosis;

    if(uname(&infosis)!=0)
    {
        perror("uname doesn't return 0, so there is an error"); // revisar y cambiar esto, está pillao de internet -R
        exit(EXIT_FAILURE);
    }

    printf("System Name = %s\n", infosis.sysname);
    printf("Node Name = %s\n", infosis.nodename);
    printf("Version = %s\n", infosis.version);
    printf("Release = %s\n", infosis.release);
    printf("Machine = %s\n", infosis.machine);

    return 1;
}

int cmd_ayuda(char **tr)
/*
 * Displays a list of available commands
 *  ayuda [CMD]   gives a brief help on the usage of the given CMD
 */
{
    int i, pos = 0;
    int max_len = 100;

    char txt[] ="autores [-l|-n]\t\tprints the names and logins of the program "                            // autores
                "authors.\n\t-l\t\tprints only the logins\n\t-n\t\tprints only the names\n%"
                "pid [-p]\t\tprints the pid of the process executing the shell.\n\t"                        // pid
                "-p\t\tprints the pid of the shell's parent process\n%"
                "carpeta [DIRECT]\t\tprints the current working directory\n\t"                              // carpeta
                "DIRECT\t\tchange the current working directory to DIRECT\n%"
                "fecha [-d|-h]\t\tprints the current date and time.\n\t-d\t\tprints the current "           // fecha
                "date in the format dd/mm/yyyy\n\t-h\t\tprints the current time in the format hh:mm:ss\n%"
                "hist [-c|-N]\t\tshows the history of commands executed by the shell.\n"            // hist
                "\t-c\t\tclears the list\n\t-N\t\tprints the first N commands\n%"
                "comando [N]\t\trepeats command number N\n%"                                                // comando
                "infosis\t\tprints information on the machine running the shell.\n%"                        // infosis
                "ayuda [CMD]\t\tdisplays a list of available commands or a brief help on the usage of CMD if given.\n%"
                "fin, salir, bye\t\tends the shell.\n";                                                     // exit

    char *piece = strtok(txt, "%");
    char **info = malloc(sizeof(char*) * max_len);

    while (piece != NULL)
    {
        info[pos++] = piece;
        piece = strtok(NULL, "%");

        if (pos >= max_len)
        {
            max_len += max_len;
            info = realloc(info, sizeof(char*) * max_len);

            if (!info) {
                perror("Error: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (tr[1] == NULL)
    {
        printf("\n%33sM&R Shell\n\n", "");
        for (i = 0; i < pos; i++)
            printf("%s\n",info[i]);
    }
    else if (strcmp(tr[1], "autores") == 0)
        printf("%s", info[0]);
    else if (strcmp(tr[1], "pid") == 0)
        printf("%s", info[1]);
    else if (strcmp(tr[1], "carpeta") == 0)
        printf("%s", info[2]);
    else if (strcmp(tr[1], "fecha") == 0)
        printf("%s", info[3]);
    else if (strcmp(tr[1], "hist") == 0)
        printf("%s", info[4]);
    else if (strcmp(tr[1], "comando") == 0)
        printf("%s", info[5]);
    else if (strcmp(tr[1], "infosis") == 0)
        printf("%s", info[6]);
    else if (strcmp(tr[1], "ayuda") == 0)
        printf("%s", info[7]);
    else if (strcmp(tr[1], "fin") == 0 || strcmp(tr[1], "salir") == 0 || strcmp(tr[1], "bye") == 0 || strcmp(tr[1], "exit") == 0)
        printf("%s", info[8]);

    return 1;
}

int cmd_exit()
/*
 * Exits the shell
 */
{
    return 0;
}

int cmd_hist(char **tr, char* list[])
/*
 * Shows the history of commands executed by the shell.
 *  -c  clears the history
 *  -N  prints the first N commands
 */
{
    if (tr[1] == NULL) {
        for (int i = 0; list[i] != NULL; i++) {
            printf("%d %s\n",i, list[i]);
        }
    }
    else
        printf("HISTORIAL");

    return 1;
}
