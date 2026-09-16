# Algoritmo de Percussão
- Esse algoritmo usa DFS para percorrer em profundida e Djikistra para ir do ponto A ao ponto B do grafo.
- Os grafos podem ser direcionados ou não
- Os grafos utilizam uma estrutura de dados chamada DEQUE ou lista duplamente encadeada
- Para percorrer os vértices utiliza iteradores que também estão implementados no código


<img width="741" height="519" alt="image" src="https://github.com/user-attachments/assets/4fce9e95-93c9-4d93-8fd9-74b0a4dc2089" />
<img width="611" height="856" alt="image" src="https://github.com/user-attachments/assets/3aa68240-2a01-41ec-8979-04a516a0907b" />
<img width="1463" height="664" alt="image" src="https://github.com/user-attachments/assets/e3530259-085f-498e-bc0a-ef215587ca22" />


# Mapa das funções

### **Criação e Destruição**
* `Grafo *createGrafo(int numVertices, bool direcionado)`
  * Aloca e inicializa a estrutura do Grafo com o número de vértices e a flag de direcionamento.
* `void destroyGrafo(Grafo *g, bool destroyData)`
  * Libera toda a memória alocada para o grafo, seus vértices e listas de adjacência.
* `VerticeVizinho *createVerticeVizinho(int indice, int pesoAresta)`
  * Cria uma conexão de vizinhança contendo o índice do destino e o peso da aresta.
* `void freeVerticeVizinho(void *v)`
  * Libera a memória de um nó de vizinhança.
* `Aresta *createAresta(int vertice1, int vertice2, int pesoAresta)`
  * Cria uma estrutura de aresta explícita (origem, destino e peso).
* `void freeAresta(void *a)`
  * Libera a memória de uma estrutura de aresta.

---

### **Manipulação de Arestas e Adjacências**
* `void addAdjacent(Grafo *g, int verticeIndex, int adjacentIndex, int edgeWeight)`
  * Adiciona uma aresta entre dois vértices (trata grafos direcionados e não direcionados automaticamente).
* `void delAdjacent(Grafo *g, int verticeIndex, int adjacentIndex, bool destroyData)`
  * Remove a conexão entre dois vértices da lista de adjacências.
* `int getPeso(Grafo *g, int verticeOrigem, int verticeDestino)`
  * Retorna o peso da aresta entre dois vértices específicos.
* `int getDeegree(Grafo *g, int verticeIndex)`
  * Retorna o grau de um determinado vértice (quantidade de vizinhos).

---

### **Algoritmos de Busca**
* `Deque *Dijikistra(Grafo *g, int verticeindex, int destIndex)`
  * Executa o algoritmo de Dijkstra para encontrar o menor caminho entre dois nós e devolve um `Deque` contendo as arestas da rota reconstruída.
* `Deque *depthFirstSearch(Grafo *g)`
  * Executa a Busca em Profundidade (DFS) em todo o grafo e retorna o conjunto de arestas visitadas.
* `void DFS(Grafo *g, int verticeIndex, Deque *listaArestas, int *cor)`
  * Função auxiliar recursiva para navegação em profundidade nos vértices.

---

### **Utilitários e Auxiliares do Grafo**
* `void zeraCores(Grafo *g)` — Reseta a cor/estado de todos os vértices para 0.
* `int getCor(Grafo *g, int index)` — Retorna o estado/cor de um vértice.
* `void setCor(Grafo *g, int index, int cor)` — Altera o estado/cor de um vértice.
* `int getNumVertices(Grafo *g)` — Retorna o total de vértices do grafo.
* `DequeIterator *getAdjacentListIT(Grafo *g, int verticeIndex)` — Retorna um iterador para percorrer a lista de vizinhos de um vértice.
* `void printGraph(Grafo *g, FILE *pfile)` — Imprime a representação em texto do grafo.
* `int compareVerticeVizinho(void *v1, void *v2)` — Função de comparação para vizinhos.
* `int cmpAresta(void *a1, void *a2)` — Função de comparação para arestas.
* `void printVerticeVizinho(void *v1, FILE *pfile)` / `printAresta(void
