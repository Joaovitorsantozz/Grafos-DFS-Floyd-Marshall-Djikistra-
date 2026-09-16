#ifndef GRAFOS_H
#define GRAFOS_H

#include "deque.h"

struct _vertice
{
    int indice;
    int peso;
    int cor;
    Deque *adjacentes;
};
typedef struct _vertice Vertice;

struct _verticeVizinho
{
    int indiceVizinho;
    int pesoAresta;
};

typedef struct _verticeVizinho VerticeVizinho;

struct _grafo
{
    int numVertices;
    bool direcionado;
    Vertice *arrVertices;
};

struct _aresta{
    int verticeOrigem;
    int verticeDestino;
    int weight;
};


typedef struct _aresta Aresta;
typedef struct _grafo Grafo;

VerticeVizinho *createVerticeVizinho(int indice, int pesoAresta);
void freeVerticeVizinho(void *v);
int compareVerticeVizinho(void *v1, void *v2);
void printVerticeVizinho(void *v, FILE *pfile);

Grafo *createGrafo(int numVertices, bool direcionado);
void destroyGrafo(Grafo *g, bool destroyData);
void addAdjacent(Grafo *g, int verticeIndex, int adjacentIndex, int edgeWeight);
void delAdjacent(Grafo *g, int verticeIndex, int adjacentIndex, bool destroyData);
void printGraph(Grafo *g, FILE *pfile);
DequeIterator *getAdjacentListIT(Grafo *g, int verticeIndex);
int getDeegree(Grafo *g, int verticeIndex);
void zeraCores(Grafo *g);
int getCor(Grafo *g, int index);
void setCor(Grafo *g, int index, int cor);
int getNumVertices(Grafo *g);

Aresta* createAresta(int vertice1,int vertice2,int pesoAresta);
void freeAresta(void* a);
int cmpAresta(void *a1, void* a2);
void printAresta(void* a, FILE* pfile);
int getPeso(Grafo *g, int verticeOrigem, int verticeDestino);
#endif