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
typedef struct tJobNode *tJobPosL;
struct tJobNode
{
    tJobItemL data;
    tJobPosL next;
};
typedef tJobPosL tJobList;

/** function prototypes **/

// Generators
void createEmptyJobList(tJobList *jobList);
bool insertJobItem(tJobItemL d, tJobList *jobList);
void updateJobItem(tJobItemL *d, char status[16]);

// Destructors
void deleteJobAtPosition(tJobPosL p, tJobList *jobList);
void clearJobList(tJobList *jobList);

// Observers
tJobPosL findJobItem(pid_t pid, tJobList jobList);
bool isEmptyJobList(tJobList jobList);
tJobItemL getJobItem(tJobPosL p);
tJobPosL firstJob(tJobList jobList);
tJobPosL nextJob(tJobPosL p);

#endif
