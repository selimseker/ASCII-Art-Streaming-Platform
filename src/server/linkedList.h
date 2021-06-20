#if !defined(LINKED_LIST)
#define LINKED_LIST

#include<stdlib.h>
#include<stdio.h>

typedef struct linkedlist
{
    void *data;
    float priority;
    struct linkedlist *next;
    struct linkedlist *prev;
    size_t dataLength;
} Node;

Node *LinkedList();

Node *addToList(Node *first, void *newData, int priority, size_t newDataType);

void *popFirst(Node **first);

void *popNodeByIndex(Node **first, int index);

Node *getNodeByIndex(Node *first, int index);

Node *getNodeByData(Node *first, void *data, size_t dataLength);

void *getDataByIndex(Node *first, int index);

int sizeLL(Node *first);

void freeNode(Node *oneNode);

void extractNode(Node *node);

void freeLinkedList(Node *first);

int isOutOfBound(Node *first, int index);

int isEmpty(Node *first);

void swapNodes(Node* nodeOne, Node *nodeTwo);

void sort(Node *first);

#endif // LINKED_LIST
