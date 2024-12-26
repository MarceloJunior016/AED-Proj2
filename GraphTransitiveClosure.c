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
Graph* GraphComputeTransitiveClosure(Graph* g) {
    assert(g != NULL);
    assert(GraphIsDigraph(g));
    assert(GraphIsWeighted(g) == 0);

    // Obtém o número de vértices do grafo original
    unsigned int numVertices = GraphGetNumVertices(g);

    // Cria um novo grafo orientado para o fecho transitivo
    // com o mesmo número de vértices do grafo original
    Graph* transitiveClosure = GraphCreate(numVertices, 1, 0);
    if (transitiveClosure == NULL) return NULL;

    // Para cada vértice do grafo
    for (unsigned int v = 0; v < numVertices; v++) {
        // Executa Bellman-Ford a partir do vértice v
        GraphBellmanFordAlg* algorithm = GraphBellmanFordAlgExecute(g, v);
        if (algorithm == NULL) {
            GraphDestroy(&transitiveClosure);
            return NULL;
        }

        // Para cada outro vértice u
        for (unsigned int u = 0; u < numVertices; u++) {
            // Se u é alcançável a partir de v
            if (GraphBellmanFordAlgReached(algorithm, u)) {
                // Adiciona um arco de v para u no fecho transitivo
                GraphAddEdge(transitiveClosure, v, u);
            }
        }

        // Libera a memória do algoritmo de Bellman-Ford
        GraphBellmanFordAlgDestroy(&algorithm);
    }

    return transitiveClosure;
}