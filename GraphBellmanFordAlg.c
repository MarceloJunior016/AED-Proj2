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
#include <time.h>

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

// Variáveis globais para medir complexidade espacial
size_t memoria_inicializacao = 0;
size_t memoria_relaxamento = 0;
size_t memoria_verificacao = 0;
size_t memoria_total = 0;

// Funções auxiliares para registrar memória
static void RegistrarMemoriaAlocada(size_t* contador, size_t bytes) {
    *contador += bytes;
    memoria_total += bytes;
}

static void RegistrarMemoriaLiberada(size_t* contador, size_t bytes) {
    *contador -= bytes;
    memoria_total -= bytes;
}

// Função para inicializar a estrutura de resultados
// Esta função configura as distâncias iniciais de todos os vértices como "infinito" (INT_MAX),
// define que nenhum vértice tem um predecessor no início (-1) e marca todos como não visitados.
// Para o vértice inicial, a distância é configurada como 0 e ele é marcado como visitado.
static void InicializarResultado(GraphBellmanFordAlg* resultado, unsigned int totalVertices, unsigned int inicio) {
    for (unsigned int i = 0; i < totalVertices; i++) {
        resultado->distance[i] = INT_MAX; // Considera que todos estão muito longe inicialmente
        resultado->predecessor[i] = -1;  // Nenhum caminho foi estabelecido ainda
        resultado->marked[i] = 0;        // Nenhum vértice foi visitado
    }
    resultado->distance[inicio] = 0; // O ponto de partida está a distância 0 de si mesmo
    resultado->marked[inicio] = 1;   // Marca o vértice inicial como visitado

    // Registrar memória alocada na inicialização
    RegistrarMemoriaAlocada(&memoria_inicializacao, totalVertices * sizeof(unsigned int) + 2 * totalVertices * sizeof(int));
}

// Função para liberar recursos de memória
// Esta função é usada para garantir que não haja desperdício de memória alocada temporariamente
// para os vizinhos e pesos enquanto percorremos o grafo.
static void LiberarMemoria(unsigned int* adjacentes, double* pesos) {
    if (adjacentes) {
        RegistrarMemoriaLiberada(&memoria_relaxamento, sizeof(unsigned int) * adjacentes[0]);
        free(adjacentes);
    }
    if (pesos) {
        RegistrarMemoriaLiberada(&memoria_relaxamento, sizeof(double) * adjacentes[0]);
        free(pesos);
    }
}

// Função para atualizar distâncias das arestas
// Esta função percorre todas as arestas do grafo e tenta relaxar (atualizar) as distâncias para os vértices adjacentes.
// Se encontrar um caminho mais curto para algum vértice, a distância é atualizada e o vértice é marcado como modificado.
int operation_count = 0;
clock_t tempo_inicializacao = 0;
clock_t tempo_relaxamento = 0;
clock_t tempo_verificacao = 0;

// Instrumentação no loop de relaxamento
static int AtualizarDistancias(Graph* grafo, GraphBellmanFordAlg* resultado, unsigned int totalVertices) {
    clock_t start = clock();
    int houveAtualizacao = 0;
    for (unsigned int origem = 0; origem < totalVertices; origem++) {
        if (resultado->distance[origem] == INT_MAX) continue;

        unsigned int* adjacentes = GraphGetAdjacentsTo(grafo, origem);
        double* pesos = GraphGetDistancesToAdjacents(grafo, origem);

        if (!adjacentes || !pesos) {
            LiberarMemoria(adjacentes, pesos);
            continue;
        }

        // Registrar memória alocada para adjacentes e pesos
        RegistrarMemoriaAlocada(&memoria_relaxamento, sizeof(unsigned int) * adjacentes[0]);
        RegistrarMemoriaAlocada(&memoria_relaxamento, sizeof(double) * adjacentes[0]);

        unsigned int totalAdjacentes = adjacentes[0];
        for (unsigned int i = 0; i < totalAdjacentes; i++) {
            operation_count++;  // Conta operações
            unsigned int destino = adjacentes[i + 1];
            int peso = (int)pesos[i + 1];

            if (resultado->distance[origem] + peso < resultado->distance[destino]) {
                resultado->distance[destino] = resultado->distance[origem] + peso;
                resultado->predecessor[destino] = origem;
                resultado->marked[destino] = 1;
                houveAtualizacao = 1;
            }
        }
        LiberarMemoria(adjacentes, pesos);
    }
    tempo_relaxamento += (clock() - start);
    return houveAtualizacao;
}

// Função para detectar ciclos negativos
// Esta função verifica se ainda é possível reduzir a distância de algum vértice após todas as iterações esperadas.
// Se for possível, significa que existe um ciclo com peso negativo no grafo.
static int DetectarCiclos(Graph* grafo, GraphBellmanFordAlg* resultado, unsigned int totalVertices) {
    clock_t start = clock();
    for (unsigned int origem = 0; origem < totalVertices; origem++) {
        if (resultado->distance[origem] == INT_MAX) continue;

        unsigned int* adjacentes = GraphGetAdjacentsTo(grafo, origem);
        double* pesos = GraphGetDistancesToAdjacents(grafo, origem);

        if (!adjacentes || !pesos) {
            LiberarMemoria(adjacentes, pesos);
            continue;
        }

        // Registrar memória alocada para adjacentes e pesos
        RegistrarMemoriaAlocada(&memoria_verificacao, sizeof(unsigned int) * adjacentes[0]);
        RegistrarMemoriaAlocada(&memoria_verificacao, sizeof(double) * adjacentes[0]);

        unsigned int totalAdjacentes = adjacentes[0];
        for (unsigned int i = 0; i < totalAdjacentes; i++) {
            operation_count++;  // Conta operações
            unsigned int destino = adjacentes[i + 1];
            int peso = (int)pesos[i + 1];

            if (resultado->distance[origem] + peso < resultado->distance[destino]) {
                LiberarMemoria(adjacentes, pesos);
                tempo_verificacao += (clock() - start);
                return 1;
            }
        }
        LiberarMemoria(adjacentes, pesos);
    }
    tempo_verificacao += (clock() - start);
    return 0;
}

GraphBellmanFordAlg* GraphBellmanFordAlgExecute(Graph* grafo, unsigned int inicio) {
    operation_count = 0;  // Reinicia o contador
    memoria_inicializacao = 0;
    memoria_relaxamento = 0;
    memoria_verificacao = 0;

    clock_t start = clock();

    // Verificações de validade dos parâmetros
    assert(grafo != NULL);
    assert(inicio < GraphGetNumVertices(grafo));
    assert(GraphIsWeighted(grafo) == 0);

    // Aloca a estrutura de resultados
    GraphBellmanFordAlg* resultado = (GraphBellmanFordAlg*)malloc(sizeof(struct _GraphBellmanFordAlg));
    RegistrarMemoriaAlocada(&memoria_total, sizeof(struct _GraphBellmanFordAlg));

    unsigned int totalVertices = GraphGetNumVertices(grafo);

    resultado->graph = grafo;
    resultado->startVertex = inicio;
    resultado->marked = (unsigned int*)calloc(totalVertices, sizeof(unsigned int));
    resultado->distance = (int*)malloc(totalVertices * sizeof(int));
    resultado->predecessor = (int*)malloc(totalVertices * sizeof(int));

    RegistrarMemoriaAlocada(&memoria_total, totalVertices * sizeof(unsigned int) + 2 * totalVertices * sizeof(int));

    InicializarResultado(resultado, totalVertices, inicio);

    for (unsigned int iteracao = 1; iteracao < totalVertices; iteracao++) {
        if (!AtualizarDistancias(grafo, resultado, totalVertices)) {
            break;
        }
    }

    if (DetectarCiclos(grafo, resultado, totalVertices)) {
        GraphBellmanFordAlgDestroy(&resultado);
        return NULL;
    }

    clock_t end = clock();
    printf("Tempo total de execução: %f segundos\n", ((double)(end - start)) / CLOCKS_PER_SEC);
    printf("Tempo de Inicialização: %f segundos\n", ((double)tempo_inicializacao) / CLOCKS_PER_SEC);
    printf("Tempo de Relaxamento: %f segundos\n", ((double)tempo_relaxamento) / CLOCKS_PER_SEC);
    printf("Tempo de Verificação de Ciclos: %f segundos\n", ((double)tempo_verificacao) / CLOCKS_PER_SEC);
    printf("Memória total utilizada: %zu bytes\n", memoria_total);
    printf("Memória Inicialização: %zu bytes\n", memoria_inicializacao);
    printf("Memória Relaxamento: %zu bytes\n", memoria_relaxamento);
    printf("Memória Verificação de Ciclos: %zu bytes\n", memoria_verificacao);
    printf("Operações realizadas: %d\n", operation_count);

    return resultado; // Retorna o resultado final com as distâncias calculadas
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
