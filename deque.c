#include <stdio.h>
#include <stdlib.h>
#include "deque.h"
#include "grafos.h"

void printNumber(void *data, FILE *pfile)
{
    Number *n = data;
    fprintf(pfile, "%d \n", n->value);
}

int cmpNumber(void *d1, void *d2)
{
    Number *n1 = d1;
    Number *n2 = d2;

    return (n1->value - n2->value); // se for 0, é pq é o mesmo valor
}

void printDeque(Deque *l, FILE *p)
{
    DequeElement *cur = l->first;

    while (cur != NULL)
    {
        l->func_print(cur->data, p);
        fprintf(p, ", ");
        cur = cur->next;
    }
}

void freeNumber(void *data)
{
    Number *n = data;
    free(n);
}

Deque *createDeque(void (*func_free)(void *),
                   int (*func_cmp)(void *, void *),
                   void (*func_print)(void *, FILE *pfile))

{
    Deque *deque = malloc(sizeof(Deque));
    if (deque == NULL)
    {
        return NULL;
    }
    deque->count = 0;
    deque->first = NULL;
    deque->last = NULL;

    deque->func_free = func_free;
    deque->func_cmp = func_cmp;
    deque->func_print = func_print;

    return deque;
}
DequeIterator *createDequeIterator(Deque *q)
{
    if (q == NULL)
        return NULL;
    DequeIterator *it = malloc(sizeof(DequeIterator));
    if (it == NULL)
        return NULL;

    it->deque = q;
    it->cur = q->first;
    it->iteratordir = DEQUE_FORWARD;
    return it;
}
bool DIteratorHasNext(DequeIterator *h)
{
    if (h->cur == NULL || h == NULL)
        return false;

    return h->cur != NULL;
}

void *DIteratorNext(DequeIterator *it)
{
    if (it == NULL || it->cur == NULL)
        return NULL;

    void *data = it->cur->data;
    it->cur = it->cur->next;
    return data;
}
void *DIteratorPrevious(DequeIterator *it)
{
    if (it == NULL || it->deque == NULL)
        return NULL;

    if (it->cur == NULL)
    {
        it->cur = it->deque->last;
    }
    else
        it->cur = it->cur->prev;

    if (it->cur == NULL)
        return NULL;
        
    return it->cur->data;
}
bool DIteratorHasPrevious(DequeIterator *h)
{
    if (h->deque == NULL || h == NULL)
        return false;

    if (h->cur == NULL)
    {
        return h->deque->last != NULL;
    }
    return h->cur->prev != NULL;
}
void destroyDequeIterator(DequeIterator *it)
{
    DequeIterator *i = it;
    if (it != NULL)
        free(i);
}

Number *createNumber(int value)
{
    Number *n = malloc(sizeof(Number));

    if (n == NULL)
    {
        exit(0);
    }
    n->value = value;
    return n;
}
DequeElement *createElement(void *data)
{
    DequeElement *el = malloc(sizeof(DequeElement));
    if (el == NULL)
        return NULL;
    el->data = data;
    el->next = NULL;
    el->prev = NULL;

    // por questão de separar responsabilidades, eu não defino aqui qual é o proximo ou anterior, eu so crio um nó isolado
    return el;
}

void pushBackElement(Deque *l, void *data)
{
    // supondo a sequencia 1-2-3-4 entao 4 é back e 1 é front
    DequeElement *el = createElement(data);
    if (l == NULL || el == NULL)
        return;
    l->count++;
    if (l->last == NULL)
    {
        l->first = el;
        l->last = el;
    }
    else
    {
        DequeElement *temp = l->last;
        l->last->next = el;
        el->prev = temp;
        l->last = el;
    }
}

void pushFrontElement(Deque *l, void *data)
{
    DequeElement *el = createElement(data);
    if (l == NULL || el == NULL)
        return;

    if (l->first == NULL)
    {
        // significa que é o primeiro item a inserir
        l->first = el;
        l->last = el;
    }
    else
    {
        el->next = l->first;
        l->first->prev = el;
        l->first = el;
    }
    l->count++;
}

Number *popBackElement(Deque *l)
{
    if (l == NULL || l->last == NULL)
        return NULL;

    DequeElement *temp = l->last;
    Number *data = (Number *)temp->data;
    if (l->last->prev == NULL)
    {
        // é o ultimo a ser removido
        l->last = NULL;
        l->first = NULL;
    }
    else
    {
        l->last = l->last->prev;
        l->last->next = NULL;
    }
    l->count--;

    free(temp);
    return data;
}
Number *popFrontElement(Deque *l)
{
    // tirar o first
    if (l == NULL || l->first == NULL)
        return NULL;
    DequeElement *temp = l->first;
    Number *data = (Number *)temp->data;
    if (l->first->next == NULL)
    {
        // significa que é o ultimo nó
        l->count--;
        l->first = NULL;
        l->last = NULL;
        free(temp);
    }
    else
    {
        l->count--;
        l->first = l->first->next;
        l->first->prev = NULL;
        free(temp);
    }

    return data;
}
void testaLista()
{
    Deque *lista = createDeque(freeNumber, cmpNumber, printNumber);
    DequeIterator *it = createDequeIterator(lista);
    printf("Percorrendo a lista \n");
    while (DIteratorHasNext(it))
    {
        Number *n = (Number *)DIteratorNext(it);
        printf("%d ", n->value);
    }
    printf("\n");
    destroyDequeIterator(it);
    printf("Imprime lista vazia \n");
    printDeque(lista, stdout);
    printf("\n");

    pushBackElement(lista, createNumber(3));
    Number *n = popBackElement(lista);
    printf("Retirou %d \n", n->value);
    freeNumber(n);

    pushFrontElement(lista, createNumber(4));
    n = popFrontElement(lista);
    printf("Retirou %d \n", n->value);
    freeNumber(n);

    for (int i = 0; i < 10; i++)
    {
        Number *n = createNumber(i);
        pushFrontElement(lista, n);
    }
    printf("\n");

    it = createDequeIterator(lista);
    printf("Navegando deque iterator \n");

    n = (Number *)DIteratorNext(it);
    printf("%d\n", n->value);
    n = (Number *)DIteratorPrevious(it);
    printf("%d\n", n->value);

    printf("\n");
    printf("Indo ate o fim do iterador \n");

    while (DIteratorHasNext(it))
    {
        Number *n = (Number *)DIteratorNext(it);
        printf("%d ", n->value);
    }
    printf("\n");
    printf("Voltar ate o comeco do iterador\n");

    while (DIteratorHasPrevious(it))
    {
        Number *n = (Number *)DIteratorPrevious(it);
        printf("%d ", n->value);
    }
}

bool removeElement(Deque *l, void *data, bool destroyed)
{
    if (l == NULL || l->first == NULL || data == NULL)
        return false;

    DequeElement *cur = l->first;

    while (cur != NULL)
    {
    
        bool isEqual = (l->func_cmp != NULL) ? (l->func_cmp(cur->data, data) == 0) : (cur->data == data);

        if (isEqual)
        {
            if (cur->prev != NULL)
                cur->prev->next = cur->next;
            else
                l->first = cur->next;

            if (cur->next != NULL)
                cur->next->prev = cur->prev;
            else
                l->last = cur->prev;

            if (destroyed && l->func_free != NULL)
                l->func_free(cur->data);

            free(cur);
            l->count--;
            return true;
        }
        cur = cur->next;
    }

    return false;
}

void destroyDeque(Deque *l, bool destroyed)
{
    if (l == NULL)
        return;

    DequeElement *cur = l->first;
    while (cur != NULL)
    {
        DequeElement *next = cur->next;
        if (destroyed && l->func_free != NULL)
        {
            l->func_free(cur->data);
        }
        free(cur);
        cur = next;
    }
    free(l);
}