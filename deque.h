#ifndef DEQUE_H
#define DEQUE_H

#include <stdio.h>
#include <stdbool.h>
typedef struct _dequeElement
{
    void *data;
    struct _dequeElement *prev;
    struct _dequeElement *next;
} DequeElement;

struct _deque 
{
    int count;
    DequeElement *first;
    DequeElement *last;
    void (*func_free)(void *);
    int (*func_cmp)(void *, void *);
    void (*func_print)(void *, FILE *pfile);
};

typedef struct _deque Deque;

struct number
{
    int value;
};
typedef struct number Number;

typedef enum{
    DEQUE_FORWARD,
    DEQUE_BACKWARD
}IteratorDir;

typedef struct _dequeIterator{
    Deque* deque;
    DequeElement *cur;
    IteratorDir iteratordir;

}DequeIterator;

DequeElement *createElement(void *data);
void destroyElement(DequeElement *el);

Deque *createDeque(void (*func_free)(void *),
                   int (*func_cmp)(void *, void *),
                   void (*func_print)(void *, FILE *pfile));

void destroyDeque(Deque *l, bool destroyed);

bool contains(Deque* l ,void* data);
bool removeElement(Deque*l,void* data,bool destroyed);


void pushFrontElement(Deque*l, void*data);
void pushBackElement(Deque*l , void* data);

Number* popFrontElement(Deque* l);
Number* popBackElement(Deque* l);

void* getFrontElement(Deque* l );
void* getBackElement(Deque* l);

int dequeCount(Deque* l);
void printDeque(Deque* l,FILE* );

DequeIterator *createDequeIterator(Deque* l);
void destroyDequeIterator(DequeIterator* i);
void* DIteratorNext(DequeIterator* i);
bool DIteratorHasNext(DequeIterator* it);
void* DIteratorPrevious(DequeIterator* it);
bool DIteratorHasPrevious(DequeIterator* it);

#endif