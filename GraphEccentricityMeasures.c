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
// Função para calcular medidas de excentricidade em um grafo

// Função auxiliar para inicializar as medidas de raio e diâmetro do grafo
static void InicializarRaioDiametro(GraphEccentricityMeasures* medidas) {
    medidas->graphRadius = INT_MAX; // Configura o raio inicial como infinito
    medidas->graphDiameter = 0;    // Configura o diâmetro inicial como zero
}

// Função auxiliar para calcular a maior distância de um vértice a outros
static int CalcularMaiorDistancia(GraphAllPairsShortestDistances* apsd, unsigned int vertice, unsigned int numVertices) {
    int maiorDistancia = -1; // Variável para armazenar a maior distância
    for (unsigned int w = 0; w < numVertices; w++) {
        int distancia = GraphGetDistanceVW(apsd, vertice, w);
        if (distancia != -1 && distancia > maiorDistancia) {
            maiorDistancia = distancia; // Atualiza a maior distância encontrada
        }
    }
    return maiorDistancia;
}

// Função auxiliar para determinar os vértices centrais
static unsigned int* DeterminarVerticesCentrais(GraphEccentricityMeasures* medidas, unsigned int numVertices) {
    unsigned int numCentrais = 0;
    for (unsigned int v = 0; v < numVertices; v++) {
        if (medidas->eccentricity[v] == medidas->graphRadius) {
            numCentrais++;
        }
    }

    unsigned int* centrais = (unsigned int*)malloc((numCentrais + 1) * sizeof(unsigned int));
    if (centrais == NULL) return NULL;

    centrais[0] = numCentrais; // O primeiro elemento armazena o número de vértices centrais
    unsigned int indice = 1;
    for (unsigned int v = 0; v < numVertices; v++) {
        if (medidas->eccentricity[v] == medidas->graphRadius) {
            centrais[indice++] = v;
        }
    }

    return centrais;
}

// Função principal para calcular medidas de excentricidade em um grafo
GraphEccentricityMeasures* GraphEccentricityMeasuresCompute(Graph* grafo) {
    // Verifica se o grafo é válido
    assert(grafo != NULL);

    // Aloca a estrutura que armazenará os resultados
    GraphEccentricityMeasures* medidas = 
        (GraphEccentricityMeasures*)malloc(sizeof(GraphEccentricityMeasures));
    if (medidas == NULL) return NULL;

    medidas->graph = grafo;

    // Obtém o número de vértices do grafo
    unsigned int numVertices = GraphGetNumVertices(grafo);

    // Calcula as menores distâncias entre todos os pares de vértices
    GraphAllPairsShortestDistances* apsd = GraphAllPairsShortestDistancesExecute(grafo);
    if (apsd == NULL) {
        free(medidas);
        return NULL;
    }

    // Aloca memória para armazenar a excentricidade de cada vértice
    medidas->eccentricity = (int*)malloc(numVertices * sizeof(int));
    if (medidas->eccentricity == NULL) {
        GraphAllPairsShortestDistancesDestroy(&apsd);
        free(medidas);
        return NULL;
    }

    // Inicializa o raio e o diâmetro do grafo
    InicializarRaioDiametro(medidas);

    // Calcula a excentricidade de cada vértice e atualiza raio e diâmetro
    for (unsigned int v = 0; v < numVertices; v++) {
        int maiorDistancia = CalcularMaiorDistancia(apsd, v, numVertices);
        medidas->eccentricity[v] = maiorDistancia;

        if (maiorDistancia != -1) { // Ignora vértices inacessíveis
            if (maiorDistancia < medidas->graphRadius) {
                medidas->graphRadius = maiorDistancia;
            }
            if (maiorDistancia > medidas->graphDiameter) {
                medidas->graphDiameter = maiorDistancia;
            }
        }
    }

    // Determina os vértices centrais
    medidas->centralVertices = DeterminarVerticesCentrais(medidas, numVertices);
    if (medidas->centralVertices == NULL) {
        GraphAllPairsShortestDistancesDestroy(&apsd);
        free(medidas->eccentricity);
        free(medidas);
        return NULL;
    }

    // Libera os recursos da estrutura APSD
    GraphAllPairsShortestDistancesDestroy(&apsd);

    return medidas; // Retorna as medidas calculadas
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
