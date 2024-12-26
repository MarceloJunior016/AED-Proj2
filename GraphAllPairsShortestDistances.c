//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphAllPairsShortestDistances
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphAllPairsShortestDistancesExecute FUNCTION ***/

#include "GraphAllPairsShortestDistances.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"

struct _GraphAllPairsShortestDistances {
  int** distance;  // The 2D matrix storing the all-pairs shortest distances
                   // It is stored as an array of pointers to 1D rows
                   // Idea: an INDEFINITE distance value is stored as -1
  Graph* graph;
};

// Allocate memory and initialize the distance matrix
// Compute the distances between vertices by running the Bellman-Ford algorithm
GraphAllPairsShortestDistances* GraphAllPairsShortestDistancesExecute(Graph* g) {
    assert(g != NULL);

    // Aloca memória para a estrutura principal
    GraphAllPairsShortestDistances* apsd = 
        (GraphAllPairsShortestDistances*)malloc(sizeof(GraphAllPairsShortestDistances));
    if (apsd == NULL) return NULL;

    apsd->graph = g;
    unsigned int numVertices = GraphGetNumVertices(g);

    // Aloca memória para a matriz de distâncias
    apsd->distance = (int**)malloc(numVertices * sizeof(int*));
    if (apsd->distance == NULL) {
        free(apsd);
        return NULL;
    }

    // Aloca memória para cada linha da matriz
    for (unsigned int i = 0; i < numVertices; i++) {
        apsd->distance[i] = (int*)malloc(numVertices * sizeof(int));
        if (apsd->distance[i] == NULL) {
            // Liberação da memória já alocada em caso de erro
            for (unsigned int j = 0; j < i; j++) {
                free(apsd->distance[j]);
            }
            free(apsd->distance);
            free(apsd);
            return NULL;
        }
    }

    // Para cada vértice do grafo
    for (unsigned int v = 0; v < numVertices; v++) {
        // Executa o algoritmo de Bellman-Ford a partir do vértice v
        GraphBellmanFordAlg* algorithm = GraphBellmanFordAlgExecute(g, v);
        if (algorithm == NULL) {
            // Liberação da memória em caso de erro
            for (unsigned int i = 0; i < numVertices; i++) {
                free(apsd->distance[i]);
            }
            free(apsd->distance);
            free(apsd);
            return NULL;
        }

        // Preenche a linha v da matriz de distâncias
        for (unsigned int w = 0; w < numVertices; w++) {
            if (GraphBellmanFordAlgReached(algorithm, w)) {
                // Se w é alcançável a partir de v, guarda a distância
                apsd->distance[v][w] = GraphBellmanFordAlgDistance(algorithm, w);
            } else {
                // Se w não é alcançável, marca com -1 (INDEFINITE)
                apsd->distance[v][w] = -1;
            }
        }

        // Libera a memória do algoritmo de Bellman-Ford
        GraphBellmanFordAlgDestroy(&algorithm);
    }

    return apsd;
}
void GraphAllPairsShortestDistancesDestroy(GraphAllPairsShortestDistances** p) {
  assert(*p != NULL);

  GraphAllPairsShortestDistances* aux = *p;
  unsigned int numVertices = GraphGetNumVertices(aux->graph);

  for (unsigned int i = 0; i < numVertices; i++) {
    free(aux->distance[i]);
  }

  free(aux->distance);

  free(*p);
  *p = NULL;
}

// Getting the result

int GraphGetDistanceVW(const GraphAllPairsShortestDistances* p, unsigned int v,
                       unsigned int w) {
  assert(p != NULL);
  assert(v < GraphGetNumVertices(p->graph));
  assert(w < GraphGetNumVertices(p->graph));

  return p->distance[v][w];
}

// DISPLAYING on the console

void GraphAllPairsShortestDistancesPrint(
    const GraphAllPairsShortestDistances* p) {
  assert(p != NULL);

  unsigned int numVertices = GraphGetNumVertices(p->graph);
  printf("Graph distance matrix - %u vertices\n", numVertices);

  for (unsigned int i = 0; i < numVertices; i++) {
    for (unsigned int j = 0; j < numVertices; j++) {
      int distanceIJ = p->distance[i][j];
      if (distanceIJ == -1) {
        // INFINITY - j was not reached from i
        printf(" INF");
      } else {
        printf(" %3d", distanceIJ);
      }
    }
    printf("\n");
  }
}
