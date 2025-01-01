#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Graph.h"
#include "GraphTransitiveClosure.h"

void testGraphFromFile(const char* filename) {
    printf("Starting test for: %s\n", filename);
    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Error: Could not open file %s\n", filename);
        return;
    }

    int type, weighted, numVertices, numEdges;
    fscanf(file, "%d %d %d %d", &type, &weighted, &numVertices, &numEdges);

    Graph* graph = GraphCreate(numVertices, type, weighted);
    if (!graph) {
        printf("Error: Failed to create graph for %s\n", filename);
        fclose(file);
        return;
    }

    for (int i = 0; i < numEdges; i++) {
        int u, v;
        fscanf(file, "%d %d", &u, &v);
        GraphAddEdge(graph, u, v);
    }
    fclose(file);

    printf("Graph loaded from: %s\n", filename);
    GraphDisplayDOT(graph);
    printf("\n");

    printf("Checking graph invariants...\n");
    GraphCheckInvariants(graph);

    printf("Computing Transitive Closure for: %s\n", filename);
    Graph* transitiveClosure = GraphComputeTransitiveClosure(graph);
    if (transitiveClosure) {
        GraphDisplayDOT(transitiveClosure);
        GraphDestroy(&transitiveClosure);
    } else {
        printf("Failed to compute Transitive Closure for: %s\n", filename);
    }

    GraphDestroy(&graph);
    printf("Test completed for: %s\n\n", filename);
}

int main(void) {
    const char* filenames[] = {
        "graph_tests/transitive_graph1.txt",
        "graph_tests/transitive_graph2.txt",
        "graph_tests/transitive_graph3.txt",
        "graph_tests/transitive_graph4.txt",
        "graph_tests/transitive_graph5.txt",
        "graph_tests/transitive_graph6.txt",
        "graph_tests/transitive_graph7.txt",
        "graph_tests/transitive_graph8.txt",
        "graph_tests/transitive_graph9.txt",
        "graph_tests/transitive_graph10.txt",
        "graph_tests/transitive_graph11.txt",
        "graph_tests/transitive_graph12.txt",
        "graph_tests/transitive_graph13.txt",
        "graph_tests/transitive_graph14.txt",
        "graph_tests/transitive_graph15.txt",
        "graph_tests/transitive_graph16.txt",
        "graph_tests/transitive_graph17.txt",
        "graph_tests/transitive_graph18.txt",
        "graph_tests/transitive_graph19.txt",
        "graph_tests/transitive_graph20.txt"
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
