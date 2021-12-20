/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#ifndef P2_MEM_LIST_H
#define P2_MEM_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"

/** type declaration */
typedef struct tMemNode *tMemPosL;
struct tMemNode
{
    tMemItemL data;
    tMemPosL next;
};
typedef tMemPosL tMemList;

/** function prototypes **/

// Generators
void createEmptyMemList(tMemList *memList);
bool insertMemItem(tMemItemL d, tMemList *memList);

// Destructors
void deleteMemAtPosition(tMemPosL p, tMemList *memList);
void clearMemList(tMemList *memList);

// Observers
tMemPosL findTam(size_t tam, tMemList memList);
tMemPosL findKey(key_t key, tMemList memList);
tMemPosL findName(char name[], tMemList memList);
tMemPosL findAddr(void *memAddr, tMemList memList);
bool isEmptyMemList(tMemList memList);
tMemItemL getMemItem(tMemPosL p);
tMemPosL firstMem(tMemList memList);
tMemPosL nextMem(tMemPosL p);

#endif
