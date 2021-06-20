#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helperMethods.h"
#include "linkedList.h"
#include "queue.h"

/*
Generic Queue with Linked List implementation
*/

Queue *queueConstructor()
{
    Queue *queue = malloc(sizeof(Queue));
    (*queue).frontNode = LinkedList();
    (*queue).tailNode = (*queue).frontNode;
    (*queue).tail = -1;
    return queue;
}

int queueSize(Queue *queue)
{
    if (queue == NULL)
    {
        return 0;
    }

    return (*queue).tail + 1;
}

void freeQueue(Queue *queue)
{
    (*queue).tailNode = NULL;
    freeLinkedList((*queue).frontNode);
    free(queue);
}

/////////////CORE FUNCTIONS////////////
void enqueue(Queue *queue, void *data, int priority, size_t newDataType)
{
    (*queue).tailNode = addToList((*queue).tailNode, data, priority, newDataType);
    (*queue).tail = (*queue).tail + 1;
    return;
}

void *dequeue(Queue *queue)
{
    void *targetData = popFirst(&(*queue).frontNode);
    (*queue).tail = (*queue).tail - 1;
    return targetData;
}

// void priorityQueue();
