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
typedef struct tNode *tPosL;
struct tNode
{
    tMemItemL data;
    tPosL next;
};
typedef tPosL tMemList;

/** function prototypes **/

// Generators
void createEmptyList(tMemList *memList);
bool insertItem(tMemItemL d, tMemList *memList);

// Destructors
void deleteAtPosition(tPosL p, tMemList *memList);
void clearList(tMemList *memList);

// Observers
tPosL findTam(size_t tam, tMemList memList);
tPosL findKey(key_t key, tMemList memList);
tPosL findName(char name[], tMemList memList);
tPosL findAddr(void *memAddr, tMemList memList);
bool isEmptyList(tMemList memList);
tMemItemL getItem(tPosL p, tMemList memList);
tPosL first(tMemList memList);
tPosL next(tPosL p);

#endif
