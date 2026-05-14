#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

// ---------------------------------------------------------------
// Graph construction
// ---------------------------------------------------------------

void insertNode(GraphNode** graph, Station* station) {
    // TODO
}

static void insertAdj(AdjNode** list, GraphNode* target) {
    // TODO
}

void addEdge(GraphNode* graph, int id1, int id2) {
    // TODO
}

GraphNode* findById(GraphNode* graph, int id) {
    // TODO
    return NULL;
}

// ---------------------------------------------------------------
// Display
// ---------------------------------------------------------------

void printGraph(GraphNode* graph) {
    // TODO
}

// ---------------------------------------------------------------
// Stack (used by DFS)
// ---------------------------------------------------------------

typedef struct StackNode {
    int id;
    struct StackNode* next;
} StackNode;

static void push(StackNode** top, int id) {
    // TODO
}

static int pop(StackNode** top) {
    // TODO
    return 0;
}

// ---------------------------------------------------------------
// Queue (used by BFS)
// ---------------------------------------------------------------

typedef struct QueueNode {
    int id;
    struct QueueNode* next;
} QueueNode;

typedef struct {
    QueueNode* front;
    QueueNode* back;
} Queue;

static void enqueue(Queue* q, int id) {
    // TODO
}

static int dequeue(Queue* q) {
    // TODO
    return 0;
}

// ---------------------------------------------------------------
// Traversals
// ---------------------------------------------------------------

void dfs(GraphNode* graph, int startId, int nodeCount) {
    // TODO
}

void bfs(GraphNode* graph, int startId, int nodeCount) {
    // TODO
}

// ---------------------------------------------------------------
// File loading & cleanup
// ---------------------------------------------------------------

int loadGraph(GraphNode** graph, const char* nodesFile, const char* edgesFile) {
    FILE* f = fopen(nodesFile, "r");
    if (!f) {
        printf("Error: could not open '%s'\n", nodesFile);
        return -1;
    }

    char line[256];
    int count = 0;

    while (fgets(line, sizeof(line), f)) {
        Station* s = malloc(sizeof(Station));

        char* token = strtok(line, ",");
        s->id = atoi(token);

        token = strtok(NULL, ",");
        s->name = malloc(strlen(token) + 1);
        strcpy(s->name, token);

        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = '\0';
        s->line = malloc(strlen(token) + 1);
        strcpy(s->line, token);

        insertNode(graph, s);
        count++;
    }

    fclose(f);

    f = fopen(edgesFile, "r");
    if (!f) {
        printf("Error: could not open '%s'\n", edgesFile);
        return -1;
    }

    while (fgets(line, sizeof(line), f)) {
        char* token = strtok(line, ",");
        int id1 = atoi(token);
        token = strtok(NULL, ",");
        int id2 = atoi(token);
        addEdge(*graph, id1, id2);
    }

    fclose(f);
    return count;
}

void freeGraph(GraphNode* graph) {
    while (graph) {
        AdjNode* adj = graph->adj;
        while (adj) {
            AdjNode* nextAdj = adj->next;
            free(adj);
            adj = nextAdj;
        }
        free(graph->data->name);
        free(graph->data->line);
        free(graph->data);
        GraphNode* next = graph->next;
        free(graph);
        graph = next;
    }
}
