/*******************************************************************************
 *  Author: Kyle Zrinski
 *  Purpose: Hold functions pertaining to traversal and manipulation of Linked
 *  Lists.
 *  Date: 1-10-18
 *******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "LL.h"

/*creating a new list with null/default values and assigning memory*/
LinkedList* createList(void)
{
    LinkedList* newList = (LinkedList*)malloc(sizeof(LinkedList));

    newList->head = NULL;
    newList->tail = NULL;

    newList->size = 0;

    return newList;
}

/*creating a new node with null values and assigning memory*/
LLNode* createNode(void)
{
    LLNode* newNode = (LLNode*)malloc(sizeof(LLNode));

    newNode->next = NULL;
    newNode->prev = NULL;
    newNode->data = NULL;

    return newNode;
}

/*Inserting a new node into the top of a given list*/
void insertFirst(LinkedList* inList,LLNode* newNode)
{
    if(inList->head == NULL) /*if list empty*/
    {
        inList->head = newNode;
        inList->tail = newNode;

        inList->size++;
    }
    else /*if list has one or more elements*/
    {
        LLNode* temp = inList->head;

        inList->head = newNode;
        newNode->next = temp;
        temp->prev = newNode;

        inList->size++;
    }
}

/*inserting a new node into the bottom of a given list*/
void insertLast(LinkedList* inList,LLNode* newNode)
{
    if(inList->tail == NULL) /*if list empty*/
    {
        inList->head = newNode;
        inList->tail = newNode;

        inList->size++;
    }
    else /*if list has one or more elements*/
    {
        LLNode* temp = inList->tail;

        inList->tail = newNode;
        newNode->prev = temp;
        temp->next = newNode;

        inList->size++;
    }
}

/*removing the first node from a given list and freeing its memory*/
void removeFirst(LinkedList* inList)
{
    if(inList->head != NULL) /*if the list is not empty*/
    {
        LLNode* temp = inList->head->next; /*temp used to rejoin the head to the second node*/

        temp->prev = NULL;

        free(inList->head);

        inList->head = temp;
        free(temp);
        temp = NULL;

        inList->size--;
    }
    else if(inList->tail == inList->head) /*if list has only 1 element*/
    {
        free(inList->head);

        inList->head = NULL;
        inList->tail = NULL;

        inList->size--;
    }
    else /*"sanity" check*/
    {
        perror("###Error### Error removing first item");
    }
}

/*Removes the last element from a list and frees associated memory*/
void removeLast(LinkedList* inList)
{
    if(inList->tail != NULL) /*if list not empty*/
    {
        LLNode* temp = inList->tail->prev; /*temp used to rejoin the tail to the second last node*/

        temp->next = NULL;
        free(inList->tail);

        inList->tail = temp;
        free(temp);
        temp = NULL;

        inList->size--;
    }
    else if(inList->tail == inList->head) /*if list has only one element*/
    {
        free(inList->tail);

        inList->head = NULL;
        inList->tail = NULL;

        inList->size--;
    }
    else /*"sanity" check*/
    {
        perror("###Error### Error removing last element");
    }
}

/*Traversing through an entire list and removing all element, freeing memory
 * associated with the nodes and node->data as well as the list itself*/
void freeList(LinkedList* inList)
{
    LLNode* currNode = NULL;
    LLNode* nextNode = NULL;
    if(inList->head != NULL && inList->tail != NULL) /*if list not empty*/
    {
        if(inList->head != inList->tail)/*if list has 1 or more element*/
        {
            currNode = inList->head;
            nextNode = inList->head->next;

            do /*iterate through the list and free node and data until no nodes*/
            {
                free(currNode->data);
                free(currNode);
                currNode = nextNode;
                if(nextNode != NULL) /*preventing accessing out of bounds memory*/
                {    
                    nextNode = currNode->next;
                }
            }while(currNode != NULL);
            
            free(currNode); /*ensuring all memory is cleaned (sanity check)*/
            free(nextNode);
            free(inList); /*freeing the list itself*/

            currNode = NULL;
            nextNode = NULL;
            inList = NULL;
        }
        else if(inList->head == inList->tail) /*if list has 1 element*/
        {
            free(inList->head->data);
            free(inList->head);
            inList->head = NULL;
            inList->tail = NULL;

            free(inList);
            inList = NULL;
        }
        else /*"sanity" check*/
        {
            perror("###Error### Error emptying list");
        }
    }
    else /*list empty*/
    {
        fprintf(stderr,"###Warning### List already empty or corrupt when trying to empty\n");
        free(inList); /*inList may be empty but it still has associated heap memory*/
        inList = NULL;
    }
}

