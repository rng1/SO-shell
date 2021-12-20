/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

#include "job_list.h"

void createEmptyJobList(tJobList *jobList)
// Creates an empty list.
{
    // Start a list without any nodes.
    *jobList = NULL;
}

bool createJobNode(tJobPosL *p)
// Creates an empty node.
{
    // Create and assign memory to variable.
    *p = malloc(sizeof(struct tJobNode));

    return *p != NULL;
}

tJobPosL findJobItem(pid_t pid, tJobList jobList)
// Finds the position of an item in the list based on its PID.
{
    // Create an auxiliary position.
    tJobPosL p;
    // Iterate through the list to find the position of the given item.
    p = jobList;

    while (pid != p->data.pid)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

bool insertJobItem(tJobItemL d, tJobList *jobList)
// Inserts an element in the list, unordered. If the element could be inserted,
// the value true is returned; otherwise, false is returned.
{
    // Declare two auxiliary positions.
    tJobPosL n, p;

    // Create a node in one of the lists, if there's enough memory, turn (n) into a new node.
    if (!createJobNode(&n))
        return false;
    else
    {
        // Point the node (n) to the item to be added.
        n->data = d;
        n->next = NULL;
        // If the list is empty, start the list with the new item.
        if (NULL == *jobList)
            *jobList = n;
        else
        {
            for (p = *jobList; p->next != NULL; p = p->next);
            p->next = n;
        }

        return true;
    }
}

void updateJobItem(tJobItemL *d, char status[16])
{
    strcpy(d->status,status);
}

void deleteJobAtPosition(tJobPosL p, tJobList *jobList)
{
    tJobPosL q;

    if (*jobList == p)
        *jobList = p->next;
    else {
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q;
    }
    free(p);
}

void clearJobList(tJobList *jobList)
{
    // Create an auxiliary position.
    tJobPosL p, aux;

    if (isEmptyJobList(*jobList))
        return;
    else
    {
        p = *jobList;

        while (p != NULL)
        {
            aux = p->next;
            free(p);
            p = aux;
        }

        *jobList = NULL;
    }
}

bool isEmptyJobList(tJobList jobList)
// Determines whether the list is empty or not.
{
    // Check if there are any items within the list.
    return (NULL == jobList);
}

tJobItemL getJobItem(tJobPosL p)
// Retrieves the content of the element at the indicated position.
{
    // Return the data of the item at the given position.
    return p->data;
}

tJobPosL firstJob(tJobList jobList)
// Returns the position of the first element of the list.
{
    return jobList;
}

tJobPosL nextJob(tJobPosL p)
// Returns the position preceding the one we indicate (or NULL if the specified position has no preceding element).
{
    return p->next;
}