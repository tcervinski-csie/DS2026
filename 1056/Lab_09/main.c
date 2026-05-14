#include <stdio.h>
#include "Station.h"
#include "Graph.h"

int main() {

    GraphNode* graph = NULL;
    int count = loadGraph(&graph, "stations.csv", "connections.txt");

    if (count < 0) {
        printf("Failed to load graph.\n");
        return 1;
    }

    printf("Loaded %d stations.\n\n", count);

    printf("--- Graph (adjacency list) ---\n");
    printGraph(graph);

    printf("\n--- Depth-First Search ---\n");
    dfs(graph, 1, count);

    printf("\n--- Breadth-First Search ---\n");
    bfs(graph, 1, count);

    freeGraph(graph);
    return 0;
}
