#ifndef LL
#define LL
#include "macros.h"

/*Creating Node and List structs for a double ended doubly linked list*/
typedef struct LLNode {
    void* data;
    struct LLNode* next;
    struct LLNode* prev;
}LLNode;

typedef struct {
    int size;
    LLNode* head;
    LLNode* tail;
} LinkedList;

#endif

LinkedList* createList(void);
LLNode* createNode(void);
void insertFirst(LinkedList* inList, LLNode* newNode);
void insertLast(LinkedList* inList, LLNode* newNode);
void removeFirst(LinkedList* inList);
void removeLast(LinkedList* inList);
void freeList(LinkedList* inList);

