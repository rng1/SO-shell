/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#include "p1.h"
#include "color.h"

int cmd_crear(char **tr)
{
    int i;
    char source[BUFFERSIZE];

    if (tr[1] == NULL)
        printf("%s\n",getcwd(source,BUFFERSIZE));
    else if (strcmp(tr[1], "-f") != 0)
    {
        for (i = 1; tr[i] != NULL; i++)
            if (mkdir(tr[i], 0755)) // rwxr-xr-x
                printf(COLOR_RED "crear: mkdir: %s" COLOR_RESET "\n", strerror(errno));
    }
    else
    {
        for (i = 2; tr[i] != NULL; i++)
            if (aux_createFile(tr[i]))
                printf(COLOR_RED "crear: create_file: %s" COLOR_RESET "\n", strerror(errno));
    }

    return 1;
}

int aux_createFile(char *f)
{
    int df;

    if ((df = open(f, O_CREAT | O_EXCL, 0644)) == -1) // Check if a file already exists
        return -1;
    return close(df);
}

int cmd_borrar(char **tr)
{
    int i;
    char source[BUFFERSIZE];

    if (tr[1] == NULL)
        printf("%s\n",getcwd(source,BUFFERSIZE));

    for (i = 1; tr[i] != NULL; i++)
        if (remove(tr[i]))
            printf(COLOR_RED "borrar: cannot remove '%s': %s" COLOR_RESET "\n", tr[i], strerror(errno));

    return 1;
}

int cmd_borrarRec(char **tr)
{
    int i;
    char source[BUFFERSIZE];

    if (tr[1] == NULL)
        printf("%s\n",getcwd(source,BUFFERSIZE));

    for (i = 1; tr[i] != NULL; i++)
    {
        // Iterate and remove all the contents in the directory and delete the directory itself.
        aux_rmTraverseDir(tr[i]);

        if (remove(tr[i]))
            printf(COLOR_RED "borrarRec: cannot remove '%s': %s" COLOR_RESET "\n", tr[i], strerror(errno));
    }

    return 1;
}

void aux_rmTraverseDir(char *dirname)
{
    char path[BUFFERSIZE];
    struct dirent *d;
    struct stat st;
    struct stat dst;

    lstat(dirname, &dst);

    if (S_ISDIR(dst.st_mode)) {
        DIR *dr = opendir(dirname);

        if (dr)
            while ((d = readdir(dr)) != NULL) {
                // Add files to the path for further iteration through the directory.
                sprintf(path, "%s/%s", dirname, d->d_name);

                if (lstat(path, &st))
                    printf(COLOR_RED "rmTraverseDir: lstat: %s" COLOR_RESET "\n", strerror(errno));
                else {
                    // Prevent the function from deleting the cwd or parent directory.
                    if (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0) {
                        // Iterate inside it again if the file is a directory, if not, simply delete it.
                        if (S_ISDIR(st.st_mode))
                            aux_rmTraverseDir(path);

                        if (remove(path))
                            printf(COLOR_RED "borrarrec: %s" COLOR_RESET "\n", strerror(errno));
                    }
                }
            }
        else
            printf(COLOR_RED "rmTraverseDir: dr: %s" COLOR_RESET "\n", strerror(errno));

        closedir(dr);
    }
}

int cmd_listFich(char **tr)
{
    char source[BUFFERSIZE];
    int i, start = 1;
    bool long_l = false,
            link   = false,
            acc    = false;

    // Iterate through the possible positions where a flag could be, if there are, set their boolean value accordingly
    // and the starting point of the arguments.
    for (i = 1; tr[i] != NULL && i <= 3; i++, start++)
    {
        if (strcmp(tr[i], "-long") == 0 && !long_l)
            long_l = true;
        else if (strcmp(tr[i], "-link") == 0 && !link)
            link = true;
        else if (strcmp(tr[i], "-acc") == 0 && !acc)
            acc = true;
        else
            break;
    }

    // link and acc aren't useful if the long listing isn't specified.
    if (tr[start] == NULL)
        printf("%s\n", getcwd(source, BUFFERSIZE));
    else if ((link || acc) && !long_l)
        printf(COLOR_RED "listFich: '-link' and '-acc' only meaningful for long listings" COLOR_RESET "\n");
    else
        for (i = start; tr[i] != NULL; i++)
            aux_filePrint(tr[i], long_l, link, acc);

    return 1;
}

int cmd_listDir(char **tr)
{
    char source[BUFFERSIZE];
    int i, start = 1;
    bool long_l = false,
            link   = false,
            acc    = false,
            reca   = false,
            recb   = false,
            hid    = false;

    // Iterate through the possible positions where a flag could be, if there are, set their boolean value accordingly
    // and the starting point of the arguments.
    for (i = 1; tr[i] != NULL && i <= 6; i++, start++)
    {
        if (strcmp(tr[i], "-long") == 0 && !long_l)
            long_l = true;
        else if (strcmp(tr[i], "-link") == 0 && !link)
            link = true;
        else if (strcmp(tr[i], "-acc") == 0 && !acc)
            acc = true;
        else if (strcmp(tr[i], "-reca") == 0 && !reca)
            reca = true;
        else if (strcmp(tr[i], "-recb") == 0 && !recb)
            recb = true;
        else if (strcmp(tr[i], "-hid") == 0 && !hid)
            hid = true;
        else
            break;
    }

    // link and acc aren't useful if the long listing isn't specified, also reca and recb are incompatible to avoid
    // excessive execution times.
    if (tr[start] == NULL)
        printf("%s\n", getcwd(source, BUFFERSIZE));
    else if ((link || acc) && !long_l)
        printf(COLOR_RED "listDir: '-link' and '-acc' only meaningful for long listings" COLOR_RESET "\n");
    else if (reca && recb)
        printf(COLOR_RED "listDir: '-reca' and '-recb' are incompatible" COLOR_RESET "\n");
    else
        for (i = start; tr[i] != NULL; i++)
            aux_listTraverseDir(tr[i], long_l, link, acc, hid, reca, recb);

    return 1;
}

void aux_listTraverseDir(char *dirname, bool long_l, bool link, bool acc, bool hid, bool reca, bool recb)
{
    struct dirent *d;
    struct stat st;
    char path [BUFFERSIZE];
    DIR *dr = opendir(dirname);

    if (lstat(dirname, &st))
        printf(COLOR_RED "listTraverseDir: lstat: %s" COLOR_RESET "\n", strerror(errno));
    else
    {
        // If it is a directory, print all of its content, else print the info of the file inputted
        if (S_ISDIR(st.st_mode))
        {
            if (dr)
            {
                if (recb)
                {
                    // Calls for a function that iterates through the subdirectories,
                    // printing their content BEFORE the main directory.
                    aux_lookForDir(dirname, long_l, link, acc, hid, reca, recb);
                }

                // Print the name of the directory where the operation is being performed.
                printf(COLOR_BLUE "%s\n∟" COLOR_RESET "\n", dirname);

                while ((d = readdir(dr)) != NULL)
                {
                    // Save in a string the full path of the files to work with them.
                    sprintf(path, "%s/%s", dirname, d->d_name);

                    // If hid is False AND the file is not hidden
                    // OR if hid is True AND the file name is neither "." nor ".."
                    // (to avoid infinite loops), print their info.
                    if ((!hid && d->d_name[0] != '.') || hid)
                        aux_filePrint(path, long_l, link, acc);
                }

                if (reca)
                {
                    // Calls for a function that iterates through the directories,
                    // printing their content AFTER the main directory.
                    aux_lookForDir(dirname, long_l, link, acc, hid, reca, recb);
                }
            }
            else
                printf(COLOR_RED"listTraverseDir: dr: %s" COLOR_RESET "\n", strerror(errno));
        }
        else
            aux_filePrint(dirname, long_l, link, acc);
    }

    closedir(dr);
}

void aux_lookForDir(char *dirname, bool long_l, bool link, bool acc, bool hid, bool reca, bool recb)
{
    struct dirent *d;
    struct stat st, fst;
    char source[BUFFERSIZE], aux[BUFFERSIZE];

    // Save the cwd to return to the directory were the instruction was performed at the end of the function.
    strcpy(source, getcwd(aux, BUFFERSIZE));
    DIR *dr = opendir(dirname);

    if (lstat(dirname, &st))
        printf(COLOR_RED "lookForDir: lstat_1: %s" COLOR_RESET "\n", strerror(errno));
    else
    {
        // Enter the subdirectory to work with their content.
        chdir(dirname);

        if (dr)
        {
            while ((d = readdir(dr)) != NULL)
            {
                // Recover each file's data to find out which of the files are directories, then list their content.
                if (lstat(d->d_name, &fst))
                    printf(COLOR_RED "listTraverseDir: lstat_2: %s" COLOR_RESET "\n", strerror(errno));
                else if (S_ISDIR(fst.st_mode))
                    if ((!hid && d->d_name[0] != '.')
                        || (strcmp(d->d_name, ".") != 0 && strcmp(d->d_name, "..") != 0))
                        aux_listTraverseDir(d->d_name, long_l, link, acc, hid, reca, recb);
            }
        }
        else
            printf(COLOR_RED "lookForDir: dr: %s" COLOR_RESET "\n", strerror(errno));
    }

    closedir(dr);
    // Change back to the parent directory to continue listTraverseDir operations.
    chdir(source);
}

void aux_filePrint(char *fname, bool long_l, bool link, bool acc)
{
    struct stat st;
    struct tm time;
    time_t t;
    char buf[BUFFERSIZE];
    ssize_t bufsiz;

    if (lstat(fname, &st))
        printf(COLOR_RED "filePrint: cannot access '%s': %s" COLOR_RESET "\n", fname, strerror(errno));
    else
    {
        if (long_l)
        {
            // If '-acc' flag is inputted, use the last access time instead of the last modified time.
            if (acc)
                t = st.st_atim.tv_sec;
            else
                t = st.st_mtim.tv_sec;

            // Convert the given time to the computer's local time zone for correct formatting.
            time = *localtime(&t);

            printf(" %04d/%02d/%02d-%02d:%02d %3lu (%7lu) ",
                    // tm_mon counts from 0-11, add 1 to show real month number
                    // tm_year counts the years since 1900, add that to the output to show the current year
                   time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min,
                   st.st_nlink, st.st_ino);

            // Check to see if the user and group exists, print the name if they do, print the ID if they don't.
            if(getpwuid(st.st_uid) == NULL)
                printf("%d ", st.st_uid);
            else
                printf("%s ", getpwuid(st.st_uid)->pw_name);

            if(getgrgid(st.st_gid) == NULL)
                printf("%d ", st.st_gid);
            else
                printf("%s ", getgrgid(st.st_gid)->gr_name);

            aux_getMode(st.st_mode);
        }

        // General data, this is always printed.
        printf(" %6ld ", st.st_size);

        // Formatting information:
        //      blue for directories
        //      green for links
        if(S_ISDIR(st.st_mode))
            printf(COLOR_BLUE "%s" COLOR_RESET, basename(fname));
        else if (S_ISLNK(st.st_mode))
            printf(COLOR_GREEN "%s" COLOR_RESET, basename(fname));
        else
            printf("%s", basename(fname));

        // Long listing doesn't need to be checked as link is meaningless without it and wouldn't call to this function.
        if (link && S_ISLNK(st.st_mode))
        {
            bufsiz = readlink(fname, buf, sizeof(buf) - 1);

            // If the buffer size is overflowed, display an error
            if (bufsiz != -1)
                buf[bufsiz] = '\0';
            else
            {
                printf(COLOR_RED "filePrint: bufsiz: %s" COLOR_RESET "\n", strerror(errno));
                return;
            }

            printf(" -> %s", buf);
        }

        printf("\n");
    }
}

void aux_getMode(mode_t m)
{
    char permissions[12];

    strcpy(permissions, "----------");

    permissions[0] = aux_getType(m);
    if (m&S_IRUSR) permissions[1] = 'r';    // Owner.
    if (m&S_IWUSR) permissions[2] = 'w';
    if (m&S_IXUSR) permissions[3] = 'x';
    if (m&S_IRGRP) permissions[4] = 'r';    // Group.
    if (m&S_IWGRP) permissions[5] = 'w';
    if (m&S_IXGRP) permissions[6] = 'x';
    if (m&S_IROTH) permissions[7] = 'r';    // Other.
    if (m&S_IWOTH) permissions[8] = 'w';
    if (m&S_IXOTH) permissions[9] = 'x';
    if (m&S_ISUID) permissions[3] = 's';    // setuid, setguid, stickybit
    if (m&S_ISGID) permissions[6] = 's';
    if (m&S_ISVTX) permissions[9] = 't';

    printf("%s", permissions);
}

char aux_getType(mode_t m)
{
    if (S_ISFIFO(m)) // Named pipe, or FIFO.
        return 'p';
    else if (S_ISCHR(m)) // Character special.
        return 'c';
    else if (S_ISDIR(m)) // Directory.
        return 'd';
    else if (S_ISBLK(m)) // Block device.
        return 'b';
    else if (S_ISREG(m)) // Regular file.
        return '-';
    else if (S_ISLNK(m)) // Symbolic link.
        return 'l';
    else if (S_ISSOCK(m)) // Local domain socket.
        return 's';
    else
        printf(COLOR_RED "getType: %s" COLOR_RESET, strerror(errno)); // Unknown type, error.

    return '?';
}