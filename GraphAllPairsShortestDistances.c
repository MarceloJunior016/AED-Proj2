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
// Função para calcular todas as distâncias mais curtas entre pares de vértices

// Função auxiliar para inicializar a matriz de distâncias
static int** InicializarMatrizDistancias(unsigned int numVertices) {
    int** matriz = (int**)malloc(numVertices * sizeof(int*));
    if (matriz == NULL) return NULL;

    for (unsigned int i = 0; i < numVertices; i++) {
        matriz[i] = (int*)malloc(numVertices * sizeof(int));
        if (matriz[i] == NULL) {
            for (unsigned int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }
    return matriz;
}

// Função auxiliar para liberar a matriz de distâncias
static void LiberarMatrizDistancias(int** matriz, unsigned int numVertices) {
    for (unsigned int i = 0; i < numVertices; i++) {
        free(matriz[i]);
    }
    free(matriz);
}

// Função auxiliar para processar as distâncias a partir de um vértice
static void ProcessarDistanciasVertice(Graph* grafo, int** matriz, unsigned int vertice, unsigned int numVertices) {
    GraphBellmanFordAlg* algoritmoBF = GraphBellmanFordAlgExecute(grafo, vertice);
    assert(algoritmoBF != NULL); // Certifica que o algoritmo foi executado corretamente

    for (unsigned int destino = 0; destino < numVertices; destino++) {
        if (GraphBellmanFordAlgReached(algoritmoBF, destino)) {
            matriz[vertice][destino] = GraphBellmanFordAlgDistance(algoritmoBF, destino);
        } else {
            matriz[vertice][destino] = -1; // Marca como inacessível
        }
    }

    GraphBellmanFordAlgDestroy(&algoritmoBF);
}

// Função principal para calcular as menores distâncias entre todos os pares de vértices
GraphAllPairsShortestDistances* GraphAllPairsShortestDistancesExecute(Graph* grafo) {
    assert(grafo != NULL);

    // Aloca memória para a estrutura principal
    GraphAllPairsShortestDistances* resultado = 
        (GraphAllPairsShortestDistances*)malloc(sizeof(GraphAllPairsShortestDistances));
    if (resultado == NULL) return NULL;

    resultado->graph = grafo;
    unsigned int numVertices = GraphGetNumVertices(grafo);

    // Inicializa a matriz de distâncias
    resultado->distance = InicializarMatrizDistancias(numVertices);
    if (resultado->distance == NULL) {
        free(resultado);
        return NULL;
    }

    // Processa as distâncias para cada vértice
    for (unsigned int origem = 0; origem < numVertices; origem++) {
        ProcessarDistanciasVertice(grafo, resultado->distance, origem, numVertices);
    }

    return resultado;
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
