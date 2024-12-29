//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphBellmanFord - Bellman-Ford Algorithm
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphBellmanFordAlgExecute FUNCTION ***/

#include "GraphBellmanFordAlg.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Graph.h"
#include "IntegersStack.h"
#include "instrumentation.h"

struct _GraphBellmanFordAlg {
  unsigned int* marked;  // To mark vertices when reached for the first time
  int* distance;  // The number of edges on the path from the start vertex
                  // distance[i]=-1, if no path found from the start vertex to i
  int* predecessor;  // The predecessor vertex in the shortest path
                     // predecessor[i]=-1, if no predecessor exists
  Graph* graph;
  unsigned int startVertex;  // The root of the shortest-paths tree
};
GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g,
                                                unsigned int startVertex) {
  // Verifica se o grafo não é nulo e se o vértice inicial está dentro do intervalo válido
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));

  // Aloca memória para a estrutura do algoritmo Bellman-Ford
  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  // Configuração inicial da estrutura
  result->graph = g;
  result->startVertex = startVertex;

  // Obtém o número total de vértices no grafo
  unsigned int numVertices = GraphGetNumVertices(g);

  // Inicializa o array `marked` para identificar quais vértices foram visitados
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  assert(result->marked != NULL);

  // Inicializa o array `distance` para armazenar as distâncias mínimas do vértice inicial
  result->distance = (int*)malloc(numVertices * sizeof(int));
  assert(result->distance != NULL);

  // Inicializa o array `predecessor` para armazenar os predecessores de cada vértice no caminho mínimo
  result->predecessor = (int*)malloc(numVertices * sizeof(int));
  assert(result->predecessor != NULL);

  // Configura as distâncias iniciais como infinito e os predecessores como inexistentes (-1)
  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = INT_MAX;  // Distância inicial infinita
    result->predecessor[i] = -1;   // Sem predecessor inicial
  }
  result->distance[startVertex] = 0;  // A distância até o vértice inicial é 0

  // Relaxação das arestas: executada (número de vértices - 1) vezes
  for (unsigned int i = 1; i < numVertices; i++) {
    for (unsigned int u = 0; u < numVertices; u++) {
      // Obtém os vizinhos do vértice atual `u` e os pesos das arestas para eles
      unsigned int* neighbors = GraphGetAdjacentsTo(g, u);
      double* weights = GraphGetDistancesToAdjacents(g, u);

      // Verifica se o vértice `u` tem vizinhos válidos
      if (neighbors == NULL || weights == NULL) {
        if (neighbors) free(neighbors); // Libera memória caso exista
        if (weights) free(weights);     // Libera memória caso exista
        continue;
      }

      // Relaxação das arestas adjacentes
      unsigned int j = 0;
      while (neighbors[j] != (unsigned int)-1 && j < numVertices) {
        unsigned int v = neighbors[j]; // Vértice vizinho
        if (v >= numVertices) {       // Ignora vértices inválidos
          j++;
          continue;
        }

        int weight = (int)weights[j]; // Peso da aresta entre `u` e `v`
        // Verifica se a distância pode ser minimizada
        if (result->distance[u] != INT_MAX &&
            result->distance[u] + weight < result->distance[v]) {
          result->distance[v] = result->distance[u] + weight; // Atualiza a menor distância
          result->predecessor[v] = u;                        // Define `u` como predecessor de `v`
        }
        j++;
      }
      // Libera memória alocada para vizinhos e pesos
      free(neighbors);
      free(weights);
    }
  }

  // Verificação de ciclos negativos
  for (unsigned int u = 0; u < numVertices; u++) {
    unsigned int* neighbors = GraphGetAdjacentsTo(g, u);
    double* weights = GraphGetDistancesToAdjacents(g, u);

    if (neighbors == NULL || weights == NULL) {
      if (neighbors) free(neighbors); // Libera memória caso exista
      if (weights) free(weights);     // Libera memória caso exista
      continue;
    }

    unsigned int j = 0;
    while (neighbors[j] != (unsigned int)-1 && j < numVertices) {
      unsigned int v = neighbors[j];
      if (v >= numVertices) {       // Ignora vértices inválidos
        j++;
        continue;
      }

      int weight = (int)weights[j];
      // Verifica se ainda é possível reduzir a distância (indicando um ciclo negativo)
      if (result->distance[u] != INT_MAX &&
          result->distance[u] + weight < result->distance[v]) {
        free(neighbors);
        free(weights);
        GraphBellmanFordAlgDestroy(&result); // Libera memória e retorna NULL
        return NULL;
      }
      j++;
    }
    // Libera memória alocada para vizinhos e pesos
    free(neighbors);
    free(weights);
  }

  return result; // Retorna a estrutura contendo os resultados do algoritmo
}



void GraphBellmanFordAlgDestroy(GraphBellmanFordAlg** p) {
  assert(*p != NULL);

  GraphBellmanFordAlg* aux = *p;

  free(aux->marked);
  free(aux->predecessor);
  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the paths information

int GraphBellmanFordAlgReached(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->marked[v];
}

int GraphBellmanFordAlgDistance(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  return p->distance[v];
}
Stack* GraphBellmanFordAlgPathTo(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = StackCreate(GraphGetNumVertices(p->graph));

  if (p->marked[v] == 0) {
    return s;
  }

  // Store the path
  for (unsigned int current = v; current != p->startVertex;
       current = p->predecessor[current]) {
    StackPush(s, current);
  }

  StackPush(s, p->startVertex);

  return s;
}

// DISPLAYING on the console

void GraphBellmanFordAlgShowPath(const GraphBellmanFordAlg* p, unsigned int v) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));

  Stack* s = GraphBellmanFordAlgPathTo(p, v);

  while (StackIsEmpty(s) == 0) {
    printf("%d ", StackPop(s));
  }

  StackDestroy(&s);
}

// Display the Shortest-Paths Tree in DOT format
void GraphBellmanFordAlgDisplayDOT(const GraphBellmanFordAlg* p) {
  assert(p != NULL);

  Graph* original_graph = p->graph;
  unsigned int num_vertices = GraphGetNumVertices(original_graph);

  // A árvore de caminhos mínimos é um digrafo sem pesos nas arestas
  Graph* paths_tree = GraphCreate(num_vertices, 1, 0);

  // Use o array de predecessores para adicionar as arestas da árvore
  for (unsigned int w = 0; w < num_vertices; w++) {
    int v = p->predecessor[w];
    if (v != -1 && v != (int)w) {  // Evitar self-loops
      GraphAddEdge(paths_tree, (unsigned int)v, w);
    }
  }

  // Exiba a árvore no formato DOT
  GraphDisplayDOT(paths_tree);

  // Liberação de memória
  GraphDestroy(&paths_tree);
}
