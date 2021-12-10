/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#include "p0.h"
#include "main.h"
#include "color.h"

int cmd_autores(char **tr)
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
        printf(COLOR_RED "autores: invalid option" COLOR_RESET "\n");

    return 1;
}

int cmd_pid(char **tr)
{
    pid_t pid, ppid;

    if (tr[1] == NULL)
    {
        if ((pid = getpid()) < 0)
            printf(COLOR_RED "pid: getpid: %s" COLOR_RESET "\n", strerror(errno));
        else
            printf("%d\n", pid);
    }
    else if (strcmp(tr[1], "-p") == 0)
    {
        if ((ppid = getppid()) < 0)
            printf(COLOR_RED "pid: getppid: %s" COLOR_RESET "\n", strerror(errno));
        else
            printf("%d\n", ppid);
    }
    else
        printf(COLOR_RED "pid: invalid option" COLOR_RESET "\n");

    return 1;
}

int cmd_carpeta(char **tr)
{
    char source[BUFFERSIZE];

    if (tr[1] == NULL)
        printf("%s\n", getcwd(source, BUFFERSIZE));
    else if (chdir(tr[1]) && tr[1] != NULL)
        printf(COLOR_RED "carpeta: %s" COLOR_RESET "\n", strerror(errno));

    return 1;
}

int cmd_fecha(char **tr)
{
    time_t t;
    struct tm *current;

    time(&t);
    current = localtime(&t);

    if (tr[1] == NULL)
        printf("%s", asctime(current));
    else if (strcmp(tr[1], "-d") == 0)
        // tm_mon counts from 0-11, add 1 to show real month number.
        // tm_year counts the years since 1900, add that to the output to show the current year.
        printf("%02d/%02d/%d\n", current->tm_mday, current->tm_mon + 1, current->tm_year + 1900);
    else if (strcmp(tr[1], "-h") == 0)
        printf("%02d:%02d:%02d\n", current->tm_hour, current->tm_min, current->tm_sec);
    else
        printf(COLOR_RED "fecha: invalid option" COLOR_RESET "\n");

    return 1;
}

int cmd_hist(char **tr, char* history[])
{
    char *ptr = NULL;
    long n;
    int i;

    /*
     * IF no flag: print each entry in the history, +1 to each command position to make it more readable.
     * ELSE IF -c flag: iterate through the array and put NULL in each filled position.
     * ELSE IF input int: print from the beginning to the absolute value of the given number if this number is < 0.
     */

    if (tr[1] == NULL)
        for (i = 0; (history[i] != NULL) && (i < ARRAY); i++)
            printf("%5d  %s\n", i + 1, history[i]);
    else if (strcmp(tr[1], "-c") == 0)
    {
        for (i = 0; (history[i] != NULL) && (i < ARRAY); i++)
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
        printf(COLOR_RED "hist: invalid option" COLOR_RESET "\n");

    return 1;
}

int cmd_comando(char **tr, char *history[], tMemList *memList, char *envp[], char **environ)
{
    char *ptr = NULL;
    char *cmd = NULL;
    char **args = NULL;
    long n;
    int i;

    for (i = 0; (history[i] != NULL) && (i < ARRAY); i++);

    /*
     * IF no flag: wrong input.
     * ELSE IF input int: recall the command from the history and iterate through a full loop, issue a warning when
     *                    calling to itself.
     */
    if (tr[1] == NULL)
        printf(COLOR_RED "comando: no specified number" COLOR_RESET "\n");
    else if ((n = strtol(tr[1], &ptr, 10) - 1) >= 0)
    {
        if (history[n] != NULL && n <= i)
        {
            cmd = strdup(history[n]);
            args = split_cmd(cmd);

            if (strcmp(args[0], "comando") == 0)
                printf(COLOR_RED "comando: can't call itself to avoid infinite loop hazards" COLOR_RESET "\n");
            else
                process_cmd(args, history, memList, envp, environ);

            free(cmd);
            free(args);
        }
        else
            printf(COLOR_RED "comando: no entry found" COLOR_RESET "\n");
    }
    else
        printf(COLOR_RED "comando: invalid option" COLOR_RESET "\n");

    return 1;
}

int cmd_infosis()
{
    struct utsname infosis;

    if(uname(&infosis)!=0)
        printf(COLOR_RED "infosis: uname: %s" COLOR_RESET "\n", strerror(errno));

    printf("%s (%s), ", infosis.nodename, infosis.machine);
    printf("OS: %s-%s-%s\n", infosis.sysname, infosis.release, infosis.version);

    return 1;
}

int cmd_ayuda(char **tr)
{
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
                   "\t-n     print the names only.");
        else if (strcmp(tr[1], "ayuda") == 0)
            printf("NAME\n"
                   "\tayuda\n"
                   "SYNTAX\n"
                   "\tayuda [cmd]\n"
                   "DESCRIPTION\n"
                   "\tShow a list of the available command. Given the name of a command as an argument, "
                   "the information and help on the usage of said command is displayed.");
        else if (strcmp(tr[1], "borrar") == 0)
            printf("NAME\n"
                   "\tborrar\n"
                   "SYNTAX\n"
                   "\tborrar name1 name2 ...\n"
                   "DESCRIPTION\n"
                   "\tDeletes the files and directories entered by the user. Directories need to be empty and trying to"
                   " delete a non-empty directory will yield an error message.");
        else if (strcmp(tr[1], "borrarrec") == 0)
            printf("NAME\n"
                   "\tborrarrec\n"
                   "SYNTAX\n"
                   "\tborrarrec name1 name2 ...\n"
                   "DESCRIPTION\n"
                   "\tDeletes files and directories entered by the user. The directories"
                   " do not need to be empty, and are deleted recursively with all of its content.");
        else if (strcmp(tr[1], "carpeta") == 0)
            printf("NAME\n"
                   "\tcarpeta\n"
                   "SYNTAX\n"
                   "\tcarpeta [DIRECT]\n"
                   "DESCRIPTION\n"
                   "\tPrint the current working directory or, if given, change it to DIRECT.");
        else if (strcmp(tr[1], "comando") == 0)
            printf("NAME\n"
                   "\tcomando\n"
                   "SYNTAX\n"
                   "\tcomando N\n"
                   "DESCRIPTION\n"
                   "\tRepeat the command number N as seen in the history.");
        else if (strcmp(tr[1], "crear") == 0)
            printf("NAME\n"
                   "\tcrear\n"
                   "SYNTAX\n"
                   "\tcrear [-f] name\n"
                   "DESCRIPTION\n"
                   "\tCreates a directory, or a file if specified, of name NAME. If the NAME field is left empty, it "
                   " prints the name of the current working directory.\n"
                   "OPTIONS\n"
                   "\t-f     an empty .txt file with the given name is created.");
        else if (strcmp(tr[1], "fecha") == 0)
            printf("NAME\n"
                   "\tfecha\n"
                   "SYNTAX\n"
                   "\tfecha [-d|-h]\n"
                   "DESCRIPTION\n"
                   "\tPrint the current system date and time.\n"
                   "OPTIONS\n"
                   "\t-d     output date in format dd/mm/yyyy.\n"
                   "\t-h     output time in format hh:mm:ss.");
        else if (strcmp(tr[1], "hist") == 0)
            printf("NAME\n"
                   "\thist  -  history of commands\n"
                   "SYNTAX\n"
                   "\thist [-c|-N]\n"
                   "DESCRIPTION\n"
                   "\tKeep track of previous commands inputted in the shell, printing them in order and numbered.\n"
                   "OPTIONS\n"
                   "\t-c     clear the history.\n"
                   "\t-N     print the first N commands.");
        else if (strcmp(tr[1], "infosis") == 0)
            printf("NAME\n"
                   "\tinfosis\n"
                   "SYNTAX\n"
                   "\tinfosis\n"
                   "DESCRIPTION\n"
                   "\tDisplay the information of the machine running the shell.");
        else if (strcmp(tr[1], "listdir") == 0)
            printf("NAME\n"
                   "\tlistdir\n"
                   "SYNTAX\n"
                   "\tlistdir [-reca|-recb] [-hid] [-long] [-link] [-acc] name1 name2 ...\n"
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
                   "\t\tLast access time will be used instead of last modification time.");
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
                   "\t\tLast access time will be used instead of last modification time.");
        else if (strcmp(tr[1], "malloc") == 0)
            printf("NAME\n"
                   "\tmalloc\n"
                   "SYNTAX\n"
                   "\tmalloc [-free] [tam]\n"
                   "DESCRIPTION\n"
                   "\tAllocate tam bytes and show the memory address returned by the function, keeping this information"
                   " in a list. If no size is specified, show the list of addresses allocated with the malloc command.\n"
                   "OPTIONS\n"
                   "\t-free\n"
                   "\t\tDeallocate one of the blocks of size tam that has been previously allocated with the command "
                   "malloc. If no such block exists or no size is specified, show the list of addresses allocated with"
                   "this command. Should there be more than one block of size tam, deallocates only one of them in any "
                   "order.");
        else if (strcmp(tr[1], "pid") == 0)
            printf("NAME\n"
                   "\tpid\n"
                   "SYNTAX\n"
                   "\tpid [-p]\n"
                   "DESCRIPTION\n"
                   "\tPrint the ID of the process executing the shell.\n"
                   "OPTIONS\n"
                   "\t-p\n"
                   "\t\tprint the ID of the parent process of the shell.");
        else if (strcmp(tr[1], "fin") == 0 || strcmp(tr[1], "salir") == 0 ||
                 strcmp(tr[1], "bye") == 0 || strcmp(tr[1], "exit") == 0)
            printf("NAME\n"
                   "\tfin, salir, bye, exit\n"
                   "SYNTAX\n"
                   "\tfin\n\tsalir\n\tbye\n\texit\n"
                   "DESCRIPTION\n"
                   "\tEnd the shell.");
        else if (strcmp(tr[1], "mmap") == 0)
            printf("NAME\n"
                   "\tmmap\n"
                   "SYNTAX\n"
                   "\tmmap [-free] fich [perm]\n"
                   "DESCRIPTION\n"
                   "\tMaps in memory the file fich and shows the memory address where the file has been mapped, "
                   "storing the address of the mapping, the size, the name of the file, the file descriptor, and the "
                   "time of mapping in a list. perm represents the mapping permissions in a rwx format. "
                   "If fich is not specified, show the list of addresses allocated with the mmap command.\n"
                   "OPTIONS\n"
                   "\t-free\n"
                   "\t\tUnmaps and closes the file fich and removes the address where it was mapped from the list. "
                   "If fich has been mapped several times, only one of the mappings will be undone. If the file fich "
                   "is not mapped by the process or if fich is not specified, show the list of addresses allocated "
                   "with the mmap command.\n"
                   "\tperm\n"
                   "\t\tRepresents the mapping permissions in a rwx format.");
        else if (strcmp(tr[1], "shared") == 0)
            printf("NAME\n"
                   "\tshared\n"
                   "SYNTAX\n"
                   "\tshared [-free|-create|-delkey] cl [tam]\n"
                   "DESCRIPTION\n"
                   "\tGet shared memory of key cl, map it in the process address space and shows the memory address "
                   "where the shared memory has been mapped. The address, the key, the size of the shared memory block,"
                   " and the time of the mapping, will be stores in the list. tam is only used when creating a new "
                   "block of shared memory. If no cl is specified, the command will show the list of addresses "
                   "allocated with the shared command."
                   "OPTIONS\n"
                   "\t-free\n"
                   "\t\tDetach the shared memory block with the key ctl from the address space of the process and "
                   "eliminates its address from the list. If shared memory block with key cl has been attached several "
                   "times, only one of them is detached.\n"
                   "\t-create\n"
                   "\t\tCreate a shared memory block of key cl and size tam, map it in the process address space and "
                   "show the memory address where the shared memory has been mapped. Store in the list the address, "
                   "the key, the size of the shared memory, and the time of mapping. It is assumed that key cl is not "
                   "in use in the system so a new block of shared memory must be created.\n"
                   "\t-delkey\n"
                   "\t\tRemove the shared memory region of key cl.");
        else if (strcmp(tr[1], "dealloc") == 0)
            printf("NAME\n"
                   "\tdealloc\n"
                   "SYNTAX\n"
                   "\tdealloc [-malloc|-shared|-mmap] [addr|tam|cl|file]\n"
                   "DESCRIPTION\n"
                   "\tDeallocate one of the memory blocks allocated with the command malloc, mmap, or shared, and "
                   "remove it from the list. If given a specified addr, deallocate and remove it from the list. "
                   "If no arguments are given, print the list of allocated memory blocks.\n"
                   "OPTIONS\n"
                   "\t-malloc\n"
                   "\t\tSame as malloc -free tam.\n"
                   "\t-shared\n"
                   "\t\tSame as shared -free cl.\n"
                   "\t-mmap\n"
                   "\t\tSame as mmap -free file.\n");
        else if (strcmp(tr[1], "memoria") == 0)
            printf("NAME\n"
                   "\tmemoria\n"
                   "SYNTAX\n"
                   "\tmemoria [-blocks] [-vars] [-funcs] [-all] [-pmap]\n"
                   "DESCRIPTION\n"
                   "\tShow addresses inside the process memory space. If no argument is given, equivalent to -all.\n"
                   "OPTIONS\n"
                   "\t-bocks\n"
                   "\t\tShow the list of addresses allocated with the malloc, shared, mmap.\n"
                   "\t-vars\n"
                   "\t\tPrint the memory addresses of nine variables of the shell: three external, three static, and "
                   "three automatic initialized variables.\n"
                   "\t-funcs\n"
                   "\t\tPrint the memory addresses of the three program functions of the shell and three C library "
                   "functions used in the shell program.\n"
                   "\t-all\n"
                   "\t\tThe equivalent to -blocks, -vars, and -funcs combined.\n"
                   "\t-pmap\n"
                   "\t\tCall the program pmap for the shell process.");
        else if (strcmp(tr[1], "volcarmem") == 0)
            printf("NAME\n"
                   "\tvolcarmem\n"
                   "SYNTAX\n"
                   "\tvolcarmem addr [cont]\n"
                   "DESCRIPTION\n"
                   "\tSHow the contents of cont bytes starting at memory address addr. If cont is not specified, show "
                   "25 bytes. For each byte, print its hex value and its associated char, at 25 bytes per line.\n");
        else if (strcmp(tr[1], "llenarmem") == 0)
            printf("NAME\n"
                   "\tllenarmem\n"
                   "SYNTAX\n"
                   "\tllenarmem addr [cont] [byte]\n"
                   "DESCRIPTION\n"
                   "\tFill cont bytes starting at address addr with value byte. If byte is not specified, assume a "
                   "value of 65. If cont is not specified, use a default value of 128.\n");
        else if (strcmp(tr[1], "recursiva") == 0)
            printf("NAME\n"
                   "\trecursiva\n"
                   "SYNTAX\n"
                   "\trecursiva n\n"
                   "DESCRIPTION\n"
                   "\tCall a recursive function passing the integer n as its parameter. It receives the number of "
                   "times it has to call itself. This function has two variables: an automatic array of 4096 bytes "
                   "and a static array of the same size.\n"
                   "\tThis function prints the value of the received parameter as well as its memory address, the "
                   "address of the static array and the automatic array, decrements n and if n > 0, calls itself.\n");
        else if (strcmp(tr[1], "e-s") == 0)
        {
            if (tr[2] == NULL)
                printf(COLOR_RED "ayuda: e-s: specify an available command." COLOR_RESET);
            else if (strcmp(tr[2], "write") == 0)
                printf("NAME\n"
                       "\te-s write\n"
                       "SYNTAX\n"
                       "\te-s write [-o] fich addr cont\n"
                       "DESCRIPTION\n"
                       "\tWrite cont bytes from memory address addr into file fich. If file fich does not exist, "
                       "create it.\n"
                       "OPTIONS\n"
                       "\t-o\n"
                       "\t\t If a file already exists, overwrite its content with the command's new data.");
            else if (strcmp(tr[2], "read") == 0)
                printf("NAME\n"
                       "\te-s read\n"
                       "SYNTAX\n"
                       "\te-s read fich addr cont\n"
                       "DESCRIPTION\n"
                       "\tRead cont bytes from file fich into memory address addr. If cont is not specified, all of fich "
                       "is read onto memory address addr.\n");
            else
                printf(COLOR_RED "ayuda: command does not exist." COLOR_RESET);
        }
        else
            printf(COLOR_RED "ayuda: command does not exist." COLOR_RESET);
    }
    else
        printf("AVAILABLE COMMANDS\n"
               "\tautores\n"
               "\tayuda\n"
               "\tborrar\n"
               "\tborrarrec\n"
               "\tbye\n"
               "\tcarpeta\n"
               "\tcomando\n"
               "\tcrear\n"
               "\tdealloc\n"
               "\te-s read\n"
               "\te-s write\n"
               "\texit\n"
               "\tfecha\n"
               "\tfin\n"
               "\thist\n"
               "\tinfosis\n"
               "\tlistdir\n"
               "\tlistfich\n"
               "\tllenarmem\n"
               "\tmalloc\n"
               "\tmemoria\n"
               "\tmmap\n"
               "\tpid\n"
               "\trecursiva\n"
               "\tshared\n"
               "\tvolcarmem");

    printf("\n");

    return 1;
}

int cmd_exit(char* history[], tMemList *memList)
{
    int i;
    // Iterate through the history freeing all of its occupied slots when finishing the shell.
    for (i = 0; history[i] != NULL; i++)
        free(history[i]);

    clearList(memList);

    return 0;
}