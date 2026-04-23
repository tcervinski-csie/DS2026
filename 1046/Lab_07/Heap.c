#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Heap.h"

static void swapTask(Task* t1, Task* t2) {
    Task aux = *t1;
    *t1 = *t2;
    *t2 = aux;
}

static void heapify(Heap* heap, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->tasks[left].priority > heap->tasks[largest].priority) {
        largest = left;
    }

    if (right < heap->size && heap->tasks[right].priority > heap->tasks[largest].priority) {
        largest = right;
    }

    if (largest != index) {
        swapTask(&heap->tasks[largest], &heap->tasks[index]);
        heapify(heap, largest);
    }
}

Heap initHeap() {
    Heap heap;
    heap.size = 0;
    heap.tasks = malloc(0);
    return heap;
}

void insertHeap(Heap* heap, Task task) {
    heap->size++;
    heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));

    int index = heap->size - 1;
    heap->tasks[index] = task;

    int parent = (index - 1) / 2;

    while (index > 0 && heap->tasks[index].priority > heap->tasks[parent].priority) {
        swapTask(&heap->tasks[index], &heap->tasks[parent]);
        index = parent;
        parent = (index - 1) / 2;
    }
}

Task deleteFromHeap(Heap* heap) {
    Task top = heap->tasks[0];

    heap->tasks[0] = heap->tasks[heap->size - 1];
    heap->size--;
    heap->tasks = realloc(heap->tasks, heap->size * sizeof(Task));

    heapify(heap, 0);

    return top;
}

void printHeap(Heap* heap) {
    for (int i = 0; i < heap->size; i++) {
        printTask(heap->tasks[i]);
    }
}

Heap buildHeap(Task* tasks, int size) {
    Heap heap;
    heap.size = size;
    heap.tasks = malloc(size * sizeof(Task));

    for (int i = 0; i < size; i++) {
        heap.tasks[i] = tasks[i];
    }

    for (int i = heap.size / 2 - 1; i >= 0; i--) {
        heapify(&heap, i);
    }

    return heap;
}

void freeHeap(Heap* heap) {
    for (int i = 0; i < heap->size; i++) {
        free(heap->tasks[i].description);
    }
    free(heap->tasks);
    heap->tasks = NULL;
    heap->size = 0;
}

int loadTasks(const char* filename, Heap* heap) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: could not open file '%s'\n", filename);
        return -1;
    }

    char line[128];
    int count = 0;

    while (fgets(line, sizeof(line), f) != NULL) {
        Task task;

        char* token = strtok(line, ",");
        task.priority = atoi(token);

        token = strtok(NULL, ",");
        token[strcspn(token, "\n")] = '\0';
        task.description = malloc(strlen(token) + 1);
        strcpy(task.description, token);

        insertHeap(heap, task);
        count++;
    }

    fclose(f);
    return count;
}
