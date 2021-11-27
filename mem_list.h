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
#include "types.h"

/** type declaration */
typedef struct tNode *tPosL;
struct tNode
{
    tItemL data;
    tPosL next;
};
typedef tPosL tList;

/** function prototypes **/

// Generators
void createEmptyList(tList *memList);
bool insertItem(tItemL d, tList *memList);

// Destructors
void deleteAtPosition(tPosL p, tList *memList);
void clearList(tList *memList);

// Observers
tPosL findTam(size_t tam, tList memList);
tPosL findKey(key_t key, tList memList);
tPosL findName(char name[], tList memList);
tPosL findAddr(void *memAddr, tList memList);
bool isEmptyList(tList memList);
tItemL getItem(tPosL p, tList memList);
tPosL first(tList memList);
tPosL last(tList memList);
tPosL next(tPosL p);
tPosL previous(tPosL p, tList memList);

#endif
