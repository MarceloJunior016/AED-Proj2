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
    // Verifica se o grafo é válido (não NULL)
    assert(g != NULL);
    // Verifica se o grafo é orientado
    assert(GraphIsDigraph(g));        
    // Verifica se o grafo não possui pesos associados aos arcos
    assert(GraphIsWeighted(g) == 0);  

    // Obtém o número de vértices do grafo
    unsigned int numVertices = GraphGetNumVertices(g);

    // Cria um novo grafo orientado para representar o fecho transitivo
    // O novo grafo terá o mesmo número de vértices e será inicializado como vazio
    Graph* transitiveClosure = GraphCreate(numVertices, 1, 0); 
    if (transitiveClosure == NULL) return NULL; // Retorna NULL se a criação falhar

    // Para cada vértice do grafo original
    for (unsigned int v = 0; v < numVertices; v++) {
        // Executa o algoritmo de Bellman-Ford a partir do vértice atual `v`
        GraphBellmanFordAlg* algorithm = GraphBellmanFordAlgExecute(g, v);
        if (algorithm == NULL) {
            // Se ocorrer um erro no algoritmo, libera a memória do grafo criado
            GraphDestroy(&transitiveClosure);
            return NULL;
        }

        // Para cada vértice `u` do grafo
        for (unsigned int u = 0; u < numVertices; u++) {
            // Verifica se `u` é alcançável a partir de `v` e evita self-loops
            if (GraphBellmanFordAlgReached(algorithm, u) && v != u) {
                // Adiciona uma aresta de `v` para `u` no grafo de fecho transitivo
                GraphAddEdge(transitiveClosure, v, u);
            }
        }

        // Libera a memória alocada para o algoritmo de Bellman-Ford
        GraphBellmanFordAlgDestroy(&algorithm);
    }

    // Retorna o grafo de fecho transitivo
    return transitiveClosure;
}
