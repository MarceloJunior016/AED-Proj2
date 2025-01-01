#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"
#include "GraphBellmanFordAlg.h"

void testGraphFromFile(const char* filename) {
    printf("Testing graph from file: %s\n", filename);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    int type, weighted, numVertices, numEdges;
    fscanf(file, "%d %d %d %d", &type, &weighted, &numVertices, &numEdges);

    Graph* graph = GraphCreate(numVertices, type, weighted);
    if (!graph) {
        printf("Error: Failed to create graph.\n");
        fclose(file);
        return;
    }

    for (int i = 0; i < numEdges; i++) {
        int u, v;
        fscanf(file, "%d %d", &u, &v);
        GraphAddEdge(graph, u, v);
    }
    fclose(file);

    printf("The graph:\n");
    GraphDisplayDOT(graph);
    printf("\n");

    GraphCheckInvariants(graph);

    printf("Running Bellman-Ford Algorithm:\n");
    for (unsigned int i = 0; i < GraphGetNumVertices(graph); i++) {
        GraphBellmanFordAlg* BF_result = GraphBellmanFordAlgExecute(graph, i);
        if (BF_result) {
            printf("The shortest path tree rooted at %u:\n", i);
            GraphBellmanFordAlgDisplayDOT(BF_result);
            GraphBellmanFordAlgDestroy(&BF_result);
        } else {
            printf("Bellman-Ford failed for vertex %u.\n", i);
        }
    }

    GraphDestroy(&graph);
    printf("Test completed for: %s\n", filename);
}

int main(void) {
    const char* filenames[] = {
        "graph_tests_bellmanford/bellmanford_graph1.txt",
        "graph_tests_bellmanford/bellmanford_graph2.txt",
        "graph_tests_bellmanford/bellmanford_graph3.txt",
        "graph_tests_bellmanford/bellmanford_graph4.txt",
        "graph_tests_bellmanford/bellmanford_graph5.txt",
        "graph_tests_bellmanford/bellmanford_graph6.txt",
        "graph_tests_bellmanford/bellmanford_graph7.txt",
        "graph_tests_bellmanford/bellmanford_graph8.txt",
        "graph_tests_bellmanford/bellmanford_graph9.txt",
        "graph_tests_bellmanford/bellmanford_graph10.txt",
        "graph_tests_bellmanford/bellmanford_graph11.txt",
        "graph_tests_bellmanford/bellmanford_graph12.txt",
        "graph_tests_bellmanford/bellmanford_graph13.txt",
        "graph_tests_bellmanford/bellmanford_graph14.txt",
        "graph_tests_bellmanford/bellmanford_graph15.txt",
        "graph_tests_bellmanford/bellmanford_graph16.txt",
        "graph_tests_bellmanford/bellmanford_graph17.txt",
        "graph_tests_bellmanford/bellmanford_graph18.txt",
        "graph_tests_bellmanford/bellmanford_graph19.txt",
        "graph_tests_bellmanford/bellmanford_graph20.txt"
    };

    int numFiles = sizeof(filenames) / sizeof(filenames[0]);
    while (1) {
        printf("\nSelect a graph to test (1-%d) or type 'exit' to quit: ", numFiles);
        char input[10];
        scanf("%s", input);

        if (strcmp(input, "exit") == 0) {
            printf("Exiting the program.\n");
            break;
        }

        int choice = atoi(input);
        if (choice < 1 || choice > numFiles) {
            printf("Invalid choice. Please select a valid number between 1 and %d.\n", numFiles);
        } else {
            testGraphFromFile(filenames[choice - 1]);
        }
    }

    return 0;
}
