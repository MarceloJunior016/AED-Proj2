//
// Algoritmos e Estruturas de Dados --- 2024/2025
//
// Joaquim Madeira - Dec 2024
//
// GraphTransitiveClosure - Transitive Closure of a directed graph
//

// Student Name :
// Student Number :
// Student Name :
// Student Number :

/*** COMPLETE THE GraphComputeTransitiveClosure FUNCTION ***/

#include "GraphTransitiveClosure.h"

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "Graph.h"
#include "GraphBellmanFordAlg.h"
#include "instrumentation.h"

// Compute the transitive closure of a directed graph
// Return the computed transitive closure as a directed graph
// Use the Bellman-Ford algorithm
// Função para calcular o fecho transitivo de um grafo orientado

// Função auxiliar para adicionar arestas de alcance
static void AdicionarArestasDeAlcance(Graph* fecho, GraphBellmanFordAlg* algoritmo, unsigned int verticeOrigem, unsigned int totalVertices) {
    for (unsigned int destino = 0; destino < totalVertices; destino++) {
        // Verifica se o destino é alcançável e não cria self-loops
        if (GraphBellmanFordAlgReached(algoritmo, destino) && verticeOrigem != destino) {
            GraphAddEdge(fecho, verticeOrigem, destino);
        }
    }
}

// Função principal para calcular o fecho transitivo
Graph* GraphComputeTransitiveClosure(Graph* grafo) {
    // Verifica se o grafo é válido
    assert(grafo != NULL);
    assert(GraphIsDigraph(grafo));        // Deve ser orientado
    assert(GraphIsWeighted(grafo) == 0); // Não pode ter pesos

    // Obtém o número de vértices do grafo
    unsigned int totalVertices = GraphGetNumVertices(grafo);

    // Cria um novo grafo vazio para representar o fecho transitivo
    Graph* fechoTransitivo = GraphCreate(totalVertices, 1, 0);
    if (fechoTransitivo == NULL) return NULL;

    // Para cada vértice do grafo original
    for (unsigned int origem = 0; origem < totalVertices; origem++) {
        // Executa o algoritmo de Bellman-Ford a partir do vértice atual
        GraphBellmanFordAlg* algoritmoBF = GraphBellmanFordAlgExecute(grafo, origem);
        if (algoritmoBF == NULL) {
            GraphDestroy(&fechoTransitivo); // Libera memória em caso de erro
            return NULL;
        }

        // Adiciona arestas ao fecho transitivo
        AdicionarArestasDeAlcance(fechoTransitivo, algoritmoBF, origem, totalVertices);

        // Libera a memória alocada para o algoritmo de Bellman-Ford
        GraphBellmanFordAlgDestroy(&algoritmoBF);
    }

    // Retorna o grafo representando o fecho transitivo
    return fechoTransitivo;
}
