/*
 *    TITLE: SISTEMAS OPERATIVOS
 * SUBTITLE: Lab Assignment 0
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 24 / 09 / 21
 */

#include <stdlib.h>
#include <string.h>

#include "hist_list.h"

bool createNode(tHistPos *p)
{
    // Create and assign memory to variable.
    *p = malloc(sizeof(struct tHistNode));
    return *p != NULL;
}

void createEmptyList (tHistList* h)
{
    // Start a list without any nodes.
    *h = NULL;
}

bool isEmptyList(tHistList h)
{
    // Check if there are any items within the list.
    return (NULL == h);
}

bool insertCommand(tHistData d, tHistList* h)
{
    // Declare an auxiliary nodes
    tHistPos n;

    // Create a node in one of the lists, if there's enough memory, turn (n) into a new node.
    if(!createNode(&n))
        return false;
    else
    {
        // Point the node (n) to the item to be added.
        n->data = d;

        //strcpy(d, n->data);
        n->next = NULL;
        // If the list is empty, start the list with the new item.
        if (NULL == *h)
            *h = n;
        else
        {
            n->next = *h;
            *h = n;
        }

        return true;
    }
}

/*tHistPos findPosition (tHistItem d, tHistList l)
{
    // Declare a position variable.
    tHistPos p;

    // Iterate through the list to find the position of the given item.
    p = l;
    while ((NULL != p->next) && (d.cmd_number != p->next->data.cmd_number))
        p = p->next;

    return p;
}*/

tHistData *getCommand(tHistPos p, tHistList h)
{
    // Return the item in the given position
    return &p->data;
}

tHistPos first(tHistList h)
{
    return h;
}

tHistPos last(tHistList h)
{
    tHistPos p;

    for(p = h; p->next != NULL; p = p->next);
    return p;
}

tHistPos previous(tHistPos p, tHistList h)
{
    tHistPos q;

    if (p == h) {
        return NULL;
    } else {
        for (q = h; q->next != p; q = q->next);
        return q;
    }
}

tHistPos next(tHistPos p, tHistList h)
{
    return p->next;
}