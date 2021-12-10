/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

/**
 * TODO
 *      @cmd_volcarmem revisar
 */

#include "p0.h"
#include "p2.h"
#include "mem_list.h"
#include "color.h"

int global_var1 = 5, global_var2 = 6, global_var3 = 7;

int cmd_malloc(char **tr, tMemList *memList)
{
    void *ptr;
    long inputSize;
    size_t tam;

    // Print the malloc operations of the memory list.
    if (tr[1] == NULL)
        aux_memListPrint(tr[0], memList);
    else if (strcmp(tr[1], "-free") == 0)
    {
        // Check if there are multiple or no arguments, or if the memory list is empty.
        if (isEmptyList(*memList))
            printf(COLOR_RED "malloc: -free: empty list" COLOR_RESET "\n");
        else if (tr[3] != NULL)
            printf(COLOR_RED "malloc: -free: command contains more than one argument" COLOR_RESET "\n");
        else if (tr[2] == NULL)
            aux_memListPrint(tr[0], memList);
        else if (tr[2] != NULL)
        {
            inputSize = strtol(tr[2], NULL, 10);
            tam = inputSize;
            aux_liberarMalloc(tam, memList);
        }
    }
    else if (tr[2] != NULL)
        printf(COLOR_RED "malloc: invalid input" COLOR_RESET "\n");
    else
    {
        inputSize = strtol(tr[1], NULL, 10);
        tam = inputSize;

        if (inputSize <= 0 || (ptr = malloc(tam)) == NULL)
            printf(COLOR_RED "malloc: memory allocation failed" COLOR_RESET "\n");
        else
        {
            printf("allocated %ld at %p\n", tam, ptr);
            aux_addMemList(ptr, tam, tr[0], NULL, 0, 0, memList);
        }
    }

    return 1;
}

void aux_liberarMalloc(size_t tam, tMemList *memList)
{
    tPosL pos;
    tMemItemL aux;

    if ((pos = findTam(tam, *memList)) == NULL)
        aux_memListPrint("malloc", memList);
    else
    {
        aux = getItem(pos, *memList);

        printf("deallocated %ld at %p\n", aux.size, aux.memaddr);
        deleteAtPosition(pos, memList);
        free(aux.memaddr);
    }
}

int cmd_mmap(char **tr, tMemList *memList)
{
    int prot = PROT_NONE;
    int i;
    void *ptr;

    if (tr[1] == NULL)
        aux_memListPrint(tr[0], memList);
    else if (strcmp(tr[1], "-free") == 0)
    {
        if (tr[2] == NULL)
            aux_memListPrint("mmap", memList);
        else if (tr[3] != NULL)
            printf(COLOR_RED "mmap: -free: invalid input" COLOR_RESET "\n");
        else if (isEmptyList(*memList))
            printf(COLOR_RED "mmap: -free: empty list" COLOR_RESET "\n");
        else
            aux_liberarMap(tr[2], memList);
    }
    else if (tr[2] != NULL && strlen(tr[2]) > 3)
        printf(COLOR_RED "mmap: prot: invalid permissions" COLOR_RESET "\n");
    else if(tr[3] != NULL)
        printf(COLOR_RED "mmap: invalid input" COLOR_RESET "\n");
    else
    {
        // Setting permissions.
        if (tr[2] != NULL)
        {
            for (i = 0; tr[2][i] != '\0' && i < 4; i++)
            {
                if (tr[2][i] == 'r')
                    prot |= PROT_READ;
                else if (tr[2][i] == 'w')
                    prot |= PROT_WRITE;
                else if (tr[2][i] == 'x')
                    prot |= PROT_EXEC;
            }
        }

        if ((ptr = aux_mapFile(tr[1], prot, memList)) == NULL)
            printf(COLOR_RED "mmap: could not map file %s" COLOR_RESET "\n", tr[1]);
        else
            printf("file %s mapped at %p\n", tr[1], ptr);
    }

    return 1;
}

void *aux_mapFile(char *fname, int prot, tMemList *memList)
{
    int fildes, modo = O_RDONLY;
    struct stat fs;
    void *p;

    if (prot&PROT_WRITE)
        modo = O_RDWR;

    fildes = open(fname, modo);

    if (fildes == -1)
        printf(COLOR_RED "mapFile: open: %s" COLOR_RESET "\n", strerror(errno));
    else if (stat(fname, &fs))
        printf(COLOR_RED "mapFile: stat: %s" COLOR_RESET "\n", strerror(errno));
    else if ((p = mmap(NULL, fs.st_size, prot, MAP_PRIVATE, fildes, 0)) == MAP_FAILED)
        printf(COLOR_RED "mapFile: mmap: %s" COLOR_RESET "\n", strerror(errno));
    else
    {
        aux_addMemList(p, fs.st_size, "mmap", fname, fildes, 0, memList);
        close(fildes);

        return p;
    }

    return NULL;
}

void aux_liberarMap(char *fname, tMemList *memList)
{
    tPosL pos;
    tMemItemL aux;

    if((pos = findName(fname, *memList)) == NULL)
        aux_memListPrint("mmap", memList);
    else
    {
        aux = getItem(pos, *memList);
        printf("unmmaped file %s at %p\n", aux.info.name, aux.memaddr);

        if (munmap(aux.memaddr, aux.size))
            printf(COLOR_RED "mmap: munmap: %s" COLOR_RESET "\n", strerror(errno));
        else
            deleteAtPosition(pos, memList);
    }
}

int cmd_shared(char **tr, tMemList *memList)
{
    key_t key;
    size_t tam = 0;
    void *sh_mem;

    if (tr[1] == NULL)
        aux_memListPrint(tr[0], memList);
    else if (strcmp(tr[1], "-free") == 0)
    {
        if (tr[2] == NULL)
            aux_memListPrint(tr[0], memList);

        key = (key_t) strtol(tr[2], NULL, 10);
        aux_liberarShared(key, memList);
    }
    else if (strcmp(tr[1], "-create") == 0)
    {
        if (tr[2] == NULL || tr[3] == NULL)
            aux_memListPrint(tr[0], memList);
        else
        {
            key = (key_t) strtol(tr[2], NULL, 10);

            if (key == IPC_PRIVATE)
                printf(COLOR_RED "shared: -create: %s" COLOR_RESET "\n", strerror(EINVAL));
            else if (tr[3] != NULL)
            {
                tam = (size_t) strtol(tr[3], NULL, 10);

                if ((sh_mem = aux_createShared(key, tam, memList)) != NULL)
                    printf("allocated shared memory (key %d) at %p\n", key, sh_mem);
            }
        }
    }
    else if (strcmp(tr[1], "-delkey") == 0)
    {
        if (tr[2] == NULL)
            aux_memListPrint(tr[0], memList);

        key = (key_t) strtol(tr[2], NULL, 10);

        if (aux_sharedDelKey(key))
            printf("key %d removed from the system\n", key);
    }
    else {
        key = (key_t) strtol(tr[1], NULL, 10);

        if (key == IPC_PRIVATE)
            printf(COLOR_RED "shared: %s" COLOR_RESET "\n", strerror(EINVAL));

        if (tr[2] != NULL)
            tam = (size_t) strtol(tr[3], NULL, 10);

        if ((sh_mem = aux_createShared(key, tam, memList)) != NULL)
            printf("allocated shared memory (key %d) at %p\n", key, sh_mem);
    }

    return 1;
}

int cmd_dealloc(char **tr, tMemList *memList)
{
    void *memAddr;
    tPosL pos;
    tMemItemL aux;
    size_t tam;

    if (tr[1] == NULL)
        aux_memListPrint("all", memList);
    else if (strcmp(tr[1], "-malloc") == 0)
    {
        if (tr[2] == NULL)
            aux_memListPrint("malloc", memList);
        else
        {
            tam = (size_t) strtol(tr[2], NULL, 10);
            aux_liberarMalloc(tam, memList);
        }
    }
    else if (strcmp(tr[1], "-mmap") == 0)
    {
        if (tr[2] == NULL)
            aux_memListPrint("mmap", memList);
        else
            aux_liberarMap(tr[2], memList);
    }
    else if (strcmp(tr[1], "-shared") == 0)
    {
        if (tr[2] == NULL)
            aux_memListPrint("shared", memList);
        else
            aux_liberarShared((key_t) strtol(tr[2], NULL, 10), memList);
    }
    else
    {
        memAddr = (void *) strtol(tr[1], NULL, 16);

        if ((pos = findAddr(memAddr, *memList)) == NULL)
            aux_memListPrint("all", memList);
        else
        {
            aux = getItem(pos, *memList);

            if (strcmp(aux.type, "malloc") == 0)
                aux_liberarMalloc(aux.size, memList);
            if (strcmp(aux.type, "mmap") == 0)
                aux_liberarMap(aux.info.name, memList);
            if (strcmp(aux.type, "shared") == 0)
                aux_liberarShared(aux.info.key, memList);
        }

    }

    return 1;
}

void *aux_createShared(key_t key, size_t tam, tMemList *memList)
{
    int id, flags = 0777;
    void *sh_mem;
    struct shmid_ds s;

    if (tam)
        flags |= IPC_CREAT | IPC_EXCL;

    if(key == IPC_PRIVATE)
        printf(COLOR_RED "createShared: key: %s" COLOR_RESET "\n", strerror(EINVAL));
    else if ((id = shmget(key, tam, flags)) == -1)
        printf(COLOR_RED "createShared: shmget: %s" COLOR_RESET "\n", strerror(errno));
    else if ((sh_mem = shmat(id, NULL, 0)) == (void*) -1)
    {
        printf(COLOR_RED "createShared: shmat: %s" COLOR_RESET "\n", strerror(errno));
        if(tam)
            shmctl(id, IPC_RMID, NULL);
    }
    else
    {
        shmctl(id, IPC_STAT, &s);
        aux_addMemList(sh_mem, s.shm_segsz, "shared", NULL, 0, key, memList);

        return sh_mem;
    }

    return NULL;
}

int aux_sharedDelKey(key_t key)
{
    int id;

    if (key == IPC_PRIVATE)
    {
        printf(COLOR_RED "shared: -delkey: invalid key" COLOR_RESET "\n");
        return 0;
    }
    else if ((id = shmget(key, 0, 0666)) == -1)
    {
        printf(COLOR_RED "liberarShared: shmget: %s" COLOR_RESET "\n", strerror(errno));
        return 0;
    }
    else if (shmctl(id, IPC_RMID, NULL) == -1)
    {
        printf(COLOR_RED "liberarShared: shmctl: %s" COLOR_RESET "\n", strerror(errno));
        return 0;
    }

    return 1;
}

void aux_liberarShared(key_t key, tMemList *memList)
{
    tPosL pos;
    tMemItemL aux;

    if ((pos = findKey(key, *memList)) == NULL)
        aux_memListPrint("shared", memList);
    else
    {
        aux = getItem(pos, *memList);
        printf("deallocated shared memory block at %p (key %d)\n", aux.memaddr, aux.info.key);

        if (shmdt(aux.memaddr))
            printf(COLOR_RED "liberarShared: shmdt: %s" COLOR_RESET "\n", strerror(errno));
        else
            deleteAtPosition(pos, memList);
    }
}

int cmd_memoria(char **tr, tMemList *memList)
{
    int local_1 = 0, local_2 = 2, local_3 = 5054324;
    static int st_var1 = 1, st_var2 = 3, st_var3 = 5054325;
    bool all = false;

    if (tr[1] == NULL || strcmp(tr[1], "-all") == 0)
        all = true;

    if (all || strcmp(tr[1], "-blocks") == 0)
    {
        aux_memListPrint("all", memList);
        if (all) printf("\n");
    }
    if (all || strcmp(tr[1], "-vars") == 0)
    {
        printf("Local  variables \t%14p, \t%14p, \t%14p\n", &local_1, &local_2, &local_3);
        printf("Global variables \t%14p, \t%14p, \t%14p\n", &global_var1, &global_var2, &global_var3);
        printf("Static variables \t%14p, \t%14p, \t%14p\n", &st_var1, &st_var2, &st_var3);
        if (all) printf("\n");
    }
    if (all || strcmp(tr[1], "-funcs") == 0)
    {
        printf("Shell functions \t%14p, \t%14p, \t%14p\n", cmd_pid, cmd_es, cmd_mmap);
        printf("Library functions \t%14p, \t%14p, \t%14p\n", printf, scanf, strtol);
    }
    if (tr[1] != NULL && strcmp(tr[1], "-pmap") == 0)
        aux_dopmap();

    return 1;
}

void aux_dopmap()
{
    pid_t pid;
    char elpid[32];
    char *argv[3] = {"pmap", elpid, NULL};

    sprintf(elpid, "%d", (int) getpid());

    if ((pid = fork()) == -1)
    {
        printf(COLOR_RED "dopmap: %s" COLOR_RESET "\n", strerror(errno));
        return;
    }
    if (pid == 0)
    {
        if (execvp(argv[0], argv) == -1)
            printf(COLOR_RED "dopmap: %s" COLOR_RESET "\n", strerror(errno));
        exit(1);
    }

    waitpid(pid, NULL, 0);
}

int cmd_volcarmem(char **tr)
{
    long len = 25;
    void *memAddr = (void *) strtol(tr[1], NULL, 16);

    if (tr[2] != NULL)
        len = strtol(tr[2], NULL, 10);

    aux_volcarmem(memAddr, len);

    return 1;
}

void aux_volcarmem(void *addr, long len)
{
    int i, j;
    char buff_char[256];
    char buff_hex[512];
    char aux[4];
    char *pc = (char*) addr;

    if (len <= 0)
    {
        printf(COLOR_RED "volcarmem: invalid length" COLOR_RESET "\n");
        return;
    }

    for (i = 0; i <= len; i++) {
        if ((i % 25) == 0)
        {
            if (i != 0)
            {
                for (j = 0; j < 25; j++)
                    printf(" %2c", buff_char[j]);

                printf("\n%s\n", buff_hex);
            }

            strcpy(buff_hex, "");
            strcpy(buff_char, "");
        }

        if (i == len) break;

        sprintf(aux, "%02x", pc[i]);
        strcat(buff_hex, " ");
        strcat(buff_hex, aux);

        if ((pc[i] < 0x20) || (pc[i] > 0x7e)) {
            strcat(buff_char, " ");
        } else {
            sprintf(aux, "%2c", pc[i]);
            strcat(buff_hex, " ");
            strcat(buff_hex, aux);
        }
    }

    for (j = 0; j < i % 25; j++)
        printf(" %2c", buff_char[j]);

    if ((i % 25) != 0)
        printf("\n%s\n", buff_hex);
}

int cmd_llenarmem(char **tr)
{
    void *memAddr = NULL;
    size_t cont = 128;
    int byte;

    if (tr[1] == NULL)
        printf(COLOR_RED "llenarmem: invalid input" COLOR_RESET "\n");
    else
    {
        memAddr = (void *) strtol(tr[1], NULL, 16);

        if (tr[2] != NULL)
            cont = (size_t) strtol(tr[2], NULL, 10);

        if (tr[3] != NULL)
        {
            if (tr[3][1] == 'x')
                byte = (int) strtol(tr[3], NULL, 16);
            else
                byte = (int) strtol(tr[3], NULL, 10);
        }

        if (cont <= 0 || byte <= 0)
            printf(COLOR_RED "llenarmem: invalid input" COLOR_RESET "\n");
        else
        {
            memset(memAddr, byte, cont);
            printf("filled %zu bytes at %p\n", cont, memAddr);
        }
    }

    return 1;
}

int cmd_recursiva(char **tr)
{
    long n;

    n = strtol(tr[1], NULL, 10);
    aux_recursiva(n);

    return 1;
}

void aux_recursiva(long n)
{
    char aut[ARRAY];
    static char st[ARRAY];

    printf("parameter: %ld(%p) array %p, static array %p\n", n, &n, aut, st);
    n--;

    if (n > 0)
        aux_recursiva(n);
}

int cmd_es(char **tr)
{
    if (strcmp(tr[1], "read") == 0)
    {
        if (!aux_es_read(tr))
            printf(COLOR_RED "e-s read: could not read file" COLOR_RESET "\n");
    }
    else if (strcmp(tr[1], "write") == 0)
    {
        if (!aux_es_write(tr))
            printf(COLOR_RED "e-s write: could not write onto file" COLOR_RESET "\n");
    }
    else
        printf(COLOR_RED "%s: command not found" COLOR_RESET "\n", tr[0]);

    return 1;
}

int aux_es_write(char **tr)
{
    ssize_t nleidos, tam;
    void *ptr;
    int df, aux;
    struct stat s;
    int flags = O_WRONLY | O_CREAT;

    if (strcmp(tr[2], "-o") == 0)
    {
        tr += 1;
        flags |= O_TRUNC;
    }
    else
        flags |= O_EXCL;

    ptr = (void *) strtol(tr[3], NULL, 16);

    if ((df = open(tr[2], flags, 0664)) == -1 || stat(tr[2], &s) == -1)
    {
        printf(COLOR_RED "e-s write: stat: %s" COLOR_RESET "\n", strerror(errno));
        return 0;
    }

    if (tr[4] == NULL)
        tam = (ssize_t) s.st_size;
    else
        tam = (ssize_t) strtol(tr[4], NULL, 10);

    if ((nleidos = write(df, ptr, tam)) == -1)
    {
        aux = errno;
        close(df);
        errno = aux;
        return 0;
    }

    close(df);

    printf("wrote %zd bytes from %s at %p\n", nleidos, tr[2], ptr);
    return 1;
}

int aux_es_read(char **tr)
{
    ssize_t nleidos, tam;
    void *ptr;
    int df, aux;
    struct stat s;

    ptr = (void *) strtol(tr[3], NULL, 16);

    if (stat(tr[2], &s) == -1 || (df = open(tr[2], O_RDONLY)) == -1)
    {
        printf(COLOR_RED "e-s read: stat: %s" COLOR_RESET "\n", strerror(errno));
        return 0;
    }

    if (tr[4] == NULL)
        tam = (ssize_t) s.st_size;
    else
        tam = (ssize_t) strtol(tr[4], NULL, 10);

    if ((nleidos = read(df, ptr, tam)) == -1)
    {
        aux = errno;
        close(df);
        errno = aux;
        return 0;
    }

    close(df);

    printf("read %zd bytes from %s at %p\n", nleidos, tr[2], ptr);
    return 1;
}

void aux_memListPrint(char *func, tMemList *memList)
{
    tPosL pos;
    tMemItemL aux;

    if (isEmptyList(*memList) == true)
        printf(COLOR_RED "aux_memListPrint: empty list" COLOR_RESET "\n");
    else
    {
        pos = first(*memList);

        do
        {
            aux = getItem(pos, *memList);

            if ((strcmp(func, "all") == 0 || strcmp(func, aux.type) == 0) && strcmp("malloc", aux.type) == 0)
                printf("%p: size:%zu. %s %s",
                       aux.memaddr, aux.size, aux.type, ctime(&aux.time));

            if ((strcmp(func, "all") == 0 || strcmp(func, aux.type) == 0) && strcmp("mmap", aux.type) == 0)
                printf("%p: size:%zu. %s %s (fd:%d) %s",
                       aux.memaddr, aux.size, aux.type, aux.info.name, aux.info.fildes, ctime(&aux.time));

            if ((strcmp(func, "all") == 0 || strcmp(func, aux.type) == 0) && strcmp("shared", aux.type) == 0)
                printf("%p: size:%zu. %s memory (key %d) %s",
                       aux.memaddr, aux.size, aux.type, aux.info.key, ctime(&aux.time));

            pos = next(pos);
        }
        while (pos != NULL);
    }
}

void aux_addMemList(void *ptr, size_t tam, char *func, char *fname, int fildes, key_t key, tMemList *memList)
{
    tMemItemL item;

    sprintf(item.type, "%s", func);
    item.memaddr = ptr;
    item.size = tam;
    item.time = time(NULL);

    if (strcmp(func, "mmap") == 0)
    {
        item.info.fildes = fildes;
        sprintf(item.info.name, "%s", fname);
    }

    if (strcmp(func, "shared") == 0)
        item.info.key = key;

    insertItem(item, memList);
}








