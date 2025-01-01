
# Testes dos Algoritmos Bellman-Ford e Fecho Transitivo

Este projeto contém dois algoritmos principais, com os testes organizados da seguinte forma:

## Organização dos Arquivos

1. **Pastas de Testes**:
   - `graph_tests`: Contém os arquivos `.txt` para os testes do algoritmo **Fecho Transitivo**.
   - `graph_tests_bellmanford`: Contém os arquivos `.txt` para os testes do algoritmo **Bellmanford**.


2. **Arquivos de Código para Testes**:
   - `BellmanFordTest.c`: Testa o algoritmo **Bellman-Ford** com grafos específicos.
   - `FinalTransitiveClosureTest.c`: Testa o algoritmo **Fecho Transitivo**.

## Como Compilar e Executar

1. **Compilar**:
   - Para Bellman-Ford:
     ```bash
     gcc -o BellmanFordTest BellmanFordTest.c Graph.c GraphBellmanFordAlg.c IntegersStack.c SortedList.c instrumentation.c -I. -lm
     ```
   - Para Fecho Transitivo:
     ```bash
gcc -o TransitiveClosureInteractiveTest FinalTransitiveClosureTest.c Graph.c GraphTransitiveClosure.c GraphBellmanFordAlg.c IntegersStack.c SortedList.c instrumentation.c -I. -lm

     ```

2. **Executar**:
   - Para Bellman-Ford:
     ```bash
     ./BellmanFordTest
     ```
   - Para Fecho Transitivo:
     ```bash
    ./TransitiveClosureInteractiveTest

     ```

