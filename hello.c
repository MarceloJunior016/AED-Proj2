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
GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* g, unsigned int startVertex) {
 
  assert(g != NULL);
  assert(startVertex < GraphGetNumVertices(g));
  assert(GraphIsWeighted(g) == 0);


  // Alocação e inicialização da estrutura de dados para o algoritmo de Bellman-Ford.
  GraphBellmanFordAlg* result = (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
  assert(result != NULL);

  result->graph = g;
  result->startVertex = startVertex;

  unsigned int numVertices = GraphGetNumVertices(g);

  // Arrays para rastrear os vértices alcançados, as distâncias e os predecessores.
  result->marked = (unsigned int*)calloc(numVertices, sizeof(unsigned int));
  assert(result->marked != NULL);

  result->distance = (int*)malloc(numVertices * sizeof(int));
  assert(result->distance != NULL);

  result->predecessor = (int*)malloc(numVertices * sizeof(int));
  assert(result->predecessor != NULL);

  // Inicialização: define distâncias como infinito, predecessores como -1,
  // e todos os vértices como não alcançados.
  for (unsigned int i = 0; i < numVertices; i++) {
    result->distance[i] = INT_MAX; // Distância inicial infinita
    result->predecessor[i] = -1;  // Sem predecessores no início
    result->marked[i] = 0;        // Nenhum vértice foi alcançado ainda
  }

  // Configuração do vértice inicial
  result->distance[startVertex] = 0; // Distância do vértice inicial para si mesmo é 0
  result->marked[startVertex] = 1;   // Marca o vértice inicial como alcançado

  // Etapa principal: Relaxação de arestas
  // Realiza numVertices - 1 iterações para garantir que todas as distâncias sejam atualizadas.
  for (unsigned int i = 1; i < numVertices; i++) {
    int updated = 0; // Flag para verificar se houve atualizações
    for (unsigned int u = 0; u < numVertices; u++) {
      if (result->distance[u] == INT_MAX) continue; // Pula vértices não alcançáveis

      // Obtém os vizinhos e os pesos das arestas do vértice atual
      unsigned int* neighbors = GraphGetAdjacentsTo(g, u);
      double* weights = GraphGetDistancesToAdjacents(g, u);

      // Verifica se os vizinhos e pesos foram obtidos corretamente
      if (neighbors == NULL || weights == NULL) {
        if (neighbors) free(neighbors);
        if (weights) free(weights);
        continue;
      }

      unsigned int numAdjVertices = neighbors[0]; // Número de vizinhos
      for (unsigned int j = 0; j < numAdjVertices; j++) {
        unsigned int v = neighbors[j + 1]; // Vizinho atual
        if (v >= numVertices) { // Verificação de segurança para índices válidos
          printf("Erro: índice de vizinho fora dos limites em vértice %u!\n", u);
          free(neighbors);
          free(weights);
          GraphBellmanFordAlgDestroy(&result);
          return NULL;
        }

        // Relaxa a aresta se encontrar um caminho mais curto
        int weight = (int)weights[j + 1];
        if (result->distance[u] != INT_MAX && result->distance[u] + weight < result->distance[v]) {
          result->distance[v] = result->distance[u] + weight;
          result->predecessor[v] = u; // Atualiza o predecessor do vizinho
          result->marked[v] = 1;      // Marca o vizinho como alcançado
          updated = 1;                // Indica que houve uma atualização
        }
      }

      // Libera a memória alocada para os vizinhos e pesos
      free(neighbors);
      free(weights);
    }
    if (!updated) break; // Se nenhuma atualização ocorreu, encerra a iteração
  }

  // Verificação de ciclos negativos
  // Realiza uma última iteração para verificar se existem ciclos negativos
  for (unsigned int u = 0; u < numVertices; u++) {
    if (result->distance[u] == INT_MAX) continue; // Pula vértices não alcançáveis

    // Obtém os vizinhos e pesos novamente
    unsigned int* neighbors = GraphGetAdjacentsTo(g, u);
    double* weights = GraphGetDistancesToAdjacents(g, u);

    if (neighbors == NULL || weights == NULL) {
      if (neighbors) free(neighbors);
      if (weights) free(weights);
      continue;
    }

    unsigned int numAdjVertices = neighbors[0]; // Número de vizinhos
    for (unsigned int j = 0; j < numAdjVertices; j++) {
      unsigned int v = neighbors[j + 1];
      if (v >= numVertices) {
        printf("Erro: índice de vizinho fora dos limites em vértice %u!\n", u);
        free(neighbors);
        free(weights);
        GraphBellmanFordAlgDestroy(&result);
        return NULL;
      }

      int weight = (int)weights[j + 1];
      if (result->distance[u] != INT_MAX && result->distance[u] + weight < result->distance[v]) {
        // Se ainda puder relaxar uma aresta, há um ciclo negativo
        printf("Erro: Ciclo negativo detectado em vértice %u!\n", u);
        free(neighbors);
        free(weights);
        GraphBellmanFordAlgDestroy(&result);
        return NULL;
      }
    }

    // Libera a memória alocada
    free(neighbors);
    free(weights);
  }

  return result; // Retorna o resultado com as distâncias calculadas
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
