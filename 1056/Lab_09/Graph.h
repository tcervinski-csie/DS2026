#pragma once

#include "Station.h"

typedef struct AdjNode AdjNode;
typedef struct GraphNode GraphNode;

struct AdjNode {
    GraphNode* target;
    AdjNode* next;
};

struct GraphNode {
    Station* data;
    AdjNode* adj;
    GraphNode* next;
};

// Building the graph
void insertNode(GraphNode** graph, Station* station);
void addEdge(GraphNode* graph, int id1, int id2);
GraphNode* findById(GraphNode* graph, int id);

// Display
void printGraph(GraphNode* graph);

// Traversals
void dfs(GraphNode* graph, int startId, int nodeCount);
void bfs(GraphNode* graph, int startId, int nodeCount);

// File loading & cleanup
int loadGraph(GraphNode** graph, const char* nodesFile, const char* edgesFile);
void freeGraph(GraphNode* graph);
