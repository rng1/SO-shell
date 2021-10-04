/*
 *    TITLE: SISTEMAS OPERATIVOS
 * SUBTITLE: Lab Assignment 1
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 22 / 10 / 21
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <time.h>
#include <sys/stat.h>

#define DELIM " \t\n"
#define ARRAY 4096

/** FUNCTION DECLARATION */
void loop(char *history[]);

int process_cmd(char **tr, char *history[]);
void save_cmd(char *cmd, char *history[]);
char *read_cmd();
char **split_cmd(char *cmd);

int cmd_authors(char **tr);
int cmd_ayuda(char **tr);
int cmd_date(char **tr);
int cmd_exit();
int cmd_carpeta(char **tr);
int cmd_comando(char **tr, char *history09[]);
int cmd_crear(char **tr);
int cmd_hist(char **tr, char* history[]);
int cmd_infosis();
int cmd_pid(char **tr);
int cmd_rm(char **tr);

/** MAIN FUNCTIONS */
int main()
{
    char *history[ARRAY];
    history[0] = NULL;
    loop(history);

    return 1;
}

// Main loop
void loop(char *history[])
{
    char *cmd = NULL;
    char **args = NULL;
    int status;

    do
    {
        printf("$ ");
        cmd = read_cmd();
        save_cmd(cmd, history);
        args = split_cmd(cmd);
        status = process_cmd(args, history);

        free(cmd);
        free(args);
    } while (status);
}

// Save command to history
void save_cmd(char *cmd, char *history[])
{
    int i;
    char* line = strtok(cmd,"\n");
    if (line != NULL) // Check if only input is Enter
    {
        for (i = 0; (history[i] != NULL) && (i < ARRAY); i++);
        if(i == ARRAY) {
            for (i = 0; i < ARRAY - 1; i++) { history[i] = history[i+1]; }
            i++;
            //free(history[i]);
            //history[i] = NULL;
            history[i] = strdup(line);
        }
        else {
            history[i] = strdup(line);
            history[i+1] = NULL;
        }
    }
}

// Read command
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

// Split string into pieces
char **split_cmd(char *cmd)
{
    int max_len = 50, pos = 0;
    char *piece = strtok(cmd, DELIM);
    char **tokens = malloc(sizeof(char*) * max_len);

    if (!tokens) {
        perror("error (split_1)\n");
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
                perror("error (split_2)\n");
                exit(EXIT_FAILURE);
            }
        }
    }

    tokens[pos] = NULL;
    return tokens;
}

// Process the first word of the given array looking for a valid command
int process_cmd(char **tr, char *history[])
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
            return cmd_hist(tr, history);
        else if (strcmp(tr[0], "comando") == 0)
            return cmd_comando(tr, history);
        else if (strcmp(tr[0], "infosis") == 0)
            return cmd_infosis();
        else if (strcmp(tr[0], "ayuda") == 0)
            return cmd_ayuda(tr);
        else if (strcmp(tr[0], "fin") == 0 || strcmp(tr[0], "salir") == 0 ||
                 strcmp(tr[0], "bye") == 0 || strcmp(tr[0], "exit") == 0)
            return cmd_exit();
        else if (strcmp(tr[0], "crear") == 0)
            return cmd_crear(tr);
        else if (strcmp(tr[0], "borrar") == 0)
            return cmd_rm(tr);
            /*else if (strcmp(tr[0], "borrarrec") == 0)
                return cmd_rmrec(tr);*/

        else
            printf("%s: command not found\n", tr[0]);
    }

    return 1;
}

/** COMMANDS */
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
        printf("%s:%s\n%s:%s\n", aut1, log1, aut2, log2);
    else if (strcmp(tr[1],"-l") == 0)
        printf("%s\n%s\n", log1, log2);
    else if (strcmp(tr[1],"-n") == 0)
        printf("%s\n%s\n", aut1, aut2);
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
            perror("pid");
        else
            printf("%d\n", pid);
    }
    else if (strcmp(tr[1], "-p") == 0)
    {
        if ((ppid = getppid()) < 0)
            perror("pid -p");
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
        perror("carpeta");

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

int cmd_hist(char **tr, char* history[])
/*
 * Shows the history of commands executed by the shell.
 *  -c  clears the history
 *  -N  prints the first N commands
 */
{
    char *ptr = NULL;
    long n;
    int i;

    /*
     * IF no flag: print each entry in the history, +1 to each command position to make it more readable
     * ELSE IF -c flag: iterate through the array and put NULL in each filled position
     * ELSE IF input int: print from the beginning to the absolute value of the given number if this number is < 0
     */

    if (tr[1] == NULL)
        for (i = 0; (history[i] != NULL) && (i < ARRAY); i++)
            printf("%5d  %s\n", i + 1, history[i]);
    else if (strcmp(tr[1], "-c") == 0)
        for (i = 0; (history[i] != NULL) && (i < ARRAY); i++) {
            free(history[i]);
            history[0] = NULL;
        }
    else if ((n = strtol(tr[1], &ptr, 10)) < 0)
    {
        for (i = 0; i < -n; i++)
            if (history[i] != NULL)
                printf("%5d  %s\n", i + 1, history[i]);
    }
    else
        printf("hist: invalid option\n");

    return 1;
}

int cmd_comando(char **tr, char *history[])
/*
 * Repeats command number N
 */
{
    char *ptr = NULL;
    char *cmd = NULL;
    char **args = NULL;
    long n;

    /*
     * IF no flag: wrong input
     * ELSE IF input int: recall the command from the history and iterate through a full loop, issue a warning when
     *                    calling to itself.
     */
    if (tr[1] == NULL)
        printf("comando: no specified number\n");
    else if ((n = strtol(tr[1], &ptr, 10) - 1) >= 0)
    {
        if (history[n] != NULL)
        {
            //cmd = history[n];
            cmd = strdup(history[n]);
            args = split_cmd(cmd);

            if (strcmp(args[0], "comando") == 0)
                printf("comando: can't call itself to avoid infinite loop hazards\n");
            else
                process_cmd(args, history);

            free(cmd);
            free(args);

            loop(history);
        }
        else
            printf("comando: no entry found\n");
    }
    else
        printf("comando: invalid option\n");

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
        perror("infosis: uname:");
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
    printf("\n");

    if (tr[1] != NULL)
    {
        if (strcmp(tr[1], "autores") == 0)
            printf("NAME\n"
                   "\tautores\n"
                   "SYNTAX\n"
                   "\tautores [-l|-n]\n"
                   "DESCRIPTION\n"
                   "\tPrint the names and logins of the shell authors.\n"
                   "OPTIONS\n"
                   "\t-l     print the logins only.\n"
                   "\t-n     print the names only.\n");
        else if (strcmp(tr[1], "ayuda") == 0)
            printf("NAME\n"
                   "\tayuda\n"
                   "SYNTAX\n"
                   "\tayuda [cmd]\n"
                   "DESCRIPTION\n"
                   "\tShow a list of the available command. Given the name of a command as an argument, "
                   "the information and help on the usage of said command is displayed.\n");
        else if (strcmp(tr[1], "borrar") == 0)
            printf("NAME\n"
                   "\tborrar\n"
                   "SYNTAX\n"
                   "\tborrar name1 name2 ...\n"
                   "DESCRIPTION\n"
                   "\tDeletes the files and directories entered by the user. Directories need to be empty and trying to"
                   " delete a non-empty directory will yield an error message.\n");
        else if (strcmp(tr[1], "borrarrec") == 0)
            printf("NAME\n"
                   "\tborrarrec\n"
                   "SYNTAX\n"
                   "\tborrarrec name1 name2 ...\n"
                   "DESCRIPTION\n"
                   "\tDeletes files and directories entered by the user. The directories"
                   " do not need to be empty, and are deleted recursively with all of its content.\n");
        else if (strcmp(tr[1], "carpeta") == 0)
            printf("NAME\n"
                   "\tcarpeta\n"
                   "SYNTAX\n"
                   "\tcarpeta [DIRECT]\n"
                   "DESCRIPTION\n"
                   "\tPrint the current working directory or, if given, change it to DIRECT.\n");
        else if (strcmp(tr[1], "comando") == 0)
            printf("NAME\n"
                   "\tcomando\n"
                   "SYNTAX\n"
                   "\tcomando N\n"
                   "DESCRIPTION\n"
                   "\tRepeat the command number N as seen in the history.\n");
        else if (strcmp(tr[1], "crear") == 0)
            printf("NAME\n"
                   "\tcrear\n"
                   "SYNTAX\n"
                   "\tcrear [-f] NAME\n"
                   "DESCRIPTION\n"
                   "\tCreates a directory, or a file if specified, of name NAME. If the NAME field is left empty, it "
                   " prints the name of the current working directory.\n"
                   "OPTIONS\n"
                   "\t-f     an empty .txt file with the given name is created.\n");
        else if (strcmp(tr[1], "fecha") == 0)
            printf("NAME\n"
                   "\tfecha\n"
                   "SYNTAX\n"
                   "\tfecha [-d|-h]\n"
                   "DESCRIPTION\n"
                   "\tPrint the current system date and time.\n"
                   "OPTIONS\n"
                   "\t-d     output date in format dd/mm/yyyy.\n"
                   "\t-h     output time in format hh:mm:ss.\n");
        else if (strcmp(tr[1], "hist") == 0)
            printf("NAME\n"
                   "\thist  -  history of commands\n"
                   "SYNTAX\n"
                   "\thist [-c|-N]\n"
                   "DESCRIPTION\n"
                   "\tKeep track of previous commands inputted in the shell, printing them in order and numbered.\n"
                   "OPTIONS\n"
                   "\t-c     clear the history.\n"
                   "\t-N     print the first N commands.\n");
        else if (strcmp(tr[1], "infosis") == 0)
            printf("NAME\n"
                   "\tinfosis\n"
                   "SYNTAX\n"
                   "\tinfosis\n"
                   "DESCRIPTION\n"
                   "\tDisplay the information of the machine running the shell.\n");
        else if (strcmp(tr[1], "listdir") == 0)
            printf("NAME\n"
                   "\tlistdir\n"
                   "SYNTAX\n"
                   "\thist [-reca] [-recb] [-hid] [-long] [-link] [-acc] name1 name2 ...\n"
                   "DESCRIPTION\n"
                   "\tShows the information of files, directories, or devices. If the user fails to enter the names they"
                   " want to know the information of, it prints the current working directory. \n"
                   "OPTIONS\n"
                   "\t-reca\n"
                   "\t\tWhen listing the content in a directory, print every subdirectory after the files in the "
                   "directory.\n"
                   "\t-recb\n"
                   "\t\tWhen listing the content in a directory, print every subdirectory before the files in the "
                   "directory.\n"
                   "\t-hid\n"
                   "\t\tHidden files and directories will also get listed.\n"
                   "\t-long\n"
                   "\t\tLong listing. It will print out the date of the last modification, number of links, owner, "
                   "group, mode, size, and name of all the files inside a directory.\n"
                   "\t-link\n"
                   "\t\tOnly meaningful for long listings. If the files are symbolic links the name of the files they "
                   "points to are also printed.\n"
                   "\t-acc\n"
                   "\t\tLast access time will be used instead of last modification time.\n");
        else if (strcmp(tr[1], "listfich") == 0)
            printf("NAME\n"
                   "\tlistfich\n"
                   "SYNTAX\n"
                   "\tlistfich [-long] [-link] [-acc] name1 name2 ...\n"
                   "DESCRIPTION\n"
                   "\tShows the information of files, directories, or devices. If the user fails to enter the names they"
                   " want to know the information of, it prints the current working directory. \n"
                   "OPTIONS\n"
                   "\t-long\n"
                   "\t\tLong listing. It will print out the date of the last modification, number of links, owner, "
                   "group, mode, size, and name of the file. If any of the names is a directory, information on the "
                   "directory file itself will be printed.\n"
                   "\t-link\n"
                   "\t\tOnly meaningful for long listings. If the file is a symbolic link the name of the file it "
                   "points to is also printed.\n"
                   "\t-acc\n"
                   "\t\tLast access time will be used instead of last modification time.\n");
        else if (strcmp(tr[1], "pid") == 0)
            printf("NAME\n"
                   "\tpid\n"
                   "SYNTAX\n"
                   "\tpid [-p]\n"
                   "DESCRIPTION\n"
                   "\tPrint the ID of the process executing the shell.\n"
                   "OPTIONS\n"
                   "\t-p     print the ID of the parent process of the shell.\n");
        else if (strcmp(tr[1], "fin") == 0 || strcmp(tr[1], "salir") == 0 ||
                 strcmp(tr[1], "bye") == 0 || strcmp(tr[1], "exit") == 0)
            printf("NAME\n"
                   "\tfin, salir, bye, exit\n"
                   "SYNTAX\n"
                   "\tfin\n\tsalir\n\tbye\n\texit\n"
                   "DESCRIPTION\n"
                   "\tEnd the shell\n");
        else
            printf("ayuda: command not found\n");
    }
    else
        printf("AVAILABLE COMMANDS\n"
               "\tautores\n"
               "\tayuda\n"
               "\tbye\n"
               "\tcarpeta\n"
               "\tcomando\n"
               "\texit\n"
               "\tfecha\n"
               "\tfin\n"
               "\thist\n"
               "\tinfosis\n"
               "\tpid\n");

    printf("\n");
    return 1;
}

int cmd_exit()
/*
 * End the shell
 */
{
    return 0;
}

// COMANDO A TESTEAR:
/*
 * TODO:
 *  arreglar la creación múltiple de archivos
 *  reconocer y marcar los permisos al crear ficheros
 */

int cmd_crear(char **tr)
/*
 * Creates a file or directory in the file system
 *  crear [-f] name     creates a directory (or file with the -f flag) with the given name
 */
{
    int i;
    char s[100];
    struct stat st = {0};

    if (tr[1] == NULL)
        printf("%s\n",getcwd(s,100));
    else
    {
        for (i = 1; tr[i] != NULL; i++)
        {
            if (stat(tr[i], &st) == -1)
            {
                if (strcmp(tr[1], "-f") != 0)
                    mkdir(tr[i], 0700);
                else if (strcmp(tr[1], "-f") == 0)
                    fopen(tr[i + 1], "w");
                break;
            }
            else
            {
                printf("crear: cannot create file '%s': file already exists\n", tr[i]);
            }
        }
    }

    return 1;
}

int cmd_rm(char **tr)
/*
 * Delete files or empty directories
 */
{
    int i;

    for (i = 1; tr[i] != NULL; i++)
        if (remove(tr[i]))
            perror(tr[i]);

    return 1;
}

/*
 * TODO:
 *  arreglar borrado recursivo
 */
/*int cmd_rmrec(char **tr)
{
    struct dirent *de;  // Pointer for directory entry
    DIR *dr = opendir(tr[1]);
    if (dr == NULL)  // opendir returns NULL if couldn't open directory
        perror("borrarrec:" );
    while ((de = readdir(dr)) != NULL)
        remove(de->d_name);
    closedir(dr);
    return 1;
}*/
