/*
 *    TITLE: SISTEMAS OPERATIVOS SHELL
 * SUBTITLE: Lab Assignment 2
 * AUTHOR 1: Martin do Rio Rico       LOGIN 1: martin.dorio
 * AUTHOR 2: Rodrigo Naranjo Gonzalez LOGIN 2: r.naranjo
 *    GROUP: 6.1
 *     DATE: 19 / 11 / 21
 */

#include "mem_list.h"

void createEmptyMemList(tMemList *memList)
// Creates an empty list.
{
    // Start a list without any nodes.
    *memList = NULL;
}

bool createMemNode(tMemPosL *p)
// Creates an empty node.
{
    // Create and assign memory to variable.
    *p = malloc(sizeof(struct tMemNode));

    return *p != NULL;
}

tMemPosL findTam(size_t tam, tMemList memList)
// Finds the position of an item in the list based on its size.
{
    // Create an auxiliary position.
    tMemPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    if (isEmptyMemList(memList))
        return NULL;

    while (tam != p->data.size)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

tMemPosL findKey(key_t key, tMemList memList)
// Finds the position of an item in the list based on its key.
{
    // Create an auxiliary position.
    tMemPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    if (isEmptyMemList(memList))
        return NULL;

    while (key != p->data.info.key)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

tMemPosL findName(char name[], tMemList memList)
// Finds the position of an item in the list based on its name.
{
    // Create an auxiliary position.
    tMemPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    if (isEmptyMemList(memList))
        return NULL;

    while (strcmp(name, p->data.info.name) != 0)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

tMemPosL findAddr(void *memAddr, tMemList memList)
// Finds the position of an item in the list based on its memory address.
{
    // Create an auxiliary position.
    tMemPosL p;
    // Iterate through the list to find the position of the given item.
    p = memList;

    if (isEmptyMemList(memList))
        return NULL;

    while (memAddr != p->data.memaddr)
    {
        p = p->next;

        if (p == NULL)
            return NULL;
    }

    return p;
}

bool insertMemItem(tMemItemL d, tMemList *memList)
// Inserts an element in the list, unordered. If the element could be inserted,
// the value true is returned; otherwise, false is returned.
{
    // Declare two auxiliary positions.
    tMemPosL n, p;

    // Create a node in one of the lists, if there's enough memory, turn (n) into a new node.
    if (!createMemNode(&n))
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

void deleteMemAtPosition(tMemPosL p, tMemList *memList)
{
    // Create an auxiliary position.
    tMemPosL q;

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

void clearMemList(tMemList *memList)
{
    // Create an auxiliary position.
    tMemPosL p, aux;

    if (isEmptyMemList(*memList))
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

bool isEmptyMemList(tMemList memList)
// Determines whether the list is empty or not.
{
    // Check if there are any items within the list.
    return (NULL == memList);
}

tMemItemL getMemItem(tMemPosL p)
// Retrieves the content of the element at the indicated position.
{
    // Return the data of the item at the given position.
    return p->data;
}

tMemPosL firstMem(tMemList memList)
// Returns the position of the first element of the list.
{
    return memList;
}

tMemPosL nextMem(tMemPosL p)
// Returns the position preceding the one we indicate (or NULL if the specified position has no preceding element).
{
    return p->next;
}