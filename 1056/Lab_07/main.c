#include <stdio.h>
#include "Task.h"
#include "Heap.h"

int main() {

    Heap heap = initHeap();
    int count = loadTasks("scheduler.txt", &heap);

    if (count < 0) {
        printf("Failed to load tasks.\n");
        return 1;
    }

    printf("Loaded %d tasks:\n", count);
    printHeap(&heap);

    printf("\n--- Processing tasks by priority ---\n\n");

    while (heap.size > 0) {
        Task task = deleteFromHeap(&heap);
        printf("Processing: ");
        printTask(task);
        free(task.description);
    }

    freeHeap(&heap);

    printf("\n--- Build heap from unordered array ---\n\n");

    Task unordered[] = {
        {"Fix typo in docs",        3},
        {"Patch SQL injection",     28},
        {"Update dependencies",     6},
        {"Resolve merge conflict",  11},
        {"Rotate API keys",         20},
        {"Restart hung service",    15},
        {"Archive old logs",        2}
    };

    int size = sizeof(unordered) / sizeof(Task);
    Heap heap2 = buildHeap(unordered, size);

    printf("Heap built from unordered array:\n");
    printHeap(&heap2);

    freeHeap(&heap2);

    return 0;
}
