/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#ifndef P2_TYPES_H
#define P2_TYPES_H

#include <sys/types.h>

// mem list

typedef struct tAdditionalInfo
{
    char name[256];
    int fildes; // file descriptor
    key_t key;
} tAdditionalInfo;

typedef struct tMemItemL
{
    void *memaddr;
    size_t size;
    time_t time;
    char type[6];
    tAdditionalInfo info;
} tMemItemL;

// process list

typedef struct tJobItemL
{
    pid_t pid;
    char user[32];
    char command[1024];
    time_t time;
    char status[9];
    int val;
} tJobItemL;

#endif
