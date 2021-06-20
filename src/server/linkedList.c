#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helperMethods.h"
#include "linkedList.h"

Node *LinkedList()
{
    Node *first = malloc(sizeof(Node));
    (*first).next = NULL;
    (*first).prev = NULL;
    (*first).data = NULL;
    return first;
}

Node *addToList(Node *first, void *newData, int priority, size_t newDataType)
{
    // if linkedList is empty then start with first node
    if ((*first).data == NULL)
    {
        (*first).data = newData;
        (*first).priority = priority;
        (*first).dataLength = newDataType;
        return first;
    }
    // creation of newNode
    Node *newNode = malloc(sizeof(Node));
    (*newNode).data = newData;
    (*newNode).priority = priority;
    (*newNode).next = NULL;
    (*newNode).dataLength = newDataType;

    Node *lastNode = first;
    while ((*lastNode).next != NULL)
    {
        lastNode = (*lastNode).next;
    }
    // binding
    (*lastNode).next = newNode;
    (*newNode).prev = lastNode;
    return newNode;
}

void *popFirst(Node **first)
{
    if (isEmpty(*first))
    {
        printf("Empty list!\n");
        return NULL;
    }
    Node *targetNode;
    void *targetData = (**first).data;
    (**first).data = NULL;
    // if linked list size is one then just free the first node's data
    if ((**first).next == NULL)
    {
        return targetData;
    }
    targetNode = *first;
    *first = (*targetNode).next;
    (**first).prev = NULL;
    freeNode(targetNode);
    return targetData;
}

void *popNodeByIndex(Node **first, int index)
{
    Node *prevNode;
    Node *targetNode;
    Node *nextNode;
    void *targetData;

    if (isEmpty(*first))
    {
        printf("Empty list!\n");
        return NULL;
    }
    else if (index == 0)
    {
        return popFirst(first);
    }
    else if (isOutOfBound(*first, index))
    {
        return NULL;
    }

    targetData = malloc((**first).dataLength);
    memcpy(targetData, (**first).data, (**first).dataLength);

    prevNode = getNodeByIndex(*first, index - 1);
    targetNode = (*prevNode).next;
    nextNode = (*targetNode).next;

    (*prevNode).next = nextNode;
    if (nextNode != NULL)
    {
        (*nextNode).prev = prevNode;
    }

    freeNode(targetNode);
    return targetData;
}

////////////RECURSIVE FUNCTION//////////
Node *getNodeByIndex(Node *first, int index)
{
    if (index == 0)
    {
        return first;
    }
    else if (isOutOfBound(first, index))
    {
        return NULL;
    }
    else
    {
        return getNodeByIndex((*first).next, index - 1);
    }
}

Node *getNodeByData(Node *first, void *data, size_t dataLength)
{
    Node *tempNode = first;
    int found;

    while (tempNode != NULL && data != NULL)
    {
        found = 1;
        if ((*tempNode).dataLength == dataLength)
        {
            for (int i = 0; i < dataLength; i++)
            {
                if (*((char *)(*tempNode).data + i) != *((char *)data + i))
                {
                    found = 0;
                    break;
                }
            }
            if (found == 1)
            {
                return tempNode;
            }
        }
        tempNode = tempNode->next;
    }
    return NULL;
}

void *getDataByIndex(Node *first, int index)
{
    Node *node = getNodeByIndex(first, index);
    if (node == NULL)
    {
        return NULL;
    }
    else
    {
        return (*getNodeByIndex(first, index)).data;
    }
}

int sizeLL(Node *first)
{
    Node *node = first;
    if ((*node).data == NULL)
    {
        return 0;
    }
    int counter = 1;
    while ((*node).next != NULL)
    {
        node = (*node).next;
        counter++;
    }
    return counter;
}

void freeNode(Node *oneNode)
{
    if (oneNode != NULL)
    {
        free((*oneNode).data);
        free(oneNode);
        oneNode = NULL;
    }
    return;
}

void extractNode(Node *node)
{
    if (node != NULL)
    {
        if (node->next != NULL)
        {
            (node->next)->prev = node->prev;
        }
        if (node->prev != NULL)
        {
            node->prev->next = node->next;
        }
        node->data = NULL;
    }
    return;
}

////////////RECURSIVE FUNCTION//////////
void freeLinkedList(Node *first)
{
    // if list size is one
    if ((*first).next == NULL)
    {
        if ((*first).data != NULL && first != NULL)
        {
            free((*first).data);
            free(first);
            first = NULL;
            return;
        }
        freeNode(first);
        return;
    }
    Node *nextNode = (*first).next;
    freeNode(first);
    freeLinkedList(nextNode);
    return;
}

int isOutOfBound(Node *first, int index)
{
    if (index < 0)
    {
        printf("Out of bound!\n");
        return 1;
    }
    return 0;
}

int isEmpty(Node *first)
{
    return (first == NULL || (*first).data == NULL);
}

void swapNodes(Node *nodeOne, Node *nodeTwo)
{
    void *tempData = (*nodeOne).data;
    float priority = (*nodeOne).priority;
    size_t length = (*nodeOne).dataLength;
    (*nodeOne).data = (*nodeTwo).data;
    (*nodeOne).priority = (*nodeTwo).priority;
    (*nodeOne).dataLength = (*nodeTwo).dataLength;
    (*nodeTwo).data = tempData;
    (*nodeTwo).priority = priority;
    (*nodeTwo).dataLength = length;
    return;
}

// sorts the list by node's priorities
void sort(Node *first)
{
    Node *node = first;
    Node *node2 = first;
    while (node != NULL)
    {
        while ((*node2).next != NULL)
        {
            if ((*node2).priority > (*(*node2).next).priority)
            {
                swapNodes(node2, (*node2).next);
            }
            node2 = (*node2).next;
        }
        node = (*node).next;
        node2 = first;
    }
    return;
}
