#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Graph.h"

// ---------------------------------------------------------------
// Graph construction
// ---------------------------------------------------------------

void insertNode(GraphNode** graph, Station* station) {
    GraphNode* newNode = malloc(sizeof(GraphNode));
    newNode->data = station;
    newNode->adj = NULL;
    newNode->next = NULL;
    if (*graph == NULL) {
        *graph = newNode;
        return;
    }
    GraphNode* aux = *graph;
    while (aux->next) aux = aux->next;
    aux->next = newNode;
}

static void insertAdj(AdjNode** list, GraphNode* target) {
    AdjNode* newNode = malloc(sizeof(AdjNode));
    newNode->target = target;
    newNode->next = NULL;
    if (*list == NULL) {
        *list = newNode;
        return;
    }
    AdjNode* aux = *list;
    while (aux->next) aux = aux->next;
    aux->next = newNode;
}

void addEdge(GraphNode* graph, int id1, int id2) {
    GraphNode* n1 = findById(graph, id1);
    GraphNode* n2 = findById(graph, id2);
    if (n1 && n2) {
        insertAdj(&n1->adj, n2);
        insertAdj(&n2->adj, n1);
    }
}

GraphNode* findById(GraphNode* graph, int id) {
    while (graph && graph->data->id != id)
        graph = graph->next;
    return graph;
}

// ---------------------------------------------------------------
// Display
// ---------------------------------------------------------------

void printGraph(GraphNode* graph) {
    while (graph) {
        printStation(graph->data);
        AdjNode* adj = graph->adj;
        while (adj) {
            printf("    -> [%d] %s\n", adj->target->data->id, adj->target->data->name);
            adj = adj->next;
        }
        graph = graph->next;
    }
}

// ---------------------------------------------------------------
// Stack (used by DFS)
// ---------------------------------------------------------------

typedef struct StackNode {
    int id;
    struct StackNode* next;
} StackNode;

static void push(StackNode** top, int id) {
    StackNode* node = malloc(sizeof(StackNode));
    node->id = id;
    node->next = *top;
    *top = node;
}

static int pop(StackNode** top) {
    int id = (*top)->id;
    StackNode* del = *top;
    *top = (*top)->next;
    free(del);
    return id;
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
    QueueNode* node = malloc(sizeof(QueueNode));
    node->id = id;
    node->next = NULL;
    if (q->back) q->back->next = node;
    else q->front = node;
    q->back = node;
}

static int dequeue(Queue* q) {
    int id = q->front->id;
    QueueNode* del = q->front;
    q->front = q->front->next;
    if (!q->front) q->back = NULL;
    free(del);
    return id;
}

// ---------------------------------------------------------------
// Traversals
// ---------------------------------------------------------------

void dfs(GraphNode* graph, int startId, int nodeCount) {
    int* visited = calloc(nodeCount, sizeof(int));
    StackNode* stack = NULL;

    visited[startId - 1] = 1;
    push(&stack, startId);

    printf("DFS from station [%d]:\n", startId);
    while (stack) {
        int currentId = pop(&stack);
        GraphNode* node = findById(graph, currentId);
        printStation(node->data);

        AdjNode* adj = node->adj;
        while (adj) {
            int neighborId = adj->target->data->id;
            if (!visited[neighborId - 1]) {
                visited[neighborId - 1] = 1;
                push(&stack, neighborId);
            }
            adj = adj->next;
        }
    }
    free(visited);
}

void bfs(GraphNode* graph, int startId, int nodeCount) {
    int* visited = calloc(nodeCount, sizeof(int));
    Queue q = { NULL, NULL };

    visited[startId - 1] = 1;
    enqueue(&q, startId);

    printf("BFS from station [%d]:\n", startId);
    while (q.front) {
        int currentId = dequeue(&q);
        GraphNode* node = findById(graph, currentId);
        printStation(node->data);

        AdjNode* adj = node->adj;
        while (adj) {
            int neighborId = adj->target->data->id;
            if (!visited[neighborId - 1]) {
                visited[neighborId - 1] = 1;
                enqueue(&q, neighborId);
            }
            adj = adj->next;
        }
    }
    free(visited);
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
