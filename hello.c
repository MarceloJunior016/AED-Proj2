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
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));

  GraphBellmanFordAlg* result =
      (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  // Configurações iniciais
  result->graph = g;
  result->startVertex = startVertex;

  unsigned int numVertices = GraphGetNumVertices(g);

  // Inicialização dos arrays
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  assert(result->marked != NULL);

  result->distance = (int*)malloc(numVertices * sizeof(int));
  assert(result->distance != NULL);

  result->predecessor = (int*)malloc(numVertices * sizeof(int));
  assert(result->predecessor != NULL);

  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = INT_MAX;  // Inicializa com "infinito"
    result->predecessor[i] = -1;   // Sem predecessor
  }
  result->distance[startVertex] = 0;  // Distância para o vértice inicial é 0

  // Relaxação das arestas (numVertices - 1) vezes
  for (unsigned int i = 1; i < numVertices; i++) {
    for (unsigned int u = 0; u < numVertices; u++) {
      unsigned int* neighbors = GraphGetAdjacentsTo(g, u);
      double* weights = GraphGetDistancesToAdjacents(g, u);

      if (neighbors == NULL || weights == NULL) {
        if (neighbors) free(neighbors);
        if (weights) free(weights);
        continue;
      }

      unsigned int j = 0;
      while (neighbors[j] != (unsigned int)-1 && j < numVertices) {
        unsigned int v = neighbors[j];
        if (v >= numVertices) {
          j++;
          continue;
        }

        int weight = (int)weights[j];
        if (result->distance[u] != INT_MAX &&
            result->distance[u] + weight < result->distance[v]) {
          result->distance[v] = result->distance[u] + weight;
          result->predecessor[v] = u;
        }
        j++;
      }
      free(neighbors);
      free(weights);
    }
  }

  // Verificação de ciclos negativos
  for (unsigned int u = 0; u < numVertices; u++) {
    unsigned int* neighbors = GraphGetAdjacentsTo(g, u);
    double* weights = GraphGetDistancesToAdjacents(g, u);

    if (neighbors == NULL || weights == NULL) {
      if (neighbors) free(neighbors);
      if (weights) free(weights);
      continue;
    }

    unsigned int j = 0;
    while (neighbors[j] != (unsigned int)-1 && j < numVertices) {
      unsigned int v = neighbors[j];
      if (v >= numVertices) {
        j++;
        continue;
      }

      int weight = (int)weights[j];
      if (result->distance[u] != INT_MAX &&
          result->distance[u] + weight < result->distance[v]) {
        free(neighbors);
        free(weights);
        GraphBellmanFordAlgDestroy(&result);
        return NULL;
      }
      j++;
    }
    free(neighbors);
    free(weights);
  }

  return result;
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
