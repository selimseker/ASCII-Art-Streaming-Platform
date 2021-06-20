#if !defined(QUEUE)
#define QUEUE

#include "linkedList.h"

typedef struct queue
{
    Node *frontNode;
    Node *tailNode;
    int tail;
} Queue;

Queue *queueConstructor();

int queueSize(Queue *queue);

void freeQueue(Queue *queue);

//////////CORE FUNCTIONS//////////
void enqueue(Queue *queue, void *data, int priority, size_t newDataType);

void *dequeue(Queue *queue);

#endif // QUEUE
