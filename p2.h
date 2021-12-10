/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#ifndef SHELL_P2_H
#define SHELL_P2_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define DELIM " \t\n"
#define ARRAY 4096
#define BUFFERSIZE 1024

#include "mem_list.h"

int cmd_malloc(char **tr, tMemList *memList);
void aux_liberarMalloc(size_t tam, tMemList *memList);
/*
 * Allocate tam bytes and show the memory address returned by the function, keeping this information in a list. If no
 * size is specified, show the list of addresses allocated with the malloc command.
 *  -free       Deallocate one of the blocks of size tam that has been previously allocated with the command
 *                          malloc. If no such block exists or no size is specified, show the list of addresses
 *                          allocated with this command. Should there be more than one block of size tam, deallocates
 *                          only one of them in any order.
 */

int cmd_mmap(char **tr, tMemList *memList);
void *aux_mapFile(char *fname, int prot, tMemList *memList);
void aux_liberarMap(char *fname, tMemList *memList);
/*
 * Map in memory the file fich and show the mem address where the file has been mapped.
 *  -free       Unmap and close the file fich and remove the address where it was mapped from the list.
 *   perm       Mapping permissions.
 */

int cmd_shared(char **tr, tMemList *memList);
void *aux_createShared(key_t key, size_t tam, tMemList *memList);
int aux_sharedDelKey(key_t key);
void aux_liberarShared(key_t key, tMemList *memList);
/*
 * Get shared memory of key cl, map it in the process address space, and show the mem address where the shared memory
 * has been mapped.
 *      tam     Size, only used when creating a new block of shared memory.
 *    -free     Detach the shared memory block with key cl from the process' address space.
 *  -create     Create a new block of shared memory.
 *  -delkey     Remove the shared memory region of key cl. Nothing gets unmapped.
 */

int cmd_dealloc(char **tr, tMemList *memList);
/*
 * Deallocate one of the memory blocks allocated with the command malloc, mmap, or shared, and remove it from the list.
 * -malloc size     Same as malloc -free size.
 *   -shared cl     Same as shared -free cl.
 *   -mmap file     Same as mmap -free file.
 *         addr     Deallocate addr and remove it from the list.
 */

int cmd_memoria(char **tr, tMemList *memList);
void aux_dopmap();
/*
 * Show addresses inside the process memory space. If no arguments are given, is equivalent to -all.
 *  -blocks     Show the list of addresses allocated with the malloc, mmap, and shared commands.
 *    -vars     Print the mem addresses of nine variables of the shell.
 *   -funcs     Print the mem addresses of three program functions of the shell and three C library functions.
 *     -all     Equivalent to -blocks, -vars, and -funcs together.
 *    -pmap     Call the program pmap for the shell process.
 */

int cmd_volcarmem(char **tr);
void aux_volcarmem(void *addr, long len);
/*
 * Show the content of cont bytes starting at mem address addr. cont defaults to 25.
 */

int cmd_llenarmem(char **tr);
/*
 * Fill cont bytes of memory starting at address addr with the value byte. byte defaults to 65.
 */

int cmd_recursiva(char **tr);
void aux_recursiva(long n);
/*
 * Call a recursive function passing the integer n as its parameter.
 */

int cmd_es(char **tr);
int aux_es_read(char **tr);
int aux_es_write(char **tr);
/*
 * Read or write cont bytes from file fich into mem address addr.
 */

void aux_addMemList(void *ptr, size_t tam, char *func, char *fname, int fildes, key_t key, tMemList *memList);
/*
 * Add items to the memory list.
 */

void aux_memListPrint(char *func, tMemList *memList);
/*
 * Print the memory list according to some parameters.
 */

#endif
