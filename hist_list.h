/*
 *    TITLE: SISTEMAS OPERATIVOS
 * SUBTITLE: Lab Assignment 0
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 24 / 09 / 21
 */

#ifndef P0_HIST_LIST_H
#define P0_HIST_LIST_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct tHistNode *tHistPos;
typedef char *tHistData;
struct tHistNode
{
    tHistPos next;
    tHistData data;
};
typedef tHistPos tHistList;

void createEmptyList (tHistList *h);
bool isEmptyList(tHistList h);
bool insertCommand(tHistData d, tHistList *h);
/*tHistPos findItem(char[], tHistList);*/
tHistData *getCommand(tHistPos p, tHistList h);
tHistPos first(tHistList h);
tHistPos last(tHistList h);
tHistPos previous(tHistPos p, tHistList h);
tHistPos next(tHistPos p, tHistList h);

#endif //P0_HIST_LIST_H
