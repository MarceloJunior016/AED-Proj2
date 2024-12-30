//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphEccentricityMeasures
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphEccentricityMeasuresCompute FUNCTION ***/
/*** COMPLETE THE GraphGetCentralVertices FUNCTION ***/
/*** COMPLETE THE GraphEccentricityMeasuresPrint FUNCTION ***/

#include "GraphEccentricityMeasures.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#include "Graph.h"
#include "GraphAllPairsShortestDistances.h"

struct _GraphEccentricityMeasures {
  unsigned int*
      centralVertices;  // centralVertices[0] = number of central vertices
                        // array size is (number of central vertices + 1)
  int* eccentricity;    // the eccentricity value of each vertex
  Graph* graph;         // the graph
  int graphRadius;      // the graph radius
  int graphDiameter;    // the graph diameter
};

// Allocate memory
// Compute the vertex eccentricity values
// Compute graph radius and graph diameter
// Compute the set of central vertices
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* g) {
    assert(g != NULL);

    // Inicializa a estrutura principal
    GraphEccentricityMeasures* medidas = 
        (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
    if (medidas == NULL) return NULL;

    medidas->graph = g;

    // Obtém o número de vértices do grafo
    unsigned int numVertices = GraphGetNumVertices(g);

    // Calcula as distâncias entre todos os pares de vértices usando o módulo ALL-PAIRS-SHORTEST-DISTANCES
    GraphAllPairsShortestDistances* apsd = GraphAllPairsShortestDistancesExecute(g);
    if (apsd == NULL) {
        free(medidas);
        return NULL;
    }

    // Aloca memória para a excentricidade de cada vértice
    medidas->eccentricity = (int*)malloc(numVertices * sizeof(int));
    if (medidas->eccentricity == NULL) {
        GraphAllPairsShortestDistancesDestroy(&apsd);
        free(medidas);
        return NULL;
    }

    // Inicializa o raio e o diâmetro do grafo
    medidas->graphRadius = INT_MAX;
    medidas->graphDiameter = 0;

    // Calcula a excentricidade de cada vértice
    for (unsigned int v = 0; v < numVertices; v++) {
        int maxDistance = -1;
        for (unsigned int w = 0; w < numVertices; w++) {
            int distance = GraphGetDistanceVW(apsd, v, w);
            if (distance != -1) {  // Ignora vértices não alcançáveis
                if (distance > maxDistance) {
                    maxDistance = distance;
                }
            }
        }
        medidas->eccentricity[v] = maxDistance;

        // Atualiza o raio e o diâmetro do grafo
        if (maxDistance != -1) {  // Ignora vértices isolados
            if (maxDistance < medidas->graphRadius) {
                medidas->graphRadius = maxDistance;
            }
            if (maxDistance > medidas->graphDiameter) {
                medidas->graphDiameter = maxDistance;
            }
        }
    }

    // Determina o conjunto de vértices centrais (aqueles com excentricidade igual ao raio)
    unsigned int numCentralVertices = 0;
    for (unsigned int v = 0; v < numVertices; v++) {
        if (medidas->eccentricity[v] == medidas->graphRadius) {
            numCentralVertices++;
        }
    }

    // Aloca memória para os vértices centrais
    medidas->centralVertices = (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
    if (medidas->centralVertices == NULL) {
        GraphAllPairsShortestDistancesDestroy(&apsd);
        free(medidas->eccentricity);
        free(medidas);
        return NULL;
    }

    medidas->centralVertices[0] = numCentralVertices; // O primeiro elemento armazena o número de vértices centrais
    unsigned int index = 1;
    for (unsigned int v = 0; v < numVertices; v++) {
        if (medidas->eccentricity[v] == medidas->graphRadius) {
            medidas->centralVertices[index++] = v;
        }
    }

    // Libera a estrutura APSD
    GraphAllPairsShortestDistancesDestroy(&apsd);

    return medidas;
}

unsigned int* GraphGetCentralVertices(const GraphEccentricityMeasures* p) {
    assert(p != NULL);
    assert(p->centralVertices != NULL);

    // Retorna uma cópia do array de vértices centrais
    unsigned int numCentralVertices = p->centralVertices[0];
    unsigned int* copia = (unsigned int*)malloc((numCentralVertices + 1) * sizeof(unsigned int));
    if (copia == NULL) return NULL;

    for (unsigned int i = 0; i <= numCentralVertices; i++) {
        copia[i] = p->centralVertices[i];
    }

    return copia;
}
void GraphEccentricityMeasuresDestroy(GraphEccentricityMeasures** p) {
    assert(p != NULL);       // Verifica que o ponteiro não é nulo
    assert(*p != NULL);      // Verifica que a estrutura não é nula

    GraphEccentricityMeasures* aux = *p;

    // Libera a memória alocada para os vértices centrais
    free(aux->centralVertices);

    // Libera a memória alocada para os valores de excentricidade
    free(aux->eccentricity);

    // Libera a estrutura principal
    free(*p);

    // Garante que o ponteiro original agora aponte para NULL
    *p = NULL;
}


void GraphEccentricityMeasuresPrint(const GraphEccentricityMeasures* p) {
    assert(p != NULL);

    printf("Raio do grafo: %d\n", p->graphRadius);
    printf("Diâmetro do grafo: %d\n", p->graphDiameter);

    printf("Excentricidade dos vértices:\n");
    unsigned int numVertices = GraphGetNumVertices(p->graph);
    for (unsigned int v = 0; v < numVertices; v++) {
        printf("Vértice %u: %d\n", v, p->eccentricity[v]);
    }

    printf("Conjunto de vértices centrais (excentricidade igual ao raio):\n");
    unsigned int numCentralVertices = p->centralVertices[0];
    for (unsigned int i = 1; i <= numCentralVertices; i++) {
        printf("Vértice %u\n", p->centralVertices[i]);
    }
}
