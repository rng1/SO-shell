/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#ifndef SHELL_JOB_LIST_H
#define SHELL_JOB_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

/** type declaration */
typedef struct tNode *tPosL;
struct tNode
{
    tProcItemL data;
    tPosL next;
};
typedef tPosL tProcList;

/** function prototypes **/

// Generators
void createEmptyList(tProcList *procList);
bool insertItem(tMemItemL d, tProcList *procList);

// Destructors
void deleteAtPosition(tPosL p, tProcList *procList);
void clearList(tProcList *procList);

// Observers
tPosL findTam(size_t tam, tProcList procList);
tPosL findKey(key_t key, tProcList procList);
tPosL findName(char name[], tProcList procList);
tPosL findAddr(void *memAddr, tProcList procList);
bool isEmptyList(tProcList procList);
tProcItemL getItem(tPosL p, tProcList procList);
tPosL first(tProcList procList);
tPosL next(tPosL p);

#endif
