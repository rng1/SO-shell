/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 10 / 12 / 21
 */

/**
 * TODO(roi)
 *      terminar la implementacion
 */

#include "job_list.h"

void createEmptyList(tProcList *memList)
// Creates an empty list.
{
    // Start a list without any nodes.
    *memList = NULL;
}

bool createNode(tPosL *p)
// Creates an empty node.
{
    // Create and assign memory to variable.
    *p = malloc(sizeof(struct tNode));

    return *p != NULL;
}

tPosL findTam(size_t tam, tProcList memList)
// Finds the position of an item in the list based on its size.
{
    // Create an auxiliary position.
    tPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    while (tam != p->data.size)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

tPosL findKey(key_t key, tProcList memList)
// Finds the position of an item in the list based on its key.
{
    // Create an auxiliary position.
    tPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    if (isEmptyList(memList))
        return NULL;

    while (key != p->data.info.key)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

tPosL findName(char name[], tProcList memList)
// Finds the position of an item in the list based on its name.
{
    // Create an auxiliary position.
    tPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    while (strcmp(name, p->data.info.name) != 0)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

tPosL findAddr(void *memAddr, tProcList memList)
// Finds the position of an item in the list based on its memory address.
{
    // Create an auxiliary position.
    tPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    while (memAddr != p->data.memaddr)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

bool insertItem(tMemItemL d, tProcList *memList) // todo: cambiar comentarios
// Inserts an element in the list ordered by the field nickname. If the element could be inserted,
// the value true is returned; otherwise, false is returned.
{
    // Declare two auxiliary positions.
    tPosL n, p;

    // Create a node in one of the lists, if there's enough memory, turn (n) into a new node.
    if (!createNode(&n))
        return false;
    else
    {
        // Point the node (n) to the item to be added.
        n->data = d;
        n->next = NULL;
        // If the list is empty, start the list with the new item.
        if (NULL == *memList)
            *memList = n;
        else
        {
            for (p = *memList; p->next != NULL; p = p->next);
            p->next = n;
        }

        return true;
    }
}

void deleteAtPosition(tPosL p, tProcList *memList)
{
    // Create an auxiliary position.
    tPosL q;

    // Delete the first element.
    if (*memList == p)
        *memList = p->next;
        // Delete the last element.
    else if (p->next == NULL) {
        for(q = *memList; q->next != p; q = q->next);
        // When 'q' is the previous node to 'p'.
        q->next = NULL;
    }
        // Delete at an intermediate position.
    else {
        q = p->next;
        p->data = q->data;
        p->next = q->next;
        p = q;
    }
    // Free the unused memory.
    free(p);
}

void clearList(tProcList *memList)
{
    // Create an auxiliary position.
    tPosL p, aux;

    if (isEmptyList(*memList))
        return;
    else
    {
        p = *memList;

        while (p != NULL)
        {
            if (strcmp(p->data.type, "malloc") == 0)
                free(p->data.memaddr);
            aux = p->next;
            free(p);
            p = aux;
        }

        *memList = NULL;
    }
}

bool isEmptyList(tProcList memList)
// Determines whether the list is empty or not.
{
    // Check if there are any items within the list.
    return (NULL == memList);
}

tProcItemL getItem(tPosL p, tProcList memList)
// Retrieves the content of the element at the indicated position.
{
    // Return the data of the item at the given position.
    return p->data;
}

tPosL first(tProcList memList)
// Returns the position of the first element of the list.
{
    return memList;
}

tPosL next(tPosL p)
// Returns the position preceding the one we indicate (or NULL if the specified position has no preceding element).
{
    return p->next;
}