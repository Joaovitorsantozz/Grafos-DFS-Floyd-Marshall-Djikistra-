#include <stdio.h>
#include <stdlib.h>
#include "deque.h"
#include "grafos.h"
#include <limits.h>
void testaGrafo();
int main()
{
    testaGrafo();
    return 0;
}

VerticeVizinho *createVerticeVizinho(int indice, int pesoAresta)
{
    VerticeVizinho *verticevizinho = malloc(sizeof(VerticeVizinho));
    if (verticevizinho == NULL)
        return NULL;
    verticevizinho->indiceVizinho = indice;
    verticevizinho->pesoAresta = pesoAresta;

    return verticevizinho;
}

void freeVerticeVizinho(void *v)
{
    VerticeVizinho *t = (VerticeVizinho *)v;
    free(t);
}

int compareVerticeVizinho(void *v1, void *v2)
{
    // serve pra achar um dterminado indice, se for 0 entao voce esta no indice
    VerticeVizinho *p1 = (VerticeVizinho *)v1;
    VerticeVizinho *p2 = (VerticeVizinho *)v2;

    return (p1->indiceVizinho - p2->indiceVizinho);
}

void printVerticeVizinho(void *v1, FILE *pfile)
{
    VerticeVizinho *v = (VerticeVizinho *)v1;
    printf("(%d%d)", v->indiceVizinho, v->pesoAresta);
}

Grafo *createGrafo(int numVertices, bool direcionado)
{
    Grafo *g = malloc(sizeof(Grafo));
    if (g == NULL)
        return NULL;
    g->numVertices = numVertices;
    g->direcionado = direcionado;

    g->arrVertices = malloc(numVertices * sizeof(Vertice));
    if (g->arrVertices == NULL)
    {
        free(g);
        return NULL;
    }

    for (int i = 0; i < numVertices; i++)
    {
        g->arrVertices[i].indice = i;
        g->arrVertices[i].peso = 0;
        g->arrVertices[i].cor = 0;

        g->arrVertices[i].adjacentes = createDeque(freeVerticeVizinho, compareVerticeVizinho, printVerticeVizinho);
    }
    return g;
}

void destroyGrafo(Grafo *g, bool destroyData)
{
    if (g == NULL)
        return;

    if (g->arrVertices != NULL)
    {
        for (int i = 0; i < g->numVertices; i++)
        {
            if (g->arrVertices[i].adjacentes != NULL)
            {
                destroyDeque(g->arrVertices[i].adjacentes, destroyData);
            }
        }
        free(g->arrVertices);
    }
    free(g);
}

void addAdjacent(Grafo *g, int verticeIndex, int adjacentIndex, int edgeWeight)
{
    if (g == NULL || verticeIndex < 0 || verticeIndex >= g->numVertices || adjacentIndex < 0 || adjacentIndex >= g->numVertices)
        return;

    VerticeVizinho *v1 = createVerticeVizinho(adjacentIndex, edgeWeight);
    if (v1 != NULL)
    {
        pushBackElement(g->arrVertices[verticeIndex].adjacentes, v1);
    }
    if (!g->direcionado)
    {
        VerticeVizinho *v2 = createVerticeVizinho(verticeIndex, edgeWeight);
        if (v2 != NULL)
            pushBackElement(g->arrVertices[adjacentIndex].adjacentes, v2);
    }
}

void delAdjacent(Grafo *g, int verticeIndex, int adjacentIndex, bool destroyData)
{
    if (g == NULL || verticeIndex < 0 || verticeIndex >= g->numVertices || adjacentIndex < 0 || adjacentIndex >= g->numVertices)
        return;

    Deque *deque = g->arrVertices[verticeIndex].adjacentes;
    DequeIterator *it = createDequeIterator(deque);
    while (DIteratorHasNext(it))
    {
        VerticeVizinho *v1 = (VerticeVizinho *)DIteratorNext(it);
        if (v1->indiceVizinho == adjacentIndex)
        {
            removeElement(deque, v1, 1);
            if (destroyData)
            {
                freeVerticeVizinho(v1);
                break;
            }
        }
    }
    destroyDequeIterator(it);
    if (!g->direcionado)
    {
        // nao é direcionado, pode ter volta;
        Deque *deque2 = g->arrVertices[adjacentIndex].adjacentes;
        DequeIterator *it2 = createDequeIterator(deque2);
        while (DIteratorHasNext(it2))
        {
            VerticeVizinho *v2 = (VerticeVizinho *)DIteratorNext(it2);
            if (v2->indiceVizinho == verticeIndex)
            {
                removeElement(deque2, v2, 1);
                if (destroyData)
                {
                    freeVerticeVizinho(v2);
                    break;
                }
            }
        }
        destroyDequeIterator(it2);
    }
}

void printGraph(Grafo *g, FILE *pfile)
{
    if (g == NULL)
        return;

    if (pfile == NULL)
        pfile = stdout;

    fprintf(pfile, "Grafo (%s)\n", g->direcionado ? "Direcionado" : "Nao Direcionado");

    for (int i = 0; i < g->numVertices; i++)
    {
        fprintf(pfile, "Vertice %d", g->arrVertices[i].indice);
        Deque *deque = g->arrVertices[i].adjacentes;
        DequeIterator *it = createDequeIterator(deque);

        while (DIteratorHasNext(it))
        {
            VerticeVizinho *v = DIteratorNext(it);
            fprintf(pfile, " -> (Vizinho: %d, Peso: %d)", v->indiceVizinho, v->pesoAresta);
        }
        fprintf(pfile, "\n");
        destroyDequeIterator(it);
    }
    fprintf(pfile, "------------------\n");
}

DequeIterator *getAdjacentListIT(Grafo *g, int verticeIndex)
{
    if (g == NULL || verticeIndex >= g->numVertices || verticeIndex < 0)
        return NULL;

    return createDequeIterator(g->arrVertices[verticeIndex].adjacentes);
}

int getDeegree(Grafo *g, int verticeIndex)
{
    if (g == NULL || verticeIndex >= g->numVertices || verticeIndex < 0)
        return -1;

    DequeIterator *it = getAdjacentListIT(g, verticeIndex);
    int degree = 0;
    while (DIteratorHasNext(it))
    {
        DIteratorNext(it);
        degree++;
    }

    destroyDequeIterator(it);

    return degree;
}

void zeraCores(Grafo *g)
{
    if (g == NULL)
        return;

    for (int i = 0; i < g->numVertices; i++)
        g->arrVertices[i].cor = 0;
}

int getCor(Grafo *g, int index)
{
    if (g == NULL || index < 0 || index >= g->numVertices)
        return 0;
    return g->arrVertices[index].cor;
}

void setCor(Grafo *g, int index, int cor)
{
    if (g == NULL || index < 0 || index >= g->numVertices)
        return;

    g->arrVertices[index].cor = cor;
}
int getNumVertices(Grafo *g)
{
    if (g == NULL)
        return 0;

    return g->numVertices;
}
Aresta *createAresta(int vertice1, int vertice2, int pesoAresta)
{
    Aresta *ret = malloc(sizeof(Aresta));
    if (ret == NULL)
    {
        exit(0);
    }
    ret->verticeDestino = vertice2;
    ret->verticeOrigem = vertice1;
    ret->weight = pesoAresta;

    return ret;
}

void freeAresta(void *a)
{
    Aresta *ar = a;
    free(ar);
}

int cmpAresta(void *a1, void *a2)
{
    Aresta *ar1 = a1;
    Aresta *ar2 = a2;

    if (ar1 == NULL || ar2 == NULL)
        exit(0);

    if (ar1->verticeOrigem == ar2->verticeOrigem && ar1->verticeDestino == ar2->verticeDestino)
        return 0;
    else
        return 1;
}

void printAresta(void *a, FILE *pfile)
{
    Aresta *ar = (Aresta *)a;
    fprintf(pfile, "(%d, %d, w:%d) ", ar->verticeOrigem, ar->verticeDestino, ar->weight);
}
void DFS(Grafo *g, int verticeIndex, Deque *listaArestas, int *cor)
{
    (*cor)++;
    setCor(g, verticeIndex, *cor);
    DequeIterator *it = getAdjacentListIT(g, verticeIndex);
    while (DIteratorHasNext(it))
    {
        VerticeVizinho *vizinho = DIteratorNext(it);
        if (getCor(g, vizinho->indiceVizinho) == 0)
        {
            Aresta *arestaVisitada = createAresta(verticeIndex, vizinho->indiceVizinho, vizinho->pesoAresta);
            pushBackElement(listaArestas, arestaVisitada);
            DFS(g, vizinho->indiceVizinho, listaArestas, cor);
        }
    }
}
Deque *depthFirstSearch(Grafo *g)
{
    zeraCores(g);
    Deque *edges = createDeque(freeAresta, cmpAresta, printAresta);
    int cor = 0;
    for (int i = 0; i < getNumVertices(g); i++)
    {
        if (getCor(g, i) == 0)
        {
            DFS(g, i, edges, &cor);
        }
    }
    return edges;
}

Deque *Dijikistra(Grafo *g, int verticeindex, int destIndex)
{

    for (int i = 0; i < g->numVertices; i++)
    {
        g->arrVertices[i].peso = INT_MAX;
    }
    g->arrVertices[verticeindex].peso = 0;
    int *tobeChecked = (int *)malloc((g->numVertices * sizeof(int)));
    int *pai = (int *)malloc((g->numVertices * sizeof(int)));
    for (int i = 0; i < g->numVertices; i++)
    {
        tobeChecked[i] = 1; // o array ta interligado entao todo vertice precisa ser checado
        pai[i] = -1;
    }
    for (int loops = 0; loops < g->numVertices; loops++)
    {
        int v = -1;
        int minPeso = INT_MAX;
        for (int i = 0; i < g->numVertices; i++)
        {
            if (tobeChecked[i] == 1)
            {
                if (g->arrVertices[i].peso < minPeso)
                {
                    v = i;
                    minPeso = g->arrVertices[i].peso;
                }
            }
        }
        if (v == -1 || g->arrVertices[v].peso == INT_MAX)
            break; // nao achou nenhum vertice

        tobeChecked[v] = 0;
        DequeIterator *it = getAdjacentListIT(g, v);
        while (DIteratorHasNext(it))
        {
            VerticeVizinho *vizinho = DIteratorNext(it);
            int u = vizinho->indiceVizinho;
            int pesoAresta = vizinho->pesoAresta;

            if (tobeChecked[u] == 1)
            {
                if (g->arrVertices[v].peso != INT_MAX)
                {
                    if (g->arrVertices[u].peso > (g->arrVertices[v].peso + pesoAresta))
                    {
                        g->arrVertices[u].peso = g->arrVertices[v].peso + pesoAresta;
                        pai[u] = v;
                    }
                }
            }
        }
        free(it);
    }
    Deque *caminho = createDeque(freeAresta, cmpAresta, printAresta);

    if (g->arrVertices[destIndex].peso == INT_MAX)
    {
        free(tobeChecked);
        free(pai);
        return caminho;
    }
    int atual = destIndex;
    while (atual != verticeindex && pai[atual] != -1)
    {
        int vpai = pai[atual];
        Aresta *a = createAresta(vpai, atual, getPeso(g, vpai, atual));
        pushFrontElement(caminho, a);
        atual = vpai;
    }
    free(tobeChecked);
    free(pai);

    return caminho;
}

int getPeso(Grafo *g, int verticeOrigem, int verticeDestino)
{
    DequeIterator *it = getAdjacentListIT(g, verticeOrigem);

    while (DIteratorHasNext(it))
    {
        VerticeVizinho *vizinho = DIteratorNext(it);
        if (vizinho->indiceVizinho == verticeDestino)
        {
            return vizinho->pesoAresta;
        }
    }
    return 0;
}
void testaGrafo()
{
    printf("=== INICIANDO TESTES DO GRAFO ===\n\n");

    Grafo *g = createGrafo(9, false);

    printf("--> Grafo Inicial (Vazio):\n");
    printGraph(g, stdout);

    addAdjacent(g, 0, 4, 1);
    addAdjacent(g, 0, 5, 1);
    addAdjacent(g, 0, 6, 1);
    addAdjacent(g, 0, 8, 1);

    addAdjacent(g, 1, 6, 1);

    addAdjacent(g, 2, 7, 1);

    addAdjacent(g, 3, 7, 1);

    addAdjacent(g, 4, 8, 1);

    addAdjacent(g, 5, 8, 1);

    printf("--> Grafo apos insercoes:\n");
    printGraph(g, stdout);

    printf("Iniciando do ponto A ate o ponto B");
    Deque *listaArestas = Dijikistra(g, 8, 1);

    printDeque(listaArestas, stdout);
    printf("\n\n------------------\n");

    destroyDeque(listaArestas, true);
    destroyGrafo(g, true);
}